/*

Helper functions for recursive function calculator

*/

#ifndef CALC
#define CALC

#include "calc.h"

//BEDMAS

/*
Calculates result from string
*/
std::string calc(std::string statement, bool cmd_enabled=false, std::map<char, std::string> &vars=gbl_vars) {

    std::cout << "Calculating result of statement " << statement << std::endl;

    remove_whitespace(statement);

    if (cmd_enabled) {
        int res = run_commands(statement);
        if (res)
            //return statement;
            return "";
    }

    replace_vars(statement, vars);
    std::cout << "Statement with vars replaced: " << statement << std::endl;
    if (run_functions(statement)) {
        std::cout << "Got error on run_functions\n";
        return "";
    }
    std::cout << "Statement with fcns replaced: " << statement << std::endl;
    calc_brackets(statement);
    run_predef_fcns(statement);
    calc_exponents(statement);
    calc_multdiv(statement);
    calc_addsub(statement);
    return statement;
}

/*
Runs commands in the statement
Returns 0 if no commands are found. 
Returns non-zero value if calc should be stopped.
*/
int run_commands(std::string &statement) {
    for (size_t i = 1; i < statement.length(); i++) {
        char c = statement[i];

        switch (c) {
        case '=': {
            char vname = statement[i - 1];
            if (functions.count(vname)) {
                functions.erase(vname);
            }
            std::string vval = statement.substr(i + 1);
            if (vval.find(vname) != std::string::npos) {
                std::cout << "Variable set to itself. Contents not saved." << std::endl;
                return 1;
            }
            gbl_vars[vname] = vval;
            std::cout << "Got var: " << vname << ", value: " << vval << std::endl;
            return 1;
            break;
        }
        
        default:
            break;
        }
    }
    return 0;
}

/*
Replaces variables with their values
*/
void replace_vars(std::string &statement, std::map<char, std::string> &vars=gbl_vars) {
    for (size_t i = 0; i < statement.length(); i++) {
        char c = statement[i];
        if (vars.count(c)) {
            std::string rep = calc(vars[c]);
            statement.replace(i, 1, rep);
        }
    }
}

/*
Runs functions in statement and replaces them with their result
Returns 0 on a success
Returns 1 if the function is invalid
Returns 2 if a variable fails a restriction
*/
int run_functions(std::string &statement) {
    for (size_t i = 0; i < statement.length(); i++) {
        char c = statement[i];
        /*
        TODO: Consider adding limit to recusion depth
        */
        if (functions.count(c)) {
            std::map<char, std::string> fcn_vars;
            size_t b_pos = statement.find('(', i);
            size_t e_pos = find_pair_idx(statement, b_pos);
            //Stop running if an unfinished function is found
            if (e_pos == std::string::npos) {
                return 1;
            }
            std::string varcache = "";
            Function* fcn = &functions[c];
            size_t v = 0;
            for (size_t j = b_pos + 1; j < e_pos; j++) {
                if (statement[j] == ',') {
                    if (v >= fcn->fcn_vars.size()) {
                        std::cout << "Too many arguments for function " << c << '\n'; 
                        break;
                    }
                    char var = fcn->fcn_vars[v];
                    fcn_vars[var] = varcache;
                    std::cout << "Got var " << var << ": " << varcache << '\n';
                    v++;
                    varcache = "";
                    continue;
                }
                varcache += statement[j];
            }
            //Add last variable if it exists
            if (fcn->fcn_vars.size()) {
                if (v >= fcn->fcn_vars.size()) {
                    std::cout << "Too many arguments for function " << c << '\n';
                    break;
                }
                char var = fcn->fcn_vars[v];
                fcn_vars[var] = varcache;
                std::cout << "Got var " << var << ": " << varcache << '\n';
            }

            //Check variables against restrictions
            for (const auto &var: fcn_vars) {
                if (!fcn->check_restrictions(var.first, var.second)) {
                    std::cout << "Variable " << var.first << " with value " << var.second << " fails restriction." << '\n';
                    return 2;
                }
            }
            
            //Check for base cases
            int case_idx = fcn->get_matching_case(fcn_vars);
            std::string val;
            if (case_idx > -1) {
                std::cout << "Got matching base case at index " << case_idx << '\n';
                val = calc(fcn->base_cases[case_idx].val);
            } else {
                std::cout << "Running function " << fcn->fcn_name << ", fcn: " << fcn->fcn << '\n';
                val = calc(fcn->fcn, false, fcn_vars);
            }
            std::cout << "Running function " << fcn->fcn_name << ", fcn: " << fcn->fcn << std::endl;
            //val = calc(fcn->fcn, false, fcn_vars);

            std::cout << "Got result of function " << fcn->fcn_name << ": " << val << std::endl;
            statement.replace(i, (e_pos - i) + 1, val);
            i += (e_pos - i);
        }
    }
    return 0;
}

