
#include <cmath>
#include <string>
#include <map>
#include <list>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cstdbool>

#include "str.cxx"
#include "function.cxx"

#ifndef CALC_H
#define CALC_H

std::map<char, std::string> gbl_vars;
std::list<std::string> history;

#endif

std::string calc(std::string, bool, std::map<char, std::string>&);

int run_commands(std::string&);

void replace_vars(std::string&, std::map<char, std::string>&);
int run_functions(std::string&);
void run_predef_fcns(std::string&);
void calc_brackets(std::string&);
void calc_exponents(std::string&);
void calc_multdiv(std::string&);
void calc_addsub(std::string&);

bool match_restriction(double, std::string&);

void remove_whitespace(std::string&);

double get_num_at(std::string&, int);

size_t get_n_start(std::string&, int);
size_t get_n_end(std::string&, int);

double get_num_before(std::string&, int);
double get_num_after(std::string&, int);
