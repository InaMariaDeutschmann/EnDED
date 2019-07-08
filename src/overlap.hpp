#ifndef overlap_Hpp
#define overlap_Hpp

#include <string> // operator + for string concatenation


class Overlap{
public:
    // Constructor
    Overlap();

    // Set info
    void set_cols( int length, int startX, int startY );
    void set_sep( std::string s );

    // Indirect edge detection
    double get_overlap( std::string XE, std::string YE, std::string XY );

private:
    int col_length;
    int col_startX;
    int col_startY;
    std::string sep;
};


#endif
