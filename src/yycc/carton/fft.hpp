#pragma once
#include <concepts>
#include <type_traits>
#include <numbers>
#include <bit>
#include <cstdint>
#include <complex>
#include <vector>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <algorithm>

namespace yycc::carton::fft {

    /// @private
    /// @brief Meta-programming utilities for FFT modules.
    namespace util {

        template<std::floating_point TFloat>
        inline constexpr TFloat tau_v = static_cast<TFloat>(2) * std::numbers::pi_v<TFloat>;

        // NOTE:
        // We use std::has_single_bit() to check whether given number is an integral power of 2.
        // And use (std::bit_width() - 1) to get the exponent of given number based on 2.

        template<typename TIndex, typename TFloat, TIndex VN>
        struct validate_args {
        private:
            static constexpr bool is_unsigned_int = std::is_unsigned_v<TIndex> && std::is_integral_v<TIndex>;
            static constexpr bool is_float_point = std::is_floating_point_v<TFloat>;
            static constexpr bool n_is_pow_2 = std::has_single_bit<TIndex>(static_cast<TIndex>(VN)) && VN >= static_cast<TIndex>(2);

        public:
            static constexpr bool value = is_unsigned_int && is_float_point && n_is_pow_2;
        };

        template<typename TIndex, typename TFloat, TIndex VN>
        inline constexpr bool validate_args_v = validate_args<TIndex, TFloat, VN>::value;

    } // namespace util

#pragma region Window

    enum class WindowType { HanningWindow };

    template<typename TIndex, typename TFloat, TIndex VN>
        requires util::validate_args_v<TIndex, TFloat, VN>
    class Window {
    private:
        static constexpr TIndex N = VN;

    public:
        Window(WindowType win_type) : window_type(win_type), window_data(nullptr) {
            // Pre-compute window data
            // Allocate window buffer
            window_data = std::make_unique<TFloat[]>(N);
            // Assign window data
            switch (win_type) {
                case WindowType::HanningWindow:
                    for (TIndex i = 0u; i < N; ++i) {
                        window_data[i] = static_cast<TFloat>(0.5)
                                         * (static_cast<TFloat>(1)
                                            - std::cos(util::tau_v<TFloat>
                                                       * static_cast<TFloat>(i) / static_cast<TFloat>(N - static_cast<TIndex>(1))));
                    }
                    break;
                default:
                    throw std::invalid_argument("invalid window function type");
            }
        }

    private:
        WindowType window_type;
        std::unique_ptr<TFloat[]> window_data;

    public:
        /**
		 * @brief Apply window function to given data sequence.
		 * @param[in,out] data 
		 * The float-point data sequence for applying window function.
		 * The length of this sequence must be N.
		*/
        void apply_window(TFloat* data) const {
            if (data == nullptr) [[unlikely]] {
                throw std::invalid_argument("nullptr data is not allowed for applying window.");
            }
            for (TIndex i = static_cast<TIndex>(0); i < N; ++i) {
                data[i] *= window_data[i];
            }
        }
        /**
		 * @brief Get underlying window function data for custom applying.
		 * @return 
		 * The pointer to the start address of underlying window function data sequence.
		 * The length of this sequence is N.
		*/
        const TFloat* get_window_data() const { return window_data.get(); }
    };

#pragma endregion

#pragma region FFT

    template<typename TIndex, typename TFloat, TIndex VN>
        requires util::validate_args_v<TIndex, TFloat, VN>
    struct FftProperties {
    public:
        using TComplex = std::complex<TFloat>;
        static constexpr TIndex N = static_cast<TIndex>(VN);
        static constexpr TIndex M = static_cast<TIndex>(std::bit_width<TIndex>(N) - 1);
        static constexpr TIndex HALF_POINT = N >> static_cast<TIndex>(1);
    };

    /**
     * @brief The core FFT class.
     * @details The core class implementing FFT algorithm (base-2 version).
     * @tparam TIndex 
     * @tparam TFloat 
     * @tparam N 
     */
    template<typename TIndex, typename TFloat, TIndex VN>
        requires util::validate_args_v<TIndex, TFloat, VN>
    class Fft {
    private:
        using TProperties = FftProperties<TIndex, TFloat, VN>;
        using TComplex = TProperties::TComplex;
        static constexpr TIndex N = TProperties::N;
        static constexpr TIndex M = TProperties::M;
        static constexpr TIndex HALF_POINT = TProperties::HALF_POINT;

    public:
        Fft() : wnp_cache(nullptr) {
            // Generate WNP cache
            wnp_cache = std::make_unique<TComplex[]>(N);
            for (TIndex P = static_cast<TIndex>(0); P < N; ++P) {
                TFloat angle = util::tau_v<TFloat> * static_cast<TFloat>(P) / static_cast<TFloat>(N);
                // e^(-jx) = cosx - j sinx
                wnp_cache[P] = TComplex(std::cos(angle), -std::sin(angle));
            }
        }

    private:
        std::unique_ptr<TComplex[]> wnp_cache;

