#include "output_extended_nw.hpp"

#include <iostream>

Output_extended_nw::Output_extended_nw(){
    // Methods used
    use_SP = 0;   // SignPatter
    use_OL = 0;   // Overlap
    use_II = 0;   // InteractionInformation
    use_DPI = 0;  // DataProcessingInequality
    use_CO = 0;   // Co-Occurrence

    // Separators of input files
    sep_nw = "\t";         // separator for network file
};


// Set info
void Output_extended_nw::set_sep_nw( std::string s ){
    sep_nw = s;
};
void Output_extended_nw::set_methods( int sp, int ol, int ii, int dpi, int co ){
    use_SP = sp;
    use_OL = ol;
    use_II = ii;
    use_DPI = dpi;
    use_CO = co;
};

// Get output string
std::string Output_extended_nw::get_extended_nw_header( std::string l ){
    std::string outputline = l;
    // header for extended network output file: outputline = line;
    // extended network output
    // Number triplets
    outputline += (std::string)sep_nw + "num_Triplets";
    // SignPattern method
    if( use_SP )
    {
        outputline += (std::string)sep_nw + "SignPattern"; // 0 if at least one triplet suggests it is indirect
    }
    // Overlap method
    if( use_OL )
    {
        outputline += (std::string)sep_nw + "Overlap"; // max procentage from all triplets
    }
    // Interaction Information method
    if( use_II )
    {
        outputline += (std::string)sep_nw + "MutualInformation" + (std::string)sep_nw + "InterationInformation" + (std::string)sep_nw + "II_p_value"; // with MI and II, you can determine CMI, min II of all triplets II, Significance of min II(if <0)
    }
    // Data Processing Inequality
    if( use_DPI )
    {
        outputline += (std::string)sep_nw + "DataProcessingInequality_MI_rank" + (std::string)sep_nw + "DataProcessingInequality_indirect";
    }
    // Combining methods
    if( use_SP )
    {
        if( use_OL )
        {
            if( use_II )
            {
                if( use_DPI ){ outputline += (std::string)sep_nw + "COMBI_SP_OL_II_DPI"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                else{ outputline += (std::string)sep_nw + "COMBI_SP_OL_II"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
            }
            else
            {
                if( use_DPI ){ outputline += (std::string)sep_nw + "COMBI_SP_OL_DPI"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                else{ outputline += (std::string)sep_nw + "COMBI_SP_OL"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
            }
        }
        else
        {
            if( use_II )
            {
                if( use_DPI ){ outputline += (std::string)sep_nw + "COMBI_SP_II_DPI"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                else{ outputline += (std::string)sep_nw + "COMBI_SP_II"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
            }
            else
            {
                if( use_DPI ){ outputline += (std::string)sep_nw + "COMBI_SP_DPI"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
            }
        }
    }
    else
    {
        if( use_OL )
        {
            if( use_II )
            {
                if( use_DPI ){ outputline += (std::string)sep_nw + "COMBI_OL_II_DPI"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                else{ outputline += (std::string)sep_nw + "COMBI_OL_II"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
            }
            {
                if( use_DPI ){ outputline += (std::string)sep_nw + "COMBI_OL_DPI"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
            }
        }
        else
        {
            if( use_II & use_DPI){ outputline += (std::string)sep_nw + "COMBI_II_DPI"; } // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
        }
    }
    if( use_CO )
    {
        outputline += (std::string)sep_nw + "percentage_co_occurrence"; // co = [presence_X][presence_Y][presence_X_AND_Y][presence_X_OR_Y][percentage_co_occurrence]
    }
    return outputline;
};
std::string Output_extended_nw::get_no_triplet_edge( std::string l, int num_triplets, double mi, double co ){
    std::string outputline = l;
    // Number triplets
    outputline += (std::string)sep_nw + std::to_string( num_triplets );
    if( use_SP )
    {
        outputline += (std::string)sep_nw + "nan"; // SignPattern: 0 if at least one triplet suggests it is indirect
    }
    if( use_OL )
    {
        outputline += (std::string)sep_nw + "nan"; // Overlap: max procentage from all triplets
    }
    if( use_II || use_DPI )
    {
        outputline += (std::string)sep_nw + std::to_string( mi ); // InteractionInformation or DataProcessingInequality: with MI and II, you can determine CMI}
    }
    if( use_II ) // InteractionInformation
    {
        outputline += (std::string)sep_nw + "nan"; // min II of all triplets II
        outputline += (std::string)sep_nw + "nan"; // Significance of min II(if <0)
    }
    if( use_DPI )
    {
        outputline += (std::string)sep_nw + "nan"; // DataProcessingInequality_Rank
        outputline += (std::string)sep_nw + "nan"; // DataProcessingInequality_indirect: 0 if at least one triplet suggests it is indirect
    }
    if( (use_SP + use_OL + use_II + use_DPI) > 1 )
    {
        outputline += (std::string)sep_nw + "nan"; // Combi: 0 if at least one triplet suggests it is indirect
    }
    if( use_CO )
    {
        outputline += (std::string)sep_nw + std::to_string( co ); // co = [presence_X][presence_Y][presence_X_AND_Y][presence_X_OR_Y][percentage_co_occurrence]
    }
    return outputline;
};
std::string Output_extended_nw::get_triplet_edge( std::string l, int num_triplets, double sp, double ol, double mi, double ii, double ii_p, double dpi, double dpi_indirect, double combi, double co ){
    std::string outputline = l;
    // Number triplets
    outputline += (std::string)sep_nw + std::to_string( num_triplets );
    if( use_SP )
    {
        if( sp > -1 )
        {
            outputline += (std::string)sep_nw + std::to_string( (int)sp ); // SignPattern: 0 if at least one triplet suggests it is indirect
        }
        else
        {
            outputline += (std::string)sep_nw + "nan"; // SignPattern: 0 if at least one triplet suggests it is indirect
        }
    }
    if( use_OL )
    {
        outputline += (std::string)sep_nw + std::to_string( ol ); // Overlap: max procentage from all triplets
    }
    if( use_II || use_DPI )
    {
        outputline += (std::string)sep_nw + std::to_string(mi); // InteractionInformation or DataProcessingInequality: with MI and II, you can determine CMI}
    }
    if( use_II ) // InteractionInformation
    {
        outputline += (std::string)sep_nw + std::to_string( ii ); // min II of all triplets II
        outputline += (std::string)sep_nw + std::to_string( ii_p ); // Significance of min II(if <0)
    }
    if( use_DPI )
    {
        if( dpi > -1 )
        {
            outputline += (std::string)sep_nw + std::to_string( (int)dpi ); // DataProcessingInequality_rank
        }
        else
        {
            outputline += (std::string)sep_nw + "nan"; // DataProcessingInequality_rank
        }
        if( dpi_indirect > -1 )
        {
            outputline += (std::string)sep_nw + std::to_string( (int)dpi_indirect ); // DataProcessingInequality_indirect: 0 if at least one triplet suggests it is indirect
        }
        else
        {
            outputline += (std::string)sep_nw + "nan"; // DataProcessingInequality_indirect: 0 if at least one triplet suggests it is indirect
        }
    }
    if( (use_SP + use_OL + use_II + use_DPI) > 1 )
    {
        if( combi > -1 )
        {
            outputline += (std::string)sep_nw + std::to_string( (int)combi ); // Combi: 0 if at least one triplet suggests it is indirect
        }
        else
        {
            outputline += (std::string)sep_nw + "nan"; // Combi: 0 if at least one triplet suggests it is indirect
        }
    }
    if( use_CO )
    {
        outputline += (std::string)sep_nw + std::to_string( co ); // Percentage Co-Occurrence
    }
    return outputline;
};

// Get output triplet_info
std::string Output_extended_nw::get_triplet_info_header( std::string X, std::string Y, std::string ENV ){
    std::string outputline = (std::string)X + (std::string)"\t" + (std::string)Y + (std::string)"\t";

    outputline += "num_triplets\t";
    outputline += ENV;
    outputline += "s";

    if( use_SP )
    {
        outputline += "\tSignPattern";
    }
    if( use_OL )
    {
        outputline += "\tOverlap";
    }
    if( use_II || use_DPI )
    {
        outputline += "\tMutualInformation";
    }
    if( use_II )
    {
        outputline += "\tConditionalMutualInformation\tInteractionInformation\tII_p_value";
    }
    if( use_DPI )
    {
        outputline += "\tDataProcessingInequality_MI_rank\tDataProcessingInequality_indirect";
        outputline += "\tDPI_num_MI_rank1\tDPI_num_MI_rank2\tDPI_num_MI_rank3";
    }
    // Combining methods
    if( use_SP )
    {
        if( use_OL )
        {
            if( use_II )
            {
                if( use_DPI )
                {
                    outputline += "\tCOMBI_SP_OL_II_DPI"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
                else
                {
                    outputline += "\tCOMBI_SP_OL_II"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
            }
            else
            {
                if( use_DPI )
                {
                    outputline += "\tCOMBI_SP_OL_DPI"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
                else
                {
                    outputline += "\tCOMBI_SP_OL"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
            }
        }
        else
        {
            if( use_II )
            {
                if( use_DPI )
                {
                    outputline += "\tCOMBI_SP_II_DPI"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
                else
                {
                    outputline += "\tCOMBI_SP_II"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
            }
            else
            {
                if( use_DPI )
                {
                    outputline += "\tCOMBI_SP_DPI"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
            }
        }
    }
    else
    {
        if( use_OL )
        {
            if( use_II )
            {
                if( use_DPI )
                {
                    outputline += "\tCOMBI_OL_II_DPI"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
                else
                {
                    outputline += "\tCOMBI_OL_II"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
            }
            else
            {
                if( use_DPI )
                {
                    outputline += "\tCOMBI_OL_DPI"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
                }
            }
        }
        else
        {
            if( use_II & use_DPI)
            {
                outputline += "\tCOMBI_II_DPI"; // IF SP_OL_II_DPI suggest indirect = 0, otherwise 1
            }
        }
    }
    if( use_DPI || use_II )
    {
        outputline += (std::string)"\t" + "portion_nan_" + (std::string)X + "_" + (std::string)Y;
    }
    if( use_II )
    {
        outputline += (std::string)"\t" + "portion_nan_" + (std::string)X + "_" + (std::string)Y + "_" + (std::string)ENV;
    }
    if( use_CO )
    {
        // co = [presence_X][presence_Y][presence_X_AND_Y][presence_X_OR_Y][percentage_co_occurrence]
        outputline += (std::string)"\t" + "presence_X";
        outputline += (std::string)"\t" + "presence_Y";
        outputline += (std::string)"\t" + "presence_X_AND_Y";
        outputline += (std::string)"\t" + "presence_X_OR_Y";
        outputline += (std::string)"\t" + "percentage_co_occurrence";
    }
    return outputline;
};
// Get output triplet_info
std::string Output_extended_nw::get_triplet_info( std::string X, std::string Y, int num_triplets, std::string ENVs, std::string sp, std::string ol, double mi, std::string cmi, std::string ii, std::string ii_p, std::string dpi, std::string dpi_indirect, int num_rank1, int num_rank2, int num_rank3, std::string combi, double portion_nan_XY, std::string portion_nan_XYENV, std::vector< double > co ){

    std::string outputline = (std::string)X + (std::string)"\t" + (std::string)Y + (std::string)"\t";

    outputline += std::to_string( num_triplets ) + (std::string)"\t" + (std::string)ENVs;

    if( use_SP )
    {
        outputline += (std::string)sep_nw + sp; // SignPattern: 0 if at least one triplet suggests it is indirect
    }
    if( use_OL )
    {
        outputline += (std::string)"\t" + (std::string)ol;
    }
    if( use_II || use_DPI )
    {
        outputline += (std::string)"\t" + std::to_string( mi );
    }
    if( use_II )
    {
        outputline += (std::string)"\t" + (std::string)cmi;
        outputline += (std::string)"\t" + (std::string)ii;
        outputline += (std::string)"\t" + (std::string)ii_p;
    }
    if( use_DPI )
    {
        outputline += (std::string)"\t" + (std::string)dpi;
        outputline += (std::string)"\t" + (std::string)dpi_indirect;
        outputline += (std::string)"\t" + std::to_string( num_rank1 ) + (std::string)"\t" + std::to_string( num_rank2 ) + (std::string)"\t" + std::to_string( num_rank3 );
    }
    if( (use_SP + use_OL + use_II + use_DPI) > 1 )
    {
        outputline += (std::string)"\t" + (std::string)combi;
    }
    if( use_DPI || use_II )
    {
        outputline += (std::string)"\t" + std::to_string( portion_nan_XY );
    }
    if( use_II )
    {
        outputline += (std::string)"\t" + (std::string)portion_nan_XYENV;
    }
    if( use_CO )
    {
        // co = [presence_X][presence_Y][presence_X_AND_Y][presence_X_OR_Y][percentage_co_occurrence]
        if( co[ 0 ] > -1 )
        {
            outputline += (std::string)"\t" + std::to_string( (int)co[0] );
            outputline += (std::string)"\t" + std::to_string( (int)co[1] );
            outputline += (std::string)"\t" + std::to_string( (int)co[2] );
            outputline += (std::string)"\t" + std::to_string( (int)co[3] );
            outputline += (std::string)"\t" + std::to_string( co[4] );
        }
        else
        {
            outputline += (std::string)"\t" + "nan";
            outputline += (std::string)"\t" + "nan";
            outputline += (std::string)"\t" + "nan";
            outputline += (std::string)"\t" + "nan";
            outputline += (std::string)"\t" + "nan";
        }
    }

    return outputline;
};
std::string Output_extended_nw::get_rand_II_header( std::string X, std::string Y, std::string ENV, int num_permut ){
    std::string outputline = "";
    outputline += (std::string)X + (std::string)"\t" + (std::string)Y + (std::string)"\t" + (std::string)ENV + (std::string)"\t";
    outputline += "II_ori_score";
    for( int i = 1; i <= num_permut; i++ )
    {
        outputline += "\tII_rand_score" + std::to_string( i );
    }
    return outputline;
};
