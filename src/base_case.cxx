/*

Source code for base case class

Written by Quinn Neufeld

*/

#ifndef BASE_CASE
#define BASE_CASE

#include "base_case.h"

class BaseCase {
    public:

    int err = OK;
    char fcn_name = 0;

    std::vector<var_val_t> var_vals;

    std::string val;

    BaseCase(std::string &s, std::vector<char> &fcn_vars) {
        remove_whitespace(s);
        parse_var_vals(s, fcn_vars);
        if (err != OK) {
            return;
        }
        parse_val_str(s);
        parse_funcname(s);
    }

    BaseCase() {
        err = NOT_DEFINED;
    }

    BaseCase(const BaseCase& bc) {
        err = bc.err;
        fcn_name = bc.fcn_name;
        var_vals = bc.var_vals;
        val = bc.val;
    }

    /*
    Returns true if a variable set matches the base case
    */
    bool matches(std::map<char, std::string> &vars) {
        for (var_val_t &vval: var_vals) {
            if (atof(calc(vars[vval.var], false, gbl_vars).c_str()) != atof(calc(vval.val, false, gbl_vars).c_str())) {
                return false;
            }
        }
        return true;
    }

    /*
    Returns the base case as a human readable string
    */
    std::string to_string() {
        std::stringstream ss;
        ss << fcn_name << '(';
        for (size_t i = 0; i < var_vals.size(); i++) {
            ss << var_vals[i].val;
            if (i + 1 < var_vals.size()) {
                ss << ", ";
            }
        }
        ss << ") = " << val;
        return ss.str();
    }

    private:

    //Parses out variable values
    void parse_var_vals(std::string &s, std::vector<char> &fcn_vars) {
        size_t b_pos = s.find('(');
        size_t e_pos = find_pair_idx(s, b_pos);
        if (b_pos == std::string::npos || e_pos == std::string::npos) {
            err = INVALID_FCN;
            return;
        }
        std::string cache = "";
        int var = 0;
        for (size_t i = b_pos + 1; i <= e_pos; i++) {
            char c = s[i];
            if (c == ',' || c == ')') {
                var_val_t vval;
                vval.val = cache;
                //vval.var = functions[fcn_name].fcn_vars[var];
                vval.var = fcn_vars[var];
                var_vals.push_back(vval);
                cache = "";
            }
            else {
                cache += c;
            }
        }
    }

    /*
    Parses out & calcs value string
    */
    void parse_val_str(std::string &s) {
        size_t eq_pos = s.find('=');
        if (eq_pos == std::string::npos) {
            err = NO_EQ;
            return;
        }
        val = s.substr(eq_pos + 1);
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

};

#endif
