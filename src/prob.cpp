#include "prob.hpp"

#include <iostream>
#include <fstream>                    // read in and out files
#include <math.h>                     // log
#include <algorithm>                  // std::shuffle
#include <random>                     // std::default_random_engine
#include <chrono>                     // std::chrono::system_clock


Prob::Prob(){
    // Methods parameters
    int II_num_permutations = 100;
    double II_significance_level = 0.05;
    unsigned seed;
    int indicator_pre_permut = 0;

    // MarginalProbability
    std::unordered_map < std::string , std::unordered_map < double, double > > MarginalP;

    // JointProbability for ENV_ID
    // ID -> ENV -> ID_ab -> ENV_ab -> number of times the pair appeared
    std::unordered_map < std::string , std::unordered_map < std::string, std::unordered_map < double, std::unordered_map < double, double > > > > JointP_ID_ENV;

    // Randomized ENV
    std::unordered_map < std::string, std::unordered_map < double, std::vector< double > > > rand_ENV_iteration_abundance;
    std::unordered_map < std::string , std::unordered_map < std::string, std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > > > > rand_ID_ENV_iteration_JointP;
};
// Set info
void Prob::set_num_permutations( int i ){
    II_num_permutations = i;
};
void Prob::set_sign_level( double d ){
    II_significance_level = d;
};
unsigned Prob::set_permut_seed(){
    // Setting seed for CMI significance determination
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    return seed;
};
void Prob::set_indicator_pre_permut( int i ){
    indicator_pre_permut = i;
};

// Get info
int Prob::is_in_Marginal_Prob_Table( std::string s ){
    std::unordered_map < std::string , std::unordered_map < double, double > >::iterator i = MarginalP.find( s );
    if( i == MarginalP.end() )
    {
        return 0;
    }
    else
    {
        return 1;
    }
};
std::unordered_map < double, double > Prob::get_MarginalP( std::string s ){
    return MarginalP[ s ];
};
std::unordered_map < double, std::unordered_map < double, double > > Prob::get_JointP( std::string o, std::string e ){
    return JointP_ID_ENV[ o ][ e ];
};
std::unordered_map < double, std::unordered_map < double, double > > Prob::get_randJointP_ID_ENV( std::string O, std::string E, int i, std::vector< double > O_ab, std::vector< double > E_ab){
    if( indicator_pre_permut )
    {
        return rand_ID_ENV_iteration_JointP[ O ][ E ][ i ];
    }
    else
    {
        return compute_JointProb_2Var( O_ab, E_ab );
    }
};

// Add info
void Prob::add_Marginal_Prob( std::string s, std::vector< double > disc_ab ){
    // Compute and store in MarginalP
    MarginalP[ s ] = compute_MarginalProb( disc_ab );
};
void Prob::add_Joint_Prob( std::string o, std::string e, std::vector< double > X_ab, std::vector< double > Y_ab ){
    JointP_ID_ENV[ o ][ e ] = compute_JointProb_2Var( X_ab, Y_ab );
};
void Prob::add_permut_ENV( std::string s, std::vector< double > v ){
    std::vector< double > E_ab = v;
    for( int i = 0; i < II_num_permutations; i++ )
    {
        // randomly shuffle ENV vector
        std::mt19937 random_engine( seed ); // create random engines with the same state
        std::shuffle(E_ab.begin(), E_ab.end(), random_engine ); // permutation of permuted vector, so permuted vectors vary from original vector
        rand_ENV_iteration_abundance[ s ][ i ] = E_ab;
    }
};
void Prob::add_rand_JointP( std::string o, std::string e, std::vector< double > O_ab ){
    for( int i = 0; i < II_num_permutations; i++ )
    {
        rand_ID_ENV_iteration_JointP[ o ][ e ][ i ] = compute_JointProb_2Var( O_ab, rand_ENV_iteration_abundance[ e ][ i ] );
    }
};

