// #############################################################################
// network_indirect_edge_detection.cpp
//
// Predict indirect edges in network based on
// + sign pattern, and/or
// + overlap, and/or
// + interaction information, and/or
// + data processing inequality
// Prediction of indirect edges between two IDs if both are connected to
//   minimum one environmental parameter.
//
// Compiling: make
// Usage:
// ./ied --network_file [filename] --methods [list of method\s] <option(s)>
//
// Input: network file and optionally ID-ENV edge file and/or abundance+ENV
//        table and optionally ENV parameter table extra
//
// Output: File containing the extended network with prediction of indirect
//         edges and optionally Triplet-info file and randomized
//
// Ina Maria Deutschmann
// Latest update: April 2019
// #############################################################################

// #############################################################################
// Dependencies
#include <iostream>
#include <fstream>                    // read in and out files
#include <getopt.h>                   // parsing options
#include <boost/version.hpp>          // boost version
#include <time.h>                     // time_t, time, ctime
#include <math.h>                     // floor
#include <chrono>                     // steady_clock

#include "output_help_message.hpp"
#include "output_version_message.hpp"
#include "log.hpp"
#include "error_handling.hpp"
#include "triplet.hpp"
// #############################################################################

// #############################################################################
int main(int argc, char *argv[]) {
    // Start time of program
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    //Starttime
    time_t starttime;
    struct tm * timeinfo;
    char buffer [80];
    time (&starttime);
    timeinfo = localtime (&starttime);

    strftime (buffer, 80, "%Y-%m-%d_%Hh%Mmin%Ssec", timeinfo);

    // Constructors
    Log log = Log();
    log.set_filename_output_logfile( (std::string)buffer );
    Error_handling ERR = Error_handling();

    // Parsing Options
    int opt = 0;
    int longIndex = 0;
    static const char *optString = "hvdf:m:o:t:";

    static const struct option longOpts[] = {
        { "help", no_argument, NULL, 'h' },
        { "version", no_argument, NULL, 'v' },
        { "defaults", no_argument, NULL, 'd' },
        // Required options
        { "input_network_file", required_argument, NULL, 'f' },
        { "methods", required_argument, NULL, 'm' },
        // methods input
        { "SP_colnum_interaction_score", required_argument, NULL, 0 },
        { "OL_colnum_interactionlength_startX_startY", required_argument, NULL, 0 },
        { "OL_percentage_threshold", required_argument, NULL, 0 },
        { "II_DPI_abundance_file", required_argument, NULL, 0 },
        { "II_DPI_ENVparameter_file", required_argument, NULL, 0 },
        { "II_DPI_max_nan_threshold", required_argument, NULL, 0 },
        { "II_significance_level", required_argument, NULL, 0 },
        { "II_permutation_iteration", required_argument, NULL, 0 },
        { "DPI_minMI_threshold", required_argument, NULL, 0 },
        { "method_count_threshold", required_argument, NULL, 0},
        { "triplet_count_threshold", required_argument, NULL, 0},
        // Significance for II
        { "do_pre_jointP_comp", no_argument, NULL, 0 },
        // Inputfile Separators
        { "separator_network_file", required_argument, NULL, 0 },
        { "separator_abundance_file", required_argument, NULL, 0 },
        // Outputfiles
        { "output_network_file", required_argument, NULL, 'o' },
        { "output_ID_ENV_edge_dont_print", no_argument, NULL, 0 },
        { "output_triplet_info", required_argument, NULL, 't' },
        { "output_discretized_vectors", required_argument, NULL, 0},
        // Other
        { "input_node_col", required_argument, NULL, 0 },
        { "input_ENV_identicator", required_argument, NULL, 0 },
        { "input_ID_ENV_nw_file", required_argument, NULL, 0 },
        { NULL, no_argument, NULL, 0 } // <- needed?
    };

    opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
    while ( opt != -1 )
    {
        switch ( opt )
        {
            // { "help", no_argument, NULL , 'h' },
            case 'h':
                get_help_message( argv[ 0 ]);
                std::cout << std::endl;
                return 1;
                break;

            // { "version", no_argument, NULL, 'v' },
            case 'v':
                std::cout << get_version_message( argv[0] ) << std::endl;
                return 1;
                break;

            //{ "defaults", no_argument, NULL, 'v' },
            case 'd':
                get_defaults();
                std::cout << std::endl;
                return 1;
                break;

            // { "network_file", required_argument, NULL, 'f' },
            case 'f':
                log.set_filename_input_nw( optarg );
                ERR.set_error_input_missing();
                break;

            // { "methods", required_argument, NULL, 'm' },
            case 'm':
                // Read in which methods should be used
                // and capture if error occurred
                ERR.set_error_method( log.set_methods( optarg ) );
                break;

            // Outputfiles
            // { "output_network_file", required_argument, NULL, 'o' },
            case 'o':
                log.set_filename_output_extended_network( optarg );
                break;

            // { "output_triplet_info", required_argument, NULL, 't' },
            case 't':
                log.set_indicator_output_triplets();
                log.set_filename_output_triplets( optarg );
                break;

            // long option without a short arg
            case 0:
            {
                // methods input
                // { "SP_colnum_interaction_score", required_argument, NULL, 0 },
                if( strcmp( "SP_colnum_interaction_score", longOpts[longIndex].name ) == 0 )
                {
                    log.set_col_SP_interactionscore( optarg );
                }
                else if( std::string( longOpts[longIndex].name).substr( 0 , 2 ) == "OL" )
                {
                    // { "OL_colnum_interactionlength_startX_startY", required_argument, NULL, 0 },
                    if( strcmp( "OL_colnum_interactionlength_startX_startY", longOpts[longIndex].name ) == 0 )
                    {
                        if( !log.set_cols_OL( optarg ) )
                        {
                            ERR.set_error_overlap(); // Overlap Option was not given a list of 3 columns
                        }
                    }
                    // { "OL_percentage", required_argument, NULL, 0 },
                    else if( strcmp( "OL_percentage_threshold", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_OL_percentage_threshold( optarg );
                    }
                    //else{ // ERROR, normally should not get here}
                }
                else if( std::string( longOpts[longIndex].name).substr( 0 , 2 ) == "II" )
                {
                    // { "II_DPI_abundance_file", required_argument, NULL, 0 },
                    if( strcmp( "II_DPI_abundance_file", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_filename_input_abundance( optarg );
                    }
                    // { "II_DPI_ENVparameter_file", required_argument, NULL, 0 },
                    else if( strcmp( "II_DPI_ENVparameter_file", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_filename_input_ENV( optarg );
                    }
                    // { "II_DPI_max_nan_threshold", required_argument, NULL, 0 },
                    else if( strcmp( "II_DPI_max_nan_threshold", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_max_nan_threshold( optarg );
                    }
                    // { "II_significance_level", required_argument, NULL, 0 },
                    else if( strcmp( "II_significance_level", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_II_significance_level( optarg );
                    }
                    // { "II_permutation_iteration", required_argument, NULL, 0 },
                    else if( strcmp( "II_permutation_iteration", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_II_num_permutations( optarg );
                    }
                    //else{ // ERROR, normally should not get here }
                }
                // Significance for II
                else if( std::string( longOpts[longIndex].name).substr( 0 , 2 ) == "do" )
                {
                    // { "do_pre_jointP_comp", no_argument, NULL, 0 },
                    if( strcmp( "do_pre_jointP_comp", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_indicator_pre_jointP_comp();
                    }
                    //else{ // ERROR, normally should not get here }
                }
                // Inputfile separators
                else if( std::string( longOpts[longIndex].name).substr( 0 , 3 ) == "sep" )
                {
                    // { "separator_network_file", required_argument, NULL, 0 },
                    if( strcmp( "separator_network_file", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_sep_nw( optarg );
                    }
                    // { "separator_abundance_file", required_argument, NULL, 0 },
                    else if( strcmp( "separator_abundance_file", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_sep_abundance( optarg );
                    }
                    //else{ // ERROR, normally should not get here }
                }
                // Outputfiles
                else if( std::string( longOpts[longIndex].name).substr( 0 , 6 ) == "output" )
                {
                    // { "ID_ENV_edge_dont_print", no_argument, NULL, 0 },
                    if( strcmp( "output_ID_ENV_edge_dont_print", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_indicator_print_ID_ENV();
                    }
                    // { "output_discretized_vectors", required_argument, NULL, 0},
                    else if( strcmp( "output_discretized_vectors", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_indicator_output_discretized_vectors();
                        log.set_filename_output_discretized_vectors( optarg );
                    }
                }
                // Other
                else if( std::string( longOpts[longIndex].name).substr( 0 , 5 ) == "input" )
                {
                    // { "node_col", required_argument, NULL, 0 },
                    if( strcmp( "input_node_col", longOpts[longIndex].name ) == 0 )
                    {
                        if( !log.set_cols_X_Y( optarg ) )
                        {
                            ERR.set_error_cols_X_Y(); // Node_col Option was not given a list of 2 columns
                        }
                    }
                    // { "ENV_identicator", required_argument, NULL, 0 },
                    else if( strcmp( "input_ENV_identicator", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_ENV_indicator( optarg );
                    }
                    // { "ID_ENV_nw_file", required_argument, NULL, 0 },
                    else if( strcmp( "input_ID_ENV_nw_file", longOpts[longIndex].name ) == 0 )
                    {
                        log.set_filename_input_ID_ENV_EDGES( optarg );
                    }
                }
                else if( strcmp( "DPI_minMI_threshold", longOpts[longIndex].name ) == 0 ){
                  // { "DPI_minMI_threshold", required_argument, NULL, 0 },
                  log.set_DPI_minMI_threshold( optarg );
                }
                else if( strcmp( "method_count_threshold", longOpts[longIndex].name ) == 0 ) {
                  // { "method_count_threshold", required_argument, NULL, 0},
                  log.set_method_count_threshold( optarg );
                }
                else if( strcmp( "triplet_count_threshold", longOpts[longIndex].name ) == 0 ) {
                  // { "triplet_count_threshold", required_argument, NULL, 0},
                  log.set_triplet_count_threshold( optarg );
                }
                //else{ // getopt_long sets next option right QuitNow, we shouldnt worry }
            }
                break;

            case '?':
                ERR.set_error_unexpected_option( longOpts[longIndex].name );
                break;

                // Error handling code here would be good, but obvious errors should be already catched before.
        }
        opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
    }

    // ToLog: welcome message and version
    std::ofstream toLog_SetUp( log.get_filename_output_logfile() );
    toLog_SetUp << get_version_message( argv[0] ) << std::endl;
    std::string outputline = "Versions:";
    outputline += "\nGNU GCC/G++: ";
    outputline += std::to_string( __GNUC__ );
    outputline += ".";
    outputline += std::to_string( __GNUC_MINOR__ );
    outputline += ".";
    outputline += std::to_string( __GNUC_PATCHLEVEL__ );
    outputline += "\nboost: ";
    outputline += std::to_string( BOOST_VERSION / 100000 );     // major version
    outputline += ".";
    outputline += std::to_string( BOOST_VERSION / 100 % 1000 ); // minor version
    outputline += ".";
    outputline += std::to_string( BOOST_VERSION % 100 );        // patch level
    outputline += "\n";
    toLog_SetUp << outputline;
    toLog_SetUp << log.get_bash_command( argc, argv ) << std::endl;

    // Error Report
    if( ( log.get_use_II() || log.get_use_DPI() || log.get_use_CO() ) && log.get_filename_input_abundance() == "" )
    {
        ERR.set_error_abundance_file_missing();
    }
    ERR.set_error_input_files( log.check_input_files() );
    if( ERR.did_error_occur() )
    {
        toLog_SetUp << ERR.get_error_report() << std::endl;
        std::cout << ERR.get_error_report();
        get_help_message( argv[ 0 ]);
        std::cout << std::endl;
        return 1;
    }

    // ToLog: settings
    toLog_SetUp << log.get_settings() << std::endl;

    // Constructor for indirect edge detection
    Triplet T = Triplet();
    T.set_methods( log.get_use_SP(), log.get_use_OL(), log.get_use_II(), log.get_use_DPI(), log.get_use_CO(), log.get_method_count_threshold(), log.get_triplet_count_threshold() );
    T.set_sep_nw( log.get_sep_nw() );
    T.set_filename_output_nw( log.get_filename_output_extended_nw() );

    // Methods settings
    // Sign Pattern
    if( log.get_use_SP() )
    {
        T.set_sign_pattern_parameters( log.get_col_SP_interactionscore() );
    }
    // Overlap
    if( log.get_use_OL() )
    {
        T.set_overlap_parameters( log.get_col_OL_length(), log.get_col_OL_startX(), log.get_col_OL_startY(), log.get_OL_percentage_threshold() );
    }
    // Interaction Information
    if( log.get_use_II() )
    {
        // Settings for Significance Determination
        T.set_interaction_information_significance_determination_parameters( log.get_II_num_permutations(), log.get_II_significance_level(), log.get_indicator_pre_jointP_comp() );
        
        toLog_SetUp << T.get_log_message() << std::endl;
        T.reset_log_message();
    }
    // Data Processing Inequality
    if( log.get_use_DPI() )
    {
        // Settings for min mutual informaiton threshold
        T.set_data_processing_inequality_parameter( log.get_DPI_minMI_threshold() );
    }
    
    toLog_SetUp.close();
    // Log
    std::ofstream toLog( log.get_filename_output_logfile(), std::ios::out|std::ios::app );


    // Reading in abundances and ENV parameter and discretize
    if( log.get_use_II() || log.get_use_DPI() || log.get_use_CO() )
    {
        // Read in abundances and save discretized abundances
        T.build_disc_table( log.get_filename_input_abundance(), log.get_filename_input_ENV(), log.get_sep_abundance(), log.get_indicator_output_discretized_vectors(), log.get_filename_output_discretized_vectors(), log.get_max_nan_threshold(), log.get_filename_output_logfile() );
        // toLog << Warnings and Error --> done by disc_file
        // Error catched here
        if( T.get_error_occurred() )
        {
            // toLog << T.get_error_message() << std::endl;
            std::cout << T.get_error_message();
            return 1;
        }
    }

    // Read in ID-ENV edges and if II or DPI compute MI
    T.build_ID_ENV_EDGES_table( log.get_filename_input_ID_ENV_EDGES(), log.get_sep_nw(), log.get_col_X(), log.get_col_Y(), log.get_ENV_indicator(), log.get_indicator_pre_jointP_comp() );
    toLog << T.get_log_message() << std::endl;
    T.reset_log_message();

    // Print Header of extended network file
    std::ofstream OUTPUT_NW( log.get_filename_output_extended_nw() );
    OUTPUT_NW << T.print_header_nw( log.get_header_nw() ) << std::endl;

    // Preparation: build MI_ID_ENV table
    if( ( log.get_indicator_print_ID_ENV() ) && ( log.get_use_II() || log.get_use_DPI() ) )
    {
        T.build_MI_ID_ENV_table( log.get_max_nan_threshold() );
    }

    // Pre_permut
    if( log.get_indicator_pre_jointP_comp() )
    {
        T.do_ENV_pre_permut();
        T.do_pre_JointP_comp( log.get_max_nan_threshold() );
    }

    // Prepare extra output in case it is choosen
    std::ofstream OUTPUT_TRIPLET_INFO( log.get_filename_output_triplets() );
    if( log.get_indicator_output_triplets() )
    {
        OUTPUT_TRIPLET_INFO << T.get_triplet_info_header( log.get_node_name_X(), log.get_node_name_Y(), log.get_ENV_indicator() ) << std::endl;
    }
    T.set_extra_output_info( log.get_indicator_output_triplets());

    // Indirect Edge detection
    // Read in edges from file and extend with indirect edge detection information
    std::ifstream IN( log.get_filename_input_nw() );
    if( IN.good() )
    {
        std::string l;
        // Read in header (1st line)
        if( getline( IN, l ).good() )
        {
            toLog << "\nReading in " << log.get_filename_input_nw() << " for indirect edge detection" << std::endl;

            // First line is header

            while( getline( IN, l).good() )
            {
                std::string outputline = T.print_extended_nw_edge( l, log.get_col_X(), log.get_col_Y(), log.get_ENV_indicator(), log.get_max_nan_threshold() ); // Here happenes the indirect edge detection!!!
                if( outputline != "" )
                {
                    OUTPUT_NW << outputline << std::endl;

                    if( T.get_indicator_triplet_edge() && log.get_indicator_output_triplets() )
                    {
                        OUTPUT_TRIPLET_INFO << T.print_triplet_info() << std::endl; // Add to Triplet information output
                        T.reset_triplet_info_message();
                    }
                }
            }
        }
    }
    // Close network files
    IN.close();
    OUTPUT_NW.close();
    // Close extra file
    OUTPUT_TRIPLET_INFO.close();

    // Print ID-ENV edges
    if( log.get_indicator_print_ID_ENV() )
    {
        T.print_ID_ENV_EDGES( log.get_filename_output_extended_nw() );
        if( log.get_indicator_output_triplets() )
        {
            T.print_triplet_info_ID_ENV_EDGES( log.get_filename_output_triplets() );
        }
    }

    // ToLog: Trackers
    toLog << T.get_tracker_report();

    // End time of program
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();

    // ToLog: Time the program needed to compute
    toLog << "Computing time:" << std::endl;
    // End time
    time_t endtime;
    struct tm * timeinfo2;
    char buffer2 [80];
    time (&endtime);
    timeinfo2 = localtime (&endtime);

    strftime (buffer2, 80, "%Y-%m-%d_%Hh%Mmin%Ssec", timeinfo2);

    toLog << "Start: " << buffer << std::endl;
    toLog << "End:   " << buffer2 << std::endl;

    // Difference
    double sec2 = difftime(endtime,starttime);
    double hour = std::floor( sec2/(1440) );
    double min = std::floor((sec2-hour*1440)/60);
    double sec = sec2 - hour*1440 - min*60;

    toLog << "\nin sec:          " << sec2 << std::endl;
    toLog << "in hour:min:sec: " << hour << ":" << min << ":" << sec << std::endl;
    // Close Log file
    toLog.close();

    return 0;
};
