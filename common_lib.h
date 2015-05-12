#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <unordered_map>

// accepted params: start_size, steps, out_deg_half, alpha, beta, delta
struct TModelConfig {
    uint32_t NStartVertices;
    uint32_t NSteps;
    uint32_t OutDegree;

    double Alpha;
    double Beta;
    double Delta;

    void ParseFromFile(const std::string& fileName);
};

namespace NMath {

    inline double Power(double arg, uint32_t deg) {
        double res = 1.0;
        double curVal = arg;
        while (deg > 0) {
            if (deg & 1) {
                res *= curVal;
            }
            curVal *= curVal;
            deg >>= 1;
        }
        return res;
    }

    const uint32_t BIG_PRIME = 1000000007;

    class TCachedBinomialCalcer {
    public:
        typedef std::pair<uint32_t, uint32_t> TIntPair;

        struct TIntPairHasher {
            size_t operator ()(const TIntPair& pair) const {
                return pair.first + BIG_PRIME * pair.second;
            }
        };

    public:
        TCachedBinomialCalcer(size_t maxSize = 1000000)
            : MaxSize(maxSize)
        {
            Storage.reserve(MaxSize);
        }

    uint64_t Calc(uint32_t n, uint32_t k) {
        if (!k || n <= k) {
            return 1;
        }

        const auto it = Storage.find(TIntPair(n, k));
        if (it != Storage.end()) {
            return it->second;
        }

        auto result = Calc(n - 1, k - 1) + Calc(n - 1, k);
        if (Storage.size() >= MaxSize) {
            Storage.erase(Storage.begin());
        }
        Storage[TIntPair(n, k)] = result;
        return result;
    }

    private:
        size_t MaxSize;
        std::unordered_map<TIntPair, uint64_t, TIntPairHasher> Storage;
    };

} // NMath

