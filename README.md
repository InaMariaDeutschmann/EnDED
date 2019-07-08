# EnDED - Environmentally-Driven Edge Detection Program

<img src="logo_EnDED.png" alt="drawing" width="200"/>

## Disentangling environmental effects in microbial association networks
Even though ecological interactions among microbes are fundamental for ecosystem functioning,
 most of them remain unknown. High-throughput omics can help unveiling microbial interactions
 by inferring species correlations over space or time, which can be represented as networks.
 Associations in these networks can indicate ecological interactions between species or
 alternatively, similar or different environmental preferences. Therefore, it is important to
 disentangle these associations and determine whether two species are correlated because they
 interact ecologically or because they are correlated to an abiotic or biotic environmental
 factor. We developed an approach to determine whether or not two species are associated in a
 network due to environmental preference. We use four methods (Sign Pattern, Overlap,
 Interaction Information, and Data Processing Inequality) that in combination can detect what
 associations in a network are environmentally-driven. The approach is implemented in the
 publicly available software tool EnDED. 

## GENERAL USAGE NOTES
EnDED is a program that aims to detect environmentally-driven ("indirect") edges in an association network. 
 The makefile contains the code to generate all required commands to compile the program.
 Go within the terminal into the folder EnDED and type
```
make
```
The command to use the program is:
```
EnDED/build/EnDED --input_network_file [PATH/NETWORKFILE] --methods [METHODS] [optional OPTIONS]
```

## DEPENDENCIES
The program requires the 
- gcc = GNU Compiler Collection (GCC)
- boost = boost C++ Libraries

## Get help with
```
./build/EnDED -h
```
or
```
./build/EnDED --help
```

## Options
- `-h,--help` Returns this help message
- `-v,--version` Returns the version of the program.
- `-d,--defaults` Returns the default values of the program.

### Required options
- `-f,--input_network_file [network_filename]` Path and name of
   the input network file. The first line(row) represent the
   column names.
- `-m,--methods [method IDs]` Lists the method/s you would like
   to use separated by a comma. SP=SignPattern, OL=Overlap,
   II=InteractionInformation, DPI=DataProcessingInequality 
   (minMi), CO=Co-Occurrence, e.g.: SP,II. Instead of mentioning 
   all methods, you can shortly write `-m all`

### Methods Settings:
#### Sign Pattern:
- `--SP_colnum_interaction_score [num]` Required for method
   SignPattern(SP). "num" is the column number of the interaction
   score.

#### Overlap:
- `--OL_colnum_interactionlength_startX_startY [num1,num2,num3]`
   Required for method Overlap(OL).
    "num1" = column containing the interaction length,
    "num2" and "num3" = columns indicating start point of X and Y.

- `--OL_percentage_threshold [num]` Default: 60.0, edge is
   considered not indirect, if overlap is below this number.

#### Data Processing Inequality:
- `--DPI_minMI_threshold [double]` Default: 0, threshold for how much
   smaller min MI has to be compared to other two MIs.

#### Interaction Information and Data Processing Inequality:
- `--II_DPI_max_nan_threshold [num]` Default: 20, threshold(in
   percentage) for maximum portion of 'nan' in vectors used for II
   and DPI.

   Interaction Information - Significance determination:
- `--II_significance_level [double]` Default: 0.05, significance
   level for interaction information is a number between 0 and 1.

- `--II_permutation_iteration [int]`.Default: 100, number of
   iterations to determine the significance of the interaction
   information.

- `--do_pre_jointP_comp` Default: ENV vectors are randomized anew
   for each Signifiance determination.
    Optional: ENV vectors are permutated before and stored.
    Note: might cause memory troubles, so it is not set by default.

#### Strategy for method combination:
- `--method_count_threshold [double]` Default: 100, threshold(in
    percentage) for minimum methods that suggest edge as indirect.

- `--triplet_count_threshold [double]` Default: 1, threshold for
    minimum triplets that suggest edge as indirect.


### Additional input files:
- `--II_DPI_abundance_file [filename]` Required for method
   InteractionInformation(II) and DataProcessingInequality(DPI).
   Name(if not in same folder, also with path) of the abundance file
   (first line are the column names: 1st column = ID/ENV-name, other
   columns are abundances).

- `--II_DPI_ENVparameter_file [filename]` Optional for method
   InteractionInformation(II) and DataProcessingInequality(DPI).
   Name(if not in same folder, also with path) of the ENV parameter file,
   if ENV not within the abundance file, then first line are the column
   names: 1st column = ENV-name, other columns are abundances)(Column names
   have to match the ones from the abundance file).

