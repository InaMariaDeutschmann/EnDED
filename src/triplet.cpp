#include "triplet.hpp"
#include "convert_to_string.hpp"

#include <iostream>
#include <fstream> // read in and out files
#include <cmath>   // nan("")

Triplet::Triplet(){
    // Tracking
    error_occurred = 0;
    error_message = "";
    log_message = "";
    triplet_info_message = "";
    indicator_triplet_edge = 0;

    // Filename
    filename_output_nw = "";
    sep = "\t";

    // Methods
    use_SP = 0;
    use_OL = 0;
    use_II = 0;
    use_DPI = 0;
    use_CO = 0;

    // Methods settings
    OL_threshold = 60;
    II_Sign_Level = 0.05;
    DPI_minMI_threshold = 0;
    method_count_threshold = 100;
    triplet_count_threshold = 1;

    // Extra output indicator
    indicator_output_triplet = 0;

    // Constructors
    D = Disc_vectors();
    ID_ENV_EDGES = ID_ENV_edges();
    OUT = Output_extended_nw();
    Prob();
    MI = Mutual_information();
    SP = Sign_pattern();
    OL = Overlap();
    DPI = DataProcessingInequality();
    CO = CoOccurrence();
    TRACKER = Tracker();
};

// Set info
void Triplet::set_methods( int use_sp, int use_ol, int use_ii, int use_dpi, int use_co, double set_method_count_threshold, double set_triplet_count_threshold ){
    use_SP = use_sp;
    use_OL = use_ol;
    use_II = use_ii;
    use_DPI = use_dpi;
    use_CO = use_co;
    method_count_threshold = set_method_count_threshold;
    triplet_count_threshold = set_triplet_count_threshold;

    OUT.set_methods( use_sp, use_ol, use_ii, use_dpi, use_co );
};
void Triplet::set_sep_nw( std::string s ){
    OUT.set_sep_nw( s );
    sep = s;
};
void Triplet::set_filename_output_nw( std::string s ){
    filename_output_nw = s;
};
void Triplet::set_sign_pattern_parameters( int col_score ){
    SP.set_col_score( col_score );
    SP.set_sep( sep );
};
void Triplet::set_overlap_parameters( int col_len, int col_startX, int col_startY, double threshold ){
    OL.set_cols( col_len, col_startX, col_startY );
    OL.set_sep( sep );
    OL_threshold = threshold;
};
void Triplet::set_interaction_information_significance_determination_parameters( int num_permut, double significance_level, int indicator_pre_permut ){
    P.set_num_permutations( num_permut );
    II_Sign_Level = significance_level;
    P.set_indicator_pre_permut( indicator_pre_permut );

    log_message += "\nTo compute the significance of the interaction information, we are randomizing the environmental abundance table with the seed: ";
    log_message += std::to_string( P.set_permut_seed() );
};
void Triplet::set_data_processing_inequality_parameter( double d ){
    DPI_minMI_threshold = d;
};
void Triplet::set_extra_output_info( int indicator_triplet ){
    indicator_output_triplet = indicator_triplet;
};

// Built up datastructure
void Triplet::build_disc_table( std::string filename_abundance, std::string filename_ENV, std::string sep, int indicator_output_discretized_vectors, std::string filename_output_discretized_vectors, double max_nan_threshold, std::string filename_output_logfile ){
    // Read in abundances and save discretized abundances
    D.build_disc_table( filename_abundance, filename_ENV, sep, indicator_output_discretized_vectors, filename_output_discretized_vectors, max_nan_threshold, filename_output_logfile );
    if( D.get_error_occurred() )
    {
        error_occurred = 1;
        error_message = D.get_error_message();
    }
};

