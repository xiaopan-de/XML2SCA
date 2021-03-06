/**
 * This file is automatically generated using [XML2SCA] 
 * Contact : panxiao.tech@gmail.com
 * Web     : http://panxiao.tech/tools/xml2sca/
 * 
 * @file    sensor_adc_example/src/sc_top.h
 * @author  Xiao Pan (pan@cs.uni-kl.de)
 * @date    10:46:05 Apr 16 2018
 * @section LICENSE License (ADD YOUR LICENSE HERE)
 *  
 * @section DESCRIPTION Description (ADD YOUR DESCRIPTION HERE)
 *          Example project of [XML2AMS]
 * 
 */


#ifndef sc_top_h_

#define sc_top_h_
#include "basic/json.hpp"
#include "basic/file_parse.h"
#include "sca_tdf_adc.h"
#include "sca_tdf_sine.h"
#include "sca_tdf_drain.h"
#include "sca_tdf_gnd.h"


//! 
//! @brief toplevel  module
//! 
SC_MODULE(sc_top) 
{
	sca_tdf_adc    *i_adc;
	sca_tdf_sine   *i_sensor;
	sca_tdf_drain  *i_drain;
	sca_tdf_gnd    *i_gnd;

	sc_core::sc_signal<uint16_t>     sig_from_adc_conv;
	sca_tdf::sca_signal<double>       sig_from_sensor_sine;
	sca_tdf::sca_signal<double>       sig_from_gnd_gnd;

	// sample period
	sca_core::sca_time tstep;
	// trace file
	sca_util::sca_trace_file* tf;
	std::string trace_format; 

public: 
	// Constructor
	sc_top(sc_core::sc_module_name nm,nlohmann::json  _configs):sc_module(nm)
	{ 
		tstep = sca_core::sca_time(_configs["system"]["tstep"] ,sc_core::SC_SEC);
		// trace file
		trace_format = _configs["system"]["tracefile"];
		tf = trace_format.compare("vcd") == 0 ?
			sca_util::sca_create_vcd_trace_file("tracefile_top"):
			sca_util::sca_create_tabular_trace_file("tracefile_top");

		i_adc          = new sca_tdf_adc("i_adc",
			            (double)       _configs["i_adc"]["vref"],
			            (unsigned int) _configs["i_adc"]["res"],
			            (double)       _configs["i_adc"]["fs"],
			            (int)          _configs["i_adc"]["rate"]);
		i_sensor       = new sca_tdf_sine("i_sensor",
			            (double)       _configs["i_sensor"]["freq"],
			            (double)       _configs["i_sensor"]["amp"],
			            (double)       _configs["i_sensor"]["offset"]);
		i_drain        = new sca_tdf_drain("i_drain");
		i_gnd          = new sca_tdf_gnd("i_gnd");

		{ // Start of outgoing connections 
			i_adc->pout_conv(sig_from_adc_conv);
			i_sensor->pout_sine(sig_from_sensor_sine);
			i_gnd->pout_gnd(sig_from_gnd_gnd);
		} //  End ofoutgoing connections
       
        //<<< START OF USER-ADDED-CODE >>>>
        i_adc->set_timestep(tstep);
        i_drain->set_timestep(tstep);

		{ // Start of incoming connections.
            i_adc->pin_pos(sig_from_sensor_sine);
            i_adc->pin_neg(sig_from_gnd_gnd);
            i_drain->pin_d(sig_from_adc_conv);
		} // End of incoming connections.
        //<<< END OF USER-ADDED-CODE >>>

        
		sca_util::sca_trace(tf, sig_from_adc_conv, "sig_from_adc_conv");
		sca_util::sca_trace(tf, sig_from_sensor_sine, "sig_from_sensor_sine");
		sca_util::sca_trace(tf, sig_from_gnd_gnd, "sig_from_gnd_gnd");
	};

	// Destructor
	~sc_top()
	{
		if(trace_format.compare("vcd")) 
			 sca_close_tabular_trace_file(tf);
		 else
			 sca_close_vcd_trace_file(tf);
	}
};

#endif 
