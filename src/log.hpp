#ifndef log_Hpp
#define log_Hpp

#include <string> // operator + for string concatenation std::stoi std::stod

class Log{
    public:
        // Constructor
        Log();

        // Set info
        int set_cols_X_Y( std::string s );
        int set_cols_OL( std::string s );
        void set_col_SP_interactionscore( std::string s );
        void set_filename_input_nw( std::string s );
        void set_filename_input_ID_ENV_EDGES( std::string s );
        void set_filename_input_abundance( std::string s );
        void set_filename_input_ENV( std::string s );
        void set_filename_output_extended_network( std::string s );
        void set_filename_output_triplets( std::string s );
        void set_filename_output_discretized_vectors( std::string s );
        void set_filename_output_logfile( std::string s );
        void set_sep_nw( std::string s );
        void set_sep_abundance( std::string s );
        void set_ENV_indicator( std::string s );
        void set_indicator_output_triplets();
        void set_indicator_output_discretized_vectors();
        void set_indicator_print_ID_ENV();
        void set_indicator_pre_jointP_comp();
        std::string set_methods( std::string s );
        void set_OL_percentage_threshold( std::string s );
        void set_II_significance_level( std::string s );
        void set_II_num_permutations( std::string s );
        void set_DPI_minMI_threshold( std::string s );
        void set_max_nan_threshold( std::string i );
        void set_method_count_threshold( std::string i );
        void set_triplet_count_threshold( std::string i );

        // Get info
        int get_col_X();
        int get_col_Y();
        int get_col_OL_length();
        int get_col_OL_startX();
        int get_col_OL_startY();
        int get_col_SP_interactionscore();

        std::string get_filename_input_nw();
        std::string get_filename_input_ID_ENV_EDGES();
        std::string get_filename_input_abundance();
        std::string get_filename_input_ENV();
        std::string get_filename_output_extended_nw();
        std::string get_filename_output_triplets();
        std::string get_filename_output_discretized_vectors();
        std::string get_filename_output_logfile();

        std::string get_sep_nw();
        std::string get_sep_abundance();

        std::string get_ENV_indicator();

        int get_indicator_output_triplets();
        int get_indicator_output_discretized_vectors();
        int get_indicator_print_ID_ENV();
        int get_indicator_pre_jointP_comp();

        std::string get_header_nw();
        std::string get_node_name_X();
        std::string get_node_name_Y();

        int get_use_SP();
        int get_use_OL();
        int get_use_II();
        int get_use_DPI();
        int get_use_CO();

        double get_OL_percentage_threshold();
        double get_method_count_threshold();
        double get_triplet_count_threshold();

        int get_II_num_permutations();
        double get_II_significance_level();
        double get_DPI_minMI_threshold();
        double get_max_nan_threshold();

        // Check input files
        std::string check_input_files();


        // Get output string
        std::string get_bash_command( int argc, char **argv );
        std::string get_settings();

	private:
        // Number of columns of data
        int col_X;                    // Column containining node X
        int col_Y;                    // Column containining node Y
        int col_OL_length;            // Column containining length of interaction (used for Overlap-method)
        int col_OL_startX;            // Column containining start of interaction for X (used for Overlap-method)
        int col_OL_startY;            // Column containining start of interaction for Y (used for Overlap-method)
        int col_SP_interactionscore;  // Column containing interaction score (used for SignPattern-method)

        // Filenames
        std::string filename_input_nw;                   // filename/path of network
        std::string filename_input_ID_ENV_EDGES;         // filename/path of ID-ENV edges in network
        std::string filename_input_abundance;            // filename/path of abundance file
        std::string filename_input_ENV;                  // filenmae/path of ENV parameter file (has to have same column names as abundance_filename)
        std::string filename_output_extended_nw;         // filename/path of network
        std::string filename_output_triplets;            // Filename of extra triplet info file
        std::string filename_output_discretized_vectors; // filename/path of optional file containing discretized vectors
        std::string filename_output_logfile;             // filename/path of logfile

        // Separators of input files
        std::string sep_nw;         // separator for network file
        std::string sep_abundance;  // separator for abundance file

        // Other
        std::string ENV_identicator;               // indicator for environmental parameter, needs to be included in node name
        int indicator_output_triplets;             // Indicator if extra triplet info file will be printed out
        int indicator_output_discretized_vectors;  // indicator if disc vector output is generated
        int indicator_print_ID_ENV;                // indicator whether or not ID-ENV edges should be printed
        int indicator_pre_jointP_comp;             // Indicator if permutations should be done before and stored instead of being computed anew each time the significance level is determined (might cause memory troubles, so it is not by default)
        std::string header_NW;                     // This header will be extended by indirect edge detection methods
        std::string node_name_X;                   // Node name in NW header
        std::string node_name_Y;                   // Node name in NW header

        // Methods used
        int use_SP;   // SignPatter
        int use_OL;   // Overlap
        int use_II;   // InteractionInformation
        int use_DPI;  // DataProcessingInequality
        int use_CO;      /// Co-Occurrence

        // Methods Settings
        double OL_percentage_threshold; // Threshold for percentage of overlap
        double II_significance_level;   // Significance level
        int II_num_permutations;        // Number of iterations
        double DPI_minMI_threshold;     // Default: 0, threshold for how much smaller min MI has to be compared to other two MIs
        double max_nan_threshold;       // Threshold(in percentage) for maximum portion of nan in abundance/parameters and when computing MI and II
        double method_count_threshold;  // Default: 100, threshold(in percentage) for minimum methods that suggest edge as indirect.
        double triplet_count_threshold; // Default: 1, threshold(in percentage) for minimum triplets that suggest edge as indirect.

};

#endif
