#include "BenchmarkUtils.h"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    try {
        if (argc > 2) {
            cerr << "Usage: benchmark [output-directory]\n";
            return 1;
        }
        const filesystem::path directory = argc == 2 ? argv[1] : "results";
        const auto keys = Benchmark::generateKeys();
        const auto missing = Benchmark::generateMissingKeys(1000, keys);
        vector<Benchmark::DistributionResult> polynomial, compression;
        for (const uint64_t base : {1ULL, 37ULL}) {
            const auto modulo = Benchmark::measureDistribution(
                keys, Benchmark::defaultCapacity, base, CompressionMode::MODULO);
            polynomial.push_back(modulo);
            compression.push_back(modulo);
            compression.push_back(Benchmark::measureDistribution(
                keys, Benchmark::defaultCapacity, base, CompressionMode::MAD));
        }
        vector<Benchmark::ArchitectureResult> architecture;
        for (double load : {0.25, 0.50, 0.75, 0.90}) {
            const auto rows = Benchmark::compareArchitectures(
                keys, missing, Benchmark::defaultCapacity, load);
            architecture.insert(architecture.end(), rows.begin(), rows.end());
        }
        filesystem::create_directories(directory);
        ofstream polynomialFile(directory / "polynomial_collisions.csv");
        ofstream compressionFile(directory / "compression_comparison.csv");
        ofstream loadFile(directory / "load_factor_comparison.csv");
        Benchmark::writePolynomialCsv(polynomialFile, polynomial);
        Benchmark::writeCompressionCsv(compressionFile, compression);
        Benchmark::writeLoadCsv(loadFile, architecture);
        polynomialFile.close(); compressionFile.close(); loadFile.close();
        if (!polynomialFile || !compressionFile || !loadFile) {
            throw runtime_error("Could not finish writing CSV files.");
        }
        cout << "Dataset: " << keys.size() << " unique ISBN-like keys; seed=42; N=10007\n";
        for (const auto& row : compression) {
            cout << "base=" << row.base << ' ' << row.compression
                      << " raw collisions=" << row.rawCollisions
                      << " index collisions=" << row.indexCollisions
                      << " occupied=" << row.occupiedBuckets
                      << " max bucket=" << row.maxBucketOccupancy << '\n';
        }
        cout << fixed << setprecision(4);
        for (const auto& row : architecture) {
            cout << row.architecture << " target=" << row.targetLoad
                      << " actual=" << row.actualLoad << " inserted=" << row.successful
                      << '/' << row.attempted << " failed=" << row.failed
                      << " achieved=" << (row.targetAchieved ? "yes" : "no")
                      << " max=" << row.maxLength
                      << " hit probes=" << Benchmark::average(row.successSearch.probes, row.successSearch.queries)
                      << " miss probes=" << Benchmark::average(row.missingSearch.probes, row.missingSearch.queries)
                      << '\n';
        }
        cout << "Chaining probes = key comparisons; OA probes = inspected slots.\n"
                     "Common-key comparisons and totals are included in CSV.\n"
                  << "CSV output: " << directory.string() << '\n';
        return 0;
    } catch (const exception& error) {
        cerr << "Benchmark error: " << error.what() << '\n';
        return 1;
    }
}
