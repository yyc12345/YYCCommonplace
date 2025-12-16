#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/carton/fft.hpp>
#include <initializer_list>

#define FFT ::yycc::carton::fft

namespace yycctest::carton::fft {

    using TIndex = size_t;
    using TFloat = float;
    using TComplex = std::complex<TFloat>;
    template<TIndex N>
    using TFft = FFT::Fft<TIndex, TFloat, N>;

    // YYC MARK:
    // It seems that default epsilon can not fulfill our test (too small).
    constexpr TFloat TOLERANCE = static_cast<TFloat>(0.0003);
    //constexpr TFloat tolerance = std::numeric_limits<TFloat>::epsilon();

    template<TIndex N>
    static void test_fft(const std::vector<TFloat>& real_src, const std::vector<TComplex>& dst) {
        // check given data size
        ASSERT_EQ(real_src.size(), N);
        ASSERT_EQ(dst.size(), N);

        // convert real-number source into complex-number source
        std::vector<TComplex> src(real_src.size());
        std::generate(src.begin(), src.end(), [data = real_src.begin()]() mutable -> TComplex { return TComplex(*data++); });

        // create FFT instance and compute data
        TFft<N> fft;
        fft.compute(src.data());

        // check result with tolerance
        for (TIndex i = 0u; i < src.size(); ++i) {
            EXPECT_NEAR(src[i].real(), dst[i].real(), TOLERANCE);
            EXPECT_NEAR(src[i].imag(), dst[i].imag(), TOLERANCE);
        }
    }

    TEST(CartonFft, Test1) {
        std::vector<TFloat> src = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
        std::vector<TComplex> expected = {{+3.6000e+01f, +0.0000e+00f},
                                          {-4.0000e+00f, +9.6569e+00f},
                                          {-4.0000e+00f, +4.0000e+00f},
                                          {-4.0000e+00f, +1.6569e+00f},
                                          {-4.0000e+00f, +0.0000e+00f},
                                          {-4.0000e+00f, -1.6569e+00f},
                                          {-4.0000e+00f, -4.0000e+00f},
                                          {-4.0000e+00f, -9.6569e+00f}};
        test_fft<8>(src, expected);
    }

    TEST(CartonFft, Test2) {
        std::vector<TFloat> src = {6.0f, 1.0f, 7.0f, 2.0f, 7.0f, 4.0f, 8.0f, 7.0f};
        std::vector<TComplex> expected = {{+4.2000e+01f, +0.0000e+00f},
                                          {+4.1421e-01f, +6.6569e+00f},
                                          {-2.0000e+00f, +4.0000e+00f},
                                          {-2.4142e+00f, +4.6569e+00f},
                                          {+1.4000e+01f, +0.0000e+00f},
                                          {-2.4142e+00f, -4.6569e+00f},
                                          {-2.0000e+00f, -4.0000e+00f},
                                          {+4.1421e-01f, -6.6569e+00f}};
        test_fft<8>(src, expected);
    }

    TEST(CartonFft, Test3) {
        std::vector<TFloat> src = {1.0f, 2.0f, 3.0f, 4.0f};
        std::vector<TComplex> expected = {{+1.0000e+01f, +0.0000e+00f},
                                          {-2.0000e+00f, +2.0000e+00f},
                                          {-2.0000e+00f, +0.0000e+00f},
                                          {-2.0000e+00f, -2.0000e+00f}};
        test_fft<4>(src, expected);
    }

    TEST(CartonFft, Test4) {
        std::vector<TFloat> src = {6.0f, 1.0f, 7.0f, 2.0f};
        std::vector<TComplex> expected = {{+1.6000e+01f, +0.0000e+00f},
                                          {-1.0000e+00f, +1.0000e+00f},
                                          {+1.0000e+01f, +0.0000e+00f},
                                          {-1.0000e+00f, -1.0000e+00f}};
        test_fft<4>(src, expected);
    }

    TEST(CartonFft, Test5) {
        std::vector<TFloat> src = {4.0f, 4.0f, 4.0f, 4.0f};
        std::vector<TComplex> expected = {{+1.6000e+01f, +0.0000e+00f},
                                          {+0.0000e+00f, +0.0000e+00f},
                                          {+0.0000e+00f, +0.0000e+00f},
                                          {+0.0000e+00f, +0.0000e+00f}};
        test_fft<4>(src, expected);
    }

    TEST(CartonFft, Test6) {
        std::vector<TFloat> src = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
        std::vector<TComplex> expected = {{+1.3600e+02f, +0.0000e+00f},
                                          {-8.0000e+00f, +4.0219e+01f},
                                          {-8.0000e+00f, +1.9314e+01f},
                                          {-8.0000e+00f, +1.1973e+01f},
                                          {-8.0000e+00f, +8.0000e+00f},
                                          {-8.0000e+00f, +5.3454e+00f},
                                          {-8.0000e+00f, +3.3137e+00f},
                                          {-8.0000e+00f, +1.5913e+00f},
                                          {-8.0000e+00f, +0.0000e+00f},
                                          {-8.0000e+00f, -1.5913e+00f},
                                          {-8.0000e+00f, -3.3137e+00f},
                                          {-8.0000e+00f, -5.3454e+00f},
                                          {-8.0000e+00f, -8.0000e+00f},
                                          {-8.0000e+00f, -1.1973e+01f},
                                          {-8.0000e+00f, -1.9314e+01f},
                                          {-8.0000e+00f, -4.0219e+01f}};
        test_fft<16>(src, expected);
    }

} // namespace yycctest::carton::fft
