#ifndef tracker_Hpp
#define tracker_Hpp

#include <string> // operator + for string concatenation std::stoi std::stod
#include <vector> // vector

class Tracker{
    public:
        // Constructor
        Tracker();

        // Add info
        void set_Tracker_num_ID_ENV_considered( int i );
        void set_Tracker_num_ID_ENV_all( int i );
        void set_Tracker_num_ID_connected_to_min_one_ENV( int i );
        void set_Tracker_num_ENV_connected_to_min_one_ID( int i );
        void add_plus1_Tracker_num_ID_ENV_in_triplet();
        void add_plus1_Tracker_num_ID_ID();
        void add_plus1_Tracker_num_ENV_ENV();
        void add_plus1_Tracker_num_ID_ID_in_triplet();
        void add_plus1_Tracker_num_ID_ID_not_in_triplet();

        void add_plus1_Tracker_SP_num_indirect();
        void add_plus1_Tracker_SP_num_not_indirect();

        void add_plus1_Tracker_OL_num_indirect();
        void add_plus1_Tracker_OL_num_not_indirect();

        void add_plus1_Tracker_II_num_indirect();
        void add_plus1_Tracker_II_num_not_indirect();

        void add_plus1_Tracker_DPI_num_indirect();
        void add_plus1_Tracker_DPI_num_not_indirect();

        void add_plus1_Tracker_Combi_num_indirect();
        void add_plus1_Tracker_Combi_num_not_indirect();

        // Track triplet info for triplet_info string
        void track_triplet_ENVs( std::vector< std::string > v );
        void track_triplet_sp( double d );
        void track_triplet_ol( double d );
        void track_triplet_cmi( double d );
        void track_triplet_ii( double d );
        void track_triplet_ii_pvalue( double d );
        void track_triplet_dpi( double d ); // here count number of rank1/2/3
        void track_triplet_dpi_indirect( double d ); // here if MI is smaller than other MI - threshold, then indirect, otherwise direct
        void track_triplet_combi( double d );
        void track_portion_nan_XYENV( double d );

        // Tracker_triplet Reset
        void tracker_triplet_reset();

        // Get info
        std::string get_Tracker_triplet_ENVs();
        std::string get_Tracker_triplet_sp();
        std::string get_Tracker_triplet_ol();
        std::string get_Tracker_triplet_cmi();
        std::string get_Tracker_triplet_ii();
        std::string get_Tracker_triplet_ii_pvalue();
        std::string get_Tracker_triplet_dpi();
        std::string get_Tracker_triplet_dpi_indirect();
        int get_Tracker_triplet_num_rank_1();
        int get_Tracker_triplet_num_rank_2();
        int get_Tracker_triplet_num_rank_3();
        std::string get_Tracker_triplet_combi();
        std::string get_Tracker_portion_nan_XYENV();

        // Get output string
        std::string get_Tracker_report();

	private:
        std::string Tracker_triplet_ENVs;
        std::vector< double > Tracker_triplet_sp;
        std::vector< double > Tracker_triplet_ol;
        std::vector< double > Tracker_triplet_cmi;
        std::vector< double > Tracker_triplet_ii;
        std::vector< double > Tracker_triplet_ii_pvalue;
        std::vector< double > Tracker_triplet_dpi;
        std::vector< double > Tracker_triplet_dpi_indirect;
        double Tracker_triplet_num_rank_1;
        double Tracker_triplet_num_rank_2;
        double Tracker_triplet_num_rank_3;
        std::vector< double > Tracker_triplet_combi;
        std::vector< double > Tracker_portion_nan_XYENV;

        int Tracker_num_ID_ENV_considered;
        int Tracker_num_ID_ENV_all;
        int Tracker_num_ID_ENV_in_triplet;
        int Tracker_num_ID_ID;
        int Tracker_num_ENV_ENV;
        int Tracker_num_ID_ID_in_triplet;
        int Tracker_num_ID_connected_to_min_one_ENV;
        int Tracker_num_ENV_connected_to_min_one_ID;
        int Tracker_num_ID_ID_not_in_triplet;

        int Tracker_SP_num_indirect;
        int Tracker_SP_num_not_indirect;

        int Tracker_OL_num_indirect;
        int Tracker_OL_num_not_indirect;

        int Tracker_II_num_indirect;
        int Tracker_II_num_not_indirect;

        int Tracker_DPI_num_indirect;
        int Tracker_DPI_num_not_indirect;

        int Tracker_Combi_num_indirect;
        int Tracker_Combi_num_not_indirect;
};

#endif
