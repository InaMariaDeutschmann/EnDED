#ifndef mutual_information_Hpp
#define mutual_information_Hpp

#include <string>
#include <unordered_map>

class Mutual_information{
    public:
        // Constructor
        Mutual_information();

        // Get info
        int is_in_MI_table( std::string o, std::string e );
        double get_MI( std::string o, std::string e );

        // Add info
        void add_MI( std::string o, std::string e, double mi );

	private:
        // Mutual Information for ID-ENV
        std::unordered_map < std::string , std::unordered_map < std::string, double > > MI;
};

#endif
