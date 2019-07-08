#ifndef triplet_Hpp
#define triplet_Hpp

#include "disc_vectors.hpp"
#include "ID_ENV_edges.hpp"
#include "prob.hpp"
#include "mutual_information.hpp"
#include "sign_pattern.hpp"
#include "overlap.hpp"
#include "co_occurrence.hpp"
#include "data_processing_inequality.hpp"
#include "output_extended_nw.hpp"
#include "tracker.hpp"

#include <string>
#include <vector>                     // vector
#include <unordered_map>              // unordered_maps
#include <boost/algorithm/string.hpp> // split


class Triplet{
    public:
        // Constructor
        Triplet();

        // Set info
        void set_methods( int use_sp, int use_ol, int use_ii, int use_dpi, int use_co, double set_method_count_threshold, double set_triplet_count_threshold );
        void set_sep_nw( std::string s );
        void set_filename_output_nw( std::string s );
        void set_permut_seed();
        void set_sign_pattern_parameters( int col_score );
        void set_overlap_parameters( int col_len, int col_startX, int col_startY, double threshold );
        void set_data_processing_inequality_parameter( double d );
        void set_interaction_information_significance_determination_parameters( int num_permut, double significance_level, int indicator_pre_permut );
        void set_extra_output_info( int indicator_triplet );

        // Built up datastructure
        void build_disc_table( std::string filename_abundance, std::string filename_ENV, std::string sep, int indicator_output_discretized_vectors, std::string filename_output_discretized_vectors, double max_nan_threshold, std::string filename_output_logfile );
        void build_ID_ENV_EDGES_table( std::string filename_ID_ENV_EDGES, std::string sep, int col_x, int col_y, std::string ENV_indicator, int pre_permut );
        void build_MI_ID_ENV_table( double max_nan_threshold );
        void do_ENV_pre_permut();
        void do_pre_JointP_comp( double max_nan_threshold );

        // Get info
        int get_error_occurred();
        std::string get_error_message();
        std::string get_log_message();
        std::string get_tracker_report();
        double get_mi( std::string X, std::string Y, double max_nan_threshold );
        double get_co_occurrence( std::string X, std::string Y );
        std::vector< double > get_co_occurrence_vector( std::string X, std::string Y );
        int get_indicator_triplet_edge();
        double get_num_pairwise_nan( std::vector< double > O, std::vector< double > E );
        double get_num_threewise_nan( std::vector< double > X, std::vector< double > Y, std::vector< double > E );

        // Rest info
        void reset_log_message();
        void reset_triplet_info_message();

        // Print Output
        std::string print_header_nw( std::string s );
        void print_ID_ENV_EDGES( std::string );
        std::string print_extended_nw_edge( std::string line, int col_x, int col_y, std::string ENV_indicator, double max_nan_threshold );
        std::string get_triplet_info_header( std::string X, std::string Y, std::string ENV );
        std::string print_triplet_info();
        void print_triplet_info_ID_ENV_EDGES( std::string );
        std::string get_rand_II_header( std::string X, std::string Y, std::string ENV, int num_permut );

        // Indirect Edge Detection
        std::string indirect_edge_detection( std::string line, std::string X, std::string Y, double max_nan_threshold );

	private:
        // Tracking
        int error_occurred;
        std::string error_message;
        std::string log_message;
        std::string triplet_info_message;
        int indicator_triplet_edge;

        // Filename
        std::string filename_output_nw;
        std::string sep;

        // Methods
        int use_SP;
        int use_OL;
        int use_II;
        int use_DPI;
        int use_CO;

        // Methods Settings
        double OL_threshold;
        double II_Sign_Level;
        double DPI_minMI_threshold;
        double method_count_threshold;
        double triplet_count_threshold;

        // Extra output indicator
        int indicator_output_triplet;

        // Other Datastructures
        Disc_vectors D;
        ID_ENV_edges ID_ENV_EDGES;
        Output_extended_nw OUT;
        Prob P;
        Mutual_information MI;
        Sign_pattern SP;
        Overlap OL;
        DataProcessingInequality DPI;
        CoOccurrence CO;
        Tracker TRACKER;
};

#endif