// Compute Probabilities
std::unordered_map < double, double > Prob::compute_MarginalProb( std::vector< double > disc_ab )
{
    // Vector to hold discretized abundance is disc_ab!
    // Datastructure to hold Marginal Probability
    std::unordered_map < double, double > p;

    // Determine the Marginal Probabilities
    for( int i = 0; i < disc_ab.size(); i++ )
    {
        // Check if abundance is already in p
        std::unordered_map < double, double >::iterator a = p.find( disc_ab[ i ] );
        if( a == p.end() )
        {
            // Not found -> add to p
            p[ disc_ab[ i ] ] = 1;
        }
        else
        {
            // Found -> count +1!
            p[ disc_ab[ i ] ] += 1;
        }
    }
    return p;
};
std::unordered_map < double, std::unordered_map < double, double > > Prob::compute_JointProb_2Var( std::vector< double > X_ab, std::vector< double > Y_ab ){
    // Compute Joint Probability for 2 Variables
    std::unordered_map < double, std::unordered_map < double, double > > p;

    // building up datastructure
    // build data structure for JointProbability
    int count_nan = 0;
    for(int i = 0; i < X_ab.size(); i++)
    {
        // Check if X_ab or Y_ab is nan (if nan >=0 gives False)
        if( X_ab[ i ] >= 0 && Y_ab[ i ] >= 0 )
        {
            // Is abundance of X in JointProbability
            std::unordered_map < double, std::unordered_map < double, double > >::iterator p_Xin = p.find( X_ab[ i ] );
            if( p_Xin == p.end())
            {
                // Not found, include in Hash
                p[ X_ab[ i ] ][ Y_ab[i] ] = 1;
            }
            else
            {
                // X is in Hash
                // Let's check if Y is in hash
                std::unordered_map < double, double >::iterator p_Yin = p[ X_ab[ i ] ].find( Y_ab[i] );
                if( p_Yin == p[ X_ab[ i ] ].end())
                {
                    // Not found, include in Hash
                    p[ X_ab[ i ] ][ Y_ab[i] ] = 1;
                }
                else
                {
                    // X and Y are in Hash
                    // update
                    p[ X_ab[ i ] ][ Y_ab[i] ] += 1;
                }
            }
        }
        else
        {
            // Count how often X or Y was nan
            count_nan++;
        }
    }
    p[ -1 ][ -1 ] = count_nan;

    return p;
}

std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > > Prob::compute_JointProb_3Var( std::vector< double > X_ab, std::vector< double > Y_ab, std::vector< double > E_ab ){
    // build data structure for JointProbability
    std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > > p;
    // building up datastructure
    // build data structure for JointProbability
    int count_nan = 0;
    for(int i = 0; i < X_ab.size(); i++)
    {
        // Check if X_ab or Y_ab is nan (if nan >=0 gives False)
        if( X_ab[ i ] >= 0 && Y_ab[ i ] >= 0 && E_ab[ i ] >= 0 )
        {
            std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > >::iterator Xin = p.find( X_ab[ i ] );
            if( Xin == p.end())
            {
                // Not found, include in Hash
                p[ X_ab[ i ] ][ Y_ab[ i ] ][ E_ab[ i ] ] = 1;
            }
            else
            {
                std::unordered_map < double, std::unordered_map < double, double > >::iterator Yin = p[ X_ab[ i ] ].find( Y_ab[ i ] );
                if( Yin == p[ X_ab[ i ] ].end() )
                {
                    // Not found, include in Hash
                    p[ X_ab[ i ] ][ Y_ab[ i ] ][ E_ab[ i ] ] = 1;
                }
                else
                {
                    std::unordered_map < double, double >::iterator Ein = p[ X_ab[ i ] ][ Y_ab[ i ] ].find( E_ab[ i ] );
                    if( Ein == p[ X_ab[ i ] ][ Y_ab[ i ] ].end() )
                    {
                        // Not found, include in Hash
                        p[ X_ab[ i ] ][ Y_ab[ i ] ][ E_ab[ i ] ] = 1;
                    }
                    else
                    {
                        // Found, count +1
                        p[ X_ab[ i ] ][ Y_ab[ i ] ][ E_ab[ i ] ] += 1;
                    }
                }
            }
        }
        else
        {
            // Count how often X or Y was nan
            count_nan++;
        }
    }
    p[ -1 ][ -1 ][ -1 ] = count_nan;

    return p;
};