void Triplet::build_ID_ENV_EDGES_table( std::string filename_ID_ENV_EDGES, std::string sep, int col_x, int col_y, std::string ENV_indicator, int pre_permut ){
    // Read in abundances and save discretized abundances
    ID_ENV_EDGES.build_ID_ENV_EDGES_table( col_x, col_y, ENV_indicator, pre_permut, filename_ID_ENV_EDGES, sep );
    log_message = ID_ENV_EDGES.get_log_message();

    TRACKER.set_Tracker_num_ID_ENV_considered( ID_ENV_EDGES.get_num_ID_ENV_considered() );
    TRACKER.set_Tracker_num_ID_ENV_all( ID_ENV_EDGES.get_num_ID_ENV_all() );
    TRACKER.set_Tracker_num_ID_connected_to_min_one_ENV( ID_ENV_EDGES.get_num_ID_connected_to_min_one_ENV() );
    TRACKER.set_Tracker_num_ENV_connected_to_min_one_ID( ID_ENV_EDGES.get_num_ENV_connected_to_min_one_ID() );
};

void Triplet::build_MI_ID_ENV_table( double max_nan_threshold ){
    // ID-ENV edges
    std::unordered_map< std::string, std::unordered_map< std::string, std::string > > ID_ENV_TABLE = ID_ENV_EDGES.get_table();

    for( std::unordered_map< std::string, std::unordered_map< std::string, std::string > >::iterator e_ID = ID_ENV_TABLE.begin(); e_ID != ID_ENV_TABLE.end(); e_ID++ )
    {
        std::string ID = e_ID->first;
        // Check if ID and ENV in disc_table
        if( D.is_in_table( ID ) )
        {
            // Check if O is in MarginalP table
            if( !P.is_in_Marginal_Prob_Table( ID ) )
            {
                // Compute Marginal Probabilities and save in table
                P.add_Marginal_Prob( ID, D.get_disc_vector( ID ) );
            }
            double samplesize = D.get_disc_vector( ID ).size() + 1;

            for( std::unordered_map< std::string, std::string >::iterator e_ENV = ID_ENV_TABLE[ ID ].begin(); e_ENV != ID_ENV_TABLE[ ID ].end(); e_ENV++ )
            {
                std::string ENV = e_ENV->first;

                // Check if E is in disc_table
                if( D.is_in_table( ENV ) )
                {
                    // Check if E is in MarginalP table
                    if( !P.is_in_Marginal_Prob_Table( ENV ) )
                    {
                        // Compute Marginal Probabilities and save in table
                        P.add_Marginal_Prob( ENV, D.get_disc_vector( ENV ) );
                    }
                    // Check if pairwise nan
                    double num_pairwise_nan = get_num_pairwise_nan( D.get_disc_vector( ID ), D.get_disc_vector( ENV ) );
                    if( num_pairwise_nan/(samplesize-1)*100 > max_nan_threshold )
                    {
                        // Number of pairwise missing values exceeds the max allowed portion of missing values. MI = nan."
                    }
                    else
                    {
                        // Determine Joint Probability
                        P.add_Joint_Prob( ID, ENV, D.get_disc_vector( ID ), D.get_disc_vector( ENV ) );
                        // Determine MI
                        std::vector< double > empty_vector; // nothing_happens_with_it
                        MI.add_MI( ID, ENV, P.compute_MI( samplesize, ID, ENV, 0, empty_vector, empty_vector ) );
                    }
                }
            }
        }
    }
};
void Triplet::do_ENV_pre_permut(){
    // Permut ENV vector and safe
    std::vector< std::string > v = ID_ENV_EDGES.get_ENVs();
    for( int i = 0; i < v.size(); i++ )
    {
        P.add_permut_ENV( v[ i ], D.get_disc_vector( v[ i ] ) );
    }
};
void Triplet::do_pre_JointP_comp( double max_nan_threshold ){
    // Compute Joint Probability for ID-ENV with permuted ENV vectors
    std::unordered_map< std::string, std::unordered_map< std::string, std::string > > V = ID_ENV_EDGES.get_table();
    for( std::unordered_map< std::string, std::unordered_map< std::string, std::string > >::iterator ID = V.begin(); ID != V.end(); ID++ )
    {
        for( std::unordered_map< std::string, std::string >::iterator ENV = V[ ID->first ].begin(); ENV != V[ ID->first ].end(); ENV++ )
        {
            // Check if pairwise nan
            double num_pairwise_nan = get_num_pairwise_nan( D.get_disc_vector( ID->first ), D.get_disc_vector( ENV->first ) );
            if( num_pairwise_nan/(D.get_disc_vector( ID->first ).size()-1)*100 > max_nan_threshold )
            {
                // No MI exist, therefore also no rand needed!
            }
            else
            {
                P.add_rand_JointP( ID->first, ENV->first, D.get_disc_vector( ID->first ) );
            }
        }
    }
};

