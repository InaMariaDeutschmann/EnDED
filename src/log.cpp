#include "log.hpp"
#include <vector> // vector
#include <boost/algorithm/string.hpp> // split
#include <fstream>                    // read in and out files

Log::Log(){
    // Number of columns of data
    col_X = 0;                    // Column containining node X
    col_Y = 1;                    // Column containining node Y
    col_OL_length = 0;            // Column containining length of interaction (used for Overlap-method)
    col_OL_startX = 0;            // Column containining start of interaction for X (used for Overlap-method)
    col_OL_startY = 0;            // Column containining start of interaction for Y (used for Overlap-method)
    col_SP_interactionscore = 0;  // Column containing interaction score (used for SignPattern-method)

    // Filenames
    filename_input_nw = "";                                   // filename/path of network
    filename_input_ID_ENV_EDGES = "";                         // filename/path of ID-ENV edges in network
    filename_input_abundance = "";                            // filename/path of abundance file
    filename_input_ENV = "";                                  // filenmae/path of ENV parameter file (has to have same column names as abundance_filename)
    filename_output_extended_nw = "extended_network.txt";     // filename/path of extended network
    filename_output_triplets = "triplet.txt";                 // filename/path of optional file containing info of triplets
    filename_output_discretized_vectors = "disc_vectors.txt"; // filename/path of optional file containing discretized vectors
    filename_output_logfile = "log.txt";                      // filename/path of logfile

    // Separators of input files
    sep_nw = "\t";         // separator for network file
    sep_abundance = "\t";  // separator for abundance file

    // Other
    ENV_identicator = "ENV";                   // indicator for environmental parameter, needs to be included in node name
    indicator_output_triplets = 0;             // indicator if triplet_info output is generated
    indicator_output_discretized_vectors = 0;  // indicator if disc vector output is generated
    indicator_print_ID_ENV = 1;                // indicator if ID-ENV edges should be printed
    indicator_pre_jointP_comp = 0;             // Indicator if permutations should be done before and stored instead of being computed anew each time the significance level is determined (might cause memory troubles, so it is not by default)
    header_NW = "";                            // This header will be extended by indirect edge detection methods
    node_name_X = "";                          // Node name in NW header
    node_name_Y = "";                          // Node name in NW header

    // Methods
    use_SP = 0;
    use_OL = 0;
    use_II = 0;
    use_DPI = 0;
    use_CO = 0;

    // Method Settings
    OL_percentage_threshold = 60;
    II_significance_level = 0.05;
    II_num_permutations = 100;
    DPI_minMI_threshold = 0;
    max_nan_threshold = 20;
    method_count_threshold = 100;
    triplet_count_threshold = 1;
};


// Set info
int Log::set_cols_X_Y( std::string s ){
    std::vector< std::string > v;
    split( v, s, boost::is_any_of( "," ) );
    if( v.size() == 2 )
    {
        col_X = std::stoi( v[ 0 ] ) - 1;
        col_Y = std::stoi( v[ 1 ] ) - 1;
        return 1;
    }
    else
    {
        return 0;
    }
};
int Log::set_cols_OL( std::string s ){
    std::vector< std::string > v;
    split( v, s, boost::is_any_of( "," ) );
    if( v.size() == 3 )
    {
        col_OL_length = std::stoi( v[ 0 ] ) - 1;
        col_OL_startX = std::stoi( v[ 1 ] ) - 1;
        col_OL_startY = std::stoi( v[ 2 ] ) - 1;
        return 1;
    }
    else
    {
        return 0;
    }
};

