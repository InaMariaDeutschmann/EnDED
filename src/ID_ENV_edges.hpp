#ifndef ID_ENV_edges_Hpp
#define ID_ENV_edges_Hpp

#include <string>
#include <vector>
#include <unordered_map>

class ID_ENV_edges{
    public:
        // Constructor
        ID_ENV_edges();

        // Get info
        std::string get_log_message();
        int is_in_table( std::string ID, std::string ENV );
        std::string get_ID_ENV_line( std::string ID, std::string ENV );
        std::unordered_map< std::string, std::unordered_map< std::string, std::string > > get_table();
        std::vector< std::string > get_triplet_ENVs( std::string X, std::string Y );
        int get_num_ID_connected_to_min_one_ENV();
        int get_num_ENV_connected_to_min_one_ID();
        int get_num_ID_ENV_considered();
        int get_num_ID_ENV_all();
        std::vector< std::string > get_ENVs();
        int in_DPI_rank_table( std::string ID, std::string ENV, int rank );
        int get_DPI_rank_num( std::string ID, std::string ENV, int rank );
        int get_num_triplets( std::string ID, std::string ENV);

        // Tracker
        void add_plus1_Tracker_num_triplets( std::string X, std::string Y, std::string ENV );
        void add_plus1_Tracker_dpi_rank( std::string ID, std::string ENV, int rank );

        // Build up ID-ENV EDGES table
        void build_ID_ENV_EDGES_table( int col_x, int col_y, std::string ENV_indicator, int pre_permut, std::string filename_ID_ENV_EDGES, std::string sep );


	private:
        // ID_ENV edges
        // ID->ENV->line
        std::unordered_map< std::string, std::unordered_map< std::string, std::string > > ID_ENV_table;

        // DPI rank tracker
        std::unordered_map< std::string, std::unordered_map< std::string, std::unordered_map< int, int > > > ID_ENV_DPI_rank_table_AND_rank0_num_triplets;

        // ENVs
        std::vector< std::string > ENVs;

        // Tracker
        std::string log_message;
        int Tracker_num_ID_ENV_considered;
        int Tracker_num_ID_ENV_all;
        int Tracker_num_ID;

        // Get info
        std::vector< std::string > is_ID_ENV_edge( std::string X, std::string Y, std::string ENV ); // Check if it is ID-ENV edge
        int is_ID_in_table( std::string s ); // Check if ID is in table

        // Save edge
        void save_edge( std::string ID, std::string ENV, std::string l);

};

#endif
