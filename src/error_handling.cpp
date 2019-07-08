#include "error_handling.hpp"

Error_handling::Error_handling(){
    // Tracking Errors
    error_input_missing = 1;
    error_input_files = 0;
    error_input_files_message = "";
    error_abundance_file_missing = 0;
    error_method = 0;
    error_method_message = "";
    error_overlap = 0;
    error_cols_X_Y = 0;
    error_unexpected_option = 0;
    error_unexpected_option_message = "";
};


// Set info
void Error_handling::set_error_input_missing(){
    error_input_missing = 0;
};
void Error_handling::set_error_input_files( std::string s ){
    if( s != "" )
    {
        error_input_files_message = s;
        error_input_files = 1;
    }
};
void Error_handling::set_error_abundance_file_missing(){
    error_abundance_file_missing = 1;
};
void Error_handling::set_error_method( std::string s ){
    if( s != "Error: ")
    {
        error_method = 1;
        error_method_message = s;
    }

};
void Error_handling::set_error_overlap(){
    error_overlap = 1;
};
void Error_handling::set_error_cols_X_Y(){
    error_cols_X_Y = 1;
};
void Error_handling::set_error_unexpected_option( std::string s ){
    error_unexpected_option_message = s;
    error_unexpected_option = 1;
};

// Get output string
std::string Error_handling::get_error_report(){
    // generate Error report as string
    std::string r = "Error/s detected\n";
    // check here error messages and do print out!
    if( error_input_missing )
    {
        r += "Input is missing. Please use option --network_file [network_filename]\n";
    }
    if( error_input_files )
    {
        r += error_input_files_message;
        r += "\n";
    }
    if( error_method )
    {
        r += error_method_message;
        r += "\n";
    }
    if( error_overlap )
    {
        r += "Option '--OL_colnum_interactionlength,startX,startY [num1,num2,num3]' requires a list of 3 columns.\n";
    }
    if( error_cols_X_Y )
    {
        r += "Option '--node_col [colnum_x,colnum_y]' requires a list of 2 columns.\n";
    }
    if( error_unexpected_option )
    {
        r += "An error when setting the functions occurred.\n";
        r += "Error found near:\n";
        r += error_unexpected_option_message;
        r += "\n";
    }
    if( error_abundance_file_missing )
    {
        r += "Abundance file missing.\n";
    }
    return r;
};

int Error_handling::did_error_occur(){
    if( error_input_missing || error_input_files || error_method || error_overlap || error_cols_X_Y || error_unexpected_option )
    {
        return 1;
    }
    else
    {
        return 0;
    }
};
