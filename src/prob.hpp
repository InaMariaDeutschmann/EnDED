#ifndef prob_Hpp
#define prob_Hpp

#include <string>
#include <vector>
#include <unordered_map>

class Prob{
    public:
        // Constructor
        Prob();

        // Set info
        void set_num_permutations( int i );
        void set_sign_level( double d );
        unsigned set_permut_seed();
        void set_indicator_pre_permut( int i );

        // Get info
        int is_in_Marginal_Prob_Table( std::string s );
        std::unordered_map < double, double > get_MarginalP( std::string s );
        std::unordered_map < double, std::unordered_map < double, double > > get_JointP( std::string o, std::string e );
        std::unordered_map < double, std::unordered_map < double, double > > get_randJointP_ID_ENV( std::string O, std::string E, int i, std::vector< double > O_ab, std::vector< double > E_ab);

        // Add info
        void add_Marginal_Prob( std::string s, std::vector< double > disc_ab );
        void add_Joint_Prob( std::string o, std::string e, std::vector< double > X_ab, std::vector< double > Y_ab );
        void add_permut_ENV( std::string s, std::vector< double > v );
        void add_rand_JointP( std::string o, std::string e, std::vector< double > O_ab );

        // Compute info
        double compute_MI( double samplesize, std::string X, std::string Y, int indicator_comp_JointP, std::vector< double > X_ab, std::vector< double > Y_ab );
        double compute_CMI_XY( double samplesize, std::string X, std::string Y, std::string E, std::vector< double > X_ab, std::vector< double > Y_ab, std::vector< double > E_ab );
        double compute_p_value( std::string X, std::string Y, std::string E, double cmi, double mi, std::vector< double > X_ab, std::vector< double > Y_ab, std::vector< double > E_ab_original );

	private:
        // Methods Parameters
        int II_num_permutations;
        double II_significance_level;
        unsigned seed;
        int indicator_pre_permut;

        // Marginal Probabilities
        std::unordered_map < std::string , std::unordered_map < double, double > > MarginalP;

        // Joint Probabilities for ENV_ID
        // ID -> ENV -> ID_ab -> ENV_ab -> number of times the pair appeared
        std::unordered_map < std::string , std::unordered_map < std::string, std::unordered_map < double, std::unordered_map < double, double > > > > JointP_ID_ENV;

        // Randomized ENV
        std::unordered_map < std::string, std::unordered_map < double, std::vector< double > > > rand_ENV_iteration_abundance;
        std::unordered_map < std::string , std::unordered_map < std::string, std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > > > > rand_ID_ENV_iteration_JointP;

        // Compute Probabilities
        std::unordered_map < double, double > compute_MarginalProb( std::vector< double > disc_ab );
        std::unordered_map < double, std::unordered_map < double, double > > compute_JointProb_2Var( std::vector< double > X_ab, std::vector< double > Y_ab );
        std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > > compute_JointProb_3Var( std::vector< double > X_ab, std::vector< double > Y_ab, std::vector< double > E_ab );
        double compute_CMI( double samplesize, std::unordered_map < double, std::unordered_map < double, std::unordered_map < double, double > > > j_XYE, std::unordered_map < double, double > m_E, std::unordered_map < double, std::unordered_map < double, double > > j_XE, std::unordered_map < double, std::unordered_map < double, double > > j_YE  );
};

#endif
