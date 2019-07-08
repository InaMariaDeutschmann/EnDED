#include "tracker.hpp"
#include "convert_to_string.hpp"

Tracker::Tracker(){
    Tracker_triplet_ENVs = "";
    Tracker_triplet_num_rank_1 = 0;
    Tracker_triplet_num_rank_2 = 0;
    Tracker_triplet_num_rank_3 = 0;

    Tracker_num_ID_ENV_considered = 0;
    Tracker_num_ID_ENV_all = 0;
    Tracker_num_ID_ENV_in_triplet = 0;
    Tracker_num_ID_ID = 0;
    Tracker_num_ENV_ENV = 0;
    Tracker_num_ID_ID_in_triplet = 0;
    Tracker_num_ID_connected_to_min_one_ENV = 0;
    Tracker_num_ENV_connected_to_min_one_ID = 0;
    Tracker_num_ID_ID_not_in_triplet = 0;

    Tracker_SP_num_indirect = 0;
    Tracker_SP_num_not_indirect = 0;

    Tracker_OL_num_indirect = 0;
    Tracker_OL_num_not_indirect = 0;

    Tracker_II_num_indirect = 0;
    Tracker_II_num_not_indirect = 0;

    Tracker_DPI_num_indirect = 0;
    Tracker_DPI_num_not_indirect = 0;

    Tracker_Combi_num_indirect = 0;
    Tracker_Combi_num_not_indirect = 0;
};


// Add info
void Tracker::set_Tracker_num_ID_ENV_considered( int i ){
    Tracker_num_ID_ENV_considered = i;
};
void Tracker::set_Tracker_num_ID_ENV_all( int i ){
    Tracker_num_ID_ENV_all = i;
};
void Tracker::set_Tracker_num_ID_connected_to_min_one_ENV( int i ){
    Tracker_num_ID_connected_to_min_one_ENV = i;
};
void Tracker::set_Tracker_num_ENV_connected_to_min_one_ID( int i ){
    Tracker_num_ENV_connected_to_min_one_ID = i;
};
void Tracker::add_plus1_Tracker_num_ID_ENV_in_triplet(){
    Tracker_num_ID_ENV_in_triplet++;
};
void Tracker::add_plus1_Tracker_num_ID_ID(){
    Tracker_num_ID_ID++;
};
void Tracker::add_plus1_Tracker_num_ENV_ENV(){
    Tracker_num_ENV_ENV++;
};
void Tracker::add_plus1_Tracker_num_ID_ID_in_triplet(){
    Tracker_num_ID_ID_in_triplet++;
};
void Tracker::add_plus1_Tracker_num_ID_ID_not_in_triplet(){
    Tracker_num_ID_ID_not_in_triplet++;
};
void Tracker::add_plus1_Tracker_SP_num_indirect(){
    Tracker_SP_num_indirect++;
};
void Tracker::add_plus1_Tracker_SP_num_not_indirect(){
    Tracker_SP_num_not_indirect++;
};

void Tracker::add_plus1_Tracker_OL_num_indirect(){
    Tracker_OL_num_indirect++;
};
void Tracker::add_plus1_Tracker_OL_num_not_indirect(){
    Tracker_OL_num_not_indirect++;
};

void Tracker::add_plus1_Tracker_II_num_indirect(){
    Tracker_II_num_indirect++;
};
void Tracker::add_plus1_Tracker_II_num_not_indirect(){
    Tracker_II_num_not_indirect++;
};

void Tracker::add_plus1_Tracker_DPI_num_indirect(){
    Tracker_DPI_num_indirect++;
};
void Tracker::add_plus1_Tracker_DPI_num_not_indirect(){
    Tracker_DPI_num_not_indirect++;
};

void Tracker::add_plus1_Tracker_Combi_num_indirect(){
    Tracker_Combi_num_indirect++;
};
void Tracker::add_plus1_Tracker_Combi_num_not_indirect(){
    Tracker_Combi_num_not_indirect++;
};

// Track triplet info for triplet_info string
void Tracker::track_triplet_ENVs( std::vector< std::string > v ){
    Tracker_triplet_ENVs = string_vector_to_string( v, ";" );
};
void Tracker::track_triplet_sp( double d ){
    Tracker_triplet_sp.push_back( d );
};
void Tracker::track_triplet_ol( double d ){
    Tracker_triplet_ol.push_back( d );
};
void Tracker::track_triplet_cmi( double d ){
    Tracker_triplet_cmi.push_back( d );
};
void Tracker::track_triplet_ii( double d ){
    Tracker_triplet_ii.push_back( d );
};
void Tracker::track_triplet_ii_pvalue( double d ){
    Tracker_triplet_ii_pvalue.push_back( d );
};
void Tracker::track_triplet_dpi( double d ){
    Tracker_triplet_dpi.push_back( d );
    // count number of rank1/2/3
    if( d == 1 )
    {
        Tracker_triplet_num_rank_1++;
    }
    else if( d == 2 )
    {
        Tracker_triplet_num_rank_2++;
    }
    else if( d == 3 )
    {
        Tracker_triplet_num_rank_3++;
    }
    //else it is na("")
};
void Tracker::track_triplet_dpi_indirect( double d ){
    Tracker_triplet_dpi_indirect.push_back( d );
};
void Tracker::track_triplet_combi( double d ){
    Tracker_triplet_combi.push_back( d );
};
void Tracker::track_portion_nan_XYENV( double d ){
    Tracker_portion_nan_XYENV.push_back( d );
};