- `--input_ID_ENV_nw_file [network_filename]` Optional: Name of the input
   network file that contains ID-ENV edges (first line are the column names,
   that have to be the same as the ones for the network file).


### Inputfile separators:
- `--separator_network_file "[sep]"` Default: tab, other possibilities, but
   not limited to: ";" or " "

- `--separator_abundance_file "[sep]"` Default: tab, other possibilities, but
   not limited to: ";" or " "

### Output:
- `-o,--output_network_file [filename]` Default: "extended_nw.txt"; name of
   the output network file.

- `--output_ID_ENV_edge_dont_print` Default: ID-ENV edges are printed in
   extended nw file. With this option, they will not be printed.

- `-t,--output_triplet_info [filename]` Create output with Triplets information.

- `--output_discretized_vectors [filename]` Create output with discretized vectors.


### Other:
- `--input_node_col [colnum_x,colnum_y]`Default: "1,2". Number of the 2 columns
   containing the nodes X and Y.

- `--input_ENV_identicator [indicator string]`Default: "ENV"; indicator for
   environmental parameter name, needs to be included in node name.


## Default settings
- No method is selected by default.
- Method Overlap: The percentage threshold to decide if an edge
	is regared as indirect by the method is 60.
- Method Interaction Information: The significance level of the
	Interaction Information Score is 0.05.
- Method Interaction Information: The number of permutation to
	determine the significance of the Interaction Information
	Score is 100.
- Method Data Processing Inequality compares Mutual
	Informations. Ranks Mutual Information and by default regards
	ID-ID as indirect if it is the smallest. That means, by
	default is the threshold for how much smaller the MI has to
	be: 0.
- ENV-vector will be permuted anew during examining the networks
	triplets.
- Method Interaction Information and Data Processing Inequality
	use data that may contain 'nan'. By default is the threshold(in
	percentage) for maximum portion of 'nan' in vectors: 20.
- Combination strategy of considering an edge indirect. By
	default all methods have to agree that an edge within one
	triplet is indirect before it is considered as indirect. And
	at least one triplet has to suggest the edge as being indirect
	before the edge is considered indirect.
- Inputfile separators are by default tabs
- Output: by default the only output is the network file which
	is extended by the methods information and named as
	`extended_nw.txt`
- ID-ENV edges are printed in extended nw file.
- Number of the 2 columns containing the nodes X and Y are by
	default `1,2`.
- `ENV` is the indicator for environmental parameter name that
	needs to be included in node name.

## Example
### Input data
The folder test_data contains an example datasets that can be used
to try out the program. The following files are included:
Input folder containing
     - `network.txt` containing the network file (ID-ID edges, 
        ID-ENV edges, and ENV-ENV edges)
     - `ID_ENV_edges.txt` containing ID-ENV edges
     - `ID_abundance.txt` contains the abundances/counts for 
        the IDs (rows) per sample (columns)
     - `ENV_parameters.txt` contains the environmental 
        parameters for the ENVs (rows) per sample (columns)

### Output data
#### logfile
The log-file contains information about the program and the 
indirect edge detection:
- command used to run the program
- program version
- Settings
- Inputfiles
- Information tracked during the indirect edge detection
     - number of ID-ENV edges found in input file and how many 
       of them are considered for the indirect edge detection 
       (duplicated are not considered, only first appearance 
       will be used).
     - number of ID-ENV edges that are within at least one 
       triplet.
     - number of ENV-ENV edges.
     - number of ID-ID edges.
     - number of ID-ID edges that are not in a triplet.
     - number of IDs are connected to at least one ENV
       parameter.
     - number of ID-ID edges that are in at least one triplet.
     - number of edges that are considered `indirect`, and `not
       indirect` by each selected method and (if more
       than one method got selected) by the method combination.
- Start and end time as well as the duration of the computation.

#### extended_network
The output of the EnDED program is `Output/extended_network.txt`.
It's columns are all columns from the input network file and 
additionally the following columns:
- `SignPattern` is `0` for indirect and `1` for not indirect.
- `Overlap` is the maximal detected Overlap in percentage of the 
   X-Y edge with the edges X-ENV and Y-ENV.
- `MutualInformation` is the mutual information between X and Y.
- `InteractionInformation` is the minimal significant 
   interaction information of X, Y, and ENV.
- `II_p_value` is the significance of the interaction 
   information of X, Y, and each of the ENV.
