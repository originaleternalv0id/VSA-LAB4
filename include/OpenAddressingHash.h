#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

enum class EntryStatus { ACTIVE, EMPTY, DELETED };
enum class ProbingMode { LINEAR, QUADRATIC };

struct HashEntry {
    string ip;
    EntryStatus status = EntryStatus::EMPTY;
};

class OpenAddressingHash {
public:
    explicit OpenAddressingHash(size_t initialCapacity = 11,
        ProbingMode mode = ProbingMode::LINEAR,
        uint64_t polynomialBase = 37, bool autoResize = true);

    bool insert(const string& ip);
    bool remove(const string& ip);
    bool contains(const string& ip) const;
    bool contains(const string& ip, int& probesOut) const;
    // Common comparison metric in addition to architecture-specific slot probes.
    bool contains(const string& ip, int& probesOut, int& keyComparisonsOut) const;

    size_t size() const;
    size_t capacity() const;
    double loadFactor() const;
    double effectiveOccupancy() const;
    size_t deletedCount() const;
    // ACTIVE and DELETED both occupy a cluster; circular boundary is included.
    size_t maxClusterLength() const;
    // ACTIVE encounters during the initial scan of successful user insertions.
    // Duplicates, failed inserts and internal rehash transfers do not count.
    uint64_t collisionCount() const;

private:
    struct ProbeResult {
        size_t index; // slots.size() means no reachable destination.
        bool found = false;
        int probes = 0;
        int keyComparisons = 0;
    };

    ProbeResult locate(const string& ip, const vector<HashEntry>& slots) const;
    void rehash(size_t newCapacity);

    vector<HashEntry> entries_;
    ProbingMode mode_;
    uint64_t polynomialBase_;
    bool autoResize_;
    size_t activeCount_ = 0;
    size_t deletedCount_ = 0;
    uint64_t collisionCount_ = 0;
};
