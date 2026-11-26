#include "SeparateChainingHash.h"
#include "OpenAddressingHash.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace {
bool readChoice(const string& prompt, int minimum, int maximum, int& value) {
    string line;
    while (cout << prompt, getline(cin, line)) {
        istringstream input(line);
        int candidate = 0;
        char extra = '\0';
        if ((input >> candidate) && !(input >> extra) &&
            candidate >= minimum && candidate <= maximum) {
            value = candidate;
            return true;
        }
        cout << "Invalid input. Enter a number from " << minimum
                  << " to " << maximum << ".\n";
    }
    return false;
}

void printBook(const Book& book) {
    cout << "ISBN: " << book.isbn << "\nTitle: " << book.title
              << "\nAuthor: " << book.author << "\nAvailable: "
              << (book.isAvailable ? "yes" : "no") << '\n';
}

void printStatistics(const SeparateChainingHash& table) {
    cout << "Books: " << table.size() << "\nCapacity: " << table.capacity()
              << "\nLoad factor: " << table.loadFactor()
              << "\nInsertion collisions: " << table.collisionCount()
              << "\nMaximum chain length: " << table.maxChainLength() << '\n';
}

bool readBook(Book& book) {
    cout << "ISBN: ";
    if (!getline(cin, book.isbn)) return false;
    cout << "Title: ";
    if (!getline(cin, book.title)) return false;
    cout << "Author: ";
    if (!getline(cin, book.author)) return false;
    int available = 0;
    if (!readChoice("Available (1=yes, 0=no): ", 0, 1, available)) return false;
    book.isAvailable = available == 1;
    return true;
}

void runLibrary() {
    int mode = 0;
    if (!readChoice("Compression (1=Modulo, 2=MAD): ", 1, 2, mode)) return;
    SeparateChainingHash table(11,
        mode == 1 ? CompressionMode::MODULO : CompressionMode::MAD);
    for (;;) {
        cout << "\n1. Add book\n2. Remove book\n3. Search by ISBN"
                     "\n4. Statistics\n0. Return to main menu (end session)\n";
        int choice = 0;
        if (!readChoice("Choice: ", 0, 4, choice) || choice == 0) return;
        if (choice == 1) {
            Book book;
            if (!readBook(book)) return;
            cout << (table.insert(book) ? "Book added.\n" : "Duplicate ISBN.\n");
        } else if (choice == 2 || choice == 3) {
            string isbn;
            cout << "ISBN: ";
            if (!getline(cin, isbn)) return;
            if (choice == 2) {
                cout << (table.remove(isbn) ? "Book removed.\n" : "Not found.\n");
            } else {
                int probes = 0;
                const Book* book = table.findByISBN(isbn, probes);
                if (book) printBook(*book);
                else cout << "Not found.\n";
                cout << "ISBN comparisons: " << probes << '\n';
            }
        } else {
            printStatistics(table);
        }
    }
}

void printFirewallStatistics(const OpenAddressingHash& table) {
    cout << "Active IPs: " << table.size() << "\nCapacity: " << table.capacity()
              << "\nActive load factor: " << table.loadFactor()
              << "\nEffective occupancy: " << table.effectiveOccupancy()
              << "\nDeleted slots: " << table.deletedCount()
              << "\nInsertion ACTIVE encounters: " << table.collisionCount()
              << "\nMaximum circular cluster (ACTIVE + DELETED): " << table.maxClusterLength() << '\n';
}

void runFirewall() {
    int mode = 0;
    if (!readChoice("Probing (1=Linear, 2=Quadratic): ", 1, 2, mode)) return;
    OpenAddressingHash table(11,
        mode == 1 ? ProbingMode::LINEAR : ProbingMode::QUADRATIC);
    cout << "Simulation only. Keys are stored as entered; no IP format validation.\n";
    for (;;) {
        cout << "\n1. Block IP\n2. Unblock IP\n3. Search IP"
                     "\n4. Statistics\n0. Return to main menu (end session)\n";
        int choice = 0;
        if (!readChoice("Choice: ", 0, 4, choice) || choice == 0) return;
        if (choice == 4) {
            printFirewallStatistics(table);
            continue;
        }
        string ip;
        cout << "IP: ";
        if (!getline(cin, ip)) return;
        if (choice == 1) {
            cout << (table.insert(ip) ? "IP blocked.\n" : "Duplicate or no reachable slot.\n");
        } else if (choice == 2) {
            cout << (table.remove(ip) ? "IP unblocked.\n" : "Not found.\n");
        } else {
            int probes = 0, comparisons = 0;
            cout << (table.contains(ip, probes, comparisons) ? "Blacklisted.\n" : "Not found.\n");
            cout << "Slot probes: " << probes << "\nKey comparisons: " << comparisons << '\n';
        }
    }
}
}

int main() {
    try {
        for (;;) {
            cout << "\n1. Library catalogue\n2. Firewall blacklist\n0. Exit\n";
            int system = 0;
            if (!readChoice("System: ", 0, 2, system) || system == 0) return 0;
            if (system == 1) runLibrary();
            else runFirewall();
        }
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
