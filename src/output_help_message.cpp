#include "output_help_message.hpp"

#include <iostream>

// Get help message
void get_help_message( std::string s ){
    std::cout <<  "\n##############################################################################################################################################################\n" << std::endl;
    std::cout <<  "Usage: " << s << " --network_file [network_filename] <option(s)>\n" << std::endl;
    std::cout <<  "Options:" << std::endl;

    // Help
    // used alphabetical letters: dfhmotv
    // available alphabetical letters: abcegijklnpqrsuwxyz
    std::cout <<  "\t-h,--help....................................Returns this help message\n" << std::endl;
    std::cout <<  "\t-v,--version.................................Returns version of the program.\n" << std::endl;
    std::cout <<  "\t-d,--defaults................................Returns default values of the program.\n" << std::endl;
    // Required options
    std::cout <<  "\nRequired options:" << std::endl;
    std::cout <<  "\t-f,--input_network_file [network_filename]...Name of the input network file (first line are the column names).\n" << std::endl;
    std::cout <<  "\t-m,--methods [method IDs]....................List the method/s you would like to use separated by a comma." << std::endl;
    std::cout <<  "\t\t\t\t\t\t\tSP=SignPattern, OL=Overlap, II=InteractionInformation, DPI=DataProcessingInequality(minMi), CO=Co-Occurrence" << std::endl;
    std::cout <<  "\t\t\t\t\t\t\te.g.: SP,II.Instead of mentioning all methods, you can shortly write \"-m all\"\n" << std::endl;
    // Method Settings
    std::cout <<  "\nMethods Settings:" << std::endl;
    std::cout <<  "   Sign Pattern:" << std::endl;
    std::cout <<  "\t--SP_colnum_interaction_score [num]..........Required for method SignPattern(SP). \"num\" is the column number of the interaction score.\n" << std::endl;
    std::cout <<  "   Overlap:" << std::endl;
    std::cout <<  "\t--OL_colnum_interactionlength_startX_startY [num1,num2,num3]...Required for method Overlap(OL)." << std::endl;
    std::cout <<  "\t\t\t\t\t\t\t\"num1\" = column containing the interaction length," << std::endl;
    std::cout <<  "\t\t\t\t\t\t\t\"num2\" and \"num3\" = columns indicating start point of X and Y.\n" << std::endl;
    std::cout <<  "\t--OL_percentage_threshold [num]..............Default: 60.0, edge is considered not indirect, if overlap is below this number.\n" << std::endl;
    std::cout <<  "   Data Processing Inequality:" << std::endl;
    std::cout <<  "\t--DPI_minMI_threshold [double]...............Default: 0, threshold for how much smaller min MI has to be compared to other two MIs.\n" << std::endl;
    std::cout <<  "   Interaction Information and Data Processing Inequality:" << std::endl;
    std::cout <<  "\t--II_DPI_max_nan_threshold [num]..............Default: 20, threshold(in percentage) for maximum portion of 'nan' in vectors used for II and DPI.\n" << std::endl;
    // Significance for II
    std::cout <<  "   Interaction Information - Significance determination:" << std::endl;
    std::cout <<  "\t--II_significance_level [double].............Default: 0.05, significance level for interaction information is a number between 0 and 1.\n" << std::endl;
    std::cout <<  "\t--II_permutation_iteration [int].............Default: 100, number of iterations to determine the significance of the interaction information.\n" << std::endl;
    std::cout <<  "\t--do_pre_jointP_comp.........................Default: ENV vectors are randomized anew for each Signifiance determination." << std::endl;
    std::cout <<  "\t\t\t\t\t\t\tOptional: ENV vectors are permutated before and stored." << std::endl;
    std::cout <<  "\t\t\t\t\t\t\tNote: might cause memory troubles, so it is not set by default.\n" << std::endl;
    // Method combination
    std::cout <<  "   Strategy for method combination:" << std::endl;
    std::cout <<  "\t--method_count_threshold [double]............Default: 100, threshold(in percentage) for minimum methods that suggest edge as indirect.\n" << std::endl;
    std::cout <<  "\t--triplet_count_threshold [double]...........Default: 1, threshold for minimum triplets that suggest edge as indirect.\n" << std::endl;
    // Additional input files
    std::cout <<  "\nAdditional input files:" << std::endl;
    std::cout <<  "\t--II_DPI_abundance_file [filename]...........Required for method InteractionInformation(II) and DataProcessingInequality(DPI)." << std::endl;
    std::cout <<  "\t\t\t\t\t\t\tName(if not in same folder, also with path) of the abundance file" << std::endl;
    std::cout <<  "\t\t\t\t\t\t\t(first line are the column names: 1st column = ID/ENV-name, other columns are abundances).\n" << std::endl;
    std::cout <<  "\t--II_DPI_ENVparameter_file [filename]........Optional for method InteractionInformation(II) and DataProcessingInequality(DPI)." << std::endl;
    std::cout <<  "\t\t\t\t\t\t\tName(if not in same folder, also with path) of the ENV parameter file, if ENV not within" << std::endl;
    std::cout <<  "\t\t\t\t\t\t\tthe abundance file, then first line are the column names: 1st column = ENV-name," << std::endl;
    std::cout <<  "\t\t\t\t\t\t\tother columns are abundances)(Column names have to match the ones from the abundance file).\n" << std::endl;
    std::cout <<  "\t--input_ID_ENV_nw_file [network_filename]...Optional: Name of the input network file that contains ID-ENV edges" << std::endl;
    std::cout <<  "\t\t\t\t\t\t\t(first line are the column names, that have to be the same as the ones for the network file).\n" << std::endl;
    // Inputfile separators
    std::cout <<  "\nInputfile separators:" << std::endl;
    std::cout <<  "\t--separator_network_file \"[sep]\".............Default: tab, other possibilities, but not limited to: \";\" or \" \"\n" << std::endl;
    std::cout <<  "\t--separator_abundance_file \"[sep]\"...........Default: tab, other possibilities, but not limited to: \";\" or \" \"\n" << std::endl;
    // Outputfiles
    std::cout <<  "\nOutput:" << std::endl;
    std::cout <<  "\t-o,--output_network_file [filename]..........Default: \"extended_nw.txt\"; name of the output network file.\n" << std::endl;
    std::cout <<  "\t--output_ID_ENV_edge_dont_print.............Default: ID-ENV edges are printed in extended nw file. With this option, they will not be printed.\n" << std::endl;
    std::cout <<  "\t-t,--output_triplet_info [filename]..........Create output with Triplets information.\n" << std::endl;
    std::cout <<  "\t--output_discretized_vectors [filename]......Create output with discretized vectors.\n" << std::endl;
    // Other
    std::cout <<  "\nOther:" << std::endl;
    std::cout <<  "\t--input_node_col [colnum_x,colnum_y].........Default: \"1,2\". Number of the 2 columns containing the nodes X and Y.\n" << std::endl;
    std::cout <<  "\t--input_ENV_identicator [indicator string]...Default: \"ENV\"; indicator for environmental parameter name, needs to be included in node name.\n" << std::endl;
    std::cout <<  "\n##############################################################################################################################################################\n" << std::endl;
};

