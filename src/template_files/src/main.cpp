/**
 * This file is generated using [XML2SCA]
 * Contact : panxiao.tech@gmail.com
 * Web     : http://panxiao.tech/tools/xml2sca/
 *
 * @file    main.cpp
 * @author
 * @date    Apr 16 2018
 * @section LICENSE License (ADD YOUR LICENSE HERE)
 *
 * @section DESCRIPTION Description (ADD YOUR DESCRIPTION HERE)
 *
 */





#include <sys/time.h>

#include "sc_top.h"

using namespace std;



// ----------------------------------------------------------------------------
//! @brief main
// ----------------------------------------------------------------------------
int sc_main(int argc, char* argv[])
{
    // Parse command line arguments for comfiguration file path
    string cfg_filepath = parse_opt(argc, argv);
    
    nlohmann::json  configs;                // JSON json_opts
    configs = get_options(cfg_filepath);    // Parse json json_opts file
    
    
    using sca_core::sca_time;
    sc_set_time_resolution(1, sc_core::SC_PS);
    sc_top top("top", configs);

    // Print some useful information:
    cout << "Info: Simulation options:" << endl;
    cout << "         Simulation time:     " << (double)configs["system"]["tsim"] << endl;
    cout << "         Simulation timestep: " << (double)configs["system"]["tstep"] << endl;
    cout << "Info: Simulation start. "<< endl;
    
    double sim_time = (double)configs["system"]["tsim"];
    
    int start_s=clock();
    sc_core::sc_start(sim_time, sc_core::SC_SEC);
    sc_core::sc_stop();
    int stop_s=clock();
        
    // print simulation performance 
    std::cout<< "Info: simulation of "<< sim_time <<" SEC complete. (CPU time: "
    << (stop_s-start_s)/double(CLOCKS_PER_SEC) <<" sec )"<< std::endl;
    
    
    return 0;
}
