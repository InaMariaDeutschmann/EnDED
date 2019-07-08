#include "sign_pattern.hpp"

#include <vector> // vector
#include <boost/algorithm/string.hpp> // split
#include <cmath>   // nan("")

Sign_pattern::Sign_pattern(){
    int col_score;
    std::string sep;
};

// Set info
void Sign_pattern::set_col_score( int i ){
    col_score = i;
};
void Sign_pattern::set_sep( std::string s ){
    sep = s;
};

// Indirect edge detection
double Sign_pattern::get_sign_pattern( std::string XE, std::string YE, std::string XY ){
    double sp;

    // X-Y | X-ENV | Y-ENV
    // direct: +-+ or ++- or -++ or ---
    // indirect: +++ or +-- or --+ or -+-
    double Score_XE;
    double Score_YE;
    double Score_XY;

    std::vector< std::string > v;
    split( v, XE, boost::is_any_of( sep ) );
    std::string lowercase_s = boost::algorithm::to_lower_copy( v[ col_score ] );
    if( lowercase_s == "na" || lowercase_s == "nan" )
    {
        Score_XE = nan("");
    }
    else
    {
        Score_XE = stod( v[ col_score ] );
    }
    split( v, YE, boost::is_any_of( sep ) );
    lowercase_s = boost::algorithm::to_lower_copy( v[ col_score ] );
    if( lowercase_s == "na" || lowercase_s == "nan" )
    {
        Score_YE = nan("");
    }
    else
    {
        Score_YE = stod( v[ col_score ] );
    }
    split( v, XY, boost::is_any_of( sep ) );
    lowercase_s = boost::algorithm::to_lower_copy( v[ col_score ] );
    if( lowercase_s == "na" || lowercase_s == "nan" )
    {
        Score_XY = nan("");
    }
    else
    {
        Score_XY = stod( v[ col_score ] );
    }

    if( Score_XY > 0 )
    {
        if( ( Score_XE > 0 && Score_YE > 0 ) || ( Score_XE < 0 && Score_YE < 0 ) )
        {
            sp = 0; // +++ or +-- = indirect
        }
        else if( ( Score_XE > 0 && Score_YE < 0 ) || ( Score_XE < 0 && Score_YE > 0 ) )
        {
            sp = 1; // ++- or +-+ = not indirect
        }
        else
        {
            sp = nan("");
        }
    }
    else if( Score_XY < 0 )
    {
        if( ( Score_XE < 0 && Score_YE > 0 ) || ( Score_XE > 0 && Score_YE < 0 ) )
        {
            sp = 0; // --+ or -+- = indirect
        }
        else if( ( Score_XE < 0 && Score_YE < 0 ) || ( Score_XE > 0 && Score_YE > 0 ) )
        {
            sp = 1; // --- or -++ = not indirect
        }
        else
        {
            sp = nan("");
        }
    }
    else
    {
        sp = nan("");
    }

    return sp;
};
