#include "mutual_information.hpp"

#include <math.h> // log

Mutual_information::Mutual_information(){
    // Mutual Informatino for ID-ENV
    std::unordered_map < std::string , std::unordered_map < std::string, double > > MI;
};

// Get info
int Mutual_information::is_in_MI_table( std::string o, std::string e ){
    std::unordered_map < std::string , std::unordered_map < std::string, double > >::iterator i = MI.find( o );
    if( i == MI.end() )
    {
        return 0;
    }
    else
    {
        std::unordered_map < std::string, double >::iterator j = MI[ o ].find( e );
        if( j == MI[ o ].end() )
        {
            return 0;
        }
        else
        {

            return 1;
        }
    }
};
double Mutual_information::get_MI( std::string o, std::string e ){
    return MI[ o ][ e ];
};

// Add info
void Mutual_information::add_MI( std::string o, std::string e, double mi ){
    MI[ o ][ e ] = mi;
};
