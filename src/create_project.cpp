/**
 * @file    create_project.cpp
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

#include "create_project.h"

namespace xml2sca
{
    // -------------------------------------------------------------------------
    //! creat_project
    // -------------------------------------------------------------------------
    bool creat_project (ams_project_data &_project)
    {
        if(!create_proj_struct( _project))return false;
        if(!create_cmake_fiels(_project)) return false;
        if(!copy_template_fiels(_project)) return false;
        
        if(_project.info.toplevelOnly.compare("true"))
        {
            for (auto it = _project.vect_sc_module.begin(), end =_project.vect_sc_module.end(); it !=end ; it++)
            {
                
                if(!create_sc_h_file(_project.info, *it))
                    return false;
                if(!create_sc_cpp_file(_project.info, *it))
                    return  false;
            }
            if(!creat_tb(_project)) return false;
        }
            
      
        if(!create_sc_toplevel_module(_project)) return false;
        if(!create_jscon_conf_file(_project)) return false;
        
        return true;
    } // creat_project()
    
    
    
    
    
    // -------------------------------------------------------------------------
    //! create_proj_struct
    // -------------------------------------------------------------------------
    bool create_proj_struct( ams_project_data &_project)
    {
        string project_path = _project.info.name;
        mkdir(project_path.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

        string project_src_path = _project.info.name+"/src";
        mkdir(project_src_path.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

        string project_cmakefiles_path = _project.info.name+"/cmake-files";
        mkdir(project_cmakefiles_path.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

        string project_commen_path = _project.info.name+"/src/basic";
        mkdir(project_commen_path.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

        string project_configfile_path = _project.info.name+"/configs";
        mkdir(project_configfile_path.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

        return  true;
    } //create_proj_struct()
    
    
    
    // -------------------------------------------------------------------------
    //! copy_commen_fiels
    // -------------------------------------------------------------------------
    bool copy_template_fiels(ams_project_data &_project){
        //
        // copy  cmake-files/FindPackageHandleStandardArgs.cmake
        //
        std::unique_ptr<std::ostream> pstrFile;
        pstrFile.reset(new std::ofstream(_project.info.name+"/cmake-files/FindPackageHandleStandardArgs.cmake"));
        pstrFile->write((char *)&temp_FindPackageHandleStandardArgs_cmake[0], sizeof(temp_FindPackageHandleStandardArgs_cmake));

        
        //
        // copy  cmake-files/FindSystemC.cmake
        //
        pstrFile.reset(new std::ofstream( _project.info.name+"/cmake-files/FindSystemC.cmake"));
        pstrFile->write((char *)&temp_FindSystemC_cmake[0], sizeof(temp_FindSystemC_cmake));

        
        //
        // copy  cmake-files/FindSystemCAMS.cmake
        //
        pstrFile.reset(new std::ofstream( _project.info.name+"/cmake-files/FindSystemCAMS.cmake"));
        pstrFile->write((char *)&temp_FindSystemCAMS_cmake[0], sizeof(temp_FindSystemCAMS_cmake));

        
        //
        // copy  src/main.cpp
        //
        pstrFile.reset(new std::ofstream( _project.info.name+"/src/main.cpp"));
        pstrFile->write((char *)&temp_main_cpp[0], sizeof(temp_main_cpp));

        
        //
        // copy  src/basic/cxxopts.hpp
        //
        pstrFile.reset(new std::ofstream( _project.info.name+"/src/basic/cxxopts.hpp"));
        pstrFile->write((char *)&temp_cxxopts_hpp[0], sizeof(temp_cxxopts_hpp));
        
        //
        // copy  src/basic/json.hpp
        //
        pstrFile.reset(new std::ofstream(_project.info.name+"/src/basic/json.hpp"));
        pstrFile->write((char *)&temp_json_hpp[0], sizeof(temp_json_hpp));
        
        //
        // copy  src/basic/file_parse.h"
        //
        pstrFile.reset(new std::ofstream(_project.info.name+"/src/basic/file_parse.h"));
        pstrFile->write((char *)&temp_file_parse_h[0], sizeof(temp_file_parse_h));
        
        
        return true;
    }
    
    
    // -------------------------------------------------------------------------
    //! create_cmake_fiels
    // -------------------------------------------------------------------------
    bool create_cmake_fiels( ams_project_data &_project)
    {
        //
        // creat project CMakeLists.txt
        //
        std::unique_ptr<std::ostream> pstrCmakeProjfile;
        pstrCmakeProjfile.reset(new std::ofstream(_project.info.name+"/CMakeLists.txt"));
        *pstrCmakeProjfile<<"cmake_minimum_required (VERSION 2.8)"      <<endl;
        *pstrCmakeProjfile<<"project ("<< _project.info.name << ")  "      <<endl;
        *pstrCmakeProjfile<<endl;
        *pstrCmakeProjfile<<"set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-files)"      <<endl;
        *pstrCmakeProjfile<<"set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)  "   <<endl;
        *pstrCmakeProjfile<<"set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -fpermissive -std=c++0x -DSC_INCLUDE_DYNAMIC_PROCESSES\") "  <<endl;
        *pstrCmakeProjfile<<endl;
        *pstrCmakeProjfile<<"find_package(SystemC REQUIRED)"      <<endl;
        *pstrCmakeProjfile<<"find_package(SystemCAMS REQUIRED)"   <<endl;
        *pstrCmakeProjfile<<endl;
        *pstrCmakeProjfile<<"include_directories(${SYSTEMC_INCLUDE_DIRS}  "      <<endl;
        *pstrCmakeProjfile<<"                    ${SYSTEMCAMS_INCLUDE_DIRS}) "   <<endl;
        *pstrCmakeProjfile<<endl;
        *pstrCmakeProjfile<<"ADD_SUBDIRECTORY(src bin)  "      <<endl;


        //
        // creat cmake for src-folder  files
        //
        std::unique_ptr<std::ostream> pstrCmakeSrcfile;
        pstrCmakeSrcfile.reset(new std::ofstream(_project.info.name+"/src/CMakeLists.txt"));
        *pstrCmakeSrcfile<<"ADD_EXECUTABLE(" << _project.info.name <<endl;
        for (auto it = _project.vect_sc_module.begin(), end =_project.vect_sc_module.end(); it !=end ; it++)
        {
            string module_class_name ;
            if(it->type =="sca")
            {
                module_class_name =  it->type+"_"+it->moc+"_"+it->name;
            }else{
                module_class_name =  it->type+"_"+it->name;
            }
            *pstrCmakeSrcfile << module_class_name+".cpp" <<endl;
            *pstrCmakeSrcfile << module_class_name+".h" <<endl;
        }
        *pstrCmakeSrcfile<<"sc_top.h" <<endl;
        *pstrCmakeSrcfile<<"main.cpp)" <<endl<<endl;
        *pstrCmakeSrcfile <<"target_link_libraries( "<<  _project.info.name <<endl;
        *pstrCmakeSrcfile <<" ${SYSTEMC_LIBRARIES}" <<endl;
        *pstrCmakeSrcfile <<"${SYSTEMCAMS_LIBRARIES}" <<endl;
        *pstrCmakeSrcfile<<")" <<endl;
        
      
        return  true;
    } //create_proj_struct()

}