// Get info
int Triplet::get_error_occurred(){
    return error_occurred;
};
std::string Triplet::get_error_message(){
    return error_message;
};
std::string Triplet::get_log_message(){
    return log_message;
};
std::string Triplet::get_tracker_report(){
    return TRACKER.get_Tracker_report();
};
double Triplet::get_mi( std::string X, std::string Y, double max_nan_threshold )
{
    double mi = nan("");
    if( D.is_in_table( X ) )
    {
        if( D.is_in_table( Y )  )
        {
            double samplesize = D.get_disc_vector( X ).size() + 1;
            double num_pairwise_nan = get_num_pairwise_nan( D.get_disc_vector( X ), D.get_disc_vector( Y ) );
            double portion_nan = ( num_pairwise_nan / ( samplesize - 1 ) ) * 100;
            if( portion_nan > max_nan_threshold )
            {
                /*log_message += "\nNumber of pairwise missing values for the vectors for ";
                log_message += X;
                log_message += " and ";
                log_message += Y;
                log_message += "is ";
                log_message += std::to_string( num_pairwise_nan );
                log_message += " which exceeds the max allowed portion of missing values. ";
                log_message += std::to_string( portion_nan );
                log_message += " > ";
                log_message += std::to_string( max_nan_threshold );
                log_message += ". MI is set to nan.";*/
            }
            else
            {
                mi = P.compute_MI( samplesize, X, Y, 1, D.get_disc_vector( X ), D.get_disc_vector( Y ) );
            }

        }
    }
    return mi;
}
double Triplet::get_co_occurrence( std::string X, std::string Y )
{
    double co = nan("");
    if( D.is_in_table( X ) )
    {
        if( D.is_in_table( Y )  )
        {
            std::vector< double > v = CO.get_co_occurrence( D.get_disc_vector( X ), D.get_disc_vector( Y ) );
            if( v[0] > -1 )
            {
                co = v[ 4 ];
            }
        }
    }
    return co;
}
std::vector< double > Triplet::get_co_occurrence_vector( std::string X, std::string Y )
{
    std::vector< double > co;
    if( D.is_in_table( X ) )
    {
        if( D.is_in_table( Y )  )
        {
            co = CO.get_co_occurrence( D.get_disc_vector( X ), D.get_disc_vector( Y ) );
        }
        else
        {
            co.push_back( nan("") );
        }
    }
    else
    {
        co.push_back( nan("") );
    }
    return co;
}

int Triplet::get_indicator_triplet_edge(){
    return indicator_triplet_edge;
};
double Triplet::get_num_pairwise_nan( std::vector< double > ID, std::vector< double > ENV ){
    double num_pairwise_nan = 0;
    for( int i = 0; i < ID.size(); i++ )
    {
        if( ID[ i ] >= 0 && ENV[ i ] >= 0 )
        {
            // Do nothing
        }
        else
        {
            num_pairwise_nan += 1;
        }
    }
    return num_pairwise_nan;
};
double Triplet::get_num_threewise_nan( std::vector< double > X, std::vector< double > Y, std::vector< double > E ){
    double num_threewise_nan = 0;
    for( int i = 0; i < X.size(); i++ )
    {
        if( X[ i ] >= 0 && Y[ i ] >= 0 && ( E[ i ] >= 0 || E[ i ] <= 0 ) )
        {
            // Do nothing
        }
        else
        {
            num_threewise_nan += 1;
        }
    }
    return num_threewise_nan;
};

