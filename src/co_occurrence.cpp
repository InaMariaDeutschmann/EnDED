#include "co_occurrence.hpp"

#include <cmath>   // nan("")

CoOccurrence::CoOccurrence(){
};

// Get info
std::vector<double> CoOccurrence::get_co_occurrence( std::vector< double > X, std::vector< double > Y ){
    std::vector< double > percentage_co_occurrence;
    int presence_X = 0;
    int presence_Y = 0;
    int presence_X_AND_Y = 0;
    int presence_X_OR_Y = 0;
    if( X.size() ==  Y.size() )
    {
        for( int i = 0; i < X.size(); i++)
        {
            if( X[i] > 0 )
            {
                presence_X++;
            }
            if( Y[i] > 0 )
            {
                presence_Y++;
            }
            if( X[i] > 0 && Y[i] > 0 )
            {
                presence_X_AND_Y++;
            }
            if( X[i] > 0 || Y[i] > 0 )
            {
                presence_X_OR_Y++;
            }
        }
        percentage_co_occurrence.push_back( presence_X );            // Presence of X in dataset
        percentage_co_occurrence.push_back( presence_Y );            // Presence of Y in dataset
        percentage_co_occurrence.push_back( presence_X_AND_Y );      // Presence of X and Y at same time in dataset
        percentage_co_occurrence.push_back( presence_X_OR_Y );       // Presence of X and/or Y in dataset
        // calculation+saving of co-occurrence percentage of X and Y in dataset
        percentage_co_occurrence.push_back( (double)presence_X_AND_Y / (double)presence_X_OR_Y * 100 );
    }
    else
    {
        percentage_co_occurrence.push_back( nan("") );
    }
    

    return percentage_co_occurrence;
};
