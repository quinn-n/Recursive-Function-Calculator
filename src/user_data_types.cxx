/*

user_data_types.cxx

Datatypes for user_data in recursive calc widget user_data
Written by Quinn Neufeld
*/

#ifndef USER_DATA_TYPES
#define USER_DATA_TYPES

struct fcn_btn_t {
    char fcn_name;
    char* label;
};

struct win_data_t {
    Fl_Widget* source;
};

#endif
