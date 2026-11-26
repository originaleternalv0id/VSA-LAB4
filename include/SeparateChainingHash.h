#pragma once

#include "Book.h"
#include "HashUtils.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

using namespace std;

enum class CompressionMode {
    MODULO,
    MAD
};

class SeparateChainingHash {
public:
    explicit SeparateChainingHash(size_t initialCapacity = 11,
        CompressionMode mode = CompressionMode::MODULO,
        uint64_t polynomialBase = 37, bool autoResize = true);

    bool insert(const Book& b);
    bool remove(const string& isbn);

    // Pointers are invalidated by removal of that book, rehash, assignment,
    // or destruction of the table. Reacquire after any mutation.
    const Book* findByISBN(const string& isbn) const;
    // A probe is one comparison against a stored ISBN; empty chains cost zero.
    const Book* findByISBN(const string& isbn, int& probesOut) const;

    double loadFactor() const;
    size_t size() const;
    size_t capacity() const;
    size_t maxChainLength() const;
    uint64_t collisionCount() const;

private:
    size_t bucketIndex(const string& isbn, size_t targetCapacity) const;
    void rehash(size_t newCapacity);

    vector<list<Book>> buckets_;
    size_t minimumCapacity_;
    CompressionMode mode_;
    uint64_t polynomialBase_;
    bool autoResize_;
    size_t elementCount_ = 0;
    uint64_t collisionCount_ = 0;
    HashUtils::MadCompressor mad_;
};