// Get default values
void get_defaults(){
    std::cout <<  "\n########################################################################\n" << std::endl;
    std::cout <<  "\nDefault settings" << std::endl;
    std::cout <<  "\n- No method is selected by default." << std::endl;
    std::cout <<  "\n- Method Overlap: The percentage threshold to decide if an edge" << std::endl;
    std::cout <<  "\tis regared as indirect by the method is 60." << std::endl;
    std::cout <<  "\n- Method Interaction Information: The significance level of the" << std::endl;
    std::cout <<  "\tInteraction Information Score is 0.05." << std::endl;
    std::cout <<  "\n- Method Interaction Information: The number of permutation to" << std::endl;
    std::cout <<  "\tdetermine the significance of the Interaction Information" << std::endl;
    std::cout <<  "\tScore is 100." << std::endl;
    std::cout <<  "\n- Method Data Processing Inequality compares Mutual" << std::endl;
    std::cout <<  "\tInformations. Ranks Mutual Information and by default regards" << std::endl;
    std::cout <<  "\tID-ID as indirect if it is the smallest. That means, by" << std::endl;
    std::cout <<  "\tdefault is the threshold for how much smaller the MI has to" << std::endl;
    std::cout <<  "\tbe: 0." << std::endl;
    std::cout <<  "\n- ENV-vector will be permuted anew during examining the networks" << std::endl;
    std::cout <<  "\ttriplets." << std::endl;
    std::cout <<  "\n- Method Interaction Information and Data Processing Inequality" << std::endl;
    std::cout <<  "\tuse data that may contain 'nan'. By default is the threshold(in" << std::endl;
    std::cout <<  "\tpercentage) for maximum portion of 'nan' in vectors: 20." << std::endl;
    std::cout <<  "\n- Combination strategy of considering an edge indirect. By" << std::endl;
    std::cout <<  "\tdefault all methods have to agree that an edge within one" << std::endl;
    std::cout <<  "\ttriplet is indirect before it is considered as indirect. And" << std::endl;
    std::cout <<  "\tat least one triplet has to suggest the edge as being indirect" << std::endl;
    std::cout <<  "\tbefore the edge is considered indirect." << std::endl;
    std::cout <<  "\n- Inputfile separators are by default tabs" << std::endl;
    std::cout <<  "\n- Output: by default the only output is the network file which" << std::endl;
    std::cout <<  "\tis extended by the methods information and named as" << std::endl;
    std::cout <<  "\t`extended_nw.txt`" << std::endl;
    std::cout <<  "\n- ID-ENV edges are printed in extended nw file." << std::endl;
    std::cout <<  "\n- Number of the 2 columns containing the nodes X and Y are by" << std::endl;
    std::cout <<  "\tdefault `1,2`." << std::endl;
    std::cout <<  "\n- `ENV` is the indicator for environmental parameter name that" << std::endl;
    std::cout <<  "\tneeds to be included in node name." << std::endl;
    std::cout <<  "\n########################################################################\n" << std::endl;
};
