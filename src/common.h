/**
 * @file    main.cpp
 * @author  Xiao Pan 
 * @date    11:02:40 Apr  6 2018
 *
 * Copyright (c) 2012-2018 Design of Cyber-Physical Systems TU Kaiserslautern
 * Contact: Xiao Pan <pan@cs.uni-kl.de | xml2sca@panxiao.tech>
 *
 * @section LICENSE License
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 * XML2SCA : Auto-generate SystemC/SystemC-AMS projects (incl. codes,
 * testbenches, cmakes) from XML file. <https://github.com/panxiao-tech/XML2SCA>
 *
 */

#ifndef _COMMON_H
#define _COMMON_H


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>


#include <vector>
using namespace std;

namespace xml2sca
{
    static std::ofstream log("log.txt");
    
    struct inoutport{       // in-output port of the module
        string name ;        // define a name, use defalut value if unset
        string porttype ;   // sca_tdf::sca_in,sca_tdf::sca_out, sc_in,  etc.
        string datatype ;   // specify the data type of the inoutput ports, T, bool etc.
        string description ;// description of the  port
        
        string get_invers_port()
        {
            string inv_porttype;
            if(!porttype.compare("sca_tdf::sca_in")) inv_porttype= "sca_tdf::sca_out";
            if(!porttype.compare("sca_tdf::sca_out")) inv_porttype= "sca_tdf::sca_in";
            if(!porttype.compare("sca_tdf::sc_in")) inv_porttype= "sca_tdf::sc_out";
            if(!porttype.compare("sca_tdf::sc_out")) inv_porttype= "sca_tdf::sc_in";
            if(!porttype.compare("sc_core::sc_in")) inv_porttype= "sc_core::sc_out";
            if(!porttype.compare("sc_core::sc_out")) inv_porttype= "sc_core::sc_in";
            return inv_porttype;
        };
        string get_signal_type()
        {
            if(porttype == "sca_tdf::sca_out" || porttype == "sca_tdf::sca_in")
            {
                return "sca_tdf::sca_signal";
            }
            else if(porttype == "sc_core::sc_out" || porttype == "sca_tdf::sc_out"
                    || porttype == "sc_core::sc_in" || porttype == "sca_tdf::sc_in")
            {
               return "sc_core::sc_signal";
            }
            return "";
        };
    };
    
    struct param{               //  parameters
        string name ;           // define a name, use defalut value if unset
        string datatype ;       // specify the data type of the parameter bool etc.
        string description ;    // description of the  parameter
        string def_value ;      // default value
        
        // overload operator < for std::map
        bool operator< (const param & obj) const{
            return (obj.name > this->name);
        }
    };
    
    struct func{                            // user defined functtion
        string name = "func_name";            // name of function
        string returntype = "void";;        // retun type, must be set , defult 'void'
        string description = "Add your description";    // description of the  parameter
        string retunrdesc = "Add your description";;     // description retun value
        string accessor = "public";         // public, protected and private.
        // string isvirtual ;                // virtual function or not
        std::vector<param> vect_para;       // parameters
        string user_code ;                  // user defined implementation code
    };
    
    
    
    // interconnects of instances
    struct interconnect {
        // TODO:
    };
    
    typedef struct sc_module sc_module_t;
    typedef struct instance instance_t;
    
    // systemc  incl.systemc-ams  module
    struct sc_module{
        string name  ;
        string type  ; // sca
        string moc   ; // tdf, eln, lsf
        
        std::vector<inoutport> vect_pin;
        std::vector<inoutport> vect_pout;
        std::vector<param> vect_cpara;
        std::vector<func> vect_func;
        
        std::vector<instance_t> vec_instance;
        std::vector<interconnect> vect_interconnects;
        
        string get_module_class_name ()
        {
            string class_name = "";
            if(type == "sc")
            {
                class_name=  type+"_"+name;
            }
            else if (type == "sca")
            {
                class_name= type+"_"+moc+"_"+name;
            }
            return class_name;
        };
    };
    
    // instance of block mdoules
    struct instance {
        string name ;
        string module ;
        struct inst_param
        {
            string name ;
            string value ;
        };
        string accessor ; // public, protected and private.

        // pare "mdoule name" to sca_module
        std::pair<string,sc_module_t> pair_module;
        
        // map "inst_param.name" to param
        std::map<param,string> map_param;
    };
    
    
    // top level structure
    struct top_level{
        string tstep; // time step
        string tsim; // simulation time
        string tracefile ; // trace file format vcd or tabluar

        std::vector<instance> vec_instance;
        std::vector<interconnect> vect_interconnects;
    };
    
    
    // overall project information
    struct ams_project_data
    {
        struct proj_info
        {
            string name = "project_name";
            string description = "project description";
            string author  = "Xiao Pan";
            string date  = "05-04-2018";
            string toplevelOnly = "false";
        };
        proj_info info;
        std::vector<sc_module> vect_sc_module;
        top_level *top = NULL; // optional
    };
    
    
    
   
}

#endif
