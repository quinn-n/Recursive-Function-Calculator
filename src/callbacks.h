
#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <string>
#include <iostream>

#include <FL/Fl_Widget.H>

#include "calc.cxx"
#include "function.cxx"
#include "recursive_fcn_calc.cxx"
#include "user_data_types.cxx"

#endif

void set_callbacks();

void cb_close_main(Fl_Widget*);

void cb_calc(Fl_Widget*);

void cb_calc_preview(Fl_Widget*);

void cb_add_fcn(Fl_Widget*);
void cb_confirm_fcn(Fl_Widget*);
void cb_cancel_fcn(Fl_Widget*);

void cb_view_fcns(Fl_Widget*);
void cb_view_fcns_back(Fl_Widget*);

void cb_edit_fcn(Fl_Widget*);

void fill_view_fcns();
void clear_view_fcns();

void update_history();

void clear_fcn_win();
