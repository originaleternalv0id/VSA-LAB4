#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace HashUtils {
using namespace std;

inline constexpr uint64_t defaultMadPrime = 4294967291ULL;
inline constexpr uint64_t defaultMadA = 1664525;
inline constexpr uint64_t defaultMadB = 1013904223;

uint64_t polynomialHash(const string& key, uint64_t base = 37);
size_t moduloCompress(uint64_t hash, size_t capacity);

// Supported p range: prime 2 <= p <= UINT32_MAX.
// Validating once avoids repeating primality checks during table operations.
class MadCompressor {
public:
    MadCompressor(uint64_t a, uint64_t b, uint64_t p);
    size_t compress(uint64_t hash, size_t capacity) const;

private:
    uint64_t a_;
    uint64_t b_;
    uint64_t p_;
};

size_t madCompress(uint64_t hash, size_t capacity,
                        uint64_t a, uint64_t b, uint64_t p);
bool isPrime(size_t number);
size_t nextPrime(size_t number);

// Nearest prime to currentCapacity / 2.0; ties choose the smaller prime.
// minimumCapacity must be prime; result never goes below it.
size_t shrinkPrimeCapacity(size_t currentCapacity,
                                size_t minimumCapacity);
}
