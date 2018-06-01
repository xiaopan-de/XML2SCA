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



#include "create_module_tb.h"

namespace xml2sca
{
   
    // -------------------------------------------------------------------------
    //! creat_tb testbenches for each module
    // -------------------------------------------------------------------------
    bool creat_tb (ams_project_data &_project)
    {
        if(!create_tb_struct( _project))return false;

        for (auto it = _project.vect_sc_module.begin(), end =_project.vect_sc_module.end(); it !=end ; it++)
        {
            if(!create_tb_cmake_file(_project.info, *it)) return false;
            if(!create_tb_main(_project.info, *it)) return false;
            if(!create_tb_top(_project.info, *it)) return false;
            if(!create_tb_json_files(_project.info, *it)) return false;
        }
        
        return true;
    } // creat_tb()
    
    // -------------------------------------------------------------------------
    //! create_tb_struct
    // -------------------------------------------------------------------------
    bool create_tb_struct( ams_project_data &_project)
    {
        string project_demo_path = _project.info.name+"/tb";
        mkdir(project_demo_path.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
        
        string project_modultb_path;
        for (auto it = _project.vect_sc_module.begin(), end =_project.vect_sc_module.end(); it !=end ; it++)
        {
            project_modultb_path = _project.info.name+"/tb/"+it->name;
            mkdir(project_modultb_path.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
        }
        
        return  true;
    } //create_tb_struct()

    // -------------------------------------------------------------------------
    //! create_tb_cmake_fiels
    // -------------------------------------------------------------------------
    bool create_tb_cmake_file(ams_project_data::proj_info _proj_info, sc_module &_module)
    {
        //
        // add subfolders to the end of main Cmakefile
        //
        std::fstream pstrCmakeProjfile;
        pstrCmakeProjfile.open (_proj_info.name+"/CMakeLists.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        pstrCmakeProjfile<<"ADD_SUBDIRECTORY(tb/"<<_module.name<<")"<<endl;
        
        //
        // creat cmakefiles for module testbench
        //
        std::unique_ptr<std::ostream> pstrCmakeTbfile;
        pstrCmakeTbfile.reset(new std::ofstream(_proj_info.name+"/tb/"+_module.name+"/CMakeLists.txt"));
        *pstrCmakeTbfile<<"ADD_EXECUTABLE(" << _module.name+"_tb" <<endl;
        string module_class_name = _module.get_module_class_name();
        *pstrCmakeTbfile << "../../src/"<<module_class_name+".cpp" <<endl;
        *pstrCmakeTbfile << "../../src/"<< module_class_name+".h" <<endl;
        *pstrCmakeTbfile<<"sc_top.h" <<endl;
        *pstrCmakeTbfile<<"main.cpp" <<endl;
        *pstrCmakeTbfile<<")"<<endl;
        *pstrCmakeTbfile <<"target_link_libraries( "<<  _module.name+"_tb" <<endl;
        *pstrCmakeTbfile <<" ${SYSTEMC_LIBRARIES}" <<endl;
        *pstrCmakeTbfile <<"${SYSTEMCAMS_LIBRARIES}" <<endl;
        *pstrCmakeTbfile<<")" <<endl;
        
        return  true;
    } //create_tb_cmake_fiels()
    
    
    // -------------------------------------------------------------------------
    //! create_tb_main file
    // -------------------------------------------------------------------------
    bool create_tb_main(ams_project_data::proj_info _proj_info, sc_module &_module)
    {
        
        std::unique_ptr<std::ostream> pstrTbMainfile;
        pstrTbMainfile.reset(new std::ofstream(_proj_info.name+"/tb/"+_module.name+"/main.cpp"));
        pstrTbMainfile->write((char *)&temp_main_cpp[0], sizeof(temp_main_cpp));

        return true;
    }
    
    
    // -------------------------------------------------------------------------
    //! create_tb_top file
    // -------------------------------------------------------------------------
    bool create_tb_top(ams_project_data::proj_info _proj_info, sc_module &_module)
    {
        //
        // creat sc_top.cpp file :
        //
        string sc_top_file_path = _proj_info.name+"/tb/"+_module.name+"/sc_top.h";
        std::unique_ptr<std::ostream> pstrTophFile;
        pstrTophFile.reset(new std::ofstream(sc_top_file_path));
        
        //
        // file header information
        //
        insert_file_header_info(pstrTophFile, sc_top_file_path, _proj_info);
        
        //
        // preprocessor
        //
        *pstrTophFile<<"#ifndef sc_top_h_"<<endl<<endl;
        *pstrTophFile<<"#define sc_top_h_"<<endl;
        *pstrTophFile<<"#include \"../../src/basic/json.hpp\""<<endl;
        *pstrTophFile<<"#include \"../../src/basic/file_parse.h\""<<endl;
    
        //
        // add module head file
        //
        *pstrTophFile<<"#include \"../../src/"<< _module.get_module_class_name()<< ".h\"" <<endl<<endl<<endl;

        //
        // stimuli module
        //
        insert_stimuli_model(pstrTophFile, _module);
        

        //
        // start of class definition
        //
        *pstrTophFile<<endl<<endl;
        *pstrTophFile<<"//! "<<endl;
        *pstrTophFile<<"//! @brief testbench toplevel module"<<endl;
        *pstrTophFile<<"//! "<<endl;
        *pstrTophFile<<"SC_MODULE(sc_top) "<<endl;
        *pstrTophFile<<"{"<<endl;

        //
        //  add module instances defeinitions
        //
        *pstrTophFile<<"\t"<<std::left << std::setw(20)<<std::setfill(' ')<<"sca_tdf_stimuli "<< " *i_stimuli;"<<endl;
        *pstrTophFile<<"\t"<<std::left << std::setw(20)<<std::setfill(' ')<<_module.get_module_class_name()<< " *"<<"i_"+_module.name<<";" <<endl;
        *pstrTophFile<<endl;

        //
        //  add signals
        //
        for (auto it=_module.vect_pout.begin(), end=_module.vect_pout.end();
             it != end; it++)
        {
            *pstrTophFile<<"\t"<<it->get_signal_type()<<std::left << std::setw(20)<<std::setfill(' ')<< "<"+it->datatype+">";
            *pstrTophFile<< "sig_from_" + _module.name +"_"+ it->name<<";"<<endl;
        }
        for (auto it=_module.vect_pin.begin(), end=_module.vect_pin.end();
             it != end; it++)
        {
            *pstrTophFile<<"\t"<<it->get_signal_type()<<std::left << std::setw(20)<<std::setfill(' ')<< "<"+it->datatype+">";
            *pstrTophFile<< "sig_to_" + _module.name +"_"+ it->name<<";"<<endl;
        }
        *pstrTophFile<<endl;
        
        //
        // defeine sample period
        //
        *pstrTophFile<<"\t// sample period" <<endl;
        *pstrTophFile<<"\tsca_core::sca_time tstep;" <<endl;
        *pstrTophFile<<endl;

        //
        // defeine trace file
        //
        *pstrTophFile<<"\t// trace file" <<endl;
        *pstrTophFile<<"\tsca_util::sca_trace_file* tf;" <<endl;
        *pstrTophFile<<"\tstd::string trace_format; "<<endl<<endl;
        *pstrTophFile<<endl;

        //
        //  add constructor
        //
        insert_tb_constructor(pstrTophFile, _module);
        *pstrTophFile<<endl;

        
      
        // insert_tb_destructor
        insert_tb_destructor(pstrTophFile, _module);
        *pstrTophFile<<endl;
        
        


        *pstrTophFile<<"};"<<endl<<endl;
        // end of class definition

        // end of header file
        *pstrTophFile<<"#endif " <<endl;
        
        return true;
    }
    
    // -------------------------------------------------------------------------
    //! insert_tb_header_info
    // -------------------------------------------------------------------------
    void insert_stimuli_model(std::unique_ptr<std::ostream> &_pstrFile, sc_module &_module)
    {
        *_pstrFile<<"//!"<<endl;
        *_pstrFile<<"//! @brief stimuli module"<<endl;
        *_pstrFile<<"//!"<<endl;
        *_pstrFile<<"SCA_TDF_MODULE(sca_tdf_stimuli)"<<endl;
        *_pstrFile<<"{"<<endl;
        //start of module
        
        //
        // stimuli input to connect to the module's output
        //
        for (auto it=_module.vect_pout.begin(), end=_module.vect_pout.end();
             it != end; it++)
        {
            *_pstrFile<<"\t "
            << std::left << std::setw(25)<<std::setfill(' ') <<it->get_invers_port()+"<"+it->datatype+">"
            << std::left << std::setw(20)<<std::setfill(' ') << " pin_"+it->name +";" ;
            *_pstrFile<<endl;
        }
        
        //
        // stimuli output to connect to the module's input
        //
        for (auto it=_module.vect_pin.begin(), end=_module.vect_pin.end();
             it != end; it++)
        {
            *_pstrFile<<"\t "
            << std::left << std::setw(25)<<std::setfill(' ') <<it->get_invers_port()+"<"+it->datatype+">"
            << std::left << std::setw(20)<<std::setfill(' ') << " pout_"+it->name +";" ;
            *_pstrFile<<endl;
        }
        
        
        *_pstrFile<<endl;
        *_pstrFile<<"\t// constructor"<<endl;
        *_pstrFile<<"\tsca_tdf_stimuli(sc_core::sc_module_name nm){}"<<endl;
        *_pstrFile<<endl;
        *_pstrFile<<endl;
        *_pstrFile<<"\tvoid processing()"<<endl;
        *_pstrFile<<"\t{"<<endl;
        
        *_pstrFile<<"\t\t//output port"<<endl;
        for (auto it=_module.vect_pin.begin(), end=_module.vect_pin.end();
             it != end; it++)
        {
            *_pstrFile<<"\t\tpout_"+it->name +".write(0);"<<endl;
        }
        *_pstrFile<<""<<endl;
        *_pstrFile<<"\t\t//input port"<<endl;
        for (auto it=_module.vect_pout.begin(), end=_module.vect_pout.end();
             it != end; it++)
        {
            *_pstrFile<<"\t\tstd::cout<< \" readout " << _module.name << "-pout["<<it->name<<"] = \" << pin_"+it->name +".read()<<std::endl; "<<endl;
        }
        
        *_pstrFile<<"\t}"<<endl;
        
        // end of module
        *_pstrFile<<"};"<<endl;
    }

    // -------------------------------------------------------------------------
    //! insert_tb_constructor
    // -------------------------------------------------------------------------
    void insert_tb_constructor(std::unique_ptr<std::ostream> &_pstrFile, sc_module &_module)
    {
        //
        // start of constructor
        //
        *_pstrFile<<"\t// Constructor"<< endl;
        *_pstrFile<<"\tsc_top(sc_core::sc_module_name nm,nlohmann::json  _configs):sc_module(nm)"<<endl;
        *_pstrFile<<"\t{ "<<endl;
        *_pstrFile<<endl;

        //
        // sample period
        //
        *_pstrFile<<"\t\t// get time step from json configuration file  "<<endl;
        *_pstrFile<<"\t\ttstep = sca_core::sca_time(_configs[\"system\"][\"tstep\"] ,sc_core::SC_SEC);" <<endl;
        *_pstrFile<<endl;

        //
        // trace file
        //
        *_pstrFile<<"\t\t// trace file"<<endl;
        *_pstrFile<<"\t\ttrace_format = _configs[\"system\"][\"tracefile\"];"<<endl;
        *_pstrFile<<"\t\ttf = trace_format.compare(\"vcd\") == 0 ?"<<endl;
        *_pstrFile<<"\t\t\tsca_util::sca_create_vcd_trace_file(\"tracefile_"<<_module.name <<"_tb\"):"<<endl;
        *_pstrFile<<"\t\t\tsca_util::sca_create_tabular_trace_file(\"tracefile_"<<_module.name <<"_tb\");"<<endl;
        *_pstrFile<<endl;
        
        //
        // implementation of top level instances
        //
        *_pstrFile<<"\t\t// top level instances"<<endl;
        *_pstrFile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<< "i_stimuli "<< "= new sca_tdf_stimuli(\"i_stimuli\");"<<endl;

        *_pstrFile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<< "i_"+_module.name <<"= new "<< _module.get_module_class_name() <<"(\"i_"<< _module.name<<"\"";
        for (auto it_para = _module.vect_cpara.begin(), end_para =_module.vect_cpara.end(); it_para !=end_para ; it_para++)
        {
            *_pstrFile<<","<<endl;
            *_pstrFile<<std::left << std::setw(15)<<std::setfill(' ')<<"\t\t\t"<<std::left << std::setw(15)<<std::setfill(' ')
            << "("+it_para->datatype+")"<<"_configs[\""<< "i_"+_module.name <<"\"][\""<<it_para->name <<"\"]" ;
        }
        *_pstrFile<<(");")<<endl;
        *_pstrFile<<endl;

        
        //
        //  add signal connections
        //
        *_pstrFile<<"\t\t// interconnections "<<endl;
        for (auto it=_module.vect_pout.begin(), end=_module.vect_pout.end();
             it != end; it++)
        {
            *_pstrFile<< "\t\ti_"+_module.name << "->" << "pout_"+it->name;
            *_pstrFile<< "(sig_from_" + _module.name +"_"+ it->name<<");"<<endl;
            *_pstrFile<< "\t\ti_stimuli->"<< "pin_"+it->name;
            *_pstrFile<< "(sig_from_" + _module.name +"_"+ it->name<<");"<<endl;
        }
        for (auto it=_module.vect_pin.begin(), end=_module.vect_pin.end();
             it != end; it++)
        {
            *_pstrFile<< "\t\ti_"+_module.name << "->" << "pin_"+it->name;
            *_pstrFile<< "(sig_to_" + _module.name +"_"+ it->name<<");"<<endl;
            *_pstrFile<< "\t\ti_stimuli->"<< "pout_"+it->name;
            *_pstrFile<< "(sig_to_" + _module.name +"_"+ it->name<<");"<<endl;
        }
        *_pstrFile<<endl;
        
        //
        //  setup timestep
        //
        *_pstrFile<<"\t\t// setup timestep"<<endl;
        *_pstrFile<<"\t\ti_stimuli->set_timestep(tstep);"<<endl;
        
        //
        //  add signals to the tracefile
        //
        for (auto it=_module.vect_pout.begin(), end=_module.vect_pout.end();
             it != end; it++)
        {
            *_pstrFile<<"\t\tsca_util::sca_trace(tf, "<<"sig_from_" + _module.name +"_"+ it->name << ", \"" <<"sig_from_" + _module.name +"_"+ it->name<< "\");"<<endl;
        }
        for (auto it=_module.vect_pin.begin(), end=_module.vect_pin.end();
             it != end; it++)
        {
            *_pstrFile<<"\t\tsca_util::sca_trace(tf, "<<"sig_to_" + _module.name +"_"+ it->name << ", \"" <<"sig_to_" + _module.name +"_"+ it->name<< "\");"<<endl;
        }
        *_pstrFile<<endl;

//        end of constructor
        *_pstrFile<<"\t};"<<endl<<endl;
    }
    
    
    // -------------------------------------------------------------------------
    //! insert_tb_header_info
    // -------------------------------------------------------------------------
    void insert_tb_destructor(std::unique_ptr<std::ostream> &_pstrFile, sc_module &_module)
    {
        // start of destructor
        *_pstrFile<<"\t// Destructor"<< endl;
        *_pstrFile<<"\t~sc_top()" << endl;
        *_pstrFile<<"\t{"<< endl;
        *_pstrFile<<"\t\tif(trace_format.compare(\"vcd\")) "<<endl;
        *_pstrFile<<"\t\t\t sca_close_tabular_trace_file(tf);"<<endl;
        *_pstrFile<<"\t\t else"<<endl;
        *_pstrFile<<"\t\t\t sca_close_vcd_trace_file(tf);"<<endl;
        *_pstrFile<<"\t}"<<endl;
        *_pstrFile<<endl;

        // end of deconstructor
    }

    // -------------------------------------------------------------------------
    //! insert_tb_header_info
    // -------------------------------------------------------------------------
    bool create_tb_json_files(ams_project_data::proj_info _proj_info, sc_module &_module)
    {
        //
        // creat json configureation file
        //
        std::unique_ptr<std::ostream> pstrJsonfile;
        pstrJsonfile.reset(new std::ofstream(_proj_info.name+"/tb/"+_module.name+"/cfg.json"));
        //
        // system configurations
        //
        *pstrJsonfile<<"{"<<endl;
        *pstrJsonfile<<"\t\"system\":{"<<endl;
        *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\"tstep\""<<": "<< "1e-9" <<"," <<endl;
        *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\"tsim\""<<": " << "1e-3"<<"," <<endl;
        *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\"tracefile\""<<": \"" << "vcd"<<"\"" <<endl;
        *pstrJsonfile<<"\t},"<<endl;
        
        //
        // module config
        //
        *pstrJsonfile<<"\t\""<< "i_"+_module.name<< "\":{"<<endl;
        for (auto it_begin = _module.vect_cpara.begin(), end_para =_module.vect_cpara.end(), it_para=it_begin ;
             it_para !=end_para ;
             it_para++)
        {
            if(it_para!=it_begin)
            {
                *pstrJsonfile<<","<<endl;
            }
            *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\""+it_para->name+"\""<<": " << it_para->def_value ;
        }
        *pstrJsonfile<<endl;
        
        *pstrJsonfile<<"\t}"<<endl;

        // end of json file
        *pstrJsonfile<<"}"<<endl;

        return true;
    }
}

