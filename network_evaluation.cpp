// network_evaluation.cpp
//
// Evaluate interactions in network given an evaluation file.
//
// Compiling: g++ -std=c++11 -o evaluate_nw network_evaluation.cpp
// Usage: ./evaluate_nw --nw_file [filename] --evaluation_file [filename] <option(s)>
//
// Input: network file and evaluation file
//
// Output: extended network file indicating if interaction was found in evaluation file or not
//
// Ina Maria Deutschmann
// Latest update: Sep. 2017

//#########################################################################################################################################################
// Include
#include <getopt.h>                   // parsing options

#include <iostream>
#include <fstream>                    // read in and out files
#include <boost/algorithm/string.hpp> // split
#include <algorithm>                  // find(v.begin(), v.end(), x) != v.end() find element in vector
//#include <exception>                  // exception

#include <vector>                     // vector
#include <string>                     // operator + for string concatenation
#include <unordered_map>              // unordered_maps

using namespace std;
//#########################################################################################################################################################

//#########################################################################################################################################################
// Datastructures and Variables
// filenames
string filename_network;                          // name of network file
string filename_evaluation;                       // name of evaluation file
string filename_output = "evaluated_network.txt"; // name of output file (Default: evluated_network.txt)
string filename_evaluation_more_info = "evaluation_info.txt"; // name of output file containing extra evaluation_info

// separators
string sep_nw = "\t";           // seaparator of network file
string sep_eval = "\t";         // separator of evaluation file
string sep_levels_in_nw = "\t"; // separator deviding levels in network file, if given in one column

// columns
vector< int > cols_X_nw;       // level information X of network file
vector< int > cols_Y_nw;       // level information Y of network file
vector< int > cols_X_eval;     // level information X of evaluation file
vector< int > cols_Y_eval;     // level information Y of evaluation file
vector< int > cols_eval_extra; // columns of evaluation file connected to interaction that should be included in outputfile

// bool
int only_evaluation_indicator = 0;       // print only one column in outputfile ("0" not found, "1" found, "NA" no info in evaluation file for min one interaction partner)
int nw_level_info_in_one_column = 0;     // the level info is in one column, so we have to split it with appropiate sep to get to the info (for this cols vector has to contain only one value)
int extra_file_evaluation_more_info = 0; // print evaluation information in extra file
int print_found_levels = 0;              // print levels that evaluated an edge

// Datastructures holding evaluation information
unordered_map< string, vector< int > > N;                     // Holding for each level an ID for the levelstring they are part of
unordered_map< int, unordered_map< int, vector<string> > > E; // ID - ID - extra infos that you may want to store (IDs sorted lexicographical for corresponding strings: Xstring < Ystring)
vector< string > IDs;                                         // ID of levelstring is it's position in the array
unordered_map< string, int > IDs_int;                         // ID hash (later converted into array)

// Tracker Variables
int num_found_interactions = 0;                // Number of interactions that were found in evaluation file
int num_not_found_interactions = 0;            // Number of interactions that were not found in evaluation file
int num_interactions_not_able_to_evaluate = 0; // Number of interactions that could not be evaluated due to missing level in evaluation file
int Tracker_edges_found = 0;
int Tracker_edges_not_found = 0;
int Tracker_min1node_but_edge_not_found = 0;
int Tracker_both_nodes_found = 0;

// Helpers
string line;                  // For reading in files
vector< string > info_vector; // For reading in files

//#########################################################################################################################################################

//#########################################################################################################################################################
// Functions
//*****************************************************************************************************************************************************
// Usage
static void show_usage(string name)
{
  cout << "\n###########################################################################\n" << endl;
  cout << "Usage: " <<  name << " --nw_file [filename] --evaluation_file [filename] <option(s)>\n" << endl;
  cout << "Options:\n";
  cout << "\t-h,--help------------------------------------------Show this help message\n" << endl;
  cout << "\t-n,--nw_file [filename]----------------------------Required! input: name of network file.\n";
  cout << "\t-e,--evaluation_file [filename]--------------------Required! input: name of evaluation file.\n" << endl;
  cout << "\t-k,--col_x_y_nw [cols_x_nw-cols_y_nw]--------------Required! input: column number or ',' seperated list of columns for levels of X and Y, e.g.: 4,5,6-7,8,9\n";
  cout << "\t-c,--col_x_y_evaluation [cols_x_eval-cols_y_eval]--Required! input: column number or ',' seperated list of columns for levels of X and Y, e.g.: 4,5,6-7,8,9\n";
  cout << "\t-x,--col_evaluation_extra_info [cols_eval_extra]---Optional input: column number or ',' seperated list of columns for extra information of evaluation file that should be added to output, e.g.: 2,4\n" << endl;;
  cout << "\t-s,--separator_network_file \"[sep]\"----------------Default:\"\\t\" , other possibilities, but not limited to: \";\" or \" \"" << endl;
  cout << "\t-r,--separator_evaluation_file \"[sep]\"-------------Default:\"\\t\" , other possibilities, but not limited to: \";\" or \" \"" << endl;
  cout << "\t-l,--separator_level \"[sep]\"-----------------------Default:\"\\t\" , other possibilities, but not limited to: \";\" or \" \". Used if the network levels are in one column.\n" << endl;
  cout << "\t-o,--output_filename [filename]--------------------Default: \"evaluated_network.txt\"\n" << endl;
  cout << "\t-u,--extra_file_evaluation_more_info [filename]----Default: evaluation information will be in network file if not choosen otherwise." << endl;
  cout << "\t-i,--only_evaluation_indicator---------------------Only add one column to indicate if interaction was found (\"0\" not found, \"1\" found, \"NA\" missing information in evaluation file).\n";
  cout << "\t-m,--nw_level_info_in_one_column-------------------Network levels are in one column, separated by \"separator_level\" (that has to be different from \"separator_network_file\")." << endl;
  cout << "\t-p,--print_found_levels----------------------------Print levels that were used to evaluate edges." << endl;
  cout << "\n###########################################################################\n" << endl;
}
//*****************************************************************************************************************************************************