- `DataProcessingInequality_Rank` is the minimal rank of the 
   mutual information of X, Y in comparison with the mutual 
   information of X-ENV and Y-ENV for each of the ENV: 1 being 
   the smallest and 3 being the highest.
- `DataProcessingInequality_indirect` is `0` for indirect and 
   `1` for not indirect. The data processing information regards
    the X-Y association as indirect if the mutual information of 
    X and Y is smaller than the other two (by default). Or it 
    has to be smaller by a certain value that can be specified 
    with: `--DPI_minMI_threshold`. That means the mutual 
    information has to be smaller than MI-DPI_minMI_threshold in 
    order for DPI to determine the XY-association as indirect.
- `COMBI_SP_OL_II_DPI` is `0` for indirect and `1`. It is based 
   on a combination of the 4 methods in detecting indirect 
   edges.
- `percentage_co_occurrence` is the percentage of how often X and Y
   co-occurred in all samples in which at least one of them occurred.

#### triplet_info.txt
An extra output that contains information about the triplets: `Output/triplet_info.txt`.
Columns:
- `X` nodes X as indicated in the network file
- `Y` nodes Y as indicated in the network file
- `num_triplets` number of triplets that contain the edge 
   between X and Y
- `ENV` environmental factor/s that is/are in a closed triplet 
   with X and Y
- `SignPattern` is a ';'-separated list that contains for each
   triplet the Sign Pattern indirect detection determination:
   `0` for indirect and `1` for not indirect.
- `Overlap` is a ';'-separated list that contains for each 
   triplet the Overlap in percentage of the X-Y edge with the 
   edges X-ENV and Y-ENV.
- `MutualInformation` is the mutual information between X and Y.
- `ConditionalMutualInformation` is a ';'-separated list that 
   contains for each triplet the conditional mutual information 
   of X, Y, and each of the ENV.
- `InteractionInformation` is a ';'-separated list that contains 
   for each triplet the interaction information of X, Y, and 
   each of the ENV.
- `II_p_value` is a ';'-separated list that contains for each 
   triplet the significance of the interaction information of X, 
   Y, and each of the ENV.
- `DataProcessingInequality_MI_rank` is a ';'-separated list
   that contains for each triplet the rank of the mutual 
   information of X, Y in comparison with the mutual 
   information of X-ENV and Y-ENV for each of the ENV: 1 being
   the smallest and 3 being the highest.
- `DataProcessingInequality_indirect` is a ';'-separated list 
   that contains for each triplet if the data processing 
   information regards the X-Y association as indirect (`0`) or 
   not indirect (`1`). It is regarded as indirect if the mutual 
   information of X and Y is smaller than the other two (by 
   default). Or it has to be smaller by a certain value that can
   be specified with: `--DPI_minMI_threshold`. That means the 
   mutual information has to be smaller than MI reduced by 
   DPI_minMI_threshold (=MI-DPI_minMI_threshold) in order for
   DPI to determine the XY-association as indirect.
- `DPI_num_MI_rank1` counts the number of times the 
   XY-association had rank 1.
- `DPI_num_MI_rank2` counts the number of times the 
   XY-association had rank 2.
- `DPI_num_MI_rank3` counts the number of times the 
   XY-association had rank 3.
- `COMBI_SP_OL_II_DPI` is a ';'-separated list that contains for
   each triplet the combination of the 4 methods in detecting 
   indirect edges: `0` for indirect and `1` for not indirect.
- `portion_NA_X_Y` gives the portion of pairwise NAs in the 
   vectors for X and Y. If it is above the threshold, MI will be
   NA and II and DPI will be NA as well.
