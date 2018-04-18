# Section 1: XML to SystemC/SystemC-AMS (XML2SCA) 

This tool enables automatic code-generation of SystemC and SystemC-AMS project through a XML file. The generated project consists of SC/SCA models (cpp and h files), CMakes, a toplevel module (option) and executable testbench for each individual model.

## 1.1 Motivation 
It was intended  to aid SystemC-AMS beginners to start up modelling and simulation quickly by avoiding manually writing testbenches and compiling utilities such as Makefile or Cmakes. This tool doesn’t choose IP-XACT (IEEE 1685-2014) as an input format, although this standard is comprehensive to define modules/systems at electronic system level and has been widely used in the industrial. From a system-level perspective, it consists too many details and not suitable for system architecture exploration in the early design phase. On the other side, learning new standard puts extra effort to the beginners. This tool, instead, defines simple syntax of input XML file that concentrates more on the development of module's behaviour, such as input/output ports, module parameters, etc. Beside above, XML based modelling management also has other benefits such as:
- Consistent code style in projects, workgroups
- Abstract information of modules
- Easier module-library  management, maintenance and documentation
- Easier module exchange, reuse, and extension
- Reduce effort in project management
- Reduce effort in setting up large number of configuration parameters
- Extensible for UI design

## 1.2 Features and Limitations 
+ Simple XML rules to define SC/SCA modules
+ Auto-generation of CMake files
+ Auto-generation of executable testbench for each individual module
+ Auto-generation of JSON configuration files and automatically parse to instances
+ Support SysemC hierarchical structure
- *No support for TLM*
- *No support for toplevel interconnections*
- *No support for template  class*



