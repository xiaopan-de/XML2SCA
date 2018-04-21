/**
 * @file    parse_project_xml.h
 * @author  Xiao Pan 
 * @date    06-04-2018
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

#ifndef _PARSE_PROJECT_XML_H
#define _PARSE_PROJECT_XML_H

#include "basics/TinyXML2/tinyxml2.h"

#include "common.h"

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cstring>
#include <algorithm>


using namespace std;
using namespace tinyxml2;

namespace xml2sca
{
    bool parse_project_XML( const char* _pfilename, ams_project_data* _project );
    
    bool add_project_info( XMLElement* _info_elm, ams_project_data* _project );
    bool add_toplevel( XMLElement* _top_elm, ams_project_data* _project );
    bool get_instance( XMLElement* _instance_elm, ams_project_data* _project,  instance *_inst);

    bool add_module( XMLElement* _module_elm, ams_project_data* _project );
    bool get_port( XMLElement* _port_elm, inoutport * _port);
    bool get_para( XMLElement* _para_elm, param* _para);
    bool get_func( XMLElement* _func_elm, func* _func);

    void dbg_print(ams_project_data* _project);
    
    string remove_whitespace(string _str);

}

#endif
