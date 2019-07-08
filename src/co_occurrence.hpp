#ifndef co_occurrence_Hpp
#define co_occurrence_Hpp

#include <vector> // vector

class CoOccurrence{
public:
    // Constructor
    CoOccurrence();

    // Indirect edge detection
    std::vector< double > get_co_occurrence( std::vector< double > X, std::vector< double > Y );
};


#endif
