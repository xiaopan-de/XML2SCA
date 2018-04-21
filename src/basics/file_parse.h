/* *****************************************************************************
 * @file temp.h
 * @author Xiao Pan <pan@cs.uni-kl.de>,
 *         AG Design of Cyber-Physical Systems
 * @version 0.1.0
 * @date  01.02.2018
 * @brief 
 *
 * @section LICENSE License
 *
 * Copyright 2012-2018 Workgroup Design of Cyber-Physical Systems. University of
 * Kaiserslautern.
 *
 * @section DESCRIPTION Description
 *
 ******************************************************************************/

// ----------------------------------------------------------------------------
// This code is commented throughout for use with Doxygen.
// ----------------------------------------------------------------------------

/* *****************************************************************************
 * Change log:
 * 04.04.2018 Initialization
 *
 ******************************************************************************/


#ifndef _FILE_PARSE_H_
#define _FILE_PARSE_H_



#include <iostream>
#include <fstream>
#include "cxxopts.hpp"

#define concat(first, second, thirt) first second thirt


/**
 * Parse the command line arguments
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return Project-XML filename, Module-XML filename
 */

std::string parse_opt(int argc, char* argv[]) {

    std::string projectxml_filename;

    try
    {
        const char* header = concat("XML2SCA", " ", "0.1");
        cxxopts::Options opt(argv[0], header);
        opt.positional_help("[optional args]").show_positional_help();
        
        opt.add_options()
        ("f,file", "Project XML file", cxxopts::value<std::string>()->default_value("sc_project.xml"))
        ("h,help", "Print help");
        
        auto result = opt.parse(argc, argv);
        
        if (result["help"].as<bool>())
        {
            std::cout << opt.help() << std::endl;
            exit(0);
        }
        
        projectxml_filename = result["file"].as<std::string>();

        std::ifstream infile(projectxml_filename);
        if(!infile.good()) {
            std::cout << "Project XML file is missing!" << std::endl;
            exit(-1);
        }
        
        
    } catch (const cxxopts::OptionException& e)
    {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(-1);
    }
    
    return projectxml_filename;
}
#endif

//eof