// Compute info
double Prob::compute_MI( double samplesize, std::string X, std::string Y, int indicator_comp_JointP, std::vector< double > X_ab, std::vector< double > Y_ab  ){
    std::unordered_map < double, double > pM_X;
    std::unordered_map < double, double > pM_Y;
    std::unordered_map < double, std::unordered_map < double, double > > pJ_XY;

    if( is_in_Marginal_Prob_Table( X ) )
    {
        pM_X = MarginalP[ X ];
    }
    else
    {
        add_Marginal_Prob( X, X_ab );
        pM_X = MarginalP[ X ];
    }
    if( is_in_Marginal_Prob_Table( Y ) )
    {
        pM_Y = MarginalP[ Y ];
    }
    else
    {
        add_Marginal_Prob( Y, Y_ab );
        pM_Y = MarginalP[ Y ];
    }
    if( indicator_comp_JointP )
    {
        pJ_XY = compute_JointProb_2Var( X_ab, Y_ab );
    }
    else
    {
        pJ_XY = JointP_ID_ENV[ X ][ Y ];
    }
    // compute mutual information MI
    // for each possible X
    // for each possible Y
    // MI = sum over X and Y [JointProbability * log(JointProbability/(MarginalP_X+MarginalP_Y))]
    // samplesize = s (= n + 1)
    // pxy/s*log((pxy/s)/((px/s)*(py/s)))
    // 1/s(pxy*log(pxy*s/(px*py)))
    //
    // Caution with nan
    // for ID-ENV, Y = ENV
    // We assume that the ID have no nan in the abundances
    // So, for ID-ENV edges, we need to determine Y(=ENV) for possible nan

    double mi = 0;

    // double samplesize = disc_abundance_table[ X ].size() + 1;
    // everytime we find a "nan" it will be substracted!
    if( pJ_XY[ -1 ][ -1 ] > 0 )
    {
        // nan appears, update samplesize
        samplesize = samplesize - pJ_XY[ -1 ][ -1 ];

        // Determine new Marginal Probabilities (pM_X and pM_Y)
        std::unordered_map < double, double > pM_X_new;
        std::unordered_map < double, double > pM_Y_new;
        for( std::unordered_map < double, std::unordered_map < double, double > >::iterator jX = pJ_XY.begin(); jX != pJ_XY.end(); jX++ )
        {
            // (if nan >=0 gives False)
            if( jX->first >= 0 )
            {
                for( std::unordered_map < double, double >::iterator jY = pJ_XY[ jX->first ].begin(); jY != pJ_XY[ jX->first ].end(); jY++ )
                {
                    // Count new marginal Probabilities
                    std::unordered_map < double, double >::iterator pM_X_new_in = pM_X_new.find( jX->first );
                    if( pM_X_new_in == pM_X_new.end() )
                    {
                        // Not found, include in Hash
                        pM_X_new[ jX->first ] = pJ_XY[ jX->first ][ jY->first ];
                    }
                    else
                    {
                        // Found, count +1
                        pM_X_new[ jX->first ] += pJ_XY[ jX->first ][ jY->first ];
                    }

                    std::unordered_map < double, double >::iterator pM_Y_new_in = pM_Y_new.find( jY->first );
                    if( pM_Y_new_in == pM_Y_new.end() )
                    {
                        // Not found, include in Hash
                        pM_Y_new[ jY->first ] = pJ_XY[ jX->first ][ jY->first ];
                    }
                    else
                    {
                        // Found, count +1
                        pM_Y_new[ jY->first ] += pJ_XY[ jX->first ][ jY->first ];
                    }
                }
            }
        }
        // Update Marginal Probabilities (pM_X and pM_Y)
        pM_X = pM_X_new;
        pM_Y = pM_Y_new;
    }

    // There is no nan in Marginal Probabilities now
    // If there was, then Joint Probability also had nan
    // And then we generated new Marginal Probabilities (without nan)
    for( std::unordered_map < double, double >::iterator pX = pM_X.begin(); pX != pM_X.end(); pX++ )
    {
        double sum = 0;

        for( std::unordered_map < double, double >::iterator pY = pM_Y.begin(); pY != pM_Y.end(); pY++ )
        {
            // is (XY) in JointProbability?
            std::unordered_map < double, double >::iterator j = pJ_XY[ pX->first ].find( pY->first );
            if( j != pJ_XY[ pX->first ].end() )
            {
                // samplesize = s
                // pxy/s*log((pxy/s)/((px/s)*(py/s)))
                // 1/s(pxy*log(pxy*s/(px*py)))
                double TEMP_pxy = (double)pJ_XY[ pX->first ][ pY->first ];
                double TEMP_px = (double)pX->second;
                double TEMP_py = (double)pY->second;
                sum += ( TEMP_pxy ) * ( log( samplesize ) + log( TEMP_pxy ) - log( TEMP_px ) - log( TEMP_py ) );
            } // else: convention 0*log(0)=0!
        }

        mi += sum;
    }
    mi = mi / samplesize;

    return mi;
};
double Prob::compute_CMI_XY( double samplesize, std::string X, std::string Y, std::string E, std::vector< double > X_ab, std::vector< double > Y_ab, std::vector< double > E_ab ){

    return compute_CMI( samplesize, compute_JointProb_3Var( X_ab, Y_ab, E_ab ), MarginalP[ E ], JointP_ID_ENV[ X ][ E ], JointP_ID_ENV[ Y ][ E ] );
};