//*****************************************************************************************************************************************************
// Functions for Options
void set_cols_nw( string info )
{
  split( info_vector, info, boost::is_any_of( "-" ) );
  string cols_X = info_vector[ 0 ];
  string cols_Y = info_vector[ 1 ];

  split( info_vector, cols_X, boost::is_any_of( "," ) );
  cols_X_nw.resize( info_vector.size() );
  for( int i = 0; i < info_vector.size(); i++ )
  {
    cols_X_nw[ i ] = stoi( info_vector[ i ] ) - 1;
  }

  split( info_vector, cols_Y, boost::is_any_of( "," ) );
  cols_Y_nw.resize( info_vector.size() );
  for( int i = 0; i < info_vector.size(); i++ )
  {
    cols_Y_nw[ i ] = stoi( info_vector[ i ] ) - 1;
  }
}
//*****************************************************************************************************************************************************

//*****************************************************************************************************************************************************
void set_cols_eval( string info )
{
  split( info_vector, info, boost::is_any_of( "-" ) );
  string cols_X = info_vector[ 0 ];
  string cols_Y = info_vector[ 1 ];

  split( info_vector, cols_X, boost::is_any_of( "," ) );
  cols_X_eval.resize( info_vector.size() );
  for( int i = 0; i < info_vector.size(); i++ )
  {
    cols_X_eval[ i ] = stoi( info_vector[ i ] ) - 1;
  }

  split( info_vector, cols_Y, boost::is_any_of( "," ) );
  cols_Y_eval.resize( info_vector.size() );
  for( int i = 0; i < info_vector.size(); i++ )
  {
    cols_Y_eval[ i ] = stoi( info_vector[ i ] ) - 1;
  }
}
//*****************************************************************************************************************************************************

//*****************************************************************************************************************************************************
void set_cols_eval_extra( string info )
{
  split( info_vector, info, boost::is_any_of( "," ) );
  cols_eval_extra.resize( info_vector.size() );
  for( int i = 0; i < info_vector.size(); i++ )
  {
    cols_eval_extra[ i ] = stoi(info_vector[ i ]) - 1;
  }
}
//*****************************************************************************************************************************************************

//*****************************************************************************************************************************************************
// int-vector to string
string int_vector_to_string( vector< int > v )
{
  string s = "";
  for( int i = 0; i < v.size(); i++ )
  {
    s += ";" + to_string( v[ i ] );
  }
  return s.erase(0, 1);
}
//*****************************************************************************************************************************************************

//*****************************************************************************************************************************************************
// string-vector to string
string string_vector_to_string( vector< string > v, string sep )
{
  string s = "";
  for( int i = 0; i < v.size(); i++ )
  {
    s += sep + v[ i ];
  }
  return s.erase(0, sep.size());
}
//*****************************************************************************************************************************************************
//#########################################################################################################################################################

