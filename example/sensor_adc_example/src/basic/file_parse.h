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
#include "json.hpp"



/**
 * Read json options from a file
 * @param filenpath: filenpath of the file containing the options in json format
 * @return json: JSON object containing the parsed options
 */
nlohmann::json get_options(std::string filenpath) {
    nlohmann::json j;

    // read a JSON file
    try {
        std::ifstream i(filenpath);
        i >> j;
    }
    catch (nlohmann::json::exception& e)
    {
        // output exception information
        std::cout << "message: " << e.what() << '\n'
        << "exception id: " << e.id << std::endl;
    }
    catch (int e)
    {
        std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }

    return j;
}


/**
 * Parse the command line arguments
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return Configuration filenpath
 */

std::string parse_opt(int argc, char* argv[]) {
    std::string filenpath;
    try
    {
        cxxopts::Options opt(argv[0], "AutoGen SystemC, SystemC-AMS  Project");
        opt.positional_help("[optional args]").show_positional_help();
        
        opt.add_options()
        ("c,config", "Configuration file",
         cxxopts::value<std::string>()->default_value("cfgs.json"))
        ("h,help", "Print help");
        
        auto result = opt.parse(argc, argv);
        
        if (result["help"].as<bool>())
        {
            std::cout << opt.help() << std::endl;
            exit(0);
        }
        
        filenpath = result["config"].as<std::string>();
        
        std::ifstream infile(filenpath);
        if(!infile.good()) {
            std::cout << "Error: The configuration  file(json) is missing! " << std::endl;
            std::cout << "Usage: " << std::endl;
            std::cout << "\t -c, --config arg  Configuration file" << std::endl;
            std::cout << "\t -h, --help        Print help " << std::endl;
            exit(-1);
        }
        
        
    } catch (const cxxopts::OptionException& e)
    {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(-1);
    }
    
    return filenpath;
}
#endif

//eof
