
/*
 * function.cxx
 * Function-managing code for the recursive function calculator
 * written by Quinn Neufeld
*/

#ifndef FUNCTION
#define FUNCTION

#include "function.h"

class Function {
    public:
    int err = OK;
    char fcn_name;

    std::string fcn;
    std::vector<char> fcn_vars;
    std::vector<InputRestriction> input_restrictions;
    std::vector<BaseCase> base_cases;

    Function(std::string &fcn_input, std::string &base_cases, std::string &input_restrictions) {
        remove_whitespace(fcn_input);
        parse_vars(fcn_input);
        if (err != OK) {
            return;
        }
        parse_funcname(fcn_input);
        if (err != OK) {
            return;
        }
        parse_fcn(fcn_input);
        if (err != OK) {
            return;
        }
        parse_restrictions(input_restrictions);
        if (err != OK) {
            return;
        }
        parse_base_cases(base_cases);
        std::cout << "Got funcname: " << fcn_name << std::endl;
        std::cout << "Got funcstr: " << fcn << std::endl;
        for (char c: fcn_vars) {
            std::cout << "Got function var: " << c << std::endl;
        }
        std::cout << "Got " << input_restrictions.size() << " restrictions." << std::endl;
        std::cout << "Got " << base_cases.size() << " base cases." << std::endl;
    }

    Function(const Function& fcn) {
        fcn_name = fcn.fcn_name;
        this->fcn = fcn.fcn;
        fcn_vars = fcn.fcn_vars;
        input_restrictions = fcn.input_restrictions;
        err = fcn.err;
    }

    Function() {
        err = FCN_NOT_DEFINED;
    }

    void operator=(Function* other) {
        fcn_name = other->fcn_name;
        fcn = other->fcn;
        fcn_vars = other->fcn_vars;
        input_restrictions = other->input_restrictions;
        err = other->err;
    }

    /*
    Checks variable against restrictions.
    Returns true if it matches all restrictions,
    returns false otherwise.
    */
    bool check_restrictions(char v, const std::string &val) {
        for (size_t i = 0; i < input_restrictions.size(); i++) {
            InputRestriction* ir = &input_restrictions[i];
            if (!ir->matches_restriction(v, val)) {
                return false;
            }
        }
        return true;
    }

    /*
    Converts function to a human-readable string
    */
    std::string to_string() {
        std::stringstream ss;
        ss << fcn_name << '(';
        for (size_t i = 0; i < fcn_vars.size(); i++) {
            ss << fcn_vars[i];
            if (i < fcn_vars.size() - 1)
                ss << ',';
        }
        ss << ") = " << fcn;
        return ss.str();
    }

    /*
    Returns index of matching base case, or -1 if not found
    */
    int get_matching_case(std::map<char, std::string> &vars) {
        for (size_t i = 0; i < base_cases.size(); i++) {
            if (base_cases[i].err != OK) {
                continue;
            }
            if (base_cases[i].matches(vars)) {
                return i;
            }
        }
        return -1;
    }

    private:

    //Parses & stores function gbl_vars
    void parse_vars(std::string &raw_fcn) {
        size_t b_pos = raw_fcn.find('(');
        size_t eq_pos = raw_fcn.find('=');
        //Check that the function is valid
        if (b_pos == std::string::npos) {
            std::cout << "Invalid function: " << raw_fcn << std::endl;
            std::cout << "Got no args." << std::endl;
            err = NO_ARGS;
            return;
        }
        if (b_pos > eq_pos || eq_pos == std::string::npos) {
            std::cout << "Invalid function: " << raw_fcn << std::endl;
            std::cout << "Got no args." << std::endl;
            err = NO_ARGS;
            return;
        }
        size_t close_pos = find_pair_idx(raw_fcn, b_pos);
        if (close_pos == std::string::npos) {
            err = INVALID_FCN;
            return;
        }

        if (close_pos > eq_pos)
            close_pos = eq_pos;
        std::string arg_str = raw_fcn.substr(b_pos + 1, (close_pos - b_pos) - 1);
        std::cout << "Got arg str: " << arg_str << std::endl;
        parse_argstr(arg_str);
    }

    /*
    Parses out function name
    */
    void parse_funcname(std::string &raw_fcn) {
        size_t b_pos = raw_fcn.find('(');
        for (size_t i = b_pos; i >= 0; i--) {
            if (isalpha(raw_fcn[i])) {
                fcn_name = raw_fcn[i];
                return;
            }
        }
    }

    /*
    Parses out function string
    eg. f(x)=x^3 -> x^3
    */
    void parse_fcn(std::string &raw_fcn) {
        size_t eq_pos = raw_fcn.find('=');
        if (eq_pos == std::string::npos) {
            std::cout << "Invalid function " << raw_fcn << '\n';
            std::cout << "Got no eq" << '\n';
            err = NO_EQ;
            return;
        }
        fcn = raw_fcn.substr(eq_pos + 1);
    }

    /*
    Separates restrictions by newlines
    */
    void parse_restrictions(std::string &raw_restrictions) {
        std::string cachestr = "";
        for (char c: raw_restrictions) {
            if (c == '\n' && cachestr != "") {
                std::cout << "Got input restriction: " << cachestr << '\n';
                remove_whitespace(cachestr);
                InputRestriction ir(cachestr);
                if (ir.err == OK) {
                    input_restrictions.push_back(ir);
                }
                else {
                    std::cout << "Got error with restriction " << cachestr << '\n';
                }

                cachestr = "";
                continue;
            }
            cachestr += c;
        }
        if (cachestr != "") {
            input_restrictions.push_back(InputRestriction(cachestr));
        }
    }

    /*
    Parses base cases from string separated by newlines
    */
    void parse_base_cases(std::string &raw_base_cases) {
        std::string cache = "";
        for (char c: raw_base_cases) {
            if (c == '\n') {
                if (cache == "") {
                    continue;
                }
                base_cases.push_back(BaseCase(cache, fcn_vars));
                std::cout << "Got base case " << cache << '\n';
                cache = "";
                continue;
            }
            cache += c;
        }
        if (cache != "") {
            base_cases.push_back(BaseCase(cache, fcn_vars));
        }
    }

    /*
    Parses function args from argstr & stores them in fcn_vars
    */
    void parse_argstr(std::string &s) {
        for (char c: s) {
            if (isalpha(c)) {
                fcn_vars.push_back(c);
            }
        }
    }
};

std::map<char, Function> functions;

#endif
