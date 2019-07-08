#include "convert_to_string.hpp"

// string-vector to string
std::string string_vector_to_string( std::vector< std::string > v, std::string sep )
{
    std::string s = "";
    for( int i = 0; i < v.size(); i++ )
    {
        s += sep + v[ i ];
    }
    return s.erase( 0, sep.size() );
};

// int-vector to string
std::string int_vector_to_string( std::vector< int > v, std::string sep )
{
    std::string s = "";
    for( int i = 0; i < v.size(); i++ )
    {
        s += sep + std::to_string( v[ i ] );
    }
    return s.erase( 0, sep.size() );
};

// double-vector to string
std::string double_vector_to_string( std::vector< double > v, std::string sep )
{
    std::string s = "";
    for( int i = 0; i < v.size(); i++ )
    {
        s += sep + std::to_string( v[ i ] );
    }
    return s.erase( 0, sep.size() );
};
