#ifndef data_processing_inequality_Hpp
#define data_processing_inequality_Hpp

#include <vector>

class DataProcessingInequality{
public:
    // Constructor
    DataProcessingInequality();

    // Indirect edge detection
    double get_mi_rank( double mi_XE, double mi_YE, double mi_XY );                               // returns rank of XY
    double edge_indirect( double mi_XE, double mi_YE, double mi_XY, double DPI_minMI_threshold ); // returns if mi_XY is indirect(=0) or direct(=1)
};


#endif
