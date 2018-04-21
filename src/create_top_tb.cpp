/**
 * @file    create_top_tb.cpp
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


#include "create_top_tb.h"

namespace xml2sca
{

    // -------------------------------------------------------------------------
    //! create_sc_toplevel_module
    // -------------------------------------------------------------------------
    bool create_sc_toplevel_module(ams_project_data &_project){
        //
        // creat sc_top.cpp file :
        //
        string sc_top_file_path = _project.info.name+"/src/sc_top.h";
        std::unique_ptr<std::ostream> pstrScTopCppFile;
        pstrScTopCppFile.reset(new std::ofstream(sc_top_file_path));
        
        //
        // file header information
        //
        insert_file_header_info(pstrScTopCppFile,sc_top_file_path, _project.info);
        
        //
        // preprocessor
        //
        *pstrScTopCppFile<<"#ifndef sc_top_h_"<<endl<<endl;
        *pstrScTopCppFile<<"#define sc_top_h_"<<endl;
        *pstrScTopCppFile<<"#include \"basic/json.hpp\""<<endl;
        *pstrScTopCppFile<<"#include \"basic/file_parse.h\""<<endl;

        //
        // add instance head file
        //
        for (auto it = _project.vect_sc_module.begin(), end =_project.vect_sc_module.end(); it !=end ; it++)
        {
            *pstrScTopCppFile<<"#include \""<< it->get_module_class_name()<< ".h\"" <<endl;
        }
        
        //
        // start of class definition
        //
        *pstrScTopCppFile<<endl<<endl;
        *pstrScTopCppFile<<"//! "<<endl;
        *pstrScTopCppFile<<"//! @brief toplevel  module"<<endl;
        *pstrScTopCppFile<<"//! "<<endl;
        *pstrScTopCppFile<<"SC_MODULE(sc_top) "<<endl;
        *pstrScTopCppFile<<"{"<<endl;
        
        //
        //  add module instances defeinitions
        //
        if(_project.top!=NULL)
        {
            string instance_name;
            for (auto it = _project.top->vec_instance.begin(), end =_project.top->vec_instance.end();
                 it !=end ; it++)
            {
                *pstrScTopCppFile<<"\t"<<std::left << std::setw(15)<<std::setfill(' ')<<it->pair_module.second.get_module_class_name()<< "*"<<"i_"+ it->name<<";" <<endl;
            }
            *pstrScTopCppFile<<endl;
            
            //
            //  add signals for further use.
            //
            for (auto it = _project.top->vec_instance.begin(), end =_project.top->vec_instance.end(); it !=end ; it++)
            {
                for (auto it_pout = it->pair_module.second.vect_pout.begin(), end_pout =it->pair_module.second.vect_pout.end(); it_pout !=end_pout ; it_pout++){
                    if(it_pout->porttype == "sca_tdf::sca_out")
                    {
                        *pstrScTopCppFile<<"\tsca_tdf::sca_signal";
                    }
                    else if(it_pout->porttype == "sc_core::sc_out" ||it_pout->porttype == "sca_tdf::sc_out")
                    {
                        *pstrScTopCppFile<<"\tsc_core::sc_signal";
                    }
                    *pstrScTopCppFile<<std::left << std::setw(15)<<std::setfill(' ')<< "<"+it_pout->datatype+">";
                    *pstrScTopCppFile<< "sig_from_" + it->name +"_"+ it_pout->name<<";"<<endl;
                }
            }
            *pstrScTopCppFile<<endl;
        }
        else{
            cout<<"TOP NOT exist" <<endl;
        }
        
        //
        // sample period
        //
        *pstrScTopCppFile<<"\t// sample period" <<endl;
        *pstrScTopCppFile<<"\tsca_core::sca_time tstep;" <<endl;
        
        //
        // trace file
        //
        *pstrScTopCppFile<<"\t// trace file" <<endl;
        *pstrScTopCppFile<<"\tsca_util::sca_trace_file* tf;" <<endl;
        *pstrScTopCppFile<<"\tstd::string trace_format; "<<endl<<endl;
        
        
        
        //
        // start of constructor
        //
        *pstrScTopCppFile<<"public: "<<endl;
        *pstrScTopCppFile<<"\t// Constructor"<< endl;
        *pstrScTopCppFile<<"\tsc_top(sc_core::sc_module_name nm,nlohmann::json  _configs):sc_module(nm)"<<endl;
        *pstrScTopCppFile<<"\t{ "<<endl;
        if(_project.top!=NULL)
        {
            //
            // sample period
            //
            *pstrScTopCppFile<<"\t\ttstep = sca_core::sca_time(_configs[\"system\"][\"tstep\"] ,sc_core::SC_SEC);" <<endl;
            
            //
            // trace file
            //
            *pstrScTopCppFile<<"\t\t// trace file"<<endl;
            *pstrScTopCppFile<<"\t\ttrace_format = _configs[\"system\"][\"tracefile\"];"<<endl;
            *pstrScTopCppFile<<"\t\ttf = trace_format.compare(\"vcd\") == 0 ?"<<endl;
            *pstrScTopCppFile<<"\t\t\tsca_util::sca_create_vcd_trace_file(\"tracefile_top\"):"<<endl;
            *pstrScTopCppFile<<"\t\t\tsca_util::sca_create_tabular_trace_file(\"tracefile_top\");"<<endl;
            *pstrScTopCppFile<<endl;
            //
            // implementation of top level instances
            //
            for (auto it = _project.top->vec_instance.begin(), end =_project.top->vec_instance.end(); it !=end ; it++)
            {
                *pstrScTopCppFile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<< "i_"+it->name <<"= new "<< it->pair_module.second.get_module_class_name() <<"(\"i_"<< it->name<<"\"";
                for (auto it_para = it->pair_module.second.vect_cpara.begin(), end_para =it->pair_module.second.vect_cpara.end(); it_para !=end_para ; it_para++)
                {
                    *pstrScTopCppFile<<","<<endl;
                    *pstrScTopCppFile<<std::left << std::setw(15)<<std::setfill(' ')<<"\t\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<< "("+it_para->datatype+")"<<"_configs[\""<< "i_"+it->name <<"\"][\""<<it_para->name <<"\"]" ;
                }
                *pstrScTopCppFile<<(");")<<endl;
            }
            *pstrScTopCppFile<<endl;
            
            //
            //  add outgoing signal connects
            //
            *pstrScTopCppFile<<"\t\t{ // Start of outgoing connections "<<endl;
            for (auto it = _project.top->vec_instance.begin(), end =_project.top->vec_instance.end(); it !=end ; it++)
            {
                for (auto it_pout = it->pair_module.second.vect_pout.begin(), end_pout =it->pair_module.second.vect_pout.end(); it_pout !=end_pout ; it_pout++)
                {
                    *pstrScTopCppFile<<"\t\t\t"<< "i_"+it->name<<"->"<< "pout_"+ it_pout->name <<"("<<"sig_from_" + it->name +"_"+ it_pout->name <<");"<<endl;
                }
            }
            *pstrScTopCppFile<<"\t\t} //  End ofoutgoing connections "<<endl;
            
            
            *pstrScTopCppFile<<endl;
            *pstrScTopCppFile<<"\t\t{ // Start of incoming connections."<<endl;
            *pstrScTopCppFile<<"\t\t\t // "<< endl;
            *pstrScTopCppFile<<"\t\t\t // Add your code here: incoming connections, set time step."<< endl;
            *pstrScTopCppFile<<"\t\t\t // "<< endl;
            *pstrScTopCppFile<<endl;
            *pstrScTopCppFile<<"\t\t} // End of incoming connections."<<endl;
            *pstrScTopCppFile<<endl;
            
            
            //
            //  add signals to the trace file
            //
            for (auto it = _project.top->vec_instance.begin(), end =_project.top->vec_instance.end(); it !=end ; it++)
            {
                for (auto it_pout = it->pair_module.second.vect_pout.begin(), end_pout =it->pair_module.second.vect_pout.end(); it_pout !=end_pout ; it_pout++)
                {
                    *pstrScTopCppFile<<"\t\tsca_util::sca_trace(tf, "<<"sig_from_" + it->name +"_"+ it_pout->name << ", \"" <<"sig_from_" + it->name +"_"+ it_pout->name<< "\");"<<endl;
                }
            }
        }
        
        
        
        // end of constructor
        *pstrScTopCppFile<<"\t};"<<endl<<endl;
        // start of destructor
        *pstrScTopCppFile<<"\t// Destructor"<< endl;
        *pstrScTopCppFile<<"\t~sc_top()" << endl;
        *pstrScTopCppFile<<"\t{"<< endl;
        *pstrScTopCppFile<<"\t\tif(trace_format.compare(\"vcd\")) "<<endl;
        *pstrScTopCppFile<<"\t\t\t sca_close_tabular_trace_file(tf);"<<endl;
        *pstrScTopCppFile<<"\t\t else"<<endl;
        *pstrScTopCppFile<<"\t\t\t sca_close_vcd_trace_file(tf);"<<endl;
        *pstrScTopCppFile<<"\t}"<<endl;
        // end of deconstructor
        
        *pstrScTopCppFile<<"};"<<endl<<endl;
        // end of class definition
        
        // end of header file
        *pstrScTopCppFile<<"#endif " <<endl;
        
        return true;
    }
    
    // -------------------------------------------------------------------------
    //! create_jscon_conf_file
    // -------------------------------------------------------------------------
    bool create_jscon_conf_file(ams_project_data &_project)
    {
        //
        // creat json configureation file
        //
        std::unique_ptr<std::ostream> pstrJsonfile;
        pstrJsonfile.reset(new std::ofstream(_project.info.name+"/configs/cfg-toplevel.json"));
        if(_project.top==NULL )
        {
            *pstrJsonfile<<"{"<<endl;
            *pstrJsonfile<<"\t\"system\":{"<<endl;
            *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\"tstep\""<<": "<< "1e-9" <<"," <<endl;
            *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\"tsim\""<<": " << "1e-3"<<"," <<endl;
            *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\"tracefile\""<<": \" " << "vcd"<<"\"," <<endl;
            *pstrJsonfile<<"\t}"<<endl;
            *pstrJsonfile<<"}"<<endl;
            
            return true;
        }
        *pstrJsonfile<<"{"<<endl;
        *pstrJsonfile<<"\t\"system\":{"<<endl;
        *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\"tstep\""<<": "<< _project.top->tstep <<"," <<endl;
        *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\"tsim\""<<": " << _project.top->tsim<<"," <<endl;
        *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\"tracefile\""<<": \"" << _project.top->tracefile<<"\"," <<endl;
        *pstrJsonfile<<"\t}";
        
        for (auto it = _project.top->vec_instance.begin(), end =_project.top->vec_instance.end(); it !=end ; it++)
        {
            if(it->pair_module.second.vect_cpara.size() == 0)
            {
                continue;
            }
            *pstrJsonfile<<","<<endl;
            *pstrJsonfile<<"\t\""<< "i_"+it->name<< "\":{"<<endl;
            for (auto it_begin = it->map_param.rbegin(), end_para =it->map_param.rend(), it_para=it_begin ;
                 it_para !=end_para ;
                 it_para++)
            {
                if(it_para!=it_begin)
                {
                    *pstrJsonfile<<","<<endl;
                }
                *pstrJsonfile<<"\t\t"<<std::left << std::setw(15)<<std::setfill(' ')<<"\""+it_para->first.name+"\""<<": " << it_para->second;
            }
            *pstrJsonfile<<endl;
            *pstrJsonfile<<("\t}");
        }
        *pstrJsonfile<<endl;
        *pstrJsonfile<<"}"<<endl;
        
        
        
        return  true;
    } //create_proj_struct()

}

