#ifndef convert_to_string_Hpp
#define convert_to_string_Hpp

#include <string>
#include <vector>

// string-vector to string
std::string string_vector_to_string( std::vector< std::string > v, std::string sep  );

// int-vector to string
std::string int_vector_to_string( std::vector< int > v, std::string sep  );

// double-vector to string
std::string double_vector_to_string( std::vector< double > v, std::string sep );

#endif
