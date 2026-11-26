#pragma once

#include <string>

using namespace std;

// ISBN is a textual identifier: leading zeroes and separators are preserved.
struct Book {
    string isbn;
    string title;
    string author;
    bool isAvailable;
};
