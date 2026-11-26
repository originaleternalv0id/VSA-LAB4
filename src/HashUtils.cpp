#include "HashUtils.h"

#include <limits>
#include <stdexcept>

using namespace std;

namespace HashUtils {
uint64_t polynomialHash(const string& key, uint64_t base) {
    uint64_t hash = 0;
    for (unsigned char character : key) {
        // Unsigned overflow is defined modulo 2^64.
        hash = hash * base + character;
    }
    return hash;
}

size_t moduloCompress(uint64_t hash, size_t capacity) {
    if (capacity == 0) {
        throw invalid_argument("Capacity must be positive.");
    }
    return static_cast<size_t>(hash % capacity);
}

MadCompressor::MadCompressor(uint64_t a, uint64_t b, uint64_t p)
    : a_(a), b_(b), p_(p) {
    if (p < 2 || p > numeric_limits<uint32_t>::max() ||
        !isPrime(static_cast<size_t>(p))) {
        throw invalid_argument("MAD p must be a prime in the 32-bit range.");
    }
    if (a == 0 || a >= p || b >= p) {
        throw invalid_argument("MAD requires 1 <= a < p and 0 <= b < p.");
    }
}

size_t MadCompressor::compress(uint64_t hash, size_t capacity) const {
    if (capacity == 0 || capacity >= p_) {
        throw invalid_argument("MAD requires 0 < capacity < p.");
    }
    // With p <= 2^32-1, the largest intermediate is p*(p-1) < 2^64.
    const uint64_t reduced = hash % p_;
    return static_cast<size_t>(((a_ * reduced + b_) % p_) % capacity);
}

size_t madCompress(uint64_t hash, size_t capacity,
                        uint64_t a, uint64_t b, uint64_t p) {
    return MadCompressor(a, b, p).compress(hash, capacity);
}

bool isPrime(size_t number) {
    if (number < 2) {
        return false;
    }
    if (number % 2 == 0) {
        return number == 2;
    }
    // Division avoids the overflow that divisor*divisor could cause.
    for (size_t divisor = 3; divisor <= number / divisor; divisor += 2) {
        if (number % divisor == 0) {
            return false;
        }
    }
    return true;
}

size_t nextPrime(size_t number) {
    const auto maximum = numeric_limits<size_t>::max();
    if (number == maximum) {
        throw overflow_error("No representable larger prime.");
    }
    size_t candidate = number + 1;
    if (candidate <= 2) {
        return 2;
    }
    if (candidate % 2 == 0) {
        ++candidate;
    }
    while (!isPrime(candidate)) {
        if (candidate > maximum - 2) {
            throw overflow_error("No representable larger prime.");
        }
        candidate += 2;
    }
    return candidate;
}

size_t shrinkPrimeCapacity(size_t currentCapacity,
                                size_t minimumCapacity) {
    if (!isPrime(minimumCapacity) || currentCapacity < minimumCapacity) {
        throw invalid_argument("Invalid minimum or current capacity.");
    }
    const size_t half = currentCapacity / 2;
    if (half <= minimumCapacity) {
        return minimumCapacity;
    }
    size_t lower = half;
    while (!isPrime(lower)) {
        --lower; // The prime minimum is a guaranteed lower bound.
    }
    const size_t upperStart = half + currentCapacity % 2;
    const size_t upper = isPrime(upperStart) ? upperStart : nextPrime(upperStart);
    // Distances to the exact half, multiplied by two; no floating-point rounding.
    const size_t lowerDistance = currentCapacity - lower - lower;
    const size_t upperDistance = upper - (currentCapacity - upper);
    return lowerDistance <= upperDistance ? lower : upper;
}
}
