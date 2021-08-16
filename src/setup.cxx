/*
setup.cxx

setup functions for recursive function calculator

writen by Quinn Neufeld
*/

#ifndef SETUP
#define SETUP

#include "setup.h"

/*
Prepares user data in widgets for use elsewhere
*/
void setup_user_data() {
    win_data_t* fcn_win_data = new win_data_t;
    add_fcn_win->user_data(fcn_win_data);
    win_data_t* view_fcn_data = new win_data_t;
    view_fcn_win->user_data(view_fcn_data);
}

#endif