double Prob::compute_CMI( double samplesize, std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > > j_XYE, std::unordered_map < double, double > m_E, std::unordered_map < double, std::unordered_map < double, double > > j_XE, std::unordered_map < double, std::unordered_map < double, double > > j_YE ){
    // compute conditional mutual information CMI
    // for each possible x
    // for each possible y
    // for each possible ENV
    // CMI = sum over X and Y and ENV
    // CMI = [j_XYE/n+1 * log(j_XYE/(j_XE+j_YE))]
    double conditional_mutual_information_XYENV = 0;

    // every "nan" should be substracted from samplesize
    // double samplesize = disc_abundance_table[ X ].size() + 1;
    // everytime we find a "nan" it will be substracted!
    if( j_XYE[ -1 ][ -1 ][ -1 ] > 0 )
    {
        // nan appears, update samplesize
        samplesize = samplesize - j_XYE[ -1 ][ -1 ][ -1 ];

        // Determine new Marginal Probabilities (m_E)
        std::unordered_map < double, double > m_E_new;
        std::unordered_map < double, std::unordered_map < double, double > > j_XE_new;
        std::unordered_map < double, std::unordered_map < double, double > > j_YE_new;
        for( std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > >::iterator jX = j_XYE.begin(); jX != j_XYE.end(); jX++ )
        {
            // (if nan >=0 gives False)
            if( jX->first >= 0 )
            {
                for( std::unordered_map < double, std::unordered_map < double, double > >::iterator jY = j_XYE[ jX->first ].begin(); jY != j_XYE[ jX->first ].end(); jY++ )
                {
                    for( std::unordered_map < double, double >::iterator jE = j_XYE[ jX->first ][ jY->first ].begin(); jE != j_XYE[ jX->first ][ jY->first ].end(); jE++ )
                    {
                        // Count new marginal Probability
                        std::unordered_map < double, double >::iterator m_E_new_in = m_E_new.find( jE->first );
                        if( m_E_new_in == m_E_new.end() )
                        {
                            // Not found, include in Hash
                            m_E_new[ jE->first ] = j_XYE[ jX->first ][ jY->first ][ jE->first ];
                        }
                        else
                        {
                            // Found, count +1
                            m_E_new[ jE->first ] += j_XYE[ jX->first ][ jY->first ][ jE->first ];
                        }

                        // Count new Joint Probability for XE
                        std::unordered_map < double, std::unordered_map < double, double > >::iterator j_XE_new_inX = j_XE_new.find( jX->first );
                        if( j_XE_new_inX == j_XE_new.end() )
                        {
                            // Not found, include in Hash
                            j_XE_new[ jX->first ][ jE->first ] = j_XYE[ jX->first ][ jY->first ][ jE->first ];
                        }
                        else
                        {
                            std::unordered_map < double, double >::iterator j_XE_new_inE = j_XE_new[ jX->first ].find( jE->first );
                            if( j_XE_new_inE == j_XE_new[ jX->first ].end() )
                            {
                                // Not found, include in Hash
                                j_XE_new[ jX->first ][ jE->first ] = j_XYE[ jX->first ][ jY->first ][ jE->first ];
                            }
                            else
                            {
                                // Found, count +1
                                j_XE_new[ jX->first ][ jE->first ] += j_XYE[ jX->first ][ jY->first ][ jE->first ];
                            }
                        }

                        // Count new Joint Probability for YE
                        std::unordered_map < double, std::unordered_map < double, double > >::iterator j_YE_new_inY = j_YE_new.find( jY->first );
                        if( j_YE_new_inY == j_YE_new.end() )
                        {
                            // Not found, include in Hash
                            j_YE_new[ jY->first ][ jE->first ] = j_XYE[ jX->first ][ jY->first ][ jE->first ];
                        }
                        else
                        {
                            std::unordered_map < double, double >::iterator j_YE_new_inE = j_YE_new[ jY->first ].find( jE->first );
                            if( j_YE_new_inE == j_YE_new[ jY->first ].end() )
                            {
                                // Not found, include in Hash
                                j_YE_new[ jY->first ][ jE->first ] = j_XYE[ jX->first ][ jY->first ][ jE->first ];
                            }
                            else
                            {
                                // Found, count +1
                                j_YE_new[ jY->first ][ jE->first ] += j_XYE[ jX->first ][ jY->first ][ jE->first ];
                            }
                        }
                    }
                }
            }
        }
        // Update Probabilities (pM_X and pM_Y)
        m_E = m_E_new;
        j_XE = j_XE_new;
        j_YE = j_YE_new;
    }

    // for each ENV
    for( std::unordered_map < double, double >::iterator marginalProbE = m_E.begin(); marginalProbE != m_E.end(); marginalProbE++ )
    {
        // marginalProbE->first = descritized number
        // marginalProbE->second = count of this number

        double sum_ENV = 0;

        // for each X
        for( std::unordered_map < double, double >::iterator JointProbEX = j_XE[ marginalProbE->first ].begin(); JointProbEX != j_XE[ marginalProbE->first ].end(); JointProbEX++ )
        {
            double sum_ID = 0;

            // for each Y
            for( std::unordered_map < double, double >::iterator JointProbEY = j_YE[ marginalProbE->first ].begin(); JointProbEY != j_YE[ marginalProbE->first ].end(); JointProbEY++ )
            {
                std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > >::iterator x = j_XYE.find( JointProbEX->first );
                if( x != j_XYE.end() )
                {
                    std::unordered_map < double, std::unordered_map < double, double > >::iterator y = j_XYE[ JointProbEX->first ].find( JointProbEY->first );
                    if( y != j_XYE[ JointProbEX->first ].end())
                    {
                        std::unordered_map < double, double >::iterator JointProbXYE = j_XYE[ JointProbEX->first ][ JointProbEY->first ].find( marginalProbE->first );
                        if( JointProbXYE != j_XYE[ JointProbEX->first ][ JointProbEY->first ].end())
                        {
                            // so p(xye) exists!
                            double pxye = (double)JointProbXYE->second;  // = j_XYE[X_abundances[i]][Y_abundances[i]][E_abundances[i]]
                            double pe = (double)marginalProbE->second; // = MarginalProbability[E]
                            double pxe = (double)JointProbEX->second; // no need to check if it is inside, because we only look at those that are in pxye, than for sure also in pxe
                            double pye = (double)JointProbEY->second; // no need to check if it is inside, because we only look at those that are in pxye, than for sure also in pye
                            //sum_ID += pxye * ( log( pxye ) + log( pe ) - log( pxe ) - log( pye ) ); // devided by samplesize later!
                            sum_ID += pxye * ( log( ( pxye * pe ) / ( pxe * pye ) ) ); // devided by samplesize later!
                        }
                    }
                }
            }
            sum_ENV += sum_ID;
        }

        conditional_mutual_information_XYENV += sum_ENV;
    }
    conditional_mutual_information_XYENV = conditional_mutual_information_XYENV/samplesize; // division by samplesize

    return conditional_mutual_information_XYENV;
};

