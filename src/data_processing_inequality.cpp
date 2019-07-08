#include "data_processing_inequality.hpp"

// Indirect edge detection
DataProcessingInequality::DataProcessingInequality(){};

double DataProcessingInequality::get_mi_rank( double mi_XE, double mi_YE, double mi_XY ){
    // Determine rank of mutual information of X-Y edge (1 min, 3 max rank)
    if( mi_XY < mi_XE && mi_XY < mi_YE )
    {
        // dpi_vector.push_back( 1 );
        return 1;
    }
    else if( mi_XY > mi_XE && mi_XY > mi_YE )
    {
        // dpi_vector.push_back( 3 );
        return 3;
    }
    else
    {
        // dpi_vector.push_back( 2 );
        return 2;
    }
};

double DataProcessingInequality::edge_indirect( double mi_XE, double mi_YE, double mi_XY, double DPI_minMI_threshold ){
    // Determine if mutual information of X-Y edge is smaller as MI of X/Y_E edge by at least DPI_minMI_threshold
    double dpi_indirect;
    double mi_XE_new = mi_XE - DPI_minMI_threshold;
    double mi_YE_new = mi_YE - DPI_minMI_threshold;
    if( mi_XY < mi_XE_new && mi_XY < mi_YE_new )
    {
        // indirect
        dpi_indirect = 0;
    }
    else
    {
        // not indirect
        dpi_indirect = 1;
    }
    return dpi_indirect;
};
