#pragma once

#include <cctype>
#include <string>

inline
void toUppercase(std::string &str)
{
    for (auto &c: str) {
        c = toupper(c);
    }
}

inline
void toLowercase(std::string &str)
{
    for (auto &c: str) {
        c = tolower(c);
    }
}
