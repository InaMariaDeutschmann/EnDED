#ifndef sign_pattern_Hpp
#define sign_pattern_Hpp

#include <string> // operator + for string concatenation


class Sign_pattern{
public:
    // Constructor
    Sign_pattern();

    // Set info
    void set_col_score( int i );
    void set_sep( std::string s );

    // Indirect edge detection
    double get_sign_pattern( std::string XE, std::string YE, std::string XY );

private:
    int col_score;
    std::string sep;
};


#endif
