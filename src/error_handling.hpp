#ifndef error_handling_Hpp
#define error_handling_Hpp

#include <string>

class Error_handling{
    public:
        // Constructor
        Error_handling();

        // Set info
        void set_error_input_missing();
        void set_error_abundance_file_missing();
        void set_error_input_files( std::string );
        void set_error_method( std::string );
        void set_error_overlap();
        void set_error_cols_X_Y();
        void set_error_unexpected_option( std::string s );

        // Get output string
        std::string get_error_report();
        int did_error_occur();

	private:
        // Tracking Errors
        int error_input_missing;
        int error_input_files;
        std::string error_input_files_message;
        int error_abundance_file_missing;
        int error_method;
        std::string error_method_message;
        int error_overlap;
        int error_cols_X_Y;
        int error_unexpected_option;
        std::string error_unexpected_option_message;
};

#endif
