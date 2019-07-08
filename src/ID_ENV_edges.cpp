#include "ID_ENV_edges.hpp"

#include <iostream>
#include <fstream>                    // read in and out files
#include <boost/algorithm/string.hpp> // split
#include <math.h>                     // sqrt, floor, ceil, log, isnan
#include <algorithm>                  // find

ID_ENV_edges::ID_ENV_edges(){
    // ID_ENV edgs
    std::unordered_map< std::string, std::unordered_map< std::string, std::string > > ID_ENV_table;

    // ENVs
    std::vector< std::string > ENVs;

    // Tracker
    log_message = "";
    Tracker_num_ID_ENV_considered = 0;
    Tracker_num_ID_ENV_all = 0;
    Tracker_num_ID = 0;
};

// Get info
std::string ID_ENV_edges::get_log_message(){
    return log_message;
};
int ID_ENV_edges::is_in_table( std::string ID, std::string ENV ){
    std::unordered_map< std::string, std::unordered_map< std::string, std::string > >::iterator i = ID_ENV_table.find( ID );
    if( i == ID_ENV_table.end() )
    {
        return 0;
    }
    else
    {
        std::unordered_map< std::string, std::string >::iterator j = ID_ENV_table[ ID ].find( ENV );
        if( j == ID_ENV_table[ ENV ].end() )
        {
            return 0;
        }
        else
        {

            return 1;
        }
    }
};
std::string ID_ENV_edges::get_ID_ENV_line( std::string ID, std::string ENV ){
    return ID_ENV_table[ ID ][ ENV ];
};
std::unordered_map< std::string, std::unordered_map< std::string, std::string > > ID_ENV_edges::get_table(){
    return ID_ENV_table;
};
std::vector< std::string > ID_ENV_edges::get_triplet_ENVs( std::string X, std::string Y ){
    // List of ENV that compose a triplet with X and Y
    std::vector< std::string > v;
    // Are X and Y in table
    if( is_ID_in_table( X ) && is_ID_in_table( Y ) )
    {
        // Have X and Y a ENV in common?
        for( std::unordered_map< std::string, std::string >::iterator ENV_of_X = ID_ENV_table[ X ].begin(); ENV_of_X != ID_ENV_table[ X ].end(); ENV_of_X++)
        {
            // check if ID_X_ENV is also a ENV in ID_Y
            std::unordered_map< std::string, std::string >::iterator ENV_of_Y = ID_ENV_table[ Y ].find( ENV_of_X->first );
            if( ENV_of_Y != ID_ENV_table[ Y ].end() )
            {
                v.push_back( ENV_of_Y->first );// ENV is in ID_X and in ID_Y, i.e. we have a triplet -> add ENV to Triplet_ENVs vector
            }
        }
    }
    return v;
};
int ID_ENV_edges::get_num_ID_connected_to_min_one_ENV(){
    return Tracker_num_ID;
};
int ID_ENV_edges::get_num_ENV_connected_to_min_one_ID(){
    return ENVs.size();
};
int ID_ENV_edges::get_num_ID_ENV_considered(){
    return Tracker_num_ID_ENV_considered;
};
int ID_ENV_edges::get_num_ID_ENV_all(){
    return Tracker_num_ID_ENV_all;
};
std::vector< std::string > ID_ENV_edges::get_ENVs(){
    return ENVs;
};
int ID_ENV_edges::in_DPI_rank_table( std::string ID, std::string ENV, int rank )
{
    std::unordered_map< std::string, std::unordered_map< std::string, std::unordered_map< int, int > > >::iterator ID_iterator = ID_ENV_DPI_rank_table_AND_rank0_num_triplets.find( ID );
    if( ID_iterator == ID_ENV_DPI_rank_table_AND_rank0_num_triplets.end() )
    {
        return 0;
    }
    else
    {
        std::unordered_map< std::string, std::unordered_map< int, int > >::iterator ENV_iterator = ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ ID ].find( ENV );
        if( ENV_iterator == ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ ID ].end() )
        {
            return 0;
        }
        else
        {
            std::unordered_map< int, int >::iterator i = ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ ID ][ ENV ].find( rank );
            if( i == ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ ID ][ ENV ].end() )
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
};
int ID_ENV_edges::get_DPI_rank_num( std::string ID, std::string ENV, int rank ){
    return ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ ID ][ ENV ][ rank ];
};
int ID_ENV_edges::get_num_triplets( std::string ID, std::string ENV){
    if( in_DPI_rank_table( ID, ENV, 0 ) )
    {
        return ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ ID ][ ENV ][ 0 ];
    }
    else
    {
        return 0;
    }

};

// Tracker
void ID_ENV_edges::add_plus1_Tracker_num_triplets( std::string X, std::string Y, std::string E ){
    // Check if edge is already in table
    if( in_DPI_rank_table( X, E, 0 ) )
    {
        ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ X ][ E ][ 0 ] = ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ X ][ E ][ 0 ] + 1;
    }
    else
    {
        ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ X ][ E ][ 0 ] = 1;
    }

    if( in_DPI_rank_table( Y, E, 0 ) )
    {
        ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ Y ][ E ][ 0 ] = ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ Y ][ E ][ 0 ] + 1;
    }
    else
    {
        ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ Y ][ E ][ 0 ] = 1;
    }
};

