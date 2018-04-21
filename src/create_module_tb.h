/**
 * @file    create_module_tb.cpp
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
#ifndef _CREATE_MODULE_TB_H
#define _CREATE_MODULE_TB_H

#include "common.h"

#include<sys/stat.h>
#include<iostream>
#include<fstream>
#include<ctime>
#include<iomanip>
#include "insert_file_header_info.h"
#include "template_files/temp_files.h"

using namespace std;


namespace xml2sca
{
    bool creat_tb (ams_project_data &_project);
    bool create_tb_struct(ams_project_data &_project);
    bool create_tb_cmake_file(ams_project_data::proj_info _proj_info, sc_module &_module);
    bool create_tb_main(ams_project_data::proj_info _proj_info, sc_module &_module);
    bool create_tb_json_files(ams_project_data::proj_info _proj_info, sc_module &_module);

    bool create_tb_top(ams_project_data::proj_info _proj_info, sc_module &_module);
    void insert_stimuli_model(std::unique_ptr<std::ostream> &_pstrFile, sc_module &_module);
    void insert_tb_constructor(std::unique_ptr<std::ostream> &_pstrFile, sc_module &_module);
    void insert_tb_destructor(std::unique_ptr<std::ostream> &_pstrFile, sc_module &_module);

}
#endif

