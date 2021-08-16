/*

Callback function definitions for recursive function calculator

Written by Quinn Neufeld

*/

#ifndef CALLBACKS
#define CALLBACKS

#include "callbacks.h"

/*
Sets callback functions
*/
void set_callbacks() {
    main_win->callback(cb_close_main);
    calc_btn->callback(cb_calc);
    statement_input->callback(cb_calc_preview);
    add_fcn_btn->callback(cb_add_fcn);

    fcn_confirm_btn->callback(cb_confirm_fcn);
    cancel_fcn_btn->callback(cb_cancel_fcn);
    add_fcn_win->callback(cb_cancel_fcn);

    view_fcns_btn->callback(cb_view_fcns);
    fcn_view_back_btn->callback(cb_view_fcns_back);
    view_fcns_add_fcn_btn->callback(cb_add_fcn);
    view_fcn_win->callback(cb_view_fcns_back);
}

/*
Closes main window & terminates program
*/
void cb_close_main(Fl_Widget* widget) {
    add_fcn_win->hide();
    view_fcn_win->hide();
    main_win->hide();
}

/*
Adds result to history
*/
void cb_calc(Fl_Widget* widget) {
    std::cout << "Running cb_calc" << std::endl;
    std::string s = statement_input->value();
    std::string result = calc(s, true);
    history.push_back(s + " = " + result);
    update_history();
}

/*
Shows result preview
*/
void cb_calc_preview(Fl_Widget* widget) {
    std::string s = statement_input->value();
    s = calc(s, false);
    output_preview->value(s.c_str());
}


/*
Shows add function window
*/
void cb_add_fcn(Fl_Widget* widget) {
    win_data_t* win_data = (win_data_t*) add_fcn_win->user_data();
    win_data->source = widget->parent();

    add_fcn_win->show();
    add_fcn_win->take_focus();
}

/*
Confirms & saves new function
*/
void cb_confirm_fcn(Fl_Widget* widget) {
    std::string fcn_str = fcn_input->value();
    std::string base_case = base_case_input->value();
    std::string restrictions = restriction_input->value();
    Function* fcn = new Function(fcn_str, base_case, restrictions);
    if (fcn->err != OK) {
        std::cout << "Got error " << fcn->err << ", not saving function." << std::endl;
        return;
    }
    if (gbl_vars.count(fcn->fcn_name)) {
        gbl_vars.erase(fcn->fcn_name);
    }
    functions[fcn->fcn_name] = *fcn;
    std::cout << "Added function \"" << fcn->fcn_name << '"' << std::endl;
    add_fcn_win->hide();

    win_data_t* fcn_win_data = (win_data_t*) add_fcn_win->user_data();

    if (fcn_win_data->source == view_fcn_win) {
        clear_view_fcns();
        fill_view_fcns();
        view_fcn_win->redraw();
    }
    fcn_win_data->source->take_focus();

    clear_fcn_win();
}

/*
Cancels adding a new function
*/
void cb_cancel_fcn(Fl_Widget* widget) {

    win_data_t* fcn_win_data = (win_data_t*) add_fcn_win->user_data();

    add_fcn_win->hide();
    fcn_win_data->source->take_focus();
    clear_fcn_win();
}

/*
Opens view functions window
*/
void cb_view_fcns(Fl_Widget* widget) {
    //view_fcns_pack->array
    if (view_fcn_win->shown()) {
        clear_view_fcns();
        view_fcn_win->redraw();
    }

    fill_view_fcns();

    win_data_t* view_fcn_data = (win_data_t*) view_fcn_win->user_data();
    view_fcn_data->source = widget->parent();

    view_fcn_win->show();
    view_fcn_win->take_focus();
}

/*
Clears view functions window and hides it
*/
void cb_view_fcns_back(Fl_Widget* widget) {
    view_fcn_win->hide();
    clear_view_fcns();

    win_data_t* view_fcn_data = (win_data_t*) view_fcn_win->user_data();
    view_fcn_data->source->take_focus();
}

/*
Opens up add function window pre-filled to edit an existing function
*/
void cb_edit_fcn(Fl_Widget* widget) {
    fcn_btn_t* btn_data = (fcn_btn_t*) widget->user_data();

    Function* fcn = &functions[btn_data->fcn_name];

    //Set function text input
    fcn_input->value(fcn->to_string().c_str());

    //Set base case text input
    std::string bc_s;
    for (size_t i = 0; i < fcn->base_cases.size(); i++) {
        bc_s += fcn->base_cases[i].to_string();
        if (i + 1 < fcn->base_cases.size()) {
            bc_s += '\n';
        }
    }
    base_case_input->value(bc_s.c_str());

    //Set input restriction text input
    std::string ir_s;
    for (size_t i = 0; i < fcn->input_restrictions.size(); i++) {
        ir_s += fcn->input_restrictions[i].to_string();
        if (i + 1 < fcn->input_restrictions.size()) {
            ir_s += '\n';
        }
    }
    restriction_input->value(ir_s.c_str());

    win_data_t* fcn_win_data = (win_data_t*) add_fcn_win->user_data();
    fcn_win_data->source = widget->parent();

    add_fcn_win->show();
    add_fcn_win->take_focus();
}

/*
Fills view functions window
*/
void fill_view_fcns() {
    for (const auto &fcn_entry: functions) {
        std::cout << "Adding function " << fcn_entry.first << '\n';
        Function* fcn = (Function*) &fcn_entry.second;

        std::string fcn_str = fcn->to_string();
        fcn_btn_t* btn_data = new fcn_btn_t;
        btn_data->fcn_name = fcn_entry.first;
        btn_data->label = new char[fcn_str.size()];
        strcpy(btn_data->label, fcn_str.c_str());
        std::cout << "Got fcn_str: " << fcn_str << std::endl;

        Fl_Button* btn = new Fl_Button(0, 0, 0, 25, btn_data->label);
        btn->user_data(btn_data);
        btn->callback(cb_edit_fcn);
        view_fcns_pack->add(btn);
    }
}

/*
Clears view functions window
*/
void clear_view_fcns() {
    while (view_fcns_pack->children()) {
        Fl_Button* fcn_btn = (Fl_Button*) view_fcns_pack->child(0);
        std::cout << "Removing button " << fcn_btn->label() << std::endl;

        fcn_btn_t* btn_data = (fcn_btn_t*) fcn_btn->user_data();
        delete btn_data->label;
        delete btn_data;

        view_fcns_pack->remove(0);
        delete fcn_btn;
    }
}

/*
Updates history display
*/
void update_history() {
    std::string hist_str;
    for (std::string h: history) {
        hist_str = h + '\n' + hist_str;
    }
    history_output->value(hist_str.c_str());
}

/*
Clears widgets in add function window
*/
void clear_fcn_win() {
    fcn_input->value("");
    base_case_input->value("");
    restriction_input->value("");
}

#endif