void ID_ENV_edges::add_plus1_Tracker_dpi_rank( std::string X, std::string E, int rank ){
    // Check if edge is already in table
    if( in_DPI_rank_table( X, E, rank ) )
    {
        ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ X ][ E ][ rank ] = ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ X ][ E ][ rank ] + 1;
    }
    else
    {
        ID_ENV_DPI_rank_table_AND_rank0_num_triplets[ X ][ E ][ rank ] = 1;
    }
};

// Build up ID-ENV edges table
void ID_ENV_edges::build_ID_ENV_EDGES_table( int col_x, int col_y, std::string ENV_indicator, int pre_permut, std::string filename_ID_ENV_EDGES, std::string sep ){
    // Read in edges from file
    std::ifstream IN( filename_ID_ENV_EDGES );
    if( IN.good() )
    {
        std::string l;
        // Read in header (1st line)
        if( getline( IN, l ).good() )
        {
            log_message += "\nReading in ";
            log_message += filename_ID_ENV_EDGES;
            log_message += " to get ID-";
            log_message += ENV_indicator;
            log_message += " edges";

            // First line is header

            while( getline( IN, l).good() )
            {
                // Remove newline signs
                size_t found = l.find( '\n' );
                if( found != std::string::npos )
                {
                    l = l.substr( 0, l.size() - 1 );
                }
                found = l.find( '\r' );
                if( found != std::string::npos )
                {
                    l = l.substr( 0, l.size() - 1 );
                }
                // Split string by separator
                std::vector< std::string > v;
                split( v, l, boost::is_any_of( sep ) );

                // Check if ID-ENV edge and if needed change so that it is vector< ID, ENV >
                std::vector< std::string > ID_ENV = is_ID_ENV_edge( v[ col_x ], v[ col_y ], ENV_indicator );

                if( ID_ENV.size() > 0 )
                {
                    Tracker_num_ID_ENV_all++;
                    // Check if edge already exist
                    if( is_in_table( ID_ENV[ 0 ], ID_ENV[ 1 ] ) )
                    {
                        log_message += "\n";
                        log_message += ID_ENV[ 0 ];
                        log_message += "-";
                        log_message += ID_ENV[ 1 ];
                        log_message += "is already in the table. The following line will be disregarded: ";
                        log_message += l;
                    }
                    else
                    {
                        Tracker_num_ID_ENV_considered++;
                        // Tracker IDs
                        if( is_ID_in_table(  ID_ENV[ 0 ] ) )
                        {
                            // std::cout << ID_ENV[ 0 ] << std::endl;
                        }
                        else
                        {
                            Tracker_num_ID++;
                        }
                        // Does not exist yet, safe in table
                        save_edge( ID_ENV[ 0 ], ID_ENV[ 1 ], l);
                        if( pre_permut )
                        {
                            // Add ENV to ENVs list
                            if(std::find(ENVs.begin(), ENVs.end(), ID_ENV[ 1 ]) != ENVs.end()) {
                                /* ENV already in vector */
                            } else {
                                /* Save ENV in vector */
                                ENVs.push_back( ID_ENV[ 1 ] );
                            }
                        }
                    }
                }
            }
        }
    }
    IN.close();
};

// Get info
// Check if it is ID-ENV edge
std::vector< std::string > ID_ENV_edges::is_ID_ENV_edge( std::string X, std::string Y, std::string E ){
    // Return vector
    std::vector< std::string > v;
    // Check if ID-ENV edge
    size_t X_is_ENV = X.find( E );
    size_t Y_is_ENV = Y.find( E );

    if( X_is_ENV != std::string::npos )
    {
        if( Y_is_ENV != std::string::npos )
        {
            // ENV-ENV edge, ignore for now
            return v;
        }
        else
        {
            // X=ENV, Y=ID
            // We want to save ID_ENV_EDGES, so we need to switch X and Y
            v.resize( 2 );
            v[ 0 ] = Y;
            v[ 1 ] = X;
            return v;
        }
    }
    else
    {
        if( Y_is_ENV != std::string::npos )
        {
            // ID-ENV edge
            // X=ID, Y=ENV
            // no need to switch
            v.resize( 2 );
            v[ 0 ] = X;
            v[ 1 ] = Y;
            return v;
        }
        else
        {
            // ID-ID edge, ignore for now!
            return v;
        }
    }
};
// Check if ID is in table
int ID_ENV_edges::is_ID_in_table( std::string O ){
    std::unordered_map< std::string, std::unordered_map< std::string, std::string > >::iterator i = ID_ENV_table.find( O );
    if( i == ID_ENV_table.end() )
    {
        return 0;
    }
    else
    {
        return 1;
    }
};

// Save edge
void ID_ENV_edges::save_edge( std::string ID, std::string ENV, std::string l){
    ID_ENV_table[ ID ][ ENV ] = l;
};
