/*
Header file for base case class
Written by Quinn Neufeld
*/

#ifndef BASE_CASE_H
#define BASE_CASE_H

#include <string>
#include <vector>
#include <cstdbool>
#include <cstdlib>

#include "str.cxx"
#include "err.h"

#include "calc.cxx"

struct var_val_t {
    char var;
    std::string val;
};

#endif