    public:
        /**
		 * @brief Compute FFT for given complex sequence.
		 * @details
		 * This is FFT core compute function but not suit for common user
		 * because it order that you have enough FFT knowledge to understand what is input data and what is output data.
		 * For convenient use, see also easy_compute().
		 * @param[in,out] data 
		 * The complex sequence for computing.
		 * The length of this sequence must be N.
		*/
        void compute(TComplex* data) const {
            if (data == nullptr) [[unlikely]] {
                throw std::invalid_argument("nullptr data is not allowed for FFT computing.");
            }

            TIndex LH, J, K, B, P;
            LH = J = HALF_POINT;

            // Construct butterfly structure
            for (TIndex I = static_cast<TIndex>(1); I <= N - static_cast<TIndex>(2); ++I) {
                if (I < J) std::swap(data[I], data[J]);

                K = LH;
                while (J >= K) {
                    J -= K;
                    K >>= static_cast<TIndex>(1);
                }
                J += K;
            }

            // Calculate butterfly
            TComplex temp, temp2;
            for (TIndex L = static_cast<TIndex>(1); L <= M; ++L) {
                B = static_cast<TIndex>(1u) << (L - static_cast<TIndex>(1));
                for (J = static_cast<TIndex>(0); J <= B - static_cast<TIndex>(1); ++J) {
                    P = J * (static_cast<TIndex>(1) << (M - L));

                    // Use pre-computed cache instead of real-time computing
                    for (TIndex KK = J; KK <= N - static_cast<TIndex>(1); KK += (static_cast<TIndex>(1) << L)) {
                        temp2 = (data[KK + B] * this->wnp_cache[P]);
                        temp = temp2 + data[KK];
                        data[KK + B] = data[KK] - temp2;
                        data[KK] = temp;
                    }
                }
            }
        }
    };

    /**
     * @brief User friendly FFT computation class.
     * @details 
     * @tparam TIndex 
     * @tparam TFloat 
     * @tparam N 
     * @warning This class is \b NOT thread safe. Please use different instance in different thread.
     */
    template<typename TIndex, typename TFloat, TIndex VN>
        requires util::validate_args_v<TIndex, TFloat, VN>
    class FriendlyFft {
    private:
        using UnderlyingFft = Fft<TIndex, TFloat, VN>;
        using TProperties = FftProperties<TIndex, TFloat, VN>;
        using TComplex = TProperties::TComplex;
        static constexpr TIndex N = TProperties::N;
        static constexpr TIndex M = TProperties::M;
        static constexpr TIndex HALF_POINT = TProperties::HALF_POINT;

    public:
        FriendlyFft() : compute_cache(N) {
            // Initialize computation used buffer.
            compute_cache = std::vector<TComplex>();
        }

    private:
        UnderlyingFft underlying_fft;
        std::vector<TComplex> compute_cache;

    public:
        /**
		 * @brief Get the maximum frequency by given sample rate.
		 * @param[in] sample_rate 
		 * The sample rate of input stream.
		 * Unit is Hz or SPS (sample point per second).
		 * @return 
		 * The last data in computed FFT drequency data represented frequency.
		 * Unit is Hz.
		*/
        TFloat get_max_freq(TFloat sample_rate) {
            // Following sample priniciple
            return sample_rate / static_cast<TFloat>(2);
        }

        /**
		 * @brief Compute FFT for given time scope data.
		 * @details
		 * This is convenient FFT compute function, comparing with compute().
		 * This function accepts time scope data and output frequency scope data automatically.
		 * Additionally, it order a window function instance to apply to time scope data before computing.
		 * @param[in] time_scope The length of this data must be N.
		 * For the time order of data, the first data should be the oldest data and the last data should be the newest data.
		 * @param[out] freq_scope The length of this data must be N / 2.
		 * The first data is 0Hz and the frequency of last data is decided by sample rate which can be computed by get_max_freq() function in this class.
		 * @param[in] window The window instance applied to data.
		 * @warning
		 * This function is \b NOT thread-safe.
		 * Please do NOT call this function in different thread for one instance.
		*/
        void easy_compute(const TFloat* time_scope, TFloat* freq_scope, const Window<TIndex, TFloat, N>& window) {
            if (time_scope == nullptr || freq_scope == nullptr) [[unlikely]] {
                throw std::invalid_argument("nullptr data is not allowed for easy FFT computing.");
            }

            // First, we copy time scope data into cache with reversed order.
            // because FFT order the first item should be the latest data.
            // At the same time we multiple it with window function.
            std::generate(compute_cache.begin(),
                          compute_cache.end(),
                          [data = &(time_scope[N]), win_data = window.get_window_data()]() mutable -> TComplex {
                              return TComplex(*(data--) * *(win_data++));
                          });

            // Do FFT compute
            underlying_fft.compute(compute_cache.data());

            // Compute amplitude
            for (TIndex i = static_cast<TIndex>(0); i < HALF_POINT; ++i) {
                freq_scope[i] = static_cast<TFloat>(10) * std::log10(std::abs(compute_cache[i + HALF_POINT]));
            }
        }
    };

#pragma endregion

#pragma region Pre-defined FFT Types

    using Fft4F = Fft<size_t, float, 4u>;
    using Fft8F = Fft<size_t, float, 8u>;
    using Fft16F = Fft<size_t, float, 16u>;
    using Fft32F = Fft<size_t, float, 32u>;
    using Fft64F = Fft<size_t, float, 64u>;
    using Fft128F = Fft<size_t, float, 128u>;
    using Fft256F = Fft<size_t, float, 256u>;
    using Fft512F = Fft<size_t, float, 512u>;
    using Fft1024F = Fft<size_t, float, 1024u>;
    using Fft2048F = Fft<size_t, float, 2048u>;

    using Fft4 = Fft<size_t, double, 4u>;
    using Fft8 = Fft<size_t, double, 8u>;
    using Fft16 = Fft<size_t, double, 16u>;
    using Fft32 = Fft<size_t, double, 32u>;
    using Fft64 = Fft<size_t, double, 64u>;
    using Fft128 = Fft<size_t, double, 128u>;
    using Fft256 = Fft<size_t, double, 256u>;
    using Fft512 = Fft<size_t, double, 512u>;
    using Fft1024 = Fft<size_t, double, 1024u>;
    using Fft2048 = Fft<size_t, double, 2048u>;

#pragma endregion

} // namespace yycc::carton::fft
