#include <benchmark/benchmark.h>
#include <yycc.hpp>
#include <yycc/carton/fft.hpp>
#include <random>
#include <chrono>

#define FFT ::yycc::carton::fft

namespace yyccbench::carton::fft {

    using TIndex = size_t;
    using TFloat = float;
    using TComplex = std::complex<TFloat>;
    template<TIndex N>
    using TFft = FFT::Fft<TIndex, TFloat, N>;

    constexpr TIndex FFT_POINTS = 1024u;

    static void BM_FftCompute(benchmark::State& state) {
        // prepare random buffer
        constexpr TIndex RND_BUF_CNT = 8u;
        std::random_device rnd_device;
        std::default_random_engine rnd_engine(rnd_device());
        std::uniform_real_distribution<TFloat> rnd_dist(0.0f, 1.0f);
        std::vector<std::vector<TComplex>> buffer_collection(RND_BUF_CNT);
        for (auto& buf : buffer_collection) {
            buf.resize(FFT_POINTS);
            std::generate(buf.begin(), buf.end(), [&rnd_engine, &rnd_dist]() mutable -> TComplex { return TComplex(rnd_dist(rnd_engine)); });
        }

        // prepare FFT engine
        TFft<FFT_POINTS> fft;
        // do benchmark
        for (auto _ : state) {
            fft.compute(buffer_collection[state.iterations() % RND_BUF_CNT].data());
        }
    }
    BENCHMARK(BM_FftCompute)->Name("FftCompute");

}
