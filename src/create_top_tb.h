/**
 * @file    create_top_tb.h
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

#ifndef _CREATE_MODULE_CPP_H
#define _CREATE_MODULE_CPP_H

#include "common.h"

#include <sys/stat.h> 
#include<iostream>
#include<fstream>
#include<ctime>
#include<iomanip>
#include "insert_file_header_info.h"
#include "template_files/temp_files.h"


using namespace std;


namespace xml2sca
{
 
    bool create_sc_toplevel_module(ams_project_data &_project);
    bool create_jscon_conf_file(ams_project_data &_project);
}
#endif

