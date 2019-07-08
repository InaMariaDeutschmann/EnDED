#include "min_max.hpp"

// Min
int find_min( int A, int B ){
    if( A < B )
    {
        return A;
    }
    else
    {
        return B;
    }
};
int find_min( int A, int B, int C ){
    if( A < B && A < C )
    {
        return A;
    }
    else if( B < C )
    {
        return B;
    }
    else
    {
        return C;
    }
};

// Max
int find_max( int A, int B ){
    if( A > B )
    {
        return A;
    }
    else
    {
        return B;
    }
};
int find_max( int A, int B, int C ){
    if( A > B && A > C )
    {
        return A;
    }
    else if( B > C )
    {
        return B;
    }
    else
    {
        return C;
    }
};
