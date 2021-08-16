
/*
 * str.cxx
 * Functions to help deal with strings for the recursive fcn calculator
*/

#ifndef STR
#define STR

#include "str.h"

/*
Removes whitespace from a string
*/
void remove_whitespace(std::string &s) {
    for (size_t i = 0; i < s.length(); i++) {
        if (isspace(s[i]))
            s.erase(i, 1);
    }
}

/*
Returns the index of a bracket's pair
Returns std::string::npos if no pair was found
*/
size_t find_pair_idx(std::string &statement, int p) {
    int lvl = 0;
    for (size_t i = p; i < statement.length(); i++) {
        char c = statement[i];
        if (c == '(') {
            lvl++;
        }
        else if (c == ')') {
            lvl--;
        }
        if (lvl <= 0)
            return i;
    }
    return std::string::npos;
}

#endif
