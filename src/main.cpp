/*
 * Recursive_Fcn_Calc
 * Calculator app capable of calculating the result of recursive functions
 * Written by Quinn Neufeld
*/

#include "recursive_fcn_calc.cxx"

#include "setup.cxx"
#include "callbacks.cxx"

int main() {
    make_window();
    make_fcn_window();
    make_view_fcn_window();
    setup_user_data();
    set_callbacks();
    main_win->show();
    return Fl::run();
}