## 1.3 Installation 
0. This tool is available at github : Go to [Download](https://github.com/panxiao-tech/XML2SCA)
1. We assume you have a UNIX style system, e.g. OSX or Linux.
2. GCC version 4.9, CMake version 2.8, or higher are required.
3. Make a directory `builddir` in the top-level directory:
```
> mkdir builddir
```
4. Go to `builddir` and build it:
```
> cd builddir
> cmake ..
> make
```

## 1.4 Usage 
XML2SCA program is installed in the folder `XML2SCA/bin/` by default. Execute with argument **-f/--file** to pass project xml file or   **-h/--help** to print help information:
```
Usage:
 ./XML2SCA [OPTION...]

 -f, --file  Project XML file (default: false)
 -h, --help  Print help (default: false)
```

# Section 2: Usage of Generated SC/SCA project 

Executed XML2SCA by passing a validate XML file, a folder with the specified project name can been found at the same location where the program is called. 

##  2.1 Project folder structure 
The generated SC/SCA Project folder structure is showing below:
```
PROJECT_NAME
├── cmake-files             # CMake files folder
│   └── ...                 # CMake files handles the arguments to find SystemC and SystemC-AMS libraries
├── CMakeLists.txt          # Top CMake configuration file
|
├── configs                 # Configuration files folder
│   └── cfg.json            # Toplevel simulation configuration  file (json)
|
├── src                             # Source files folder
│   ├── basic                       # Files for utilitie functions incl. json and file parser
│   ├── CMakeLists                  # CMake configuration
│   ├── main.cpp                    # Main function file
│   ├── sca_tdf_MODULE1_NAME.cpp    # module1 implementation file (cpp file)
│   ├── sca_tdf_MODULE1_NAME.h      # module1 declaration file    (header file)
│   ├── sca_tdf_MODULE2_NAME.cpp    # module2 implementation file (cpp file)
│   ├── sca_tdf_MODULE2_NAME.h      # module2 declaration file    (header file)
│   └── ...
│
└── tb                              # testbench folder for all modules
   ├── MODULE1_NAME                # testbench folder for module1
   │   ├── main.cpp                # Main function file
   │   ├── sc_top.h                # toplevel component
   │   ├── cfg.json                # parameter configuration for testbench toplevel
   │   └── CMakeLists.txt
   ├── MODULE2_NAME                # testbench folder for module2
   ...
```

## 2.2  Build project  
1. Set an environment variables for SysremC and SystemC-AMS
```shell
export SYSTEMC_BASE = PathToInstallationSystemC/systemc-2.3.0
export SYSTEMCAMS_BASE = PathToInstallationSystemCAMS/systemc_ams
```

2. Build the auto-generated  SC/SCA project :
```
> cd PROJECT_NAME
> mkdir builddir
> cd builddir
> cmake ..
> make
```

## 2.3 Run simulations  ###
1. Set up system configurations and parameters in the json configuration  file `/PROJECT_NAME/tb/MODULE_NAME/cfg.json`, e.g. simulation timestep, simulation duration, tracefile format, etc:
```json
{
 "system":{
     "tstep"        : 1e-9,
     "tsim"         : 10e-9,
     "tracefile"    : "vcd",
     "_comment"     : "unit of time is second, trace file format  can be [vcd] or [CVS] "
 },
 "i_MODULE_NAME":{
     "PARAMETER1"   : "VALUE1",
     "PARAMETER2"   : "VALUE2",
     "..."          : "..."
 }
}
```

2.  Implement behaviour  of module (option).
This is an option step to execute  a simulation, since the auto-generated testbench for module is executable. Although, the behaviour  of the generated module is empty, and stimuli simply writes 0 to the outputs and prints input ports' read-in.

3. Execute Simulation:
```
> cd PROJECT_NAME/bin
> ./MODULE_NAME_tb -c ../tb/MODULE_NAME/cfg.json
```

5. The following results shows the simulation works.
```
Info: Simulation options:
        Simulation time:     1e-02
        Simulation timestep: 1e-09
Info: Simulation start.
...
readout MODULE_NAME-pout[conv] = 0
...
Info: simulation of 1e-08 SEC complete. (took 0.000914 sec )
```

## 2.4 Tracefile analysis 
A tracefile (VCD or CSV format specified in the XML file) is generated in the folder `PROJECT_NAME/bin` where you simulate the testbench. Traces of all in/output ports are saved. A list of waveform viewer can be found [here](https://ipfs.io/ipfs/QmXoypizjW3WknFiJnKLwHCnL72vedxjQkDDP1mXWo6uco/wiki/Waveform_viewer.html) .





# Section 3 : SCSCA Project XML file definition and rules 
The project XML specifies the SC/SCA module and project information to the tool XML2SCA. It starts with root element `<SCSCA_Project>`. Following figure shows the structure of a complete project xml file. Detailed explanation can be found below.  *More examples can be found in the `exmaple` folder.*  

<p><img src="https://4ibutw.dm.files.1drv.com/y4m-ck3Dr9Zyt-lRpmwrbZD6oG6I2rcBFE3N16oAiqRkKxHQSvW62vZamFXqb619aPQfoBTYTePDc0tdcKQJV5bJSWC02QMa9CyqgSNILh_15DIS9HJ5NC7FDCna7KGgFTKiTgZbtDtaABtGBB-UzvVGEQ_aLKTQ9udM7hHQJzAlWByvKasPuD4yDGW7TXIGCbViyVHLmHqMI63_XIuAllqRQ?width=1225&height=1345&cropmode=none"  width ="50%" alt="SCSCA Project XML definition and structure"/>
</p> 

## 3.1 Project Information  `<info>` 
Project information is passing by the attributes in the first child element `<info>`. This element can appear multi-times but only the first appearance will be parsed. Five attributes are defined:
```xml
<info name = "name"  author = "author"  date = "date"  desc = "description"  toplevelOnly = "true" />
```

| Attributes    | Description   | Restrictions            |
| ------------- |-------------| -----------------------|
| name          | project name  | all whitespace will be removed  |
| author        | project author| -                       |
| date          | created date  | -                       |
| desc          | description   | -                       |
| toplevelOnly  | See below     |  **true** or **false**  |


Attribute **_toplevelOnly_** specifies the generation output of the project. If it is set to **_false_**, program will generate and overwrite (if they exit) all modules' components incl. C++ files, testbenches. So to prevent your code from being overwritten, always make a copy of the generated project folder to your own workspace. Set **_toplevelOnly_** to **_true_** if you have complete each individual model development and goes to make a toplevel binding.



## 3.2 SC/SCA Module  `<module>` 
Following lists a module example of an half-adder module.  A module consists of **input ports**`<port_in>`, **output ports** `<port_out>`, **parameters** `<cpara>` (pass through constructor), and user defined  **functions** `<ufunc>`. They are specified by the child element of `<module>`. In the next sections, detailed explanation  will be given. 

```xml
<module name = "adder"  type = "sca"  moc = "tdf"  desc = "addition of two inputs">
 <port_in   name = "A"  ptype = "sca_tdf::sca_in"   dtype = "bool"  desc = "input  A"/>
 <port_in   name = "B"  ptype = "sca_tdf::sca_in"   dtype = "bool"  desc = "input  B"/>
 <port_out  name = "S"  ptype = "sca_tdf::sca_out"  dtype = "bool"  desc = "output sum S"/>
 <port_out  name = "C"  ptype = "sca_tdf::sca_out"  dtype = "bool"  desc = "output carry C"/>
 <cpara name = "delay"  dtype = "double"  defval = ""   desc = "delay from input to the output, in [ns]"/>
 <cpara name = "rate"   dtype = "double"  defval = "1"  desc = "output port rate"/>
 <ufunc  name = "reset"  accessor = "public"  desc = "reset from software call">
       <return type = "bool" desc="return ture if reset successes" />
        <code > <![CDATA[ temp_c = temp_s = 0; ]]></code>
 </ufunc>
</module>
```

Module is specified by child element `<module>`. Basic information of the module such as name, type are given through its attributes.
```xml
<module name = "name"  type = "sca"  moc = "tdf"  desc = "description"></module>
```
| Attributes    | Description           | Restrictions          |
| ------------- | -------------------- | --------------------- |
| name          | module name           | all whitespace will be removed  |
| type          | module type           | **sc** (systemc) or **sca**  (systemc-ams)     |
| moc           | model of computation  | **tdf**,  **eln**, or **lsf**   |
| desc          | description           |  -                 |



### 3.2.1  Input ports : `<port_in>`   
```xml
<port_in  name = "name"  ptype = "sca_tdf::sca_in"  dtype = "double"  desc = " description"/>
```
| Attributes    | Description | Restrictions          |
| ------------- |------------ | ---------------------|
| name          | port name   | all whitespace will be removed  |
| ptype         | port type   | **sca_tdf::sca_in**/**sc_in**, **sc_core::sc_in**    |
| dtype         | data type   | all data types support by C++  |
| desc          | description |  -                 |


### 3.2.2  Output ports : `<port_out>`
```xml
<port_out  name = "name"  ptype = "sca_tdf::sc_out"  dtype = "double"  desc = " description"/>
```
| Attributes    | Description | Restrictions          |
| ------------- | ----------- | --------------------- |
| name          | port name   | all whitespace will be removed  |
| ptype         | port type   | **sca_tdf::sca_out**/**sc_in**, **sc_core::sc_in**    |
| dtype         | data type   | all data types support by C++  |
| desc          | description |  -                 |

### 3.2.3   Module parameters :  `<cpara>` 
```xml
<cpara name = "name"  dtype = "double"  defval = "default value"  desc = "description"/>
```
| Attributes    | Description     | Restrictions          |   
| ------------  |---------------  | ---------------------|   
| name          | parameter name  | all whitespace will be removed  |   
| dtype         | data type       | all data types support by C++  |   
| defval        | default value   | refers to data type, no whitespace, can be empty   |   
| desc          | description     |  -                    |


### 3.2.4  User defeined functions :    `<ufunc>` 
```xml
<ufunc  name = "name"  accessor = "public"  desc = "description">
     <return type = "void" desc="description" />
      <para name="para_name" dtype="int" defval="" desc= "description" />
      <code > <![CDATA[ "Predefined code" ]]></code>
</ufunc>
```
| Elements and Attributes    | Description     | Restrictions          |   
| ------------------ | --------------- | ---------------------|   
| name              | function name   | all whitespace will be removed  |   
| accessor          | accessor tyoe   | **public**(default), **private**, **protected**  |   
| desc              | description     |  -                    |   
| &lt;return>-type    | return value type  |whitespace will NOT be removed,  empty for ‘void‘   |   
| &lt;return>-desc    | return value description  | -   |   
| &lt;para>-name      | parameter name  | whitespace will be removed   |   
| &lt;para>-dtype     | data type       | all data types support by C++  |   
| &lt;para>-defval    | default value   | refers to data type, no whitespace, can be empty   |   
| &lt;para>->desc      | description     |  -                    |   
| &lt;code>            | pre-defeined code     |  -   |   




## 3.3 Toplevel `<toplevel>`  
The toplevel module is enclosed in the child element `<toplevel>`. Toplevel is an optional element in the project. But only the first appearance in the project xml file will be parsed. It has no attribute, and contains  sub-element of `sim_conf`, `instance`, `interconnect`:
```xml
<toplevel>
 <!-- simulation configuration -->
 <sim_conf tstep ="1e-9"   tsim="2e-4 " tracefile="vcd"/>
 <!-- instance of modules, can appear multi times -->
 <instance name="name" module="adder">
     <para name= "delay" val = "1e-12"/>
     <para name= "rate" val = "1"/>
 </instance>
 <interconnect />
</toplevel>
```

| Elements and Attributes    | Description     | Restrictions          |   
| ------------     |--------------- | ---------------------|   
| &lt;sim_conf>- tstep  | time step in second   | whitespace will be removed  |   
| &lt;sim_conf>- tsim  | simulation time insecond   | whitespace will be removed  |   
| &lt;sim_conf>- tracefile  | trace file formation   | **vcd** or **csv**(tablular) |   
| &lt;instance>- name     | instance name   | whitespace will be removed  |   
| &lt;instance>- module  | referred  module name  | refers to the module elements |   
| &lt;instance>- &lt;para>-name  | para name  | must be referred  to the parameter name of the selected module|   
| &lt;instance>- &lt;para>-val  | value pass to the parameter  | refers to data type, no whitespace, can be empty |   
| &lt;interconnect>  | NOT IMPLEMENTED YET  | - |   



<hr/>

#### Contact 
**Xiao Pan**   
Chair of Design of Cyber-Physical Systems   
TU Kaiserslautern   
Postfach 3049   
67653 Kaiserslautern   
<pan@cs.uni-kl.de>   
https://cps.cs.uni-kl.de/   


**Christoph Grimm**   
Chair of Design of Cyber-Physical Systems   
TU Kaiserslautern   
Postfach 3049   
67653 Kaiserslautern   
<grimm@cs.uni-kl.de>   