// Tracker_triplet Reset
void Tracker::tracker_triplet_reset(){
    Tracker_triplet_ENVs = "";
    Tracker_triplet_sp.clear();
    Tracker_triplet_ol.clear();
    Tracker_triplet_cmi.clear();
    Tracker_triplet_ii.clear();
    Tracker_triplet_ii_pvalue.clear();
    Tracker_triplet_dpi.clear();
    Tracker_triplet_dpi_indirect.clear();
    Tracker_triplet_num_rank_1 = 0;
    Tracker_triplet_num_rank_2 = 0;
    Tracker_triplet_num_rank_3 = 0;
    Tracker_triplet_combi.clear();
    Tracker_portion_nan_XYENV.clear();
};

// Get info
std::string Tracker::get_Tracker_triplet_ENVs(){
    return Tracker_triplet_ENVs;
};
std::string Tracker::get_Tracker_triplet_sp(){
    return double_vector_to_string( Tracker_triplet_sp, ";" );
};
std::string Tracker::get_Tracker_triplet_ol(){
    return double_vector_to_string( Tracker_triplet_ol, ";" );
};
std::string Tracker::get_Tracker_triplet_cmi(){
    return double_vector_to_string( Tracker_triplet_cmi, ";" );
};
std::string Tracker::get_Tracker_triplet_ii(){
    return double_vector_to_string( Tracker_triplet_ii, ";" );
};
std::string Tracker::get_Tracker_triplet_ii_pvalue(){
    return double_vector_to_string( Tracker_triplet_ii_pvalue, ";" );
};
std::string Tracker::get_Tracker_triplet_dpi(){
    return double_vector_to_string( Tracker_triplet_dpi, ";" );
};
std::string Tracker::get_Tracker_triplet_dpi_indirect(){
    return double_vector_to_string( Tracker_triplet_dpi_indirect, ";" );
};
int Tracker::get_Tracker_triplet_num_rank_1(){
    return Tracker_triplet_num_rank_1;
};
int Tracker::get_Tracker_triplet_num_rank_2(){
    return Tracker_triplet_num_rank_2;
};
int Tracker::get_Tracker_triplet_num_rank_3(){
    return Tracker_triplet_num_rank_3;
};
std::string Tracker::get_Tracker_triplet_combi(){
    return double_vector_to_string( Tracker_triplet_combi, ";" );
};
std::string Tracker::get_Tracker_portion_nan_XYENV(){
    return double_vector_to_string( Tracker_portion_nan_XYENV, ";" );
};

// Get output string
std::string Tracker::get_Tracker_report(){
    std::string outputline = "";

    outputline += "\nTrackers:\n";
    outputline += std::to_string( Tracker_num_ENV_connected_to_min_one_ID );
    outputline += " ENVs are connected to at least one ID parameter.\n";
    outputline += std::to_string( Tracker_num_ID_ENV_considered );
    outputline += " of ";
    outputline += std::to_string( Tracker_num_ID_ENV_all );
    outputline += " ID-ENV edges are considered.\n";
    outputline += std::to_string( Tracker_num_ID_ENV_in_triplet );
    outputline += " ID-ENV edges are within at least one triplet.\n";
    outputline += std::to_string( Tracker_num_ENV_ENV );
    outputline += " ENV-ENV edges.\n";
    outputline += std::to_string( Tracker_num_ID_ID );
    outputline += " ID-ID edges.\n";
    outputline += std::to_string( Tracker_num_ID_ID_not_in_triplet );
    outputline += " ID-ID edges that are not in a triplet.\n";
    outputline += std::to_string( Tracker_num_ID_connected_to_min_one_ENV );
    outputline += " IDs are connected to at least one ENV parameter.\n";
    outputline += std::to_string( Tracker_num_ID_ID_in_triplet );
    outputline += " ID-ID edges that are in at least one triplet.\n";
    outputline += " Determination of indirect edges:\n";
    if( ( Tracker_SP_num_indirect + Tracker_SP_num_not_indirect ) > 0 )
    {
        outputline += " Sign Pattern: ";
        outputline += std::to_string( Tracker_SP_num_indirect );
        outputline += " indirect, ";
        outputline += std::to_string( Tracker_SP_num_not_indirect );
        outputline += " not indirect\n";
    }
    if( ( Tracker_OL_num_indirect + Tracker_OL_num_not_indirect ) > 0 )
    {
        outputline += " Overlap: ";
        outputline += std::to_string( Tracker_OL_num_indirect );
        outputline += " indirect, ";
        outputline += std::to_string( Tracker_OL_num_not_indirect );
        outputline += " not indirect\n";
    }
    if( ( Tracker_II_num_indirect + Tracker_II_num_not_indirect ) > 0 )
    {
        outputline += " Interaction Information: ";
        outputline += std::to_string( Tracker_II_num_indirect );
        outputline += " indirect, ";
        outputline += std::to_string( Tracker_II_num_not_indirect );
        outputline += " not indirect\n";
    }
    if( ( Tracker_DPI_num_indirect + Tracker_DPI_num_not_indirect ) > 0 )
    {
        outputline += " Data Processing Inequality: ";
        outputline += std::to_string( Tracker_DPI_num_indirect );
        outputline += " indirect, ";
        outputline += std::to_string( Tracker_DPI_num_not_indirect );
        outputline += " not indirect\n";
    }
    if( ( Tracker_Combi_num_indirect + Tracker_Combi_num_not_indirect ) > 0 )
    {
        outputline += " Method combination: ";
        outputline += std::to_string( Tracker_Combi_num_indirect );
        outputline += " indirect, ";
        outputline += std::to_string( Tracker_Combi_num_not_indirect );
        outputline += " not indirect\n";
    }
    outputline += "\nDone!\n\n";

    return outputline;
};