// Reset info
void Triplet::reset_log_message(){
    log_message = "";
};
void Triplet::reset_triplet_info_message(){
    triplet_info_message = "";
};

// Print Output
std::string Triplet::print_header_nw( std::string s ){
    return OUT.get_extended_nw_header( s );
};
void Triplet::print_ID_ENV_EDGES( std::string s ){
    std::ofstream OUTPUT_NW( s, std::ios::out|std::ios::app );

    // Get ID-ENV table and for each print to output
    // ID-ENV edges
    std::unordered_map< std::string, std::unordered_map< std::string, std::string > > ID_ENV_TABLE = ID_ENV_EDGES.get_table();

    for( std::unordered_map< std::string, std::unordered_map< std::string, std::string > >::iterator ID = ID_ENV_TABLE.begin(); ID != ID_ENV_TABLE.end(); ID++ )
    {
        for( std::unordered_map< std::string, std::string >::iterator ENV = ID_ENV_TABLE[ ID->first ].begin(); ENV != ID_ENV_TABLE[ ID->first ].end(); ENV++ )
        {
            double mi;
            if( ( use_II || use_DPI ) && MI.is_in_MI_table( ID->first, ENV->first ) )
            {
                mi = MI.get_MI( ID->first, ENV->first );
            }
            else
            {
                mi = nan("");
            }
            double co = nan(""); // Co-occurrence only calculated for ID-ID edges
            int num_triplets = ID_ENV_EDGES.get_num_triplets( ID->first, ENV->first ); // Number of triplets the ID-ENV edge is in
            OUTPUT_NW << OUT.get_no_triplet_edge( ID_ENV_TABLE[ ID->first ][ ENV->first ], num_triplets, mi, co ) << std::endl;
        }
    }
    OUTPUT_NW.close();
};
std::string Triplet::print_extended_nw_edge( std::string line, int col_x, int col_y, std::string ENV_indicator, double max_nan_threshold ){
    // Outputline
    std::string outputline = "";

    // remove "newline" sign if there is one at the end of the line
    size_t found = line.find( '\n' );
    if( found != std::string::npos ){ line = line.substr(0, line.size()-1); }
    found = line.find( '\r' );
    if( found != std::string::npos ){ line = line.substr(0, line.size()-1); }

    // Split line by tab
    std::vector< std::string > v;
    split( v, line, boost::is_any_of( sep ) ); // Split by sep_nw, vector positions starts with 0!

    std::string X = v[ col_x ];
    std::string Y = v[ col_y ];

    // Check if it is an ID-ID or ENV-ENV edge
    size_t X_is_ENV = X.find( ENV_indicator );
    size_t Y_is_ENV = Y.find( ENV_indicator );
    if(X_is_ENV != std::string::npos)
    {
        if(Y_is_ENV != std::string::npos)
        {
            // ENV-ENV edge
            TRACKER.add_plus1_Tracker_num_ENV_ENV();
            double co = nan(""); // Co-occurrence only calculated for ID-ID edges
            outputline = OUT.get_no_triplet_edge( line, 0, get_mi( X, Y, max_nan_threshold ), co );
        }
        // else ID_ENV edge, ignore now!
    }
    else
    {
        if( Y_is_ENV != std::string::npos ){ } // ID-ENV edge, ignore now!
        else
        {
            // ID-ID edge
            TRACKER.add_plus1_Tracker_num_ID_ID();
            outputline = indirect_edge_detection( line, X, Y, max_nan_threshold ); // Indirect Edge Detection
        }
    }
    return outputline;
};
std::string Triplet::get_triplet_info_header( std::string X, std::string Y, std::string ENV ){
    return OUT.get_triplet_info_header( X, Y, ENV );
};
std::string Triplet::print_triplet_info(){
    return triplet_info_message;
};