//#########################################################################################################################################################
int main(int argc, char **argv)
{
  ofstream toLog( "logfile.txt" );

  toLog << "\n**************************************************" << endl;
  toLog << "*                                                *" << endl;
  toLog << "* ############################################## *" << endl;
  toLog << "* #                                            # *" << endl;
  toLog << "* # Welcome to the network_evaluation program. # *" << endl;
  toLog << "* # Let's get started ;)                       # *" << endl;
  toLog << "* #                                            # *" << endl;
  toLog << "* ############################################## *" << endl;
  toLog << "*                                                *" << endl;
  toLog << "**************************************************" << endl;

  toLog << "\nParameter setting:" << endl;
  //*****************************************************************************************************************************************************
  // 1. Parsing Options
  int c;
  int min_info = 0;

  struct option longopts[] = {
         { "help", no_argument, NULL, 'h' },
         { "nw_file", required_argument, NULL, 'n' },
         { "evaluation_file", required_argument, NULL, 'e' },
         { "col_x_y_nw", required_argument, NULL, 'k' },
         { "col_x_y_evaluation", required_argument, NULL, 'c' },
         { "col_evaluation_extra_info", required_argument, NULL, 'x' },
         { "separator_network_file", required_argument, NULL, 's' },
         { "separator_evaluation_file", required_argument, NULL, 'r' },
         { "separator_level", required_argument, NULL, 'l' },
         { "output_filename", required_argument, NULL, 'o' },
         { "extra_file_evaluation_more_info", required_argument, NULL, 'u'},
         { "only_evaluation_indicator", no_argument, NULL, 'i' },
         { "nw_level_info_in_one_column", no_argument, NULL, 'm' },
         { "print_found_levels", no_argument, NULL, 'p' },
         { 0, 0, 0, 0 }
  };

  while ( ( c = getopt_long( argc, argv, "hn:e:k:c:x:s:r:l:o:u:imp", longopts, NULL ) ) != -1 )
  {
    switch ( c )
    {
      case 0:
        /* getopt_long() sets a variable right now, keep going */
        break;

      // { "help", no_argument, NULL, 'h' },
      case 'h':
        show_usage( argv[ 0 ] );
        return 1;

      // { "nw_file", required_argument, NULL, 'n' },
      case 'n':
        // Name of network file
        filename_network = optarg;
        toLog << "The network filename is set to " << filename_network << endl;
        // bool indicator if min_info is given by user
        min_info += 1;
        break;

      // { "evaluation_file", required_argument, NULL, 'e' },
      case 'e':
        // Name of evaluation file
        filename_evaluation = optarg;
        toLog << "The evaluation filename is set to " << filename_evaluation << endl;
        min_info += 1;
        break;

      // { "col_x_y_nw", required_argument, NULL, 'k' },
      case 'k':
        // Set columns for network
        set_cols_nw( optarg );
        toLog << "The columns for the levels in the network file are:" << endl;
        toLog << "X: ";
        for( int i = 0; i < cols_X_nw.size(); i++ )
        {
          toLog << cols_X_nw[ i ] + 1 << ", ";
        }
        toLog << "\nand" << endl;
        toLog << "Y: ";
        for( int i = 0; i < cols_Y_nw.size(); i++ )
        {
          toLog << cols_Y_nw[ i ] + 1 << ", ";
        }
        toLog << endl;
        min_info += 1;
        break;

      // { "col_x_y_evaluation", required_argument, NULL, 'c' },
      case 'c':
        // Set columns for evaluation file
        set_cols_eval( optarg );
        toLog << "The columns for the levels in the evaluation file are:" << endl;
        toLog << "X: ";
        for( int i = 0; i < cols_X_eval.size(); i++ )
        {
          toLog << cols_X_eval[ i ] + 1 << ", ";
        }
        toLog << "\nand" << endl;
        toLog << "Y: ";
        for( int i = 0; i < cols_Y_eval.size(); i++ )
        {
          toLog << cols_Y_eval[ i ] + 1 << ", ";
        }
        toLog << endl;
        min_info += 1;
        break;

      // { "col_evaluation_extra_info", required_argument, NULL, 'x' },
      case 'x':
        // Set columns for extra information of evaluation file
        set_cols_eval_extra( optarg );
        toLog << "The columns for the extra information of the evaluation file are:" << endl;
        toLog << "Extra: ";
        for( int i = 0; i < cols_eval_extra.size(); i++ )
        {
          toLog << cols_eval_extra[ i ] + 1 << ", ";
        }
        toLog << endl;
        break;

      // { "separator_network_file", required_argument, NULL, 's' },
      case 's':
        sep_nw = optarg;
        toLog << "The separator in the network file is assumed to be \"" << sep_nw << "\"" << endl;
        break;

      // { "separator_evaluation_file", required_argument, NULL, 'r' },
      case 'r':
        sep_eval = optarg;
        toLog << "The separator in the evaluation file is assumed to be \"" << sep_eval << "\"" << endl;
        break;

      // { "separator_level", required_argument, NULL, 'l' },
      case 'l':
        sep_levels_in_nw = optarg;
        toLog << "The separator for the different levels in the network file (containing the level information in one column per interaction partner) is assumed to be \"" << sep_levels_in_nw << "\"" << endl;
        break;

      // { "output_filename", required_argument, NULL, 'o' },
      case 'o':
        filename_output = optarg;
        toLog << "The output network filename is set to " << filename_output << endl;
        break;

      // { "extra_file_evaluation_more_info", required_argument, NULL, 'u'},
      case 'u':
        filename_evaluation_more_info = optarg;
        toLog << "The extra file with evaluation information is set to " << filename_evaluation_more_info << endl;
        extra_file_evaluation_more_info = 1;
        break;

      // { "only_evaluation_indicator", no_argument, NULL, 'i' },
      case 'i':
        only_evaluation_indicator = 1;
        toLog << "The output will only contain one extra column indicating if interaction was found in the evaluation file or not. (0 = not found, 1 = found, NA = level not found in evaluation file.) " << endl;
        break;

      // { "nw_level_info_in_one_column", no_argument, NULL, 'm' },
      case 'm':
        nw_level_info_in_one_column = 1;
        toLog << "We assume that the levels of the two interaction partners are in one column each, separated by a different separator than the columns in the network file." << endl;
        break;

      // { "print_found_levels", no_argument, NULL, 'p' },
      case 'p':
        print_found_levels = 1;
        toLog << "We print the levels that have been used to evaluate edges." << endl;
        break;

      case '?':
          cout << "\nAn unexpected error occured.\n" << endl;
          // To logfile
          toLog << "\nAn unexpected error occured.\n" << endl;
          show_usage( argv[ 0 ] );
        return 1;
        break;

          // Error handling code here would be good, but obvious errors should be already catched before.
    }
  }

  // Error found
  int error_found = 0;

  // check if min needed information is given
  if( min_info != 4)
  {
    cout << "\nAttention: important information(inputfile and/or columns) are missing.\n" << endl;
    toLog << "\nAttention: important information(inputfile and/or columns) are missing.\n" << endl;
    error_found = 1;
  }

  // check if number of given columns for X and Y match
  if( cols_X_nw.size() != cols_Y_nw.size() )
  {
    cout << "Error: different number of columns for X and Y (network file)." << endl;
    toLog << "Error: different number of columns for X and Y (network file)." << endl;
    error_found = 1;
  }
  if( cols_X_eval.size() != cols_Y_eval.size() )
  {
    cout << "Error: different number of columns for X and Y (evaluation file)." << endl;
    toLog << "Error: different number of columns for X and Y (evaluation file)." << endl;
    error_found = 1;
  }

  // check if separators of network file and level within one column are different,
  // and if option was choosen and there should be just one column
  if( nw_level_info_in_one_column )
  {
    if( cols_X_nw.size() > 1 || cols_Y_nw.size() > 1)
    {
      cout << "Option \"nw_level_info_in_one_column\" was choosen. Please indicate only one column per interaction partner." << endl;
      cout << "Note: if you only want to use one level and therefore one column, do not choose option \"nw_level_info_in_one_column\"." << endl;
      toLog << "Option \"nw_level_info_in_one_column\" was choosen. Please indicate only one column per interaction partner." << endl;
      toLog << "Note: if you only want to use one level and therefore one column, do not choose option \"nw_level_info_in_one_column\"." << endl;
      error_found = 1;
    }
    if( sep_nw == sep_levels_in_nw )
    {
      cout << "Error: The assumed separator of the network file is the same as the assumed separator for the different levels that are within one column." << endl;
      cout << "Note: if you only want to use one level and therefore one column, do not choose option \"nw_level_info_in_one_column\"." << endl;
      toLog << "Error: The assumed separator of the network file is the same as the assumed separator for the different levels that are within one column." << endl;
      toLog << "Note: if you only want to use one level and therefore one column, do not choose option \"nw_level_info_in_one_column\"." << endl;
      error_found = 1;
    }
  }
  else
  {
    // Put ";" instead of tab as separator
    sep_levels_in_nw = ";";
  }

  // End program if error found
  if( error_found )
  {
    show_usage( argv[ 0 ] );
    return 1;
  }
  toLog << "\n\n###########################################################################\n" << endl;
  //*****************************************************************************************************************************************************

  //*****************************************************************************************************************************************************
  // 2. Read in evaluation file containing information to evaluate edges in the network_evaluation
  ifstream Read_evalutation_file( filename_evaluation );
  if( !Read_evalutation_file.good() )
  {
    cout << "Error opening '" << filename_evaluation << "'. Closing program." << endl;
    toLog << "Error opening '" << filename_evaluation << "'. Closing program." << endl;
    return -1;
  }
  if( getline( Read_evalutation_file, line).good() ) // First line is header
  {
    toLog << "Reading in " << filename_evaluation << "." << endl;

    // header
    // remove "newline" sign if there is one at the end of the line
    size_t found = line.find( '\n' );
    if( found != string::npos ){ line = line.substr(0, line.size()-1); }
    found = line.find( '\r' );
    if( found != string::npos ){ line = line.substr(0, line.size()-1); }
    split( info_vector, line, boost::is_any_of( sep_eval ) ); // Split by sep_eval, vector positions starts with 0!

    // check if given column numbers are within the file
    int column_out_of_range = 0;
    for( int i = 0; i < cols_X_eval.size(); i++ )
    {
      if( cols_X_eval[ i ] >= info_vector.size() ){ column_out_of_range++; }
      if( cols_Y_eval[ i ] >= info_vector.size() ){ column_out_of_range++; }
    }
    for( int i = 0; i < cols_eval_extra.size(); i++ )
    {
      if( cols_eval_extra[ i ] >= info_vector.size() ){ column_out_of_range++; }
    }
    if( column_out_of_range > 0)
    {
      cout << "Error: " << column_out_of_range << " column numbers for the evaluation file are out of range." << endl;
      toLog << "Error: " << column_out_of_range << " column numbers for the evaluation file are out of range." << endl;
      return 1;
    }

    // To bash as info
    toLog << "Evaluation level information for X: ";
    for( int i = 0; i < cols_X_eval.size(); i++ )
    {
      toLog << info_vector[ cols_X_eval[ i ] ] << ", ";
    }
    toLog << "\n" << endl;
    toLog << "Evaluation level information for Y: ";
    for( int i = 0; i < cols_Y_eval.size(); i++ )
    {
      toLog << info_vector[ cols_Y_eval[ i ] ] << ", ";
    }
    toLog << "\n" << endl;
    if( cols_eval_extra.size() > 0 )
    {
      toLog << "Keeping the following extra information: ";
      for( int i = 0; i < cols_eval_extra.size(); i++ )
      {
        toLog << info_vector[ cols_eval_extra[ i ] ] << ", ";
      }
      toLog << "\n" << endl;
    }

    // To give level strings an ID (so we just have int in datastructre and not string, should be better for storage)
    int ID_count = 0;

    // Read evaluation file line by line and build up datastructres
    while( getline( Read_evalutation_file, line).good() )
    {
      // To save info
      vector< string > X_eval;
      vector< string > Y_eval;
      vector< string > Extra_eval;

      // Get info
      // remove "newline" sign if there is one at the end of the line
      size_t found = line.find( '\n' );
      if( found != string::npos ){ line = line.substr(0, line.size()-1); }
      found = line.find( '\r' );
      if( found != string::npos ){ line = line.substr(0, line.size()-1); }
      split( info_vector, line, boost::is_any_of( sep_eval ) ); // Split by sep_eval, vector positions starts with 0!
      for( int i = 0; i < cols_X_eval.size(); i++ )
      {
        X_eval.push_back( info_vector[ cols_X_eval[ i ] ] );
        Y_eval.push_back( info_vector[ cols_Y_eval[ i ] ] );
      }
      for( int i = 0; i < cols_eval_extra.size(); i++ )
      {
        Extra_eval.push_back( info_vector[ cols_eval_extra[ i ] ] );
      }

      // Give ID
      // Check if X_Levels in IDs hash
      string X_Levels_string = string_vector_to_string( X_eval, ";" );
      unordered_map< string, int >::iterator h = IDs_int.find( X_Levels_string );
      if( h == IDs_int.end() )
      {
        // Not found -> insert
        IDs_int[ X_Levels_string ] = ID_count;
        IDs.push_back( X_Levels_string );
        ID_count++;
      }
      string Y_Levels_string = string_vector_to_string( Y_eval, ";" );
      h = IDs_int.find( Y_Levels_string );
      if( h == IDs_int.end() )
      {
        // Not found -> insert
        IDs_int[ Y_Levels_string ] = ID_count;
        IDs.push_back( Y_Levels_string );
        ID_count++;
      }

      // Safe all levels of X and Y in datastructre N
      for( int i = 0; i < X_eval.size(); i++)
      {
        // Check if level of X in hash
        unordered_map< string, vector< int > >::iterator n = N.find( X_eval[ i ] );
        if( n == N.end() )
        {
          // Not found -> insert
          vector< int > my_vector;
          my_vector.push_back( IDs_int[ X_Levels_string ] );
          N[ X_eval[ i ] ] = my_vector;
        }
        else
        {
          // Found
          // Check if X_Levels_string is already in
          if( N[ X_eval[ i ] ].back() < IDs_int[ X_Levels_string ] )
          {
            // Last ID is smaller than current ID, so its not in and must be added
            N[ X_eval[ i ] ].push_back( IDs_int[ X_Levels_string ] );          }
        }

        // Check if level of Y in hash
        n = N.find( Y_eval[ i ] );
        if( n == N.end() )
        {
          // Not found -> insert
          vector< int > my_vector;
          my_vector.push_back( IDs_int[ Y_Levels_string ] );
          N[ Y_eval[ i ] ] = my_vector;
        }
        else
        {
          // Found
          // Check if Y_Levels_string is already in
          if( N[ Y_eval[ i ] ].back() > IDs_int[ Y_Levels_string ] )
          {
            // Last ID is smaller than current ID, so its not in and must be added
            N[ Y_eval[ i ] ].push_back( IDs_int[ Y_Levels_string ] );
          }
        }
      }

      // Safe in datastructre E
      // lexicographical_compare(first<last) TRUE, else FALSE
      if(lexicographical_compare( X_Levels_string.begin(), X_Levels_string.end(), Y_Levels_string.begin(), Y_Levels_string.end() ) )
      {
        // Then don't do anything
      }
      else
      {
        // Change
        string swaper = X_Levels_string;
        X_Levels_string = Y_Levels_string;
        Y_Levels_string = swaper;
      }

      // Check is X_Levels_string in hash
      unordered_map< int, unordered_map< int, vector< string > > >::iterator e_X = E.find( IDs_int[ X_Levels_string ] );
      if( e_X == E.end() )
      {
        // Not found -> add
        vector< string > my_vector;
        my_vector.push_back( string_vector_to_string( Extra_eval, ";" ) );
        E[ IDs_int[ X_Levels_string ] ][ IDs_int[ Y_Levels_string ] ] = my_vector;
      }
      else
      {
        // Found
        // Check if Y_Levels_string in hash
        unordered_map< int, vector< string > >::iterator e_Y = E[ IDs_int[ X_Levels_string ] ].find( IDs_int[ Y_Levels_string ] );
        if( e_Y == E[ IDs_int[ X_Levels_string ] ].end() )
        {
          // Not found -> add
          vector< string > my_vector;
          my_vector.push_back( string_vector_to_string( Extra_eval, ";" ) );
          E[ IDs_int[ X_Levels_string ] ][ IDs_int[ Y_Levels_string ] ] = my_vector;
        }
        else
        {
          // Found -> update Extra_eval
          E[ IDs_int[ X_Levels_string ] ][ IDs_int[ Y_Levels_string ] ].push_back( string_vector_to_string( Extra_eval, ";" ) );
        }
      }
      // Safe extra info was done when building up E, both were out together
    }
  }
  else
  {
    cout << filename_evaluation << " is empty. Closing program." << endl;
    toLog << filename_evaluation << " is empty. Closing program." << endl;
    return -1;
  }
  Read_evalutation_file.close();

  // We no longer need ID hash, but could use him to track if string was found or not
  // For that all items should be put to 0
  //for( unordered_map< string, unordered_map< string, unordered_map< string, double > > >::iterator t1 = Tracker_OTU_ENV_edges_inTriplet.begin(); t1 != Tracker_OTU_ENV_edges_inTriplet.end(); t1++ )
  //{
    //Set to zero
  //}
  //*****************************************************************************************************************************************************

  //*****************************************************************************************************************************************************
  // 3. Read in network file line by line to evaluate edges and print output
  ifstream Read_network_file( filename_network );
  if( !Read_network_file.good() )
  {
    cout << "Error opening '" << filename_network << "'. Closing program." << endl;
    toLog << "Error opening '" << filename_network << "'. Closing program." << endl;
    return -1;
  }
  if( getline( Read_network_file, line).good() ) // First line is header
  {
    toLog << "Reading in " << filename_network << "." << endl;

    // header
    // remove "newline" sign if there is one at the end of the line
    size_t found = line.find( '\n' );
    if( found != string::npos ){ line = line.substr(0, line.size()-1); }
    found = line.find( '\r' );
    if( found != string::npos ){ line = line.substr(0, line.size()-1); }
    split( info_vector, line, boost::is_any_of( sep_nw ) ); // Split by sep_eval, vector positions starts with 0!

    // check if given column numbers are within the file
    int column_out_of_range = 0;
    for( int i = 0; i < cols_X_nw.size(); i++ )
    {
      if( cols_X_nw[ i ] >= info_vector.size() ){ column_out_of_range++; }
      if( cols_Y_nw[ i ] >= info_vector.size() ){ column_out_of_range++; }
    }
    if( column_out_of_range > 0)
    {
      cout << "Error: " << column_out_of_range << " column numbers for the network file are out of range." << endl;
      toLog << "Error: " << column_out_of_range << " column numbers for the network file are out of range." << endl;
      return 1;
    }
    // If info not in one column, than put it together to one
    string nw_X_Levels_string;
    string nw_Y_Levels_string;
    if( nw_level_info_in_one_column == 1 )
    {
      // X_nw_levelstring
      nw_X_Levels_string = info_vector[ cols_X_nw[ 0 ] ];
      // Y_nw_levelstring
      nw_Y_Levels_string = info_vector[ cols_Y_nw[ 0 ] ];
    }
    else
    {
      // Creating network level strings
      for( int i = 0; i < cols_X_nw.size(); i++ )
      {
        // X_nw_levelstring
        nw_X_Levels_string += sep_levels_in_nw + info_vector[ cols_X_nw[ i ] ];
        // Y_nw_levelstring
        nw_Y_Levels_string += sep_levels_in_nw + info_vector[ cols_Y_nw[ i ] ];
      }
      nw_X_Levels_string.erase(0, sep_levels_in_nw.length());
      nw_Y_Levels_string.erase(0, sep_levels_in_nw.length());
    }

    // To bash as info
    toLog << "Network level information for X: ";
    for( int i = 0; i < cols_X_eval.size(); i++ )
    {
      toLog << nw_X_Levels_string << ", ";
    }
    toLog << "\n" << endl;
    toLog << "Network level information for Y: ";
    for( int i = 0; i < cols_Y_eval.size(); i++ )
    {
      toLog << nw_Y_Levels_string << ", ";
    }
    toLog << "\n" << endl;

    // Output
    ofstream evalNW_output( filename_output );
    // if option "only_evaluation_indicator", print info to extra file
    if( only_evaluation_indicator )
    {
      // output
      evalNW_output << line << sep_nw << "found" << sep_nw << "eval_ID" << endl;
    }
    else
    {
      // output
      evalNW_output << line << sep_nw << "found" << sep_nw << "eval_ID" << sep_nw;
      evalNW_output << "num_level_matches" << sep_nw << "num_extra_info";
      if( print_found_levels == 1 )
      {
        evalNW_output << sep_nw << "level_matches_string" << sep_nw << "alllevel_matches_string";
      }
      if( cols_eval_extra.size() > 0 )
      {
        // collect extra info
        evalNW_output << sep_nw << "extra_info" << endl;
      }
      else
      {
        evalNW_output << endl;
      }
    }
    // option if evaluation info in extra file
    // Evaluation info
    ofstream extra_info( filename_evaluation_more_info );
    if( extra_file_evaluation_more_info )
    {
      // output
      extra_info << "eval_ID" << sep_nw << "num_level_matches" << sep_nw << "num_extra_info";
      if( print_found_levels == 1 )
      {
        extra_info << sep_nw << "level_matches_string" << sep_nw << "alllevel_matches_string";
      }
      if( cols_eval_extra.size() > 0 )
      {
        // collect extra info
        extra_info << sep_nw << "extra_info" << endl;
      }
      else
      {
        extra_info << endl;
      }
    }

    // Read nw file line by line and evaluate edges
    // Evaluation ID
    int eval_ID = 1;
    while( getline( Read_network_file, line).good() )
    {
      // Get info
      // remove "newline" sign if there is one at the end of the line
      size_t found = line.find( '\n' );
      if( found != string::npos ){ line = line.substr(0, line.size()-1); }
      found = line.find( '\r' );
      if( found != string::npos ){ line = line.substr(0, line.size()-1); }
      split( info_vector, line, boost::is_any_of( sep_nw ) ); // Split by sep_nw, vector positions starts with 0!
      if( nw_level_info_in_one_column == 1 )
      {
        // X_nw_levelstring
        nw_X_Levels_string = info_vector[ cols_X_nw[ 0 ] ];
        // Y_nw_levelstring
        nw_Y_Levels_string = info_vector[ cols_Y_nw[ 0 ] ];
      }
      else
      {
        // X_nw_levelstring
        nw_X_Levels_string = "";
        // Y_nw_levelstring
        nw_Y_Levels_string = "";
        // Creating network level strings
        for( int i = 0; i < cols_X_nw.size(); i++ )
        {
          // X_nw_levelstring
          nw_X_Levels_string += sep_levels_in_nw + info_vector[ cols_X_nw[ i ] ];
         // Y_nw_levelstring
          nw_Y_Levels_string += sep_levels_in_nw + info_vector[ cols_Y_nw[ i ] ];
        }
        nw_X_Levels_string.erase(0, sep_levels_in_nw.length());
        nw_Y_Levels_string.erase(0, sep_levels_in_nw.length());
      }

      string found_level_X_string = "";
      string found_level_Y_string = "";
      vector< string > found_level_X;
      vector< string > found_level_Y;
      vector< string > l;

      // For each level in X, check if it is in N
        if( nw_level_info_in_one_column == 1 || cols_X_nw.size() > 1 )
        {
          split( info_vector, nw_X_Levels_string, boost::is_any_of( sep_levels_in_nw ) );
        }
        else
        {
            info_vector.clear();
            info_vector.push_back(nw_X_Levels_string);
        }
      for( int i = 0; i < info_vector.size(); i++ )
      {
        if( info_vector[ i ].size() > 0 && info_vector[ i ] != "NA" && info_vector[ i ] != "na" )
        {
          // Check if level of X in hash
          unordered_map< string, vector< int > >::iterator n = N.find( info_vector[ i ] );
          if( n == N.end() )
          {
            // Not found
            found_level_X_string += sep_levels_in_nw + "NA";
          }
          else
          {
            // Found
            found_level_X_string += sep_levels_in_nw + info_vector[ i ];
            found_level_X.push_back( info_vector[ i ] );
          }
        }
      }
      found_level_X_string = found_level_X_string.erase( 0, sep_levels_in_nw.size() );


      // For each level in Y, check if it is in N
        if( nw_level_info_in_one_column == 1 || cols_Y_nw.size() > 1 )
        {
            split( info_vector, nw_Y_Levels_string, boost::is_any_of( sep_levels_in_nw ) );
        }
        else
        {
            info_vector.clear();
            info_vector.push_back(nw_Y_Levels_string);
        }
      for( int i = 0; i < info_vector.size(); i++ )
      {
        if( info_vector[ i ].size() > 0 && info_vector[ i ] != "NA" && info_vector[ i ] != "na" )
        {
          // Check if level of X in hash
          unordered_map< string, vector< int > >::iterator n = N.find( info_vector[ i ] );
          if( n == N.end() )
          {
            // Not found
            found_level_Y_string += sep_levels_in_nw + "NA";
          }
          else
          {
            // Found
            found_level_Y_string += sep_levels_in_nw + info_vector[ i ];
            found_level_Y.push_back( info_vector[ i ] );
          }
        }
      }
      found_level_Y_string = found_level_Y_string.erase( 0, sep_levels_in_nw.size() );

      // Check if edge is in evaluation datastructre
      int bool_edge_found = 0;
      int num_level_matches = 0; // number of matches we obtain while going through levels
      int num_extra_info = 0;    // in our case it will be the number of references
      string level_matches_string = "";
      string alllevel_matches_string = "";
      string extra_info_string = "";

      // level not found in string -> don't do anything
      if( found_level_X.size() > 0 && found_level_Y.size() > 0 )
      {
        Tracker_both_nodes_found++;
        vector< int > allLevelstring_of_X;
        vector< int > allLevelstring_of_Y;

        // For each level in X, check if there is a connection to a level in Y
        for( int i = 0; i < found_level_X.size(); i++ )
        {
          for( int n = 0; n < N[ found_level_X[ i ] ].size(); n++ )
          {
            if( find( allLevelstring_of_X.begin(), allLevelstring_of_X.end(), N[ found_level_X[ i ] ][ n ] ) == allLevelstring_of_X.end() )
            {
              // Vector does not contain element
              allLevelstring_of_X.push_back( N[ found_level_X[ i ] ][ n ] );
            }
          }
        }

        for( int j = 0; j < found_level_Y.size(); j++ )
        {
          for( int n = 0; n < N[ found_level_Y[ j ] ].size(); n++ )
          {
            if( find( allLevelstring_of_Y.begin(), allLevelstring_of_Y.end(), N[ found_level_Y[ j ] ][ n ] ) == allLevelstring_of_Y.end() )
            {
              // Vector does not contain element
              allLevelstring_of_Y.push_back( N[ found_level_Y[ j ] ][ n ] );
            }
          }
        }

        // matching levels
        for( int i = 0; i < found_level_X.size(); i++ )
        {
          vector< int > x = N[ found_level_X[ i ] ];
          for( int j = 0; j < found_level_Y.size(); j++ )
          {
            vector< int > y = N[ found_level_Y[ j ] ];

            // check if there is a matching level
            for( int allL_X = 0; allL_X < x.size(); allL_X++ )
            {
              for( int allL_Y = 0; allL_Y < y.size(); allL_Y++ )
              {
                // check if allL_X of (allL_X - allL_Y) is in E
                unordered_map< int, unordered_map< int, vector<string> > >::iterator e1 = E.find( x[ allL_X ] );
                if( e1 == E.end() )
                {
                  // Not found
                  // check if allL_Y of (allL_Y - allL_X) is in E
                  unordered_map< int, unordered_map< int, vector<string> > >::iterator e1_switched = E.find( y[ allL_Y ] );
                  if( e1_switched == E.end() )
                  {
                    // Not found
                  }
                  else
                  {
                    // Found
                    // check if allL_X of (allL_Y - allL_X) is in E
                    unordered_map< int, vector<string> >::iterator e2_switched = E[ y[ allL_Y ] ].find( x[ allL_X ] );
                    if( e2_switched == E[ y[ allL_Y ] ].end() )
                    {
                      // Not found
                    }
                    else
                    {
                      // matched level of X and Y
                      string s = found_level_X[ i ] + ":" + found_level_Y[ j ] + "-";
                      if(find(l.begin(), l.end(), s ) == l.end() )
                      {
                        // Vector does not contain element
                        l.push_back( s );
                      }
                    }
                  }
                }
                else
                {
                  // Found
                  // check if allL_Y of (allL_X - allL_Y) is in E
                  unordered_map< int, vector<string> >::iterator e2 = E[ x[ allL_X ] ].find( y[ allL_Y ] );
                  if( e2 == E[ x[ allL_X ] ].end() )
                  {
                      // Not found
                      // check if allL_Y of (allL_Y - allL_X) is in E
                      unordered_map< int, unordered_map< int, vector<string> > >::iterator e1_switched = E.find( y[ allL_Y ] );
                      if( e1_switched == E.end() )
                      {
                          // Not found
                      }
                      else
                      {
                          // Found
                          // check if allL_X of (allL_Y - allL_X) is in E
                          unordered_map< int, vector<string> >::iterator e2_switched = E[ y[ allL_Y ] ].find( x[ allL_X ] );
                          if( e2_switched == E[ y[ allL_Y ] ].end() )
                          {
                              // Not found
                          }
                          else
                          {
                              // matched level of X and Y
                              string s = found_level_X[ i ] + ":" + found_level_Y[ j ] + "-";
                              if(find(l.begin(), l.end(), s ) == l.end() )
                              {
                                  // Vector does not contain element
                                  l.push_back( s );
                              }
                          }
                      }
                  }
                  else
                  {
                    // matched level of X and Y
                    string s = found_level_X[ i ] + ":" + found_level_Y[ j ] + "-";
                    if(find(l.begin(), l.end(), s ) == l.end() )
                    {
                      // Vector does not contain element
                      l.push_back( s );
                    }
                  }
                }
              }
            }
          }
        }

        // Now we need to check if the neighbors of X are the level of Y or vice versa
        for( int allL_X = 0; allL_X < allLevelstring_of_X.size(); allL_X++ )
        {
          for( int allL_Y = 0; allL_Y < allLevelstring_of_Y.size(); allL_Y++ )
          {
            // check if allL_X of (allL_X - allL_Y) is in E
            unordered_map< int, unordered_map< int, vector<string> > >::iterator e1 = E.find( allLevelstring_of_X[ allL_X ] );
            if( e1 == E.end() )
            {
              // Not found
              // check if allL_Y of (allL_Y - allL_X) is in E
              unordered_map< int, unordered_map< int, vector<string> > >::iterator e1_switched = E.find( allLevelstring_of_Y[ allL_Y ] );
              if( e1_switched == E.end() )
              {
                // Not found
              }
              else
              {
                // Found
                // check if allL_X of (allL_Y - allL_X) is in E
                unordered_map< int, vector<string> >::iterator e2_switched = E[ allLevelstring_of_Y[ allL_Y ] ].find( allLevelstring_of_X[ allL_X ] );
                if( e2_switched == E[ allLevelstring_of_Y[ allL_Y ] ].end() )
                {
                  // Not found
                }
                else
                {
                  // Found
                  bool_edge_found = 1;
                  // number of matches we obtain while going through levels
                  num_level_matches++;
                  // in our case it will be the number of references
                  num_extra_info += E[ allLevelstring_of_Y[ allL_Y ] ][ allLevelstring_of_X[ allL_X ] ].size();
                  // matched level of X and Y
                  // level_matches_string += found_level_X[ i ] + ":" + found_level_Y[ j ] + "-";
                  // matched all level of X and Y
                  alllevel_matches_string += IDs[ allLevelstring_of_X[ allL_X ] ] + ":" + IDs[ allLevelstring_of_Y[ allL_Y ] ] + "-";
                  // collect extra info
                  extra_info_string += string_vector_to_string( E[ allLevelstring_of_Y[ allL_Y ] ][ allLevelstring_of_X[ allL_X ] ], "-" );
                }
              }
            }
            else
            {
              // Found
              // check if allL_Y of (allL_X - allL_Y) is in E
              unordered_map< int, vector<string> >::iterator e2 = E[ allLevelstring_of_X[ allL_X ] ].find( allLevelstring_of_Y[ allL_Y ] );
              if( e2 == E[ allLevelstring_of_X[ allL_X ] ].end() )
              {
                  // Not found
                  // check if allL_Y of (allL_Y - allL_X) is in E
                  unordered_map< int, unordered_map< int, vector<string> > >::iterator e1_switched = E.find( allLevelstring_of_Y[ allL_Y ] );
                  if( e1_switched == E.end() )
                  {
                      // Not found
                  }
                  else
                  {
                      // Found
                      // check if allL_X of (allL_Y - allL_X) is in E
                      unordered_map< int, vector<string> >::iterator e2_switched = E[ allLevelstring_of_Y[ allL_Y ] ].find( allLevelstring_of_X[ allL_X ] );
                      if( e2_switched == E[ allLevelstring_of_Y[ allL_Y ] ].end() )
                      {
                          // Not found
                      }
                      else
                      {
                          // Found
                          bool_edge_found = 1;
                          // number of matches we obtain while going through levels
                          num_level_matches++;
                          // in our case it will be the number of references
                          num_extra_info += E[ allLevelstring_of_Y[ allL_Y ] ][ allLevelstring_of_X[ allL_X ] ].size();
                          // matched level of X and Y
                          // level_matches_string += found_level_X[ i ] + ":" + found_level_Y[ j ] + "-";
                          // matched all level of X and Y
                          alllevel_matches_string += IDs[ allLevelstring_of_X[ allL_X ] ] + ":" + IDs[ allLevelstring_of_Y[ allL_Y ] ] + "-";
                          // collect extra info
                          extra_info_string += string_vector_to_string( E[ allLevelstring_of_Y[ allL_Y ] ][ allLevelstring_of_X[ allL_X ] ], "-" );
                      }
                  }
              }
              else
              {
                // Found
                bool_edge_found = 1;
                // number of matches we obtain while going through levels
                num_level_matches++;
                // in our case it will be the number of references
                num_extra_info += E[ allLevelstring_of_X[ allL_X ] ][ allLevelstring_of_Y[ allL_Y ] ].size();
                // matched level of X and Y
                // level_matches_string += found_level_X[ i ] + ":" + found_level_Y[ j ] + "-";
                // matched all level of X and Y
                alllevel_matches_string += IDs[ allLevelstring_of_X[ allL_X ] ] + ":" + IDs[ allLevelstring_of_Y[ allL_Y ] ] + "-";
                // collect extra info
                extra_info_string += string_vector_to_string( E[ allLevelstring_of_X[ allL_X ] ][ allLevelstring_of_Y[ allL_Y ] ], "-" );
              }
            }
          }
        }
      }
      // matched level of X and Y

      level_matches_string = string_vector_to_string( l, "" );
      level_matches_string = level_matches_string.substr( 0, level_matches_string.size()-1);
      // matched all level of X and Y
      alllevel_matches_string = alllevel_matches_string.substr( 0, alllevel_matches_string.size()-1);
      // collect extra info
      extra_info_string =  extra_info_string.substr( 0, extra_info_string.size()-1);

      // Output
      if( bool_edge_found == 1 )
      {
        Tracker_edges_found++;

        if( only_evaluation_indicator == 1 )
        {
          // output
          evalNW_output << line << sep_nw << "1" << sep_nw << eval_ID << endl;
        }
        else
        {
          // output
          evalNW_output << line << sep_nw << 1 << sep_nw << eval_ID << sep_nw;
          evalNW_output << num_level_matches << sep_nw << num_extra_info;
          if( print_found_levels == 1 )
          {
            evalNW_output << sep_nw << level_matches_string << sep_nw << alllevel_matches_string;
          }
          if( cols_eval_extra.size() > 0 )
          {
            // collect extra info
            evalNW_output << sep_nw << extra_info_string << endl;
          }
          else
          {
            evalNW_output << endl;
          }
        }
        // option if evaluation info in extra file
        if( extra_file_evaluation_more_info == 1 )
        {
          // Evaluation info
          // output
          extra_info << eval_ID << sep_nw << num_level_matches << sep_nw << num_extra_info;
          if( print_found_levels == 1 )
          {
            extra_info << sep_nw << level_matches_string << sep_nw << alllevel_matches_string;
          }
          if( cols_eval_extra.size() > 0 )
          {
            // collect extra info
            extra_info << sep_nw << extra_info_string << endl;
          }
          else
          {
            extra_info << endl;
          }
        }
        eval_ID++;
      }
      else
      {
        // edge was not found
        // check if levels were at least in datastructre
        // if for X and or Y we could not find a min level in the datastructre
        // then we should indicate it with "NA" instead of "0"
        if( found_level_X.size() > 0 || found_level_Y.size() > 0 )
        {
          Tracker_min1node_but_edge_not_found++;

          if( only_evaluation_indicator )
          {
            // output
            evalNW_output << line << sep_nw << "NA" << sep_nw << "NA" << endl;
          }
          else
          {
            // output
            evalNW_output << line << sep_nw << "NA" << sep_nw << "NA" << sep_nw;
            evalNW_output << "NA" << sep_nw << "NA";
            if( print_found_levels == 1 )
            {
              evalNW_output << sep_nw << "NA" << sep_nw << "NA";
            }
            if( cols_eval_extra.size() > 0 )
            {
              // collect extra info
              evalNW_output << sep_nw << "NA" << endl;
            }
            else
            {
              evalNW_output << endl;
            }
          }
        }
        else
        {
          Tracker_edges_not_found++;

          if( only_evaluation_indicator )
          {
            // output
            evalNW_output << line << sep_nw << "0" << sep_nw << "NA" << endl;
          }
          else
          {
            // output
            evalNW_output << line << sep_nw << "0" << sep_nw << "NA" << sep_nw;
            evalNW_output << "NA" << sep_nw << "NA";
            if( print_found_levels == 1 )
            {
              evalNW_output << sep_nw << "NA" << sep_nw << "NA";
            }
            if( cols_eval_extra.size() > 0 )
            {
              // collect extra info
              evalNW_output << sep_nw << "NA" << endl;
            }
            else
            {
              evalNW_output << endl;
            }
          }
        }
      }
    }
    evalNW_output.close();
    extra_info.close();
  }
  else
  {
    cout << filename_network << " is empty. Closing program." << endl;
    toLog << filename_network << " is empty. Closing program." << endl;
    return -1;
  }
  Read_network_file.close();

  //*****************************************************************************************************************************************************

  //*****************************************************************************************************************************************************


  toLog << "\nDone!" << endl;

  //*****************************************************************************************************************************************************
  // 6. Trackers
  toLog << "Number of evaluated edges: " << Tracker_edges_found << endl;
  toLog << "Number of edges not found in database: " << Tracker_edges_not_found << endl;
  toLog << "Number of edges with at least one node found, but edge not found: " << Tracker_min1node_but_edge_not_found << endl;
  toLog << "Number of edges with both nodes found: " << Tracker_both_nodes_found << endl;

  toLog.close();
  return 0;
}
//#########################################################################################################################################################