/*
Calculates the values for & replaces all brackets in the statement
eg. 3(7 * 2) -> 3*14
*/
void calc_brackets(std::string &statement) {
    for (size_t i = 0; i < statement.length(); i++) {
        char c = statement[i];

        if (c != '(')
            continue;

        size_t pair_idx = find_pair_idx(statement, i);
        if (pair_idx == std::string::npos)
            pair_idx = statement.length();

        size_t len = (pair_idx - i) + 1;
        std::string bracket_str = statement.substr(i + 1, len - 2);
        std::string result = calc(bracket_str);
        statement.replace(i, len, result);
        i += result.length();
    }
}

/*
Runs predefined functions like factorials, combinations, permutations, etc.
*/
void run_predef_fcns(std::string &statement) {
    for (size_t i = 0; i < statement.length(); i++) {
        char c = statement[i];
        if (c == '!') {
            unsigned long int s = 1;
            for (int n = floor(get_num_before(statement, i)); n > 0; n--) {
                s *= n;
            }
            std::stringstream ss;
            ss << std::fixed << s;
            size_t s_pos = get_n_start(statement, i - 1);
            if (s_pos == std::string::npos) {
                s_pos = 0;
            }
            statement.replace(s_pos, (i - s_pos) + 1, ss.str());
        }
    }
}

/*
Calculates values for all exponents
*/
void calc_exponents(std::string &statement) {
    for (size_t i = 1; i < statement.length(); i++) {
        char c = statement[i];

        if (c != '^')
            continue;
        
        double n = pow(get_num_before(statement, i), get_num_after(statement, i));
        std::stringstream ss;
        ss << std::fixed << n;

        //Replace exponent with result
        int start = get_n_start(statement, i - 1);
        int end = get_n_end(statement, i + 1);
        statement.replace(start, (end - start) + 1, ss.str());
    }
}

/*
Calculates multiplication and division
*/
void calc_multdiv(std::string &statement) {
    for (size_t i = 1; i < statement.length(); i++) {
        char c = statement[i];

        if (c != '*' && c != '/')
            continue;
        
        double n1 = get_num_at(statement, i - 1);
        double n2 = get_num_at(statement, i + 1);
        double result;
        
        if (c == '*')
            result = n1 * n2;
        else if (c == '/')
            result = n1 / n2;
        
        std::cout << "Got multdiv, n1: " << n1 << ", n2: " << n2 << ", result: " << result << std::endl;
        
        std::stringstream ss;
        ss << std::fixed << result;

        //Replace mult / div with result
        int s = get_n_start(statement, i - 1);
        int e = get_n_end(statement, i + 1);
        statement.replace(s, (e - s) + 1, ss.str());
    }
}

/*
Calculates addition and subtraction
*/
void calc_addsub(std::string &statement) {
    for (size_t i = 0; i < statement.length(); i++) {
        char c = statement[i];

        if (c != '+' && c != '-')
            continue;
        
        double n1 = get_num_at(statement, i - 1);
        double n2 = get_num_at(statement, i + 1);
        std::cout << "Got addsub at " << i << ", n1: " << n1 << ", n2: " << n2 << '\n';
        std::cout << "addsub str: " << statement << '\n';
        double result;
        
        if (c == '+')
            result = n1 + n2;
        else if (c == '-')
            result = n1 - n2;
        
        std::stringstream ss;
        ss << std::fixed << result;

        //Replace add / sub with result
        size_t s = get_n_start(statement, i - 1);
        size_t e = get_n_end(statement, i + 1);
        if (s == std::string::npos) {
            s = 0;
        }
        if (e == std::string::npos) {
            e = statement.length();
        }
        statement.replace(s, (e - s) + 1, ss.str());
        i = s;
    }
}

/*
Returns the value of the number occupying index p
Returns 0 if no number is found
*/
double get_num_at(std::string &statement, int p) {
    size_t s = get_n_start(statement, p);
    if (s == std::string::npos) {
        return 0;
    }
    return atof(statement.c_str() + s);
}

//Returns the number before p in statement
double get_num_before(std::string &statement, int p) {
    p--;
    for (int i = p; i > 0; i--) {
        char c = statement[i];

        if (isdigit(c) || isspace(c) || c == '.')
            continue;
        
        i++;
        return atof(statement.c_str() + i);
    }
    return atof(statement.c_str());
}

//Returns the number after p in statement
double get_num_after(std::string &statement, int p) {
    p++;
    return atof(statement.c_str() + p);
}

//Returns start pos of number at p
size_t get_n_start(std::string &statement, int p) {
    if (!isdigit(statement[p]) && !isspace(statement[p]) && statement[p] != '.') {
        return std::string::npos;
    }
    for (int i = p; i >= 0; i--) {
        char c = statement[i];

        if (isdigit(c) || isspace(c) || c == '.')
            continue;
        return i + 1;
    }
    return 0;
}

//Returns end pos of number at p
size_t get_n_end(std::string &statement, int p) {
    if (!isdigit(statement[p]) && !isspace(statement[p]) && statement[p] != '.') {
        return std::string::npos;
    }
    for (size_t i = p; i < statement.length(); i++) {
        char c = statement[i];

        if (isdigit(c) || isspace(c) || c == '.')
            continue;
        return i - 1;
    }
    return statement.length();
}

#endif
