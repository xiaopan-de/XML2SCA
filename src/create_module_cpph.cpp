/**
 * @file    create_module_cpph.cpp
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

#include "create_module_cpph.h"

namespace xml2sca
{
    // -------------------------------------------------------------------------
    //! create_sc_module_files SystemC head and cpp files
    // -------------------------------------------------------------------------
    bool create_sc_h_file(ams_project_data::proj_info _proj_info, sc_module &_module)
    {
        // ---- start generat modules -----
        string module_class_name = _module.get_module_class_name();
        string headfile_path = _proj_info.name+"/src/"+module_class_name+".h";
        
        // creat head file :
        std::unique_ptr<std::ostream> pstrHeadFfile;
        pstrHeadFfile.reset(new std::ofstream(headfile_path));
        
        // file header information
        insert_file_header_info(pstrHeadFfile, headfile_path, _proj_info );
        
        // preprocessor
        *pstrHeadFfile<<"#ifndef "<<module_class_name<<"_h_" <<endl;
        *pstrHeadFfile<<"#define "<<module_class_name<<"_h_" <<endl<<endl<<endl;
        
        // includes
        insert_h_include(pstrHeadFfile, _module);
        
        // TODO:  class description
        if(_module.type == "sc"){
            *pstrHeadFfile<<"SC_MODULE(" <<module_class_name <<") {" <<endl;
        }else if(_module.moc == "tdf"){
            *pstrHeadFfile<<"SCA_TDF_MODULE(" <<module_class_name <<") {"<<endl;
        }else {
            cerr <<" Module MOC not defined" <<endl;
            return  false;
        }
        
        //
        // public members
        //
        *pstrHeadFfile<<"public:" <<endl;
        // input/output ports
        insert_h_inoutports(pstrHeadFfile, _module) ;
        // constructors
        insert_h_constructor(pstrHeadFfile, _module) ;
        // public instances, sca module should not contain instances
        insert_h_instances(pstrHeadFfile, _module, "public");
        // public user defined functions
        insert_h_func(pstrHeadFfile, _module, "public");
        
        //
        // protected members
        //
        *pstrHeadFfile<<"protected:"<<endl;
        // protected instances, sca module should not contain instances
        insert_h_instances(pstrHeadFfile, _module, "protected");
        // protected user defined functions
        insert_h_func(pstrHeadFfile, _module, "protected");
        
        //
        // private members
        //
        *pstrHeadFfile<<"private:"<<endl;
        // private parameters
        insert_h_local_param(pstrHeadFfile, _module, "private");
        // private instances
        insert_h_instances(pstrHeadFfile, _module, "private");
        // private user defined functions
        insert_h_func(pstrHeadFfile, _module, "private");
        
        //  insert pre-defined functions of SCA-TDF module
        if(_module.moc== "tdf"){
            insert_h_predef_func(pstrHeadFfile, false);
        }
        
        // file end
        *pstrHeadFfile<<"};" <<endl;
        *pstrHeadFfile<<"#endif " <<endl;
        
        return true;
    } //create_sc_module_files()
    
    // -------------------------------------------------------------------------
    //! create_sc_cpp_file
    // -------------------------------------------------------------------------
    bool create_sc_cpp_file(ams_project_data::proj_info _proj_info, sc_module &_module)
    {
        string module_class_name = _module.get_module_class_name();
        string cppfile_path = _proj_info.name+"/src/"+module_class_name+".cpp";
        
        // creat cpp file :
        std::unique_ptr<std::ostream> pstrCppFile;
        pstrCppFile.reset(new std::ofstream(cppfile_path));
        
        // file header information
        insert_file_header_info(pstrCppFile,cppfile_path, _proj_info );
        
        // includes
        *pstrCppFile<<"#include \""<< module_class_name<<".h\"" <<endl<<endl;
        
        // constructor
        insert_cpp_constructor(pstrCppFile, _module);
        
        // user defined functions
        insert_cpp_user_defined_func(pstrCppFile, _module);
        
        //  insert pre-defined functions of SCA-TDF module
        if(_module.moc== "tdf"){
            insert_cpp_predef_func(pstrCppFile, module_class_name);
        }
        return true;
    }
    
    // -------------------------------------------------------------------------
    //! insert_cpp_user_defined_func
    // -------------------------------------------------------------------------
    void insert_cpp_user_defined_func(std::unique_ptr<std::ostream> &_pstrFile, sc_module& _module)
    {
        for (auto it0 = _module.vect_func.begin(), end0 = _module.vect_func.end(); it0 != end0; it0++)
        {
            *_pstrFile<<"//  " <<endl;
            *_pstrFile<<"//  @brief " <<  it0->description  <<endl;
            *_pstrFile<<"//  "    <<endl;
            *_pstrFile <<it0->returntype<<" "<<_module.get_module_class_name()<< "::" <<it0->name<<"(";
            for (auto it = it0->vect_para.begin(), end = it0->vect_para.end(); it != end; it++)
            {
                if(it->name =="" || it->datatype == "")
                {
                    continue;
                }
                *_pstrFile<< it->datatype << " " <<"_"+it->name;
                if(it != it0->vect_para.end()-1)
                {
                    *_pstrFile<<",";
                }
            }
            *_pstrFile<<")"   <<endl;
            *_pstrFile<<"{"   <<endl;
            
            if(!it0->user_code.empty())
            {
                *_pstrFile<<it0->user_code;
                *_pstrFile<<endl;
            }
            
            *_pstrFile<<"\t/* Insert your  code here */ "    <<endl;
            
            if (it0->returntype.compare("void"))
            {
                *_pstrFile<<"\t" <<  it0->returntype << " var_return; // return variable " <<endl<<endl;
                *_pstrFile<<"\treturn " <<  "var_return;" <<endl;
            }
            *_pstrFile<<"}"    <<endl<<endl;
        }
        
    } //insert_h_user_defined_func()
    
    
    
    

    
    // -------------------------------------------------------------------------
    // insert_h_inoutports
    // -------------------------------------------------------------------------
    void insert_h_inoutports(unique_ptr<ostream> &_pstrFile, sc_module &_module)
    {

        //
        // input ports
        //
        *_pstrFile<<"\t//! in/output ports" <<endl;
        for (auto it = _module.vect_pin.begin(), end = _module.vect_pin.end(); it != end; it++)
        {
            *_pstrFile<<"\t "
            << std::left << std::setw(25)<<std::setfill(' ') <<it->porttype+"<"+it->datatype+">"
            << std::left << std::setw(20)<<std::setfill(' ') << " pin_"+it->name+ ";" ;
            if(it->description != "")
            {
                *_pstrFile << "\t//!" << it->description;
            }
            *_pstrFile<<endl;
        }
        //
        // output ports
        //
        for (auto it = _module.vect_pout.begin(), end = _module.vect_pout.end(); it != end; it++)
        {
            *_pstrFile<<"\t "
            << std::left << std::setw(25)<<std::setfill(' ') <<it->porttype+"<"+it->datatype+">"
            << std::left << std::setw(20)<<std::setfill(' ') << " pout_"+it->name +";" ;
            if(it->description != "")
            {
                *_pstrFile << "\t//!" << it->description;
            }
            *_pstrFile<<endl;
        }
        *_pstrFile<<endl;
    }
    
    
    // -------------------------------------------------------------------------
    // insert_h_include
    // -------------------------------------------------------------------------
    void insert_h_include(unique_ptr<ostream> &_pstrFile, sc_module &_module)
    {
        *_pstrFile<<"#include <systemc-ams> "<<endl<<endl;
        if(_module.type == "sc")
        {
            for (auto it = _module.vec_instance.begin(), end =_module.vec_instance.end(); it !=end ; it++)
            {
                *_pstrFile<<"#include \""<< it->pair_module.second.get_module_class_name()<< ".h\"" <<endl;
            }
        }
        *_pstrFile<<endl<<endl<<endl;
    }
    
    // -------------------------------------------------------------------------
    // insert_h_constructor
    // -------------------------------------------------------------------------
    void insert_h_constructor(unique_ptr<ostream> &_pstrFile, sc_module &_module)
    {
        string module_class_name = _module.get_module_class_name();
        
        *_pstrFile<<"\t//! @brief Custom constructor of"<< module_class_name <<endl;
        
        for (auto it = _module.vect_cpara.begin(), end = _module.vect_cpara.end(); it != end; it++)
        {
            *_pstrFile<<"\t//! @para "<< it->name <<" " <<it->description << endl;
        }
        
        *_pstrFile<<"\t"<<module_class_name<<"(sc_core::sc_module_name nm";
        for (auto it = _module.vect_cpara.begin(), end = _module.vect_cpara.end(); it != end; it++)
        {
            *_pstrFile<<","<<endl;
            *_pstrFile<<"\t\t"<< it->datatype <<" _" <<it->name;
            if(!it->def_value.empty())
            {
                *_pstrFile<<" = "<<it->def_value;
            }
        }
        *_pstrFile<<" );"  <<endl<<endl;
    }
    
    // -------------------------------------------------------------------------
    // insert_h_func
    // -------------------------------------------------------------------------
    void insert_h_func(unique_ptr<ostream> &_pstrFile, sc_module  &_module, string _accessor)
    {
        for (auto it = _module.vect_func.begin(), end = _module.vect_func.end(); it != end; it++)
        {
            if(!it->accessor.compare(_accessor))
            {
                *_pstrFile<<"\t//! @brief "<< it->description <<endl;
                for (auto it1 = it->vect_para.begin(), end1 = it->vect_para.end(); it1 != end1; it1++)
                {
                    if(it1->name =="" || it1->datatype == "")
                        continue;
                    *_pstrFile<<"\t//! @para "<< "_"+it1->name <<" " <<it1->description << endl;
                }
                *_pstrFile<<"\t//! @return " << it->retunrdesc <<endl;
                *_pstrFile<<"\t" <<it->returntype<<" " <<it->name<<"(";
                for (auto it1 = it->vect_para.begin(), end1 = it->vect_para.end(); it1 != end1; it1++)
                {
                    if(it1->name =="" || it1->datatype == "")
                    {
                        continue;
                    }
                    *_pstrFile<< it1->datatype <<" " <<"_"+it1->name;
                    if(it1 != end1-1)
                    {
                        *_pstrFile<<",";
                    }
                }
                *_pstrFile<<");"   <<endl<<endl;
                
            }
        }
        *_pstrFile<<endl <<endl;
    }
    
    // -------------------------------------------------------------------------
    // insert_h_instances
    // -------------------------------------------------------------------------
    void insert_h_instances(unique_ptr<ostream> &_pstrFile, sc_module  &_module, string _accessor)
    {
        for (auto it = _module.vec_instance.begin(), end =_module.vec_instance.end(); it !=end ; it++)
        {
            if(!it->accessor.compare(_accessor))
            {
                *_pstrFile<<"\t"<<std::left << std::setw(15)<<std::setfill(' ')
                <<it->pair_module.second.get_module_class_name()<< "*"<<"i_"+ it->name<<";" <<endl;
            }
        }
        *_pstrFile<<endl <<endl;
    }
    
    // -------------------------------------------------------------------------
    // insert_h_local_param
    // -------------------------------------------------------------------------
    void insert_h_local_param(unique_ptr<ostream> &_pstrFile, sc_module  &_module, string _accessor)
    {
        *_pstrFile<<"\t // Local variables for module parameters"                                   <<endl;
        for (auto it = _module.vect_cpara.begin(), end = _module.vect_cpara.end(); it != end; it++)
        {
            *_pstrFile<<"\t"<<it->datatype  << " p_" <<it->name <<";" <<endl;
        }
    }
    
    // -------------------------------------------------------------------------
    // insert_cpp_constructor
    // -------------------------------------------------------------------------
    void insert_cpp_constructor(unique_ptr<ostream> &_pstrFile, sc_module &_module)
    {
        string module_class_name= _module.get_module_class_name();
        
        *_pstrFile<<"//  "                                                                          <<endl;
        *_pstrFile<<"//! Custom Constructor "                                                       <<endl;
        *_pstrFile<<"//  "                                                                          <<endl;
        *_pstrFile<<module_class_name <<"::"<<module_class_name <<"(sc_core::sc_module_name nm" ;
        for (auto it = _module.vect_cpara.begin(), end = _module.vect_cpara.end();
             it != end; it++)
        {
            *_pstrFile<<","<<endl;
            *_pstrFile<<"\t\t"<< it->datatype <<" _" <<it->name;
        }
        *_pstrFile<<" )"<<endl<< "{"  <<endl;
        
        //
        // Local variables store module parameters
        //
        *_pstrFile<<"\t // Local variables store module parameters"                                   <<endl;
        for (auto it = _module.vect_cpara.begin(), end = _module.vect_cpara.end(); it != end; it++)
        {
            *_pstrFile<<"\t" << " p_" <<it->name<<" = _" << it->name <<";" <<endl;
        }
        
        //
        // implementation of top level instances
        //
        for (auto it = _module.vec_instance.begin(), end =_module.vec_instance.end(); it !=end ; it++)
        {
            if(it ==_module.vec_instance.begin() )
            {
                *_pstrFile<<endl;
                *_pstrFile<<"\t//"<< endl;
                *_pstrFile<<"\t// Comment out and adapt to your implementation "<<endl;
                *_pstrFile<<"\t//"<< endl;
                
            }
            if(it->pair_module.second.type=="sca"){
                module_class_name = it->pair_module.second.type+"_"+it->pair_module.second.moc+"_"+it->pair_module.second.name;
            }
            else
            {
                module_class_name = it->pair_module.second.type+"_"+it->pair_module.second.name;
            }
            *_pstrFile<<"\t//"<<std::left << std::setw(15)<<std::setfill(' ')<< "i_"+it->name <<"= new "<< module_class_name <<"(\"i_"<< it->name<<"\"";
            for (auto it_para = it->pair_module.second.vect_cpara.begin(), end_para =it->pair_module.second.vect_cpara.end(); it_para !=end_para ; it_para++)
            {
                *_pstrFile<<",";
                *_pstrFile<<it_para->name ;
            }
            *_pstrFile<<(");")<<endl;
        }
        *_pstrFile<<endl;
        *_pstrFile<<"}" <<endl<<endl;
    }
    

    
    // -------------------------------------------------------------------------
    //! insert_h_predef_func
    // -------------------------------------------------------------------------
    void insert_h_predef_func(unique_ptr<ostream> &_pstrFile, bool _lrm)
    {
        if(_lrm){
            *_pstrFile<<"\t // The member function [set_attributes] shall provide a context to set"      <<endl;
            *_pstrFile<<"\t // attributes, which are required for TDF MoC elaboration."                  <<endl;
            *_pstrFile<<"\t // The attributes can be defined using the member functions [set_timestep]"  <<endl;
            *_pstrFile<<"\t // ,[set_max_timestep],[request_next_activation] of TDF module. And member " <<endl;
            *_pstrFile<<"\t // functions [set_timestep], [set_max_timestep], [set_delay], and "          <<endl;
            *_pstrFile<<"\t // [set_rate]  for ports of classes [sca_tdf::sca_in],[sca_tdf::sca_out]"    <<endl;
            *_pstrFile<<"\t // [sca_tdf::sca_de::sca_in], [sca_tdf::sca_de::sca_out]. And in addition"   <<endl;
            *_pstrFile<<"\t // [set_ct_delay] for ports of classes [sca_tdf::sca_out<T, "                <<endl;
            *_pstrFile<<"\t // sca_tdf::SCA_CT_CUT,INTERP>],[sca_tdf::sca_out<T,sca_tdf::SCA_DT_CUT>]."  <<endl;
            *_pstrFile<<"\t // (*See SystemC_AMS_2.0 LRM)"                                               <<endl;
        }
        *_pstrFile<<"\tvoid set_attributes(); "                                                      <<endl;

        if(_lrm){
            *_pstrFile<<"\t // The member function [initialize] shall provide a context to set initial " <<endl;
            *_pstrFile<<"\t // values to member variables and ports. In the context of member function " <<endl;
            *_pstrFile<<"\t // initialize, the application should initialize the delay samples of all "  <<endl;
            *_pstrFile<<"\t // ports if their delay attribute has been set to a value greater than zero."<<endl;
            *_pstrFile<<"\t // The member function shall only be called during simulation. The  "        <<endl;
            *_pstrFile<<"\t // application shall not call this member function."                         <<endl;
            *_pstrFile<<"\t // (* See SystemC_AMS_2.0 LRM)"                                              <<endl;
        }
        *_pstrFile<<"\tvoid initialize(); "                                                          <<endl;
        
        if(_lrm){
            *_pstrFile<<"\t // The member function [processing] shall provide a context to define"       <<endl;
            *_pstrFile<<"\t // the time-domain behavior of the TDF module. It may be replaced by a"      <<endl;
            *_pstrFile<<"\t // registered application-defined member function. It shall be a"            <<endl;
            *_pstrFile<<"\t // warning if a TDF module does not implement a single member function"      <<endl;
            *_pstrFile<<"\t // processing or a registered application-defined member function when"      <<endl;
            *_pstrFile<<"\t // time-domain simulation starts. If no application- defined member "        <<endl;
            *_pstrFile<<"\t // function is registered, this member function shall be called during "     <<endl;
            *_pstrFile<<"\t // time-domain simulation. The application shall not call this member "      <<endl;
            *_pstrFile<<"\t //  function."                                                               <<endl;
            *_pstrFile<<"\t // (* See SystemC_AMS_2.0 LRM)"                                              <<endl;
        }
        *_pstrFile<<"\tvoid processing(); "                                                          <<endl;
        
        if(_lrm){
            *_pstrFile<<"\t // The member function [change_attributes] shall provide a context to "      <<endl;
            *_pstrFile<<"\t // change attributes of the TDF module and its ports. The attributes can "   <<endl;
            *_pstrFile<<"\t // be changed using member functions [set_timestep], [set_max_timestep],"    <<endl;
            *_pstrFile<<"\t // or [request_next_activatio]n of a TDF module and member functions "       <<endl;
            *_pstrFile<<"\t // [set_timestep], [set_max_timestep], [set_delay], and [set_rate] for"      <<endl;
            *_pstrFile<<"\t //  ports of classes [sca_tdf::sca_in], [sca_tdf::sca_out], "                <<endl;
            *_pstrFile<<"\t // [sca_tdf::sca_de::sca_in], [sca_tdf::sca_de::sca_out],and in addition "   <<endl;
            *_pstrFile<<"\t // [set_ct_delay] for ports of classes [sca_tdf::sca_out<T,sca_tdf::"        <<endl;
            *_pstrFile<<"\t // SCA_CT_CUT, INTERP>] and [sca_tdf::sca_out<T, sca_tdf::SCA_DT_CUT>]."     <<endl;
            *_pstrFile<<"\t // The member function shall only be called during simulation.  "            <<endl;
            *_pstrFile<<"\t // (* See SystemC_AMS_2.0 LRM)"                                              <<endl;
        }
        *_pstrFile<<"\t// void change_attributes(); "                                                <<endl;
        if(_lrm){
            *_pstrFile<<"\t // The member function [ac_processing] shall provide a context to define the" <<endl;
            *_pstrFile<<"\t // small-signalfrequency-domain behavior of the TDF module. It may be"        <<endl;
            *_pstrFile<<"\t // replaced by a registered application-defined member function. If no"       <<endl;
            *_pstrFile<<"\t // application-defined member function is registered, this function shallon"  <<endl;
            *_pstrFile<<"\t // be called during small-signal frequency-domain simulati The application"   <<endl;
            *_pstrFile<<"\t // shall not call this member function."                                      <<endl;
            *_pstrFile<<"\t // (* See SystemC_AMS_2.0 LRM)"                                              <<endl;
        }
        *_pstrFile<<"\t// void ac_processing(); "                                                      <<endl;
        *_pstrFile<<"\t// void end_of_simulation(); "                                                          <<endl;
    }
    
    // -------------------------------------------------------------------------
    //! insert_cpp_predef_func
    // -------------------------------------------------------------------------
    void insert_cpp_predef_func(unique_ptr<ostream> &_pstrFile, string _module_class_name)
    {
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"// set_attributes()"<<endl;
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"void  "<< _module_class_name<< "::set_attributes(){ /* Insert your  code here */    }"  <<endl<<endl;
        
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"// initialize()"<<endl;
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"void  "<< _module_class_name<< "::initialize(){ /* Insert your  code here */    }" <<endl<<endl;
        
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"// processing()"<<endl;
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"void "<< _module_class_name<< "::processing(){ /* Insert your  code here */    }" <<endl<<endl;
        
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"// change_attributes()"<<endl;
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"// void  "<< _module_class_name<< "::change_attributes(){ /* Insert your  code here */    }" <<endl<<endl;
        
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"// ac_processing()"<<endl;
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"// void  "<< _module_class_name<< "::ac_processing(){  /* Insert your  code here */    }" <<endl<<endl;
        
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"// end_of_simulation()"<<endl;
        *_pstrFile<<"//"<<endl;
        *_pstrFile<<"// void "<< _module_class_name<< "::end_of_simulation(){ /* Insert your  code here */    }" <<endl<<endl;

    }
    
    


    
}

