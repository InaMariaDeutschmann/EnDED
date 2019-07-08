#include "disc_vectors.hpp"
#include "convert_to_string.hpp"

#include <fstream>                    // read in and out files
#include <boost/algorithm/string.hpp> // split
#include <math.h>                     // sqrt, floor, ceil, log, isnan
#include <cmath>                      // nan("")

Disc_vectors::Disc_vectors(){
    // Discretized vectors
    std::unordered_map< std::string, std::vector< double > > disc_vector_table;

    // Tracker
    error_occurred = 0;
    error_message = "";
};

// Get info
int Disc_vectors::get_error_occurred(){
    return error_occurred;
};
std::string Disc_vectors::get_error_message(){
    return error_message;
};
int Disc_vectors::is_in_table( std::string s ){
    std::unordered_map< std::string, std::vector< double > >::iterator i = disc_vector_table.find( s );
    if( i == disc_vector_table.end() )
    {
        return 0;
    }
    else
    {
        return 1;
    }
};
std::vector< double > Disc_vectors::get_disc_vector( std::string s ){
    return disc_vector_table[ s ];
};

// Build up discretized vectors
void Disc_vectors::build_disc_table( std::string filename_abundance, std::string filename_ENV, std::string sep, int indicator_output_discretized_vectors, std::string filename_output_discretized_vectors, double max_nan_threshold, std::string filename_output_logfile ){
    // Read in abundance file
    add_vectors_to_disc_table( filename_abundance, sep, indicator_output_discretized_vectors, filename_output_discretized_vectors, max_nan_threshold, filename_output_logfile );

    // If extra: Read in ENV parameter
    if( filename_ENV != "" )
    {
        // Read in ENV parameter
        add_vectors_to_disc_table( filename_ENV, sep, indicator_output_discretized_vectors, filename_output_discretized_vectors, max_nan_threshold, filename_output_logfile );
    }
};

void Disc_vectors::add_vectors_to_disc_table( std::string filename, std::string sep, int indicator_output_discretized_vectors, std::string filename_output_discretized_vectors, double max_nan_threshold, std::string filename_output_logfile ){
    // Read in vectors from file s
    std::ifstream IN( filename );
    // Append to log file
    std::ofstream toLog( filename_output_logfile, std::ios::out|std::ios::app );

    std::string header; // needed if extra file is printed out

    if( IN.good() )
    {
        std::string l;
        // Read in header (1st line)
        if( getline( IN, l ).good() )
        {
            toLog << "\nReading in " << filename << std::endl;
            header = l;

            while( getline( IN, l ).good() )
            {
                // Given a string, safe in disc_vector_table
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
                if( is_in_table( v[ 0 ] ) )
                {
                    toLog << v[ 0 ] << "is already in the table. The following line will be disregarded: " << l << std::endl;
                }
                else
                {
                    save_disc_vector( v, max_nan_threshold, l );
                }
            }
        }
    }
    IN.close();
    toLog.close();

    // Print out discretized vectors if choosen
    if( indicator_output_discretized_vectors )
    {
        print_disc_vectors( filename_output_discretized_vectors, header, sep );
    }
};

void Disc_vectors::save_disc_vector( std::vector< std::string > v, double max_nan_threshold, std::string line ){
    std::vector< double > current_abundances;
    std::vector< double > disc_abundances;

    // remember max and min for discretization
    double MIN = std::numeric_limits<double>::infinity();
    double MAX = -std::numeric_limits<double>::infinity();

    // memorize if vector contains missing value(s)
    int tracker_num_nan = 0;
    int tracker_num_missing_value = 0;

    // convert to double, and detect missing values
    for( int j = 1; j < v.size(); j++ ) // why 1---> at "0" is the name and starting from 1 is the abundance.
    {
        if( v[ j ].length() > 0 )
        {
            std::string lowercase_s = boost::algorithm::to_lower_copy( v[ j ] );

            if( lowercase_s == "na" || lowercase_s == "nan")
            {
                double filler = nan("");
                current_abundances.push_back( filler );
                tracker_num_nan++;
            }
            else
            {
                double abundance = stod( v[ j ] );
                current_abundances.push_back( abundance );

                // track MIN and MAX
                if( MAX < abundance )
                {
                    MAX = abundance;
                }
                if( MIN > abundance )
                {
                    MIN = abundance;
                }
            }
        }
        else
        {
            error_occurred = 1;
            tracker_num_missing_value++;
            // Below gives segmentation fault if too many values are missing
            // error_message += "\nMissing value for ";
            // error_message += v[ 0 ];
            // error_message += " in column ";
            // error_message += std::to_string( j + 1 );
            // error_message += ".\n";
        }
    }
    if( tracker_num_nan > 0 )
    {
        error_message += std::to_string( tracker_num_nan );
        error_message += " nan value/s for ";
        error_message += v[ 0 ];
        error_message += "!\n";
    }
    if( tracker_num_missing_value > 0 )
    {
        error_message += std::to_string( tracker_num_missing_value );
        error_message += " missing value/s for ";
        error_message += v[ 0 ];
        error_message += ". Please indicate missing values with \"nan\".\n\n";
    }
    if( ( tracker_num_nan / current_abundances.size() * 100 ) > max_nan_threshold )
    {
        error_message += "Percentage of nans in vector exceeds threshold";
        error_message += std::to_string( ( tracker_num_nan / current_abundances.size() * 100 ) );
        error_message += " > ";
        error_message += std::to_string( max_nan_threshold );
        error_message += "!\nThe following line will be disregarded: ";
        error_message += line;
        error_message += "!\n\n";
    }
    else
    {
        // determine discretized abundance vector
        double bin_num = floor( sqrt( current_abundances.size() ) ); // actual bin_num is one more: one extra bin for values = 0!
        // in case there have been negative values, adapt MAX
        // can happen for ENV parameters like temperature
        // will not affect count data as here MIN >= 0
        if( MIN < 0 )
        {
            MAX = MAX - MIN;
        }
        for( int i = 0; i < current_abundances.size(); i++ )
        {
            int discretized = ceil( ( current_abundances[ i ] - MIN ) * bin_num / MAX ); // may be min = 0!
            disc_abundances.push_back( discretized );
        }
        disc_vector_table[ v[ 0 ] ] = disc_abundances;
        //disc_vector_table.insert( { v[ 0 ], disc_abundances } );
    }
};

void Disc_vectors::print_disc_vectors( std::string filename, std::string header, std::string sep ){
    std::ofstream Out( filename );
    Out << header << std::endl;
    // Print out discretized vectors
    for( std::unordered_map< std::string, std::vector< double > >::iterator v = disc_vector_table.begin(); v != disc_vector_table.end(); v++)
    {
        Out << v->first << sep << double_vector_to_string( disc_vector_table[ v->first ], sep ) << std::endl;
    }
    Out.close();
};
