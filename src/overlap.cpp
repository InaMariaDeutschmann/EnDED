#include "overlap.hpp"
#include "min_max.hpp"

#include <iostream>
#include <vector> // vector
#include <boost/algorithm/string.hpp> // split
#include <cmath>   // nan("")

Overlap::Overlap(){
    int col_length;
    int col_startX;
    int col_startY;
    std::string sep;
};

// Set info
void Overlap::set_cols( int length, int startX, int startY ){
    col_length = length;
    col_startX = startX;
    col_startY = startY;
};
void Overlap::set_sep( std::string s ){
    sep = s;
};

// Get info
double Overlap::get_overlap( std::string XE, std::string YE, std::string XY ){
    double percent_overlap;

    std::vector< std::string > v_XE;
    split( v_XE, XE, boost::is_any_of( sep ) );
    std::vector< std::string > v_YE;
    split( v_YE, YE, boost::is_any_of( sep ) );
    std::vector< std::string > v_XY;
    split( v_XY, XY, boost::is_any_of( sep ) );

    // Networks interaction parameters
    double len_XY;
    double len_XE;
    double len_YE;
    double startX_XY;
    double startX_XE;
    double startX_YE;
    double startY_XY;
    double startY_XE;
    double startY_YE;


    // lengths
    std::string lowercase_s = boost::algorithm::to_lower_copy( v_XY[ col_length ]);
    if( lowercase_s == "na" || lowercase_s == "nan")
    {
        len_XY = nan("");
    }
    else
    {
        len_XY = std::stod( v_XY[ col_length ] );
    }
    lowercase_s = boost::algorithm::to_lower_copy( v_XE[ col_length ]);
    if( lowercase_s == "na" || lowercase_s == "nan")
    {
        len_XE = nan("");
    }
    else
    {
        len_XE = std::stod( v_XE[ col_length ] );
    }
    lowercase_s = boost::algorithm::to_lower_copy( v_YE[ col_length ]);
    if( lowercase_s == "na" || lowercase_s == "nan")
    {
        len_YE = nan("");
    }
    else
    {
        len_YE = std::stod( v_YE[ col_length ] );
    }
    // startX
    lowercase_s = boost::algorithm::to_lower_copy( v_XY[ col_startX ]);
    if( lowercase_s == "na" || lowercase_s == "nan")
    {
        startX_XY = nan("");
    }
    else
    {
        startX_XY = std::stod( v_XY[ col_startX ] );
    }
    lowercase_s = boost::algorithm::to_lower_copy( v_XE[ col_startX ]);
    if( lowercase_s == "na" || lowercase_s == "nan")
    {
        startX_XE = nan("");
    }
    else
    {
        startX_XE = std::stod( v_XE[ col_startX ] );
    }
    lowercase_s = boost::algorithm::to_lower_copy( v_YE[ col_startX ]);
    if( lowercase_s == "na" || lowercase_s == "nan")
    {
        startX_YE = nan("");
    }
    else
    {
        startX_YE = std::stod( v_YE[ col_startX ] );
    }
    // startY
    lowercase_s = boost::algorithm::to_lower_copy( v_XY[ col_startY ]);
    if( lowercase_s == "na" || lowercase_s == "nan")
    {
        startY_XY = nan("");
    }
    else
    {
        startY_XY = std::stod( v_XY[ col_startY ] );
    }
    lowercase_s = boost::algorithm::to_lower_copy( v_XE[ col_startY ]);
    if( lowercase_s == "na" || lowercase_s == "nan")
    {
        startY_XE = nan("");
    }
    else
    {
        startY_XE = std::stod( v_XE[ col_startY ] );
    }
    lowercase_s = boost::algorithm::to_lower_copy( v_YE[ col_startY ]);
    if( lowercase_s == "na" || lowercase_s == "nan")
    {
        startY_YE = nan("");
    }
    else
    {
        startY_YE = std::stod( v_YE[ col_startY ] );
    }
    // Check if not na
    if( startX_XE >= 0 && startY_XE >= 0 && startX_YE >= 0 && startY_YE >= 0 && startX_XY >= 0 && startY_XY >= 0 && len_XE >= 0 && len_YE >= 0 && len_XY >= 0 )
    {
        // Start of Overlap
        double XE_start = find_min( startX_XE, startY_XE );
        double YE_start = find_min( startX_YE, startY_YE );
        double XY_start = find_min( startX_XY, startY_XY );
        double max_start = find_max( XY_start, XE_start, YE_start );

        // End of Overlap
        double XE_end = find_max( startX_XE, startY_XE ) + len_XE - 1;
        double YE_end = find_max( startX_YE, startY_YE ) + len_YE - 1;
        double XY_end = find_max( startX_XY, startY_XY ) + len_XY - 1;
        double min_end = find_min( XY_end, XE_end, YE_end );

        // length of XY considering the delay
        len_XY = XY_end - XY_start + 1;

        // calculation of overlap percentage
        percent_overlap = ( min_end - max_start + 1) / ( len_XY ) * 100; // length_overlap/length*100
        
        if( percent_overlap < 0 )
        {
            percent_overlap = 0;
        }
    }
    else
    {
        percent_overlap = nan("");
    }

    return percent_overlap;
};

// Indirect edge detection
