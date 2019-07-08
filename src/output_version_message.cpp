#include "output_version_message.hpp"

// Get version message
std::string get_version_message( std::string s ){
    std::string outputline = "";

    outputline += "#####################################################\n";
    outputline += s;
    outputline += "\n\tVersion 1";
    outputline += "\n\tJune 15th 2019";
    outputline += "\n\tIna Maria Deutschmann";
    outputline += "\n#####################################################\n";

    return outputline;
};