- `portion_NA_X_Y_ENV is a ';'-separated list that gives the 
   portion of three-wise NAs in the vectors for X and Y and ENV
   for each ENV. If it is above the threshold, MI will be NA and
   II and DPI will be NA as well.
- `presence_X` is the number of samples in which X occurred.
- `presence_Y` is the number of samples in which Y occurred.
- `presence_X_AND_Y` is the number of samples in which both, i.e.
   X and Y, occurred.
- `presence_X_OR_Y` is the number of samples in which at least one
   of X and Y occurred.
- `percentage_co_occurrence` is the percentage of how often X and Y
   co-occurred in all samples in which at least one of them occurred.
Note: the column names `X` and `Y` are the column names for node X
      and node Y in the network file, and `ENV` is the 
      ENV-indicator that can be adjusted with option 
      `--input_ENV_identicator [indicator string]`.

#### disc_vectors.txt
An extra output that contains discretized vectors: 
`Output/disc_vectors.txt`.
Columns = columns from input abundance file
Content = discretized values of the original abundance/count 
          ID data and environmental parameters

### Command
After loading all required modules, and compiling the program.
The program can be run.
The command to produce the output files in the test data set is presented here:
```
./../build/EnDED --input_network_file Input/network.txt --methods SP,OL,II,DPI,CO --SP_colnum_interaction_score 3 --OL_colnum_interactionlength_startX_startY 8,6,7 --II_DPI_abundance_file Input/ID_abundance.txt --II_DPI_ENVparameter_file Input/ENV_parameters.txt --output_network_file Output/extended_network.txt --output_triplet_info Output/triplet_info.txt --output_discretized_vectors Output/disc_vectors.txt --do_pre_jointP_comp --II_permutation_iteration 100
```

## Background
Microbial communities are not a mere collection of independent
 individuals; they are interconnected being involved in various 
 ecological interactions such as symbiosis, parasitism or 
 predation. These interactions are important to maintain 
 ecosystem function. For understanding microbial ecosystems, it 
 is essential to understand microbiome interactions. They are 
 barely understood due to previous limitations in tools and 
 methods. However, during the last decade, there have been 
 advances in omics tools, analytical methods as well as 
 computing performance.
The omics-technologies allow the screening of a large number of 
 microbes; we obtain a list of microbes that are present in an 
 ecosystem and are also able to quantify them. This data can be
 used to infer associations. Analyzing and converting microbiome
 data into meaningful biological insights is challenging. 
 Furthermore, microbes and microbial interactions can be 
 affected by the environment. Thus, associations between 
 microbes and the environment should be considered when studying 
 an ecosystem.
Microbial interactions can be represented and modeled as 
 networks: The nodes of the network represent the microbes as
 well as the environmental parameters. An edge between two nodes 
 represents an association between two microbes, or between a 
 microbe and environmental parameter. State of the art network 
 construction tools are still far from perfect. The obtained 
 Microbial Association Networks are likely to contain false 
 associations due to indirect associations (indirect edges). The
 so called Effect of indirect dependencies is when two species 
 are indirectly correlated because both are correlated with a 
 third species or environmental factor, i.e. two microbes 
 respond similarly to a common environmental condition. Thus, in
 association networks we obtain two types of edges: direct and 
 indirect associations.
Only direct associations predict microbial interactions. 
 Therefore, indirect associations need to be removed from the 
 network before analyzing and interpreting the network as a 
 representation of the microbial ecosystem. In order to remove 
 indirect edges from microbial networks that are environmentally 
 driven we implemented (C++ program EnDED - indirect edge 
 detection), which contains four methods that aim to detect 
 indirect edges: Sign Pattern, Overlap, Interaction Information,
 and Data Processing Inequality.

## The indirect edge detection methods
There are four methods that aim to detect indirect edges: Sign 
 Pattern, Overlap, Interaction Information, and Data Processing
 Inequality. All four methods use closed triplets to detect 
 environmentally driven edges within the network, which is a 
 constellation of three nodes that are all connected to each 
 other. Let T={v_1,v_2,v_3} be the closed triplet. Let v_1 and 
 v_2 be species and v_3 be the environment. Such triplets have 
 been used by (Lima-Mendez, Faust, Henry et al. 2015, 
 `Determinants of community structure in the global plankton 
 interactome`) and were called environmental triplets. Note that 
 an edge between two species might be in none, one or several 
 triplets with different considered environmental parameters, 
 e.g. temperature, salinity, oxygen level.

- Sign Pattern uses the sign of the association, i.e. whether 
  the association is positive or negative.
- Overlap uses the start and length of the association.
- Interaction Information and Data Processing Inequality use the
  abundance/count and environmental data given for the two 
  species and the environmental parameter.

We build up on three methods, namely Sign Pattern, Interaction 
 Information, and Data Processing Inequality, as well as 
 developed the method Overlap to use the additional information 
 that can be obtained from temporal data. We combine these methods.
 Only if all methods suggest that the edge is indirect, the edge is
 considered to be indirect. As for now, the methods (and combination
 for them) are implemented in the C++ program, EnDED (short for
 Environmentally-Driven Edge Detection).

Additionally, the co-occurrence of the X and Y is determined that
 may be used filter and analyse the network further.

## Version
EnDED_v1.0.1, 15. June  2019

## CONTACT
Ina Maria Deutschmann (ina.m.deutschmann [at] gmail.com)