// Significance for II = Significance for CMI
double Prob::compute_p_value( std::string X, std::string Y, std::string E, double cmi, double mi, std::vector< double > X_ab, std::vector< double > Y_ab, std::vector< double > E_ab_original )
{
    std::unordered_map < double, double > m_E = get_MarginalP( E );
    double samplesize = E_ab_original.size();

    double d = 0;

    // 1. Save ENV vector in Datastructure
    std::vector< double > E_ab = E_ab_original;

    // Determine Significance
    for( int i = 0; i < II_num_permutations; i++ )
    {
        // 2. randomly shuffle ENV vector
        if( indicator_pre_permut )
        {
            rand_ENV_iteration_abundance[ E ][ i ];
        }
        else
        {
            std::mt19937 random_engine( seed ); // create random engines with the same state
            std::shuffle(E_ab.begin(), E_ab.end(), random_engine ); //permutation of permuted vector, so permuted vectors vary from original vector
        }

        // 3. calculate CMI for random. ENV vectors
        double CMI_permut = compute_CMI( samplesize, compute_JointProb_3Var( X_ab, Y_ab, E_ab ), m_E, get_randJointP_ID_ENV( X, E, i, X_ab, E_ab), get_randJointP_ID_ENV( Y, E, i, Y_ab, E_ab) );;

        if( CMI_permut <= cmi )
        {
            d++;; // Found one value smaller
        }
    }
    // 4. determine Significance
    d = ( d + 1) / ( II_num_permutations + 1 );
    return d;
};