void Triplet::print_triplet_info_ID_ENV_EDGES( std::string s ){
    std::ofstream OUTPUT_TRIPLET_INFO( s, std::ios::out|std::ios::app );
    // Get ID-ENV table and if edge in triplet print to triplet output
    // ID-ENV edges
    std::unordered_map< std::string, std::unordered_map< std::string, std::string > > ID_ENV_TABLE = ID_ENV_EDGES.get_table();

    for( std::unordered_map< std::string, std::unordered_map< std::string, std::string > >::iterator ID = ID_ENV_TABLE.begin(); ID != ID_ENV_TABLE.end(); ID++ )
    {
        for( std::unordered_map< std::string, std::string >::iterator ENV = ID_ENV_TABLE[ ID->first ].begin(); ENV != ID_ENV_TABLE[ ID->first ].end(); ENV++ )
        {
            if( ID_ENV_EDGES.get_num_triplets( ID->first, ENV->first ) > 0 )
            {
                double mi;
                double portion_nan_XY = nan("");
                if( ( use_II || use_DPI ) && MI.is_in_MI_table( ID->first, ENV->first ) )
                {
                    portion_nan_XY = get_num_pairwise_nan( D.get_disc_vector( ID->first ), D.get_disc_vector( ENV->first ) );
                    mi = MI.get_MI( ID->first, ENV->first );
                }
                else
                {
                    mi = nan("");
                }
                std::vector< double > co;
                co.push_back( nan("") );
                OUTPUT_TRIPLET_INFO << OUT.get_triplet_info( ID->first, ENV->first, ID_ENV_EDGES.get_num_triplets( ID->first, ENV->first ), "nan", "nan", "nan", mi, "nan", "nan", "nan", "nan", "nan", ID_ENV_EDGES.get_DPI_rank_num( ID->first, ENV->first, 1 ),  ID_ENV_EDGES.get_DPI_rank_num( ID->first, ENV->first, 2 ),  ID_ENV_EDGES.get_DPI_rank_num( ID->first, ENV->first, 3 ), "nan", portion_nan_XY, "nan", co ) << std::endl;

                // Track number of ID-ENV edges that are in at least one triplet
                TRACKER.add_plus1_Tracker_num_ID_ENV_in_triplet();
            }
        }
    }
    OUTPUT_TRIPLET_INFO.close();
};
std::string Triplet::get_rand_II_header( std::string X, std::string Y, std::string ENV, int num_permut ){
    return OUT.get_rand_II_header( X, Y, ENV, num_permut );
};

