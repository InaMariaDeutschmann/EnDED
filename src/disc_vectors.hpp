#ifndef disc_vectors_Hpp
#define disc_vectors_Hpp

#include <string>
#include <vector>
#include <unordered_map>

class Disc_vectors{
    public:
        // Constructor
        Disc_vectors();

        // Get info
        int get_error_occurred();
        std::string get_error_message();
        int is_in_table( std::string s );
        std::vector< double > get_disc_vector( std::string s );

        // Build up discretized vectors
        void build_disc_table( std::string filename_abundance, std::string filename_ENV, std::string sep, int indicator_output_discretized_vectors, std::string filename_output_discretized_vectors, double max_nan_threshold, std::string filename_output_logfile );

	private:
        // Discretized vectors
        std::unordered_map< std::string, std::vector< double > > disc_vector_table;

        // Tracker
        int error_occurred;
        std::string error_message;

        // Functions
        void save_disc_vector( std::vector< std::string > v, double max_nan_threshold, std::string line );
        void add_vectors_to_disc_table( std::string filename, std::string sep, int indicator_output_discretized_vectors, std::string filename_output_discretized_vectors, double max_nan_threshold, std::string filename_output_logfile );
        void print_disc_vectors( std::string filename, std::string header, std::string sep );
};

#endif
