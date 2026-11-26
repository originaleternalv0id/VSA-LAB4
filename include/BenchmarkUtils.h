#pragma once

#include "SeparateChainingHash.h"

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace Benchmark {
using namespace std;

inline constexpr uint32_t defaultSeed = 42;
inline constexpr size_t defaultCapacity = 10007;
inline constexpr size_t defaultDatasetSize = 10000;

struct DistributionResult {
    size_t datasetSize = 0;
    uint32_t seed = defaultSeed;
    uint64_t base = 37;
    size_t capacity = 0;
    string compression;
    uint64_t madA = 0, madB = 0, madP = 0;
    size_t uniqueHashes = 0, rawCollisions = 0;
    size_t occupiedBuckets = 0, indexCollisions = 0, maxBucketOccupancy = 0;
};

struct SearchTotals {
    size_t queries = 0;
    uint64_t probes = 0;
    uint64_t keyComparisons = 0;
    uint64_t accesses = 0; // Initial bucket accesses (chaining) or slot inspections (OA).
};

struct ArchitectureResult {
    string architecture;
    size_t datasetSize = 0, capacity = 0;
    uint32_t seed = defaultSeed;
    uint64_t base = 37;
    double targetLoad = 0, actualLoad = 0;
    size_t targetCount = 0, attempted = 0, successful = 0, failed = 0;
    bool targetAchieved = false;
    size_t maxLength = 0, deletedSlots = 0;
    uint64_t insertionCollisions = 0;
    string lengthMetric, collisionMetric, probeMetric;
    SearchTotals successSearch, missingSearch, commonSearch;
};

vector<string> generateKeys(size_t count = defaultDatasetSize,
                                      uint32_t seed = defaultSeed);
vector<string> generateMissingKeys(size_t count,
                                            const vector<string>& dataset);
size_t targetElementCount(size_t capacity, double targetLoad);
double average(uint64_t total, size_t count);

DistributionResult measureDistribution(const vector<string>& keys,
    size_t capacity, uint64_t base, CompressionMode mode,
    uint32_t seed = defaultSeed);

vector<ArchitectureResult> compareArchitectures(
    const vector<string>& keys, const vector<string>& missingKeys,
    size_t capacity, double targetLoad, uint32_t seed = defaultSeed,
    uint64_t base = 37);

void writePolynomialCsv(ostream& output, const vector<DistributionResult>& rows);
void writeCompressionCsv(ostream& output, const vector<DistributionResult>& rows);
void writeLoadCsv(ostream& output, const vector<ArchitectureResult>& rows);
}
