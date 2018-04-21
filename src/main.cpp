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


#include "basics/file_parse.h"
#include "create_project.h"
#include "parse_project_xml.h"
#include "common.h"

using namespace xml2sca; 

int main(int argc, char* argv[])
{
    std::string         projectxml_file; // project xml filename
    ams_project_data    project_data;    // project data structure
    
    projectxml_file = parse_opt(argc, argv);
    
//    cout<<"projectxml_file = "<<projectxml_file<<endl;


    //
    // parse xml file, generate project structure
    //
    if( !parse_project_XML(projectxml_file.c_str(), &project_data))
    {
        cerr<<"Error: Failed to parse xml filen (see log.txt file)."<<endl;
        exit(0);
    }

    //
    // debug print xml parse result; 
    //
    dbg_print(&project_data);


    //
    // create project from structure
    //
    if(!creat_project(project_data)){
        cerr<<"Error: Failed to create project."<<endl;
        exit(0);
    }
    
    cout<<"Create Project ["<<project_data.info.name <<"] Succeed! "<<endl;
	return 0;
}

