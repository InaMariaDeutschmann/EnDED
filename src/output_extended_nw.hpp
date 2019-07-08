#ifndef output_extended_nw_Hpp
#define output_extended_nw_Hpp

#include <string>
#include <vector> // vector

class Output_extended_nw{
    public:
        // Constructor
        Output_extended_nw();

        // Set info
        void set_sep_nw( std::string s );
        void set_methods( int sp, int ol, int ii, int dpi, int co );

        // Get output string
        std::string get_extended_nw_header( std::string l );
        std::string get_no_triplet_edge( std::string l, int num_triplets, double mi, double co );
        std::string get_triplet_edge( std::string l, int num_triplets, double sp, double ol, double mi, double ii, double ii_significance, double dpi, double dpi_indirect, double combi, double co );

        // Get output triplet info
        std::string get_triplet_info_header( std::string X, std::string Y, std::string ENV );
        std::string get_triplet_info( std::string X, std::string Y, int num_triplets, std::string ENVs, std::string sp, std::string ol, double mi, std::string cmi, std::string ii, std::string ii_p, std::string dpi, std::string dpi_indirect, int num_rank1, int num_rank2, int num_rank3, std::string combi, double portion_nan_XY, std::string portion_nan_XYENV, std::vector< double > co );

        // Get output random II scores
        std::string get_rand_II_header( std::string X, std::string Y, std::string ENV, int num_permut );

	private:
        // Methods used
        int use_SP;   // SignPatter
        int use_OL;   // Overlap
        int use_II;   // InteractionInformation
        int use_DPI;  // DataProcessingInequality
        int use_CO;   // Co-Occurrence

        // Separators of input files
        std::string sep_nw;  // separator for network file
};

#endif