void Log::set_col_SP_interactionscore( std::string s ){
    col_SP_interactionscore = std::stoi( s ) - 1;
};
void Log::set_filename_input_nw( std::string s ){
    filename_input_nw = s;
};
void Log::set_filename_input_ID_ENV_EDGES( std::string s ){
    filename_input_ID_ENV_EDGES = s;
};
void Log::set_filename_input_abundance( std::string s ){
    filename_input_abundance = s;
};
void Log::set_filename_input_ENV( std::string s ){
    filename_input_ENV = s;
};
void Log::set_filename_output_extended_network( std::string s ){
    filename_output_extended_nw = s;
};
void Log::set_filename_output_triplets( std::string s ){
    filename_output_triplets = s;
};
void Log::set_filename_output_discretized_vectors( std::string s ){
    filename_output_discretized_vectors = s;
};
void Log::set_filename_output_logfile( std::string s ){
    filename_output_logfile = "log_";
    filename_output_logfile += s;
    filename_output_logfile += ".txt";
};
void Log::set_sep_nw( std::string s ){
    sep_nw = s;
};
void Log::set_sep_abundance( std::string s ){
    sep_abundance = s;
};
void Log::set_ENV_indicator( std::string s ){
    ENV_identicator = s;
};
void Log::set_indicator_output_triplets(){
    indicator_output_triplets = 1;
};
void Log::set_indicator_output_discretized_vectors(){
    indicator_output_discretized_vectors = 1;
};
void Log::set_indicator_print_ID_ENV(){
    indicator_print_ID_ENV = 0;
};
void Log::set_indicator_pre_jointP_comp(){
    indicator_pre_jointP_comp = 1;
};
std::string Log::set_methods( std::string s ){
    std::string allgood = "Error: ";
    std::vector< std::string > v;
    split( v, optarg, boost::is_any_of( "," ) );
    if( v.size() > 0 )
    {
        for( int i = 0; i < v.size(); i++ )
        {
            if( v[ i ] == "all" )
            {
                use_SP = 1;
                use_OL = 1;
                use_II = 1;
                use_DPI = 1;
                use_CO = 1;
            }
            else if( v[ i ] == "SP" ){ use_SP = 1; }
            else if( v[ i ] == "OL" ){ use_OL = 1; }
            else if( v[ i ] == "II" ){ use_II = 1; }
            else if( v[ i ] == "DPI" ){ use_DPI = 1; }
            else if( v[ i ] == "CO" ) { use_CO = 1; }
            else
            {
                allgood = allgood + "unkown method: " + v[ i ] + "\n";
            }
        }
    }
    else
    {
        allgood = "no methods given";
    }
    return allgood;
};
void Log::set_OL_percentage_threshold( std::string s ){
    OL_percentage_threshold = std::stod( s );
};
void Log::set_II_significance_level( std::string s ){
    II_significance_level = std::stod( s );
};

void Log::set_II_num_permutations( std::string s ){
    II_num_permutations = std::stoi( s );
};
void Log::set_DPI_minMI_threshold( std::string s ){
    DPI_minMI_threshold = std::stod( s );
};
void Log::set_max_nan_threshold( std::string s ){
    max_nan_threshold = std::stod( s );
};
void Log::set_method_count_threshold( std::string s ){
    method_count_threshold = std::stod( s );
};
void Log::set_triplet_count_threshold( std::string s ){
    triplet_count_threshold = std::stod( s );
};

// Get info
int Log::get_col_X(){
    return col_X;
};
int Log::get_col_Y(){
    return col_Y;
};
int Log::get_col_OL_length(){
    return col_OL_length;
};
int Log::get_col_OL_startX(){
    return col_OL_startX;
};
int Log::get_col_OL_startY(){
    return col_OL_startY;
};
int Log::get_col_SP_interactionscore(){
    return col_SP_interactionscore;
};
std::string Log::get_filename_input_nw(){
    return filename_input_nw;
};
std::string Log::get_filename_input_ID_ENV_EDGES(){
    if( filename_input_ID_ENV_EDGES != "" )
    {
        return filename_input_ID_ENV_EDGES;
    }
    else
    {
        return filename_input_nw;
    }
};
std::string Log::get_filename_input_abundance(){
    return filename_input_abundance;
};
std::string Log::get_filename_input_ENV(){
    return filename_input_ENV;
};
std::string Log::get_filename_output_extended_nw(){
    return filename_output_extended_nw;
};
std::string Log::get_filename_output_triplets(){
    return filename_output_triplets;
};
std::string Log::get_filename_output_discretized_vectors(){
    return filename_output_discretized_vectors;
};
std::string Log::get_filename_output_logfile(){
    return filename_output_logfile;
};
std::string Log::get_sep_nw(){
    return sep_nw;
};
std::string Log::get_sep_abundance(){
    return sep_abundance;
};
std::string Log::get_ENV_indicator(){
    return ENV_identicator;
};
int Log::get_indicator_output_triplets(){
    return indicator_output_triplets;
};
int Log::get_indicator_output_discretized_vectors(){
    return indicator_output_discretized_vectors;
};
int Log::get_indicator_print_ID_ENV(){
    return indicator_print_ID_ENV;
};
int Log::get_indicator_pre_jointP_comp(){
    return indicator_pre_jointP_comp;
};
std::string Log::get_header_nw(){
    return header_NW;
};
int Log::get_use_SP(){
    return use_SP;
};
int Log::get_use_OL(){
    return use_OL;
};
int Log::get_use_II(){
    return use_II;
};
int Log::get_use_DPI(){
    return use_DPI;
};
int Log::get_use_CO(){
    return use_CO;
};
double Log::get_OL_percentage_threshold(){
    return OL_percentage_threshold;
};
int Log::get_II_num_permutations(){
    return II_num_permutations;
};
double Log::get_II_significance_level(){
    return II_significance_level;
};
double Log::get_DPI_minMI_threshold(){
    return DPI_minMI_threshold;
};
std::string Log::get_node_name_X(){
    return node_name_X;
};
std::string Log::get_node_name_Y(){
    return node_name_Y;
};
double Log::get_max_nan_threshold(){
    return max_nan_threshold;
};
double Log::get_method_count_threshold(){
    return method_count_threshold;
};
double Log::get_triplet_count_threshold(){
    return triplet_count_threshold;
};

