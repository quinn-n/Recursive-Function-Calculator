

#ifndef INPUT_RESTRICTION
#define INPUT_RESTRICTION

#include "inputrestriction.h"

/*
Class to manage a single input restriction
*/
class InputRestriction {
    public:
    int err;

    char var;
    std::string types;
    std::string val_rest;

    InputRestriction(std::string &s) {
        remove_whitespace(s);
        size_t var_idx = s.find_first_not_of(VAL_CHARS);
        if (var_idx == std::string::npos) {
            err = NO_VAR;
            return;
        }

        var = s[var_idx];
        std::cout << "Got restriction var: " << var << std::endl;
        size_t type_s = s.find_first_of(VAL_CHARS);
        if (type_s == std::string::npos) {
            err = NO_EQ;
            return;
        }
        size_t type_e = s.find_first_not_of(VAL_CHARS, type_s);
        if (type_e == std::string::npos) {
            err = NO_EQ;
            return;
        }
        types = s.substr(type_s, (type_e - type_s));
        std::cout << "Got restriction types: " << types << std::endl;
        val_rest = s.substr(type_e);
        std::cout << "Got restriction value: " << val_rest << std::endl;

        std::cout << "Got restriction: " << var << types << val_rest << std::endl;
    }

    InputRestriction(const InputRestriction& ir) {
        var = ir.var;
        types = ir.types;
        val_rest = ir.val_rest;
    }

    InputRestriction() {
        var = 0;
        types = "";
        val_rest = "";
    }

    /*
    Returns true if a given value for variable v matches its restriction
    Returns true if v does not match internal variable
    */
    bool matches_restriction(char v, const std::string &val) {
        if (v != var)
            return true;
        double val_d = atof(calc(val, false, gbl_vars).c_str());
        std::cout << "Got val_d: " << val_d << '\n';
        double rest_d = atof(calc(val_rest, false, gbl_vars).c_str());
        std::cout << "Got rest_d: " << rest_d << '\n';
        bool n = false;
        for (char c: types) {
            switch (c) {
                case '>':
                    if (n) {
                        if (val_d <= rest_d)
                            return true;
                    } else {
                        if (val_d > rest_d)
                            return true;
                        break;
                    }
                case '<':
                    if (n) {
                        if (val_d >= rest_d)
                            return true;
                    } else {
                        if (val_d < rest_d)
                            return true;
                        break;
                    }
                case '=':
                    if (n) {
                        if (val_d != rest_d)
                            return true;
                    } else {
                        if (val_d == rest_d)
                            return true;
                        break;
                    }
                case '!':
                    n = true;
                    break;
            }
        }
        return false;
    }

    void operator=(InputRestriction *other) {
        var = other->var;
        types = other->types;
        val_rest = other->val_rest;
    }

    /*
    Returns the input restriction as a human-readable string
    */
    std::string to_string() {
        std::stringstream ss;
        ss << var << types << val_rest;
        return ss.str();
    }
};

#endif