// Indirect Edge Detection
std::string Triplet::indirect_edge_detection( std::string line, std::string X, std::string Y, double max_nan_threshold ){
    std::string outputline;
    indicator_triplet_edge = 0;
    // Check if ID-ID is in triplet -> obtain list of ENV
    std::vector< std::string > ENVs = ID_ENV_EDGES.get_triplet_ENVs( X, Y );

    if( ENVs.size() > 0 )
    {
        // Reset (they need to be double, so they can be "nan" if needed!)
        double sp = 1;
        double ol = 0;
        double mi = get_mi( X, Y, max_nan_threshold );
        double ii = std::numeric_limits<double>::infinity();
        int bool_ii_got_update = 0; // to indicate whether or not ii got updated, if not it is still infinity and should be set to "nan"
        double ii_p = nan("");
        double dpi = 3;
        double dpi_indirect = 1;
        double combi;
        double portion_nan_XY = nan("");
        std::vector< double > co = get_co_occurrence_vector( X, Y );

        // Tracker
        indicator_triplet_edge = 1;
        TRACKER.add_plus1_Tracker_num_ID_ID_in_triplet();
        if( indicator_output_triplet )
        {
            TRACKER.track_triplet_ENVs( ENVs );
        }
        // indirect edge detection
        // For each ENV-ID-ID triplet determine if ID-ID edge is indirect
        double triplet_indirect_count = 0;
        for( int i = 0; i < ENVs.size(); i++ )
        {
            // Combination of methods
            double combi_new;
            double method_indirect_count = 0;

            // if all methods are nan(""), then combi also nan("")
            double count_methods_na = 0;
            // Sign Pattern
            if( use_SP )
            {
                double sp_new = SP.get_sign_pattern( ID_ENV_EDGES.get_ID_ENV_line( X, ENVs[ i ] ), ID_ENV_EDGES.get_ID_ENV_line( Y, ENVs[ i ] ), line );
                if( sp_new == 0 )
                {
                    // SP indirect
                    sp = 0;
                    method_indirect_count++;
                }
                else if( sp_new == 1 )
                {
                    // SP not indirect
                }
                else
                {
                    count_methods_na++;
                }

                // Tracker
                if( indicator_output_triplet )
                {
                    TRACKER.track_triplet_sp( sp_new );
                }
            }

            // Overlap
            if( use_OL )
            {
                double ol_new = OL.get_overlap( ID_ENV_EDGES.get_ID_ENV_line( X, ENVs[ i ] ), ID_ENV_EDGES.get_ID_ENV_line( Y, ENVs[ i ] ), line );
                if( ol_new >= 0 )
                {
                    if( ol_new > ol )
                    {
                        ol = ol_new;
                    }
                    if( ol_new >= OL_threshold )
                    {
                        // OL indirect
                        method_indirect_count++;
                    }
                }
                else
                {
                    count_methods_na++;
                }
                // Tracker
                if( indicator_output_triplet )
                {
                    TRACKER.track_triplet_ol( ol_new );
                }
            }
            // Interaction Information
            if( use_II )
            {
                double samplesize = D.get_disc_vector( X ).size() + 1;
                //double mi = get_mi( X, Y, max_nan_threshold );
                double cmi;
                double ii_new;
                double ii_p_new;

                double num_threewise_nan = get_num_threewise_nan( D.get_disc_vector( X ), D.get_disc_vector( Y ), D.get_disc_vector( ENVs[ i ] ) );
                if( num_threewise_nan/(samplesize-1)*100 > max_nan_threshold )
                {
                    // Number of threewise missing values exceeds the max allowed portion of missing values. II = nan.
                    cmi = nan("");
                    ii_new = nan("");
                    ii_p_new = nan("");

                    count_methods_na++;
                }
                else
                {
                    cmi = P.compute_CMI_XY( samplesize, X, Y, ENVs[ i ], D.get_disc_vector( X ), D.get_disc_vector( Y ), D.get_disc_vector( ENVs[ i ] ) );
                    ii_new = cmi - mi;
                    // Only compute significance when the interaction information is negative
                    if(ii_new < 0)
                    {
                      ii_p_new = P.compute_p_value( X, Y, ENVs[ i ], cmi, mi, D.get_disc_vector( X ), D.get_disc_vector( Y ), D.get_disc_vector( ENVs[ i ] ));
                    }
                    else
                    {
                      ii_p_new = nan("");
                    }

                    if( ( ii_new < ii && ii_p_new < II_Sign_Level ) || ( ii_p_new < II_Sign_Level && ii_p >= II_Sign_Level ) || ( !( ii_p >= 0 ) ) )
                    {
                        ii = ii_new;
                        ii_p = ii_p_new;
                        bool_ii_got_update = 1; // to indicate whether or not ii got updated, if not it is still infinity and should be set to "nan"
                    }

                    // II indirect
                    if( ii_new < 0 && ii_p_new < II_Sign_Level )
                    {
                        method_indirect_count++;
                    }
                }
                // Tracker
                if( indicator_output_triplet )
                {
                    TRACKER.track_portion_nan_XYENV( num_threewise_nan );
                    TRACKER.track_triplet_cmi( cmi );
                    TRACKER.track_triplet_ii( ii_new );
                    TRACKER.track_triplet_ii_pvalue( ii_p_new );
                }
            }
            // Data Processing Inequality
            //double mi = get_mi( X, Y, max_nan_threshold );
            if( use_DPI )
            {
                double dpi_new;
                double dpi_indirect_new;
                if( MI.get_MI( X, ENVs[ i ] ) >= 0 && MI.get_MI( Y, ENVs[ i ] ) >= 0 && mi >= 0 )
                {
                    dpi_new = DPI.get_mi_rank( MI.get_MI( X, ENVs[ i ] ), MI.get_MI( Y, ENVs[ i ] ), mi );
                    dpi_indirect_new = DPI.edge_indirect( MI.get_MI( X, ENVs[ i ] ), MI.get_MI( Y, ENVs[ i ] ), mi, DPI_minMI_threshold );

                    if( ( dpi_new < dpi && dpi_indirect_new == 0 ) || ( dpi_new < dpi && dpi_indirect == 1 ) )
                    {
                        dpi = dpi_new;
                        dpi_indirect = dpi_indirect_new;
                    }

                    // DPI indirect and min MI threshold!!!!!
                    if ( dpi_new == 1 && dpi_indirect_new == 0 )
                    {
                        method_indirect_count++;
                    }

                    // track MI rank for ID-ENV edge if wanted
                    if( indicator_output_triplet )
                    {
                        // XE edge
                        ID_ENV_EDGES.add_plus1_Tracker_dpi_rank( X, ENVs[i], DPI.get_mi_rank( mi, MI.get_MI( Y, ENVs[ i ] ), MI.get_MI( X, ENVs[ i ] ) ) );
                        // YE edge
                        ID_ENV_EDGES.add_plus1_Tracker_dpi_rank( Y, ENVs[i], DPI.get_mi_rank( mi, MI.get_MI( X, ENVs[ i ] ), MI.get_MI( Y, ENVs[ i ] ) ) );
                    }
                }
                else
                {
                    dpi = nan("");

                    count_methods_na++;
                }

                // Tracker
                if( indicator_output_triplet )
                {
                    TRACKER.track_triplet_dpi( dpi_new ); // here count number of rank1/2/3
                    TRACKER.track_triplet_dpi_indirect( dpi_indirect_new );
                }
            }

            // Combination
            if( (use_SP + use_OL + use_II + use_DPI) > 1 )
            {
                if( (use_SP + use_OL + use_II + use_DPI) == count_methods_na )
                {
                    combi_new = nan("");
                }
                else
                {
                    double num_methods = use_SP + use_OL + use_II + use_DPI - count_methods_na;
                    double method_indirect_percentage = ( method_indirect_count / num_methods ) * 100;
                    if( method_indirect_percentage >= method_count_threshold )
                    {
                        combi_new = 0;
                        triplet_indirect_count++;
                    }
                    else
                    {
                        combi_new = 1;
                    }
                }

                // Tracker
                if( indicator_output_triplet )
                {
                    TRACKER.track_triplet_combi( combi_new );
                }
            }


            // Tracker
            if( indicator_output_triplet )
            {
                // Track the number of triplets an ID-ENV edge is in
                ID_ENV_EDGES.add_plus1_Tracker_num_triplets( X, Y, ENVs[ i ] );
            }
        }
        // if bool_ii_got_update == 0, then ii was not updated and is still infinity and should be set to "nan"
        if( bool_ii_got_update == 0 )
        {
            // means ii didnt get an update
            // instead of having it at inf, we put it to nan("")
            ii = nan("");
        }
        if( use_II || use_DPI )
        {
            portion_nan_XY = get_num_pairwise_nan(D.get_disc_vector( X ), D.get_disc_vector( Y ));
        }

        // Combi
        double num_triplets = ENVs.size();
        double triplet_indirect_percentage = ( triplet_indirect_count / num_triplets ) * 100;
        if( triplet_indirect_percentage >= triplet_count_threshold )
        {
            combi = 0;
        }
        else
        {
            combi = 1;
        }

        // Output
        double co_out = nan("");
        if( co[0] > -1 )
        {
            co_out = co[4];
        }
        outputline = OUT.get_triplet_edge( line, ENVs.size(), sp, ol, mi, ii, ii_p, dpi, dpi_indirect, combi, co_out );

        // Track info (number indirect, not indirect)
        // for ID-ID edge that is in at least one triplet
        if( use_SP )
        {
            if( sp == 0 )
            {
                TRACKER.add_plus1_Tracker_SP_num_indirect();
            }
            else // sp == 1 || nan
            {
                TRACKER.add_plus1_Tracker_SP_num_not_indirect();
            }
        }
        if( use_OL )
        {
            if( ol >= OL_threshold )
            {
                TRACKER.add_plus1_Tracker_OL_num_indirect();
            }
            else // ol < OL_threshold || nan
            {
                TRACKER.add_plus1_Tracker_OL_num_not_indirect();
            }
        }
        if( use_II )
        {
            if( ii < 0 && ii_p < II_Sign_Level )
            {
                TRACKER.add_plus1_Tracker_II_num_indirect();
            }
            else // ii > 0 || ii_p > II_Sign_Level || nan
            {
                TRACKER.add_plus1_Tracker_II_num_not_indirect();
            }
        }
        if( use_DPI )
        {
            if ( dpi == 1 && dpi_indirect == 0 )
            {
                TRACKER.add_plus1_Tracker_DPI_num_indirect();
            }
            else // dpi > 0 || nan
            {
                TRACKER.add_plus1_Tracker_DPI_num_not_indirect();
            }
        }
        if( ( use_SP + use_OL + use_II + use_DPI ) > 1 )
        {
            if( combi == 0 )
            {
                TRACKER.add_plus1_Tracker_Combi_num_indirect();
            }
            else // combi == 1 || nan
            {
                TRACKER.add_plus1_Tracker_Combi_num_not_indirect();
            }
        }

        // Set triplet info
        if( indicator_output_triplet )
        {
            triplet_info_message = OUT.get_triplet_info( X, Y, ENVs.size(), TRACKER.get_Tracker_triplet_ENVs(), TRACKER.get_Tracker_triplet_sp(), TRACKER.get_Tracker_triplet_ol(), mi, TRACKER.get_Tracker_triplet_cmi(), TRACKER.get_Tracker_triplet_ii(), TRACKER.get_Tracker_triplet_ii_pvalue(), TRACKER.get_Tracker_triplet_dpi(), TRACKER.get_Tracker_triplet_dpi_indirect(), TRACKER.get_Tracker_triplet_num_rank_1(), TRACKER.get_Tracker_triplet_num_rank_2(), TRACKER.get_Tracker_triplet_num_rank_3(), TRACKER.get_Tracker_triplet_combi(), portion_nan_XY, TRACKER.get_Tracker_portion_nan_XYENV(), co );
            TRACKER.tracker_triplet_reset();
        }
    }
    else
    {
        TRACKER.add_plus1_Tracker_num_ID_ID_not_in_triplet();
        double co;
        if( use_CO )
        {
            co = get_co_occurrence( X, Y );
        }
        else
        {
            co = nan("");
        }
        outputline = OUT.get_no_triplet_edge( line, 0, get_mi( X, Y, max_nan_threshold ), co );

        // Track info (number indirect, not indirect)
        // all not indirect because it is ID-ID edge that is not in triplet
        if( use_SP )
        {
            TRACKER.add_plus1_Tracker_SP_num_not_indirect();
        }
        if( use_OL )
        {
            TRACKER.add_plus1_Tracker_OL_num_not_indirect();
        }
        if( use_II )
        {
            TRACKER.add_plus1_Tracker_II_num_not_indirect();
        }
        if( use_DPI )
        {
            TRACKER.add_plus1_Tracker_DPI_num_not_indirect();
        }
        if( ( use_SP + use_OL + use_II + use_DPI ) > 1 )
        {
            TRACKER.add_plus1_Tracker_Combi_num_not_indirect();
        }
    }

    return outputline;
};