// Check input files
std::string Log::check_input_files(){
    std::string error_message = "";
    header_NW = "";
    std::string header_ID_ENV = "";
    std::string header_abundance = "";
    std::string header_ENV = "";

    std::ifstream IN( filename_input_nw );
    if( !IN.good() )
    {
        error_message += "\n";
        error_message += filename_input_nw;
        error_message += " could not be opened.\n";
    }
    else
    {
        std::string l;
        // Read in header (1st line)
        if( getline( IN, l ).good() )
        {
            header_NW = l;

            // Check if all columns are in
            std::vector< std::string > v;
            split( v, l, boost::is_any_of( sep_nw ) );
            int file_size = v.size();
            if( col_X >= file_size )
            {
                error_message += "\nColumn number (col_X) bigger than number of columns in file:";
                error_message += std::to_string( col_X );
                error_message += "\nvs.\n";
                error_message += std::to_string( file_size );
            }
            else
            {
                node_name_X = v[ col_X ]; // Set name for node given in header
            }
            if( col_Y >= file_size )
            {
                error_message += "\nColumn number (col_Y) bigger than number of columns in file:";
                error_message += std::to_string( col_Y );
                error_message += "\nvs.\n";
                error_message += std::to_string( file_size );
            }
            else
            {
                node_name_Y = v[ col_Y ]; // Set name for node given in header
            }
            if( use_SP && col_SP_interactionscore >= file_size )
            {
                error_message += "\nColumn number (interaction_score) bigger than number of columns in file:";
                error_message += std::to_string( col_SP_interactionscore );
                error_message += "\nvs.\n";
                error_message += std::to_string( file_size );
            }
            if( use_OL )
            {
                if( col_OL_length >= file_size )
                {
                    error_message += "\nColumn number (length) bigger than number of columns in file:";
                    error_message += std::to_string( col_OL_length );
                    error_message += "\nvs.\n";
                    error_message += std::to_string( file_size );
                }
                if( col_OL_startX >= file_size )
                {
                    error_message += "\nColumn number (startX) bigger than number of columns in file:";
                    error_message += std::to_string( col_OL_startX );
                    error_message += "\nvs.\n";
                    error_message += std::to_string( file_size );
                }
                if( col_OL_startX >= file_size )
                {
                    error_message += "\nColumn number (startY) bigger than number of columns in file:";
                    error_message += std::to_string( col_OL_startY );
                    error_message += "\nvs.\n";
                    error_message += std::to_string( file_size );
                }
            }
        }
        else
        {
            error_message += filename_input_nw;
            error_message += " is empty.\n";
        }
    }
    IN.close();

    if( filename_input_ID_ENV_EDGES != "" )
    {
        std::ifstream IN( filename_input_ID_ENV_EDGES );
        if( !IN.good() )
        {
            error_message += filename_input_ID_ENV_EDGES;
            error_message += " could not be opened.\n";
        }
        else
        {
            std::string l;
            // Read in header (1st line)
            if( getline( IN, l ).good() )
            {
                header_ID_ENV = l;

                if( header_NW != header_ID_ENV )
                {
                    error_message += "\nHeader of Network file and ID-ENV edges file are different:";
                    error_message += header_NW;
                    error_message += "\nvs.\n";
                    error_message += header_ID_ENV;
                }
            }
            else
            {
                error_message += filename_input_ID_ENV_EDGES;
                error_message += " is empty.\n";
            }
        }
        IN.close();
    }

    if( filename_input_abundance != "" )
    {
        std::ifstream IN( filename_input_abundance );
        if( !IN.good() )
        {
            error_message += "\n";
            error_message += filename_input_abundance;
            error_message += " could not be opened.\n";
        }
        else
        {
            std::string l;
            // Read in header (1st line)
            if( getline( IN, l ).good() )
            {
                header_abundance = l;
            }
            else
            {
                error_message += filename_input_abundance;
                error_message += " is empty.\n";
            }
        }
        IN.close();

        if( filename_input_ENV != "" )
        {
            std::ifstream IN( filename_input_ENV );
            if( !IN.good() )
            {
                error_message += filename_input_ENV;
                error_message += " could not be opened.\n";
            }
            else
            {
                std::string l;
                // Read in header (1st line)
                if( getline( IN, l ).good() )
                {
                    header_ENV = l;

                    if( header_abundance != header_ENV )
                    {
                        error_message += "\nHeader of abundance file and ENV parameter file are different:";
                        error_message += header_abundance;
                        error_message += "\nvs.\n";
                        error_message += header_ENV;
                    }
                }
                else
                {
                    error_message += filename_input_ENV;
                    error_message += " is empty.\n";
                }
            }
            IN.close();
        }
    }

    return error_message;
};

