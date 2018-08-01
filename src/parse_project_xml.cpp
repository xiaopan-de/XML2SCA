/**
 * @file    parse_project_xml.cpp
 * @author  Xiao Pan <panxiao.tech@gmail.com>
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

#include "parse_project_xml.h"


namespace xml2sca
{
    bool parse_project_XML( const char* _pfilename, ams_project_data* _project )
    {
        if(_project == NULL)
        {
            log<<"[XML Parse Error]  project data not set. "<<endl;
            return false;
        }
        
        XMLDocument xmldoc;
        if(XML_SUCCESS != xmldoc.LoadFile(_pfilename))
        {
            log<<"[XML Parse Error] Fail to load xml fil. MSG : " <<xmldoc.ErrorStr();
            return false;
        }
        
        XMLElement* project_root = xmldoc.FirstChildElement("SCSCA_Project");
        if(project_root == NULL)
        {
            log<<"[XML Parse Error]: XML root element \"SCAMS_Project\" not found. "<<endl;
            return false;
        }
        
        //
        // add project infomration to the project data object
        //
        XMLElement* info_elm = project_root->FirstChildElement("info");
        if(info_elm == NULL)
        {
            log<<"[XML Parse Error]: XML element \"info\" not found. "<<endl;
            return false;
        }
        else if(!add_project_info( info_elm, _project))
        {
            log<<"[XML Parse Error]: Add project information failed.  "<<endl;
            return false;
        }
        
        
        //
        // add sc_tdf modules to the project data object
        //
        for (XMLElement* sca_module_elm = project_root->FirstChildElement("module");
             sca_module_elm!=NULL;
             sca_module_elm = sca_module_elm->NextSiblingElement("module"))
        {
            if(!add_module( sca_module_elm, _project))
            {
                log<<"[XML Parse Error]: Add module failed.  "<<endl;
                return false;
            }
        }
        
        //
        // add top level to the project data object
        //
        XMLElement* top_elm = project_root->FirstChildElement("toplevel");
        if(top_elm == NULL)
        {
            log<<"[XML Parse Info]: No top level information. "<<endl;
            return true;
        }
        else if (!add_toplevel(top_elm, _project))
        {
            log<<"[XML Parse Error]: failed to add top level. "<<endl;
            return false;
        }

        return true;
    }
    

    
    // -------------------------------------------------------------------------
    // add_project_info
    // -------------------------------------------------------------------------
    bool add_project_info(  XMLElement* _info_elm, ams_project_data* _project  )
    {
        
        if(_info_elm->Attribute("name"))
        {
            _project->info.name = remove_whitespace(_info_elm->Attribute("name"));
            if(!_project->info.name.compare(""))
            {
                log<<"[XML Parse Error]: Project name can not be empty. "<<endl;
                return false;
            }
        }
        else
        {
            log<<"[XML Parse Error]: Project name is not defined. "<<endl;
            return false;
        }
        
        if(_info_elm->Attribute("author"))
        {
            _project->info.author = _info_elm->Attribute("author");
        }
        else
        {
            log<<"[XML Parse Info]: Project author is not specified. "<<endl;
        }
        
        if(_info_elm->Attribute("desc"))
        {
            _project->info.description = _info_elm->Attribute("desc");
        }
        else
        {
            log<<"[XML Parse Info]: Project description information is not specified. "<<endl;
        }
        
        if(_info_elm->Attribute("date"))
        {
            _project->info.date= _info_elm->Attribute("date");
        }
        else
        {
            log<<"[XML Parse Info]: Project data is not specified. "<<endl;
        }
        
        if(_info_elm->Attribute("toplevelOnly"))
        {
            _project->info.toplevelOnly= remove_whitespace(  _info_elm->Attribute("toplevelOnly"));
            if(_project->info.toplevelOnly.compare("true") && _project->info.toplevelOnly.compare("false") )
            {
                log<<"[XML Parse Info]: Project attribute \"toplevelOnly\" is not set correctly ([true] or [false]). It is set to default value \"false\". "<<endl;
                _project->info.toplevelOnly = "false";
            }

        }
        else
        {
            log<<"[XML Parse Info]: Project attribute \"toplevelOnly\" is not specified ([true] or [false]). It is set to default value \"false\". "<<endl;
            _project->info.toplevelOnly = "false";
        }
        return true;
    }
    

    
    
    // -------------------------------------------------------------------------
    // add_module
    // -------------------------------------------------------------------------
    bool add_module(XMLElement* _module_elm, ams_project_data* _project ){
        sc_module module;
        if(_module_elm->Attribute("name"))
        {
            module.name = remove_whitespace(_module_elm->Attribute("name"));
            if(!module.name.compare(""))
            {
                log<<"[XML Parse Error]: Module name can not be empty. "<<endl;
                return false;
            }
        }
        else
        {
            log<<"[XML Parse Error]: Module name is not specified. ";
            return false;
        }
        
        if(_module_elm->Attribute("type"))
        {
            module.type = remove_whitespace(_module_elm->Attribute("type"));
            if(module.type.compare("sc") && module.type.compare("sca") )
            {
                log<<"[XML Parse Error]: module " <<module.name <<"'s type is not specified correctly, can only be set to [sc] or [sca]. "<<endl;
                return false;
            }
        }
        else
        {
            log<<"[XML Parse Error]: module" <<module.name <<"'s type is not specified. "<<endl;
            return false;
        }
        
        if(_module_elm->Attribute("moc"))
        {
            module.moc = remove_whitespace(_module_elm->Attribute("moc"));
            if(   module.moc.compare("tdf")
               && module.moc.compare("eln")
               && module.moc.compare("lsf")  )
            {
                log<<"[XML Parse Error]: module " <<module.name <<"'s moc is not specified correctlt, options [eln], [tdf], [lsf] "<<endl;
                return false;
            }

        }
        else
        {
            if(module.type == "sca")
            {
                log<<"[XML Parse Error]: module " <<module.name <<"'s moc is not specified, options [eln], [tdf], [lsf] "<<endl;
                return false;
            }
        }

        XMLElement* portin ;
        for(portin = _module_elm->FirstChildElement("port_in");
            portin!=NULL;
            portin = portin->NextSiblingElement("port_in"))
        {
            inoutport port;
            if(!get_port( portin, &port))
            {
                log<<"[XML Parse Error]: " <<module.name <<" module failed to add input port. "<<endl;
                return false;
            }
            else
            {
                module.vect_pin.push_back(port);
            }
        }
        
        XMLElement* portout ;
        for(portout = _module_elm->FirstChildElement("port_out");
            portout!=NULL;
            portout = portout->NextSiblingElement("port_out"))
        {
            inoutport port;
            if(!get_port(portout , &port))
            {
                log<<"[XML Parse Error]:  " <<module.name <<" module failed to add output port. "<<endl;
                return false;
            }
            else
            {
                module.vect_pout.push_back(port);
            }
        }

        XMLElement* cpara_elm ;
        for(cpara_elm = _module_elm->FirstChildElement("cpara");
            cpara_elm!=NULL;
            cpara_elm = cpara_elm->NextSiblingElement("cpara"))
        {
            param cpara;
            if(!get_para( cpara_elm, &cpara))
            {
                log<<"[XML Parse Error]:  " <<module.name <<" module failed to add c-parameter. "<<endl;
                return false;
            }
            else
            {
                module.vect_cpara.push_back(cpara);
            }
        }

        XMLElement* ufunc_elem ;
        for(ufunc_elem = _module_elm->FirstChildElement("ufunc");
            ufunc_elem!=NULL;
            ufunc_elem = ufunc_elem->NextSiblingElement("ufunc"))
        {
            func ufunc;
            if(!get_func(ufunc_elem, &ufunc))
            {
                log<<"[XML Parse Error]:  " <<module.name <<" module failed to add user defined function. "<<endl;
                return false;
            }
            else
            {
                module.vect_func.push_back(ufunc);
            }
        }

        
        //
        // instances
        //
        XMLElement* instance_elm ;
        for(instance_elm = _module_elm->FirstChildElement("instance");
            instance_elm!=NULL;
            instance_elm = instance_elm->NextSiblingElement("instance"))
        {
            instance inst;
            if(!get_instance(instance_elm,_project,  &inst))
            {
                log<<"[XML Parse Error]:  " <<module.name <<" module failed to add instance. "<<endl;
                return false;
            }
            else
            {
                module.vec_instance.push_back(inst);
            }
        }
        
        _project->vect_sc_module.push_back(module);
        return true;
    }

    // -------------------------------------------------------------------------
    // get_port
    // -------------------------------------------------------------------------
    bool get_port( XMLElement* _port_elm, inoutport * _port){
        if(_port_elm->Attribute("name"))
        {
            _port->name = remove_whitespace( _port_elm->Attribute("name"));
            if (!_port->name.compare(""))
            {
                log<<"[XML Parse Error]: port name can not be empty. "<<endl;
                return false;
            }
        }
        else
        {
            log<<"[XML Parse Error]: port name is not defined, . "<<endl;
            return false;
        }
        
        if(_port_elm->Attribute("ptype"))
        {
            _port->porttype = remove_whitespace( _port_elm->Attribute("ptype"));
            if (   _port->porttype.compare("sca_tdf::sca_in")
                && _port->porttype.compare("sca_tdf::sca_out")
                && _port->porttype.compare("sca_tdf::sc_in")
                && _port->porttype.compare("sca_tdf::sc_out")
                && _port->porttype.compare("sc_core::sc_in")
                && _port->porttype.compare("sc_core::sc_out"))
            {
                log<<"[XML Parse Error]: port name it not set correctly."<<endl;
                return false;
            }
        }
        else
        {
            log<<"[XML Parse Error]: port type is not defined. "<<endl;
            return false;
        }
        
        if(_port_elm->Attribute("dtype"))
        {
            _port->datatype = remove_whitespace(  _port_elm->Attribute("dtype"));
        }
        else
        {
            log<<"[XML Parse Error]: port data type is not defined. "<<endl;
            return false;
        }
        
        if(_port_elm->Attribute("desc"))
        {
            _port->description = _port_elm->Attribute("desc");
        }
        else
        {
            log<<"[XML Parse Info]: port description  is not defined. "<<endl;
        }
        return true;
    }
    
    
    
    // -------------------------------------------------------------------------
    // get_func
    // -------------------------------------------------------------------------
    bool get_func( XMLElement* _func_elm, func* _func)
    {
        if(_func_elm->Attribute("name"))
        {
            _func->name = remove_whitespace(  _func_elm->Attribute("name"));
            if (!_func->name.compare(""))
            {
                log<<"[XML Parse Error]: function name can not be empty. "<<endl;
                return false;
            }
        }
        else
        {
            log<<"[XML Parse Error]: Function name is not defined. "<<endl;
            return false;
        }
        
        if(_func_elm->Attribute("accessor"))
        {
            _func->accessor = remove_whitespace(  _func_elm->Attribute("accessor"));
            if(   _func->accessor.compare("public")
               && _func->accessor.compare("private")
               && _func->accessor.compare("protected"))
            {
                _func->accessor = "public";
                log<<"[XML Parse Info]: User function [" <<_func->name<<"] is not correctly specified,  set to default \"public \"."<<endl ;
            }
        }
        else
        {
            _func->accessor = "public";
            log<<"[XML Parse Info]: User function [" <<_func->name<<"] is set to public by default"<<endl ;
        }
        
        if(_func_elm->Attribute("desc"))
        {
            _func->description = _func_elm->Attribute("desc");
        }
        else
        {
            log<<"[XML Parse Info]: user defined function description  is not defined. "<<endl;
        }
        
    
        // retun type
        XMLElement* return_element ;
        return_element = _func_elm->FirstChildElement("return");
        if(return_element->Attribute("type"))
        {
            _func->returntype =   return_element->Attribute("type");
            if(_func->returntype == "")
            {
                _func->returntype = "void";
                log<<"[XML Parse Info]: user defined function retuen is set to default 'void'. "<<endl;
            }
        }
        else
        {
            _func->returntype = "void";
            log<<"[XML Parse Info]: user defined function retuen is set to default 'void'. "<<endl;
        }
        
        // retun description
        if(return_element->Attribute("desc"))
        {
            _func->retunrdesc = return_element->Attribute("desc");
        }
        else
        {
            log<<"[XML Parse Info]: user defined return description  is not defined. "<<endl;
        }
        
        XMLElement* fpara_elm ;
        for(fpara_elm = _func_elm->FirstChildElement("para");
            fpara_elm!=NULL;
            fpara_elm = fpara_elm->NextSiblingElement("para"))
        {
            param fpara;
            if(!get_para( fpara_elm, &fpara))
            {
                return false;
            }
            else
            {
                _func->vect_para.push_back(fpara);
            }
        }
        
        // retun type
        XMLElement* code_element ;
        code_element = _func_elm->FirstChildElement("code");
        if(code_element)
        {
            if(code_element->GetText() !=NULL)
            {
                _func->user_code=code_element->GetText() ;
            }
        }
       
        return true;
    }

    
    // -------------------------------------------------------------------------
    // get_para
    // -------------------------------------------------------------------------
    bool get_para( XMLElement* _para_elm, param* _para)
    {
        if(_para_elm->Attribute("name"))
        {
            _para->name = remove_whitespace( _para_elm->Attribute("name") );
            if (! _para->name.compare(""))
            {
                log<<"[XML Parse Error]: Parameter name can not be empty. "<<endl;
                return false;
            }
        }
        else
        {
            log<<"[XML Parse Error]: Parameter name is not defined. "<<endl;
            return false;
        }
        
        if(_para_elm->Attribute("dtype"))
        {
            _para->datatype = _para_elm->Attribute("dtype");
            if (! _para->datatype.compare(""))
            {
                log<<"[XML Parse Error]: Parameter data type can not be empty. "<<endl;
                return false;
            }
        }
        else
        {
            log<<"[XML Parse Info]: Parameter data type is not defined. "<<endl;
            return false;
        }
        
        if(_para_elm->Attribute("desc"))
        {
            _para->description = _para_elm->Attribute("desc");
        }
        else
        {
            log<<"[XML Parse Info]: Parameter description  is not defined. "<<endl;
        }
        
        if(_para_elm->Attribute("defval"))
        {
            _para->def_value = remove_whitespace( _para_elm->Attribute("defval"));
        }
        else
        {
            log<<"[XML Parse Info]: def_valuetype is not defined. "<<endl;
        }
        return true;

    }
    
    
    // -------------------------------------------------------------------------
    //! add_toplevel
    // -------------------------------------------------------------------------
    bool add_toplevel( XMLElement* _top_elm, ams_project_data* _project  )
    {
        top_level top;
        
        //
        // system configuration
        //
        XMLElement* sys_elm = _top_elm->FirstChildElement("sim_conf");
        if(_top_elm == NULL)
        {
            log<<"[XML Parse Info]: sim_conf not defined. "<<endl;
        }
        else
        {
            if(sys_elm->Attribute("tstep"))
            {
                top.tstep = remove_whitespace( sys_elm->Attribute("tstep"));
            }
            else
            {
                log<<"[XML Parse Info]: Simulation Timestep is not defined. "<<endl;
            }
            
            if(sys_elm->Attribute("tsim"))
            {
                top.tsim = remove_whitespace(  sys_elm->Attribute("tsim"));
            }
            else
            {
                log<<"[XML Parse Info]: Simulation Time is not defined. "<<endl;
            }
            
            if(sys_elm->Attribute("tracefile"))
            {
                top.tracefile = remove_whitespace( sys_elm->Attribute("tracefile") );
                if(top.tracefile.compare("vcd") && top.tracefile.compare("csv") )
                {
                    top.tracefile = "vcd";
                    log<<"[XML Parse Info]: Tracefile format is not correct. Set to default value \"vcd\". "<<endl;
                }
            }
            else
            {
                top.tracefile = "vcd";
                log<<"[XML Parse Info]: Tracefile format is not defined. Set to default value \"vcd\". "<<endl;
            }
        }
        
        //
        // instances
        //
        XMLElement* instance_elm ;
        for(instance_elm = _top_elm->FirstChildElement("instance");
            instance_elm!=NULL;
            instance_elm = instance_elm->NextSiblingElement("instance"))
        {
            instance inst;
            if(!get_instance( instance_elm,_project,  &inst))
            {
                return false;
            }
            else
            {
                top.vec_instance.push_back(inst);
            }
        }
        _project->top = new top_level(top);
        return true;
    }
    
   
    // -------------------------------------------------------------------------
    // get_instance
    // -------------------------------------------------------------------------
    bool get_instance( XMLElement* _instance_elm,  ams_project_data* _project , instance *_inst)
    {
        sc_module inst_module;
        if(_instance_elm->Attribute("name"))
        {
            _inst->name = remove_whitespace(  _instance_elm->Attribute("name"));
            if(_inst->name =="")
            {
                log<<"[XML Parse Error]: Instance name can not be empty. "<<endl;
                return  false;
            }
        }
        else{
            log<<"[XML Parse Error]: Instance name is not defined. "<<endl;
            return  false;
        }
        
        if(_instance_elm->Attribute("accessor"))
        {
            _inst->accessor = remove_whitespace(  _instance_elm->Attribute("accessor"));
            if(   _inst->accessor.compare("public")
               && _inst->accessor.compare("private")
               && _inst->accessor.compare("protected"))
            {
                _inst->accessor = "public";
                log<<"[XML Parse Info]: Instance accessor is empty, set to default \"public\". "<<endl;
            }
        }
        else{
            log<<"[XML Parse Info]: Instance accessor is not defined, set to default \"public\". "<<endl;
        }
        
        if(_instance_elm->Attribute("module"))
        {
            _inst->module = remove_whitespace( _instance_elm->Attribute("module"));
            if(_inst->module =="")
            {
                log<<"[XML Parse Error]: Instance module can not be empty. "<<endl;
                return  false;
            }
            for (int i= 0, end =_project->vect_sc_module.size(); i<end; i++ )
            {
                if(_inst->module  == _project->vect_sc_module[i].name)
                {
                    inst_module = _project->vect_sc_module[i];
                    _inst->pair_module = std::make_pair(_inst->module,inst_module);
                    break;
                }
                if(i == end-1)
                {
                    log<<"[XML Parse Error]: Instance module ["<<_inst->module <<"] can not be found, please check instance->module name.. " <<endl;
                    return  false;
                }
            }
        }
        else{
            log<<"[XML Parse Error]: Instance module is not defined. " <<endl;
            return  false;
        }
        
        XMLElement* inst_param_elm ;
        for(inst_param_elm = _instance_elm->FirstChildElement("para");
            inst_param_elm!=NULL;
            inst_param_elm = inst_param_elm->NextSiblingElement("para"))
        {
            instance::inst_param inst_param;
            if(inst_param_elm->Attribute("name")){
                inst_param.name = remove_whitespace(  inst_param_elm->Attribute("name"));
                if(inst_param.name =="")
                {
                    log<<"[XML Parse Error]: Instance parameter name can not be empty. "<<endl;
                    return  false;
                }
            }
            else{
                log<<"[XML Parse Error]: Instance parameter name is not defined. "<<endl;
                return  false;
            }
            if(inst_param_elm->Attribute("val"))
            {
                inst_param.value = inst_param_elm->Attribute("val");
                if(inst_param.value =="")
                {
                    log<<"[XML Parse Info]: Instance parameter value is empty. "<<endl;
                }
            }
            else
            {
                log<<"[XML Parse Info]: Instance parameter value is not defined. "<<endl;
            }
            
            for (int i= 0, end =inst_module.vect_cpara.size(); i<end; i++ )
            {
                if( inst_param.name  == inst_module.vect_cpara[i].name)
                {
                    _inst->map_param.insert(std::pair<param, string>(inst_module.vect_cpara[i], inst_param.value));
                    break;
                }
                if(i == end-1)
                {
                    log<<"[XML Parse Error]: Instance module parameter ["<<inst_param.name <<"] can not be found. " <<endl;
                    return  false;
                }
            }
        }
        
        return true;
    }
    
    // -------------------------------------------------------------------------
    // remove_whitespace
    // -------------------------------------------------------------------------
    string remove_whitespace(string _str)
    {
        _str.erase(remove_if(_str.begin(), _str.end(), ::isspace), _str.end());
        return _str;
    }

    
    
    // -------------------------------------------------------------------------
    // dbg_print
    // -------------------------------------------------------------------------
    void dbg_print(ams_project_data* _project)
    {
        log<<"[parse xml] Debug print XML Parse result:  "<<endl;
        log<<"[parse xml] proj_name    = ["<<_project->info.name<<"]"<<endl;
        log<<"[parse xml] proj_author  = "<<_project->info.author<<endl;
        log<<"[parse xml] proj_desc    = "<<_project->info.description<<endl;
        log<<"[parse xml] proj_date    = "<<_project->info.date<<endl;
        log<<"[parse xml] proj_toponly    = "<<_project->info.toplevelOnly<<endl;

        log<<std::left << std::setw(80)<<std::setfill('-')<<" "<<endl;
        for (auto it = _project->vect_sc_module.begin(), end =_project->vect_sc_module.end(); it !=end ; it++)
        {
            log<<"[parse xml]\t - Module:   ["<< it->name <<"], type=["<<it->type<<"], moc= ["<<it->moc<<"]" <<endl;
            for (auto it1 = it->vect_pin.begin(), end1 =it->vect_pin.end(); it1 !=end1 ; it1++)
            {
                log<<"[parse xml]\t\t  Pin   = ["<< it1->name << "], porttype="<<it1->porttype<<  ", datatype="<<it1->datatype<<", description:"<<it1->description <<endl;
            }
            for (auto it1 = it->vect_pout.begin(), end1 =it->vect_pout.end(); it1 !=end1 ; it1++)
            {
                log<<"[parse xml]\t\t  Pout  = ["<< it1->name << "], porttype="<<it1->porttype<<  ", datatype="<<it1->datatype<<", description:"<<it1->description <<endl;
            }
            for (auto it1 = it->vect_cpara.begin(), end1 =it->vect_cpara.end(); it1 !=end1 ; it1++)
            {
                log<<"[parse xml]\t\t  cpara = ["<< it1->name<< "], datatype="<<it1->datatype<<  ", def_value="<<it1->def_value<<", description:"<<it1->description <<endl;
            }
            for (auto it1 = it->vect_func.begin(), end1 =it->vect_func.end(); it1 !=end1 ; it1++)
            {
                log<<"[parse xml]\t\t  func  = ["<< it1->name << "], accessor="<<it1->accessor<<  ", description:"<<it1->description << endl;
                log<<"[parse xml]\t\t\t  return = ["<< it1->returntype <<  "], retunr desc:"<<it1->retunrdesc  <<endl;
                for (auto it2 = it1->vect_para.begin(), end2 =it1->vect_para.end(); it2 !=end2 ; it2++)
                {
                    log<<"[parse xml]\t\t\t  para   = ["<< it2->name<< "], datatype="<<it2->datatype<<  ", def_value="<<it2->def_value<<", description:"<<it2->description <<endl;
                }
                log<<"[parse xml]\t\t\t  user code = "<< it1->user_code  <<endl;

            }
            for (auto it1 = it->vec_instance.begin(), end1 =it->vec_instance.end(); it1 !=end1 ; it1++)
            {
                log<<"[parse xml]\t\t - Instance: "<<endl;
                log<<"[parse xml]\t\t   name = ["<< it1->name <<"], module = ["<< it1->pair_module.first <<", "<< it1->pair_module.second.name << "]"<<endl;
                for (auto it2 = it1->map_param.begin(), end2 =it1->map_param.end(); it2 !=end2 ; it2++)
                {
                    log<<"[parse xml]\t\t\t  Para   = ["<< it2->first.name << ", "<<it2->second<<"]" <<endl;
                }
            }
        }
        
        log<<std::left << std::setw(80)<<std::setfill('-')<<" "<<endl;

        if( _project->top)
        {
            log<<"[parse xml]\t - TOP: "<<endl;
            for (auto it = _project->top->vec_instance.begin(), end =_project->top->vec_instance.end(); it !=end ; it++)
            {
                log<<"[parse xml]\t\t - Instance: "<<endl;
                log<<"[parse xml]\t\t   name = "<< it->name <<", module = ["<< it->pair_module.first <<", "<< it->pair_module.second.name << "]"<<endl;
                for (auto it1 = it->map_param.begin(), end1 =it->map_param.end(); it1 !=end1 ; it1++)
                {
                    log<<"[parse xml]\t\t\t  Para   = ["<< it1->first.name << ", "<<it1->second<<"]" <<endl;
                }
            }
            log<<std::left << std::setw(80)<<std::setfill('-')<<" "<<endl;
            log<<"[parse xml]\t - System Settings:  tstep = "<< _project->top->tstep <<", tsim = "<< _project->top->tsim <<", tracefile="<< _project->top->tracefile <<endl;
        }
      
    }

}