// Get output string
std::string Log::get_bash_command( int argc, char **argv ){
    std::string outputline;
    outputline += "\nYour command was: \n";
    for( int i = 0; i < argc; i++ )
    {
        outputline += argv[ i ];
        outputline += " ";
    }
    return outputline;
};
std::string Log::get_settings(){
    std::string outputline = "\n\nSettings:\n";

    // Files
    outputline += "\nInput Files\n\tThe input network filename: ";
    outputline += filename_input_nw;
    if( filename_input_ID_ENV_EDGES != "" )
    {
        outputline += "\n\tThe ID-ENV edges filename: ";
        outputline += filename_input_ID_ENV_EDGES;
    }
    if( use_II == 1 || use_DPI )
    {
        if( filename_input_ENV == "" )
        {
            outputline += "\n\tThe abundances and ENV-parameter filename: ";
            outputline += filename_input_abundance;
        }
        else
        {
            outputline += "\n\tThe abundances filename: ";
            outputline += filename_input_abundance;
            outputline += "\n\tThe ENV-parameter filename: ";
            outputline += filename_input_ENV;
        }
    }
    outputline += "\n\nOutput Files\n\tThe output network filename: ";
    outputline += filename_output_extended_nw;
    if( indicator_output_triplets )
    {
        outputline += "\n\tThe triplet output filename: ";
        outputline += filename_output_triplets;
    }
    if( indicator_output_discretized_vectors )
    {
        outputline += "\n\tThe discretized vectors output filename: ";
        outputline += filename_output_discretized_vectors;
    }

    // Nodes
    outputline += "\n\nNodes\n\tnode X is assumed to be in column ";
    outputline +=  std::to_string( col_X + 1);
    outputline += "\n\tnode Y is assumed to be in column ";
    outputline +=  std::to_string( col_Y + 1);

    // ID-ENV edges
    outputline += "\n\nID-ENV edges\n\tThe features names are assumed to contain the identifier: \"";
    outputline += ENV_identicator;
    outputline += "\"";
    if( indicator_print_ID_ENV )
    {
        outputline += "\n\tThe extended network file (output) will contain ID-";
        outputline += ENV_identicator;
        outputline += " edges.\n";
    }
    else
    {
        outputline += "\n\tThe extended network file (output) will not contain ID-";
        outputline += ENV_identicator;
        outputline += " edges.\n";
    }

    // Separator(s)
    outputline += "\nSeparators\n\tThe separator in the network file is assumed to be \"";
    if( sep_nw == "\t" )
    {
        outputline += "tab\"\n";
    }
    else if( sep_nw == " " )
    {
        outputline += "space\", \" \"\n";
    }
    else
    {
        outputline += sep_nw;
        outputline += "\"";
    }
    // { "separator_abundance_file", required_argument, NULL, 'a' },
    if( use_II || use_DPI )
    {
        outputline += "\tThe separator in the abundance file is assumed to be \"";
        if( sep_abundance == "\t" )
        {
            outputline += "tab\"\n";
        }
        else if( sep_abundance == " " )
        {
            outputline += "space\", \" \"\n";
        }
        else
        {
            outputline += sep_abundance;
            outputline += "\"";
        }
    }

    // Methods
    outputline += "\n\nselected Method(s)\n";
    if( use_SP )
    {
        outputline += "\tSign Pattern (SP)\n";
    }
    if( use_OL )
    {
        outputline += "\tOverlap (OL)\n";
    }
    if( use_II )
    {
        outputline += "\tInteraction Information (II)\n";
    }
    if( use_DPI )
    {
        outputline += "\tData Processing Inequality (DPI) (minimum Mutual Information)\n";
    }
    if( use_CO )
    {
        outputline += "\tCo-Occurrence (CO)\n";
    }
    outputline += "not selected Method(s)\n";
    if( !use_SP )
    {
        outputline += "\tSign Pattern (SP)\n";
    }
    if( !use_OL )
    {
        outputline += "\tOverlap (OL)\n";
    }
    if( !use_II )
    {
        outputline += "\tInteraction Information (II)\n";
    }
    if( !use_DPI )
    {
        outputline += "\tData Processing Inequality (DPI) (minimum Mutual Information)\n";
    }
    if( !use_CO )
    {
        outputline += "\tCo-Occurrence (CO)\n";
    }
    if( use_SP && use_OL && use_II && use_DPI && use_CO )
    {
        outputline += "\t-\n";
    }

    // Method Settings
    outputline += "\nMethod(s) Setting(s)\n";
    outputline += "\n  Combination strategy of considering an edge indirect.";
    outputline += "\n\tThreshold(in percentage) for minimum methods that suggest edge as indirect: ";
    outputline += std::to_string( method_count_threshold );
    outputline += "\n\tThreshold(in percentage) for minimum triplets that suggest edge as indirect: ";
    outputline += std::to_string( triplet_count_threshold );
    if( use_SP )
    {
        outputline += "\n\n  Sign Pattern\n\tThe interaction score is assumed to be in column ";
        outputline += std::to_string( col_SP_interactionscore + 1);
    }
    if( use_OL )
    {
        outputline += "\n\n  Overlap\n\tThe interaction length is assumed to be in column ";
        outputline +=  std::to_string( col_OL_length + 1);
        outputline += "\n\tThe interaction start of node X is assumed to be in column ";
        outputline +=  std::to_string( col_OL_startX + 1);
        outputline += "\n\tThe interaction start of node Y is assumed to be in column ";
        outputline +=  std::to_string( col_OL_startY + 1);
        outputline += "\n\tThreshold of the overlap in percentage: ";
        outputline +=  std::to_string( OL_percentage_threshold );
    }
    if( use_II )
    {
        outputline += "\n\n  Interaction Information\n\tsignificance_level: ";
        outputline +=  std::to_string( II_significance_level );
        outputline += "\n\tnumber of permutations: ";
        outputline +=  std::to_string( II_num_permutations );
        if( indicator_pre_jointP_comp )
        {
            outputline += "\n\t";
            outputline += get_ENV_indicator();
            outputline += "-vector will be permuted and stored before examining the networks triplets.\n\t";
            outputline += "And in addition Joint Probabilities with them will be precomputed and also stored.";
        }
    }
    if( use_DPI )
    {
        outputline += "\n\n  Data Processing Inequality\n\tmin mutual information(MI) threshold for MI comparison: ";
        outputline +=  std::to_string( DPI_minMI_threshold );
    }
    if( use_II || use_DPI )
    {
        outputline += "\n\n  Interaction Information and Data Processing Inequality\n";
        outputline += "\tThreshold(in percentage) for maximum portion of 'nan' in vectors used for II and DPI: ";
        outputline +=  std::to_string( max_nan_threshold );
    }
    outputline += "\n";

    return outputline;
};
