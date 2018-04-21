 # XML to SystemC/SystemC-AMS (XML2SCA) #

This tool enables auto-generation of SystemC and SystemC-AMS codes through a XML file. The generated project includes: SC/SCA module codes(.cpp, .h), CMake files, a toplevel module (if a toplevel structure is specified), and it also  creates executable testbench for each individual module.

## Motivation ##
It was intented to aid SystemC-AMS beginners to start up modeling and simulation quickly by avoiding manually writing testbenches and building utilities such as makefile or cmakes. This tool does not take IP-XACT (IEEE 1685-2014) as an input, because it puts extra effort to the beginners. Moreover IP-XACT consists too mamy details from a system-level perspective, most of which are often not used by non-vendor users. Instead, input of this tool has simple syntax that concentrates more on the development of module's behaviour, such as input/output ports, module parameters, etc. Beside this, XML based modeling management also has other benefits such as:

- Consistent code style in projects, workgroups
- Intuitive abstruct information of modules
- Easier in module-libary management, maintenance and documentation
- Easier in module exchange, reuse, and extension
- Reduce effort in setting up projects
- Reduce errort in setting up massive configuration parameters
- Extensiable for UI design

## Features and Limitations ##
- Simple XML rules to define SC/SAC modules
- Auto-generation of CMake files
- Auto-generation of executable testbench for each individual module
- Auto-generation of JSON configuration files and automatically parse to instances
- Support SysemC hierarchical structure
- No support for TLM
- No support for toplevel interconnections
- No support for templete classß


## Installation ##
0. We assume you have a UNIX style system, e.g. OSX or Linux.
1. GCC version 4.9 or higher and CMake version 2.8 or higher are required.
2. Make a directory `builddir` in the top level directory:
```
> mkdir builddir
```
4. Go to `builddir` and build it:
```
> cd builddir
> cmake ..
> make
```

## Usage ##
Target is installed in the folder `XML2SCA/bin/XML2SCA`. Excute with parameter option *-f* to pass project xml file to the program or option  *-h* to print help information.
```
Usage:
  ./XML2SCA [OPTION...]

  -f, --file  Project XML file (default: false)
  -h, --help  Print help (default: false)
```

## Project XML file definition and Rules##
The project XML starts with root element `<SCSCA_Project>`.


### Project Information  ###
Project information is passing by the attributes in the first child element `<info>`. This element can appear multi-times but only the first appearance will be parsed. Five attributes are defined:
```xml
<info
  name   = "project_name [no whitespace allowed]"
  author = "project author"
  date   = "project created date"
  desc   = "project description"
  toplevelOnly = "true or false"
>
</info>
```


Attribute **_toplevelOnly_** specifies the generation output of the project. If it is set to **_false_**, program will generate and overwrite (if they exit) all modules' components incl. C++ files, testbenches. So to prevent your code from being overwritten, always make a copy of the generated project folder to your own workspace. Set **_toplevelOnly_** to **_true_** if you have complet each individual model development and want to make a toplevel binding.

### Module  ###
Module is specified by child element `<module>`. Basic information of the module such as name, type are given through its attributes.
```xml
<module
  name  = "module_name [no whitespace allowed]"
  type  = "sca [sca for SystemC-AMS, sc for SystemC]"
  moc   = "tdf [eln, lsf]"
  desc  = "module description"
  >
</module>
```
A module consists of input ports`<port_in>`, output ports `<port_out>`, parameters `<cpara>` (pass through constructor), and user defined  functions `<ufunc>`. They are specified by the child element of `<module>`:
```xml
<module name="module_name" type="sca"  moc="tdf"  desc="module description">
  <port_in
    name  = "input_port_name [no whitespace allowed]"
    ptype = "sca_tdf::sca_in [options:sca_tdf::sca_in/sc_in, sc_core::sc_in]"
    dtype = "double [data types support by C++]"
    desc  = "port description"/>

  <port_out
    name  = "input_port_name [no whitespace allowed]"
    ptype = "sca_tdf::sca_out [options: sca_tdf::sca_out/sc_out  sc_core::sc_out]"
    dtype = "double [data types support by C++]"
    desc  = "port description"/>

  <cpara
    name  = "parameter_name [no whitespace allowed]"
    dtype = "double [data types support by C++]"
    defval= "default value [can be empty but with no whitespace]"
    desc  = "description"/>

  <ufunc
    name     = "function_name, [no whitespace allowed]"
    accessor = "public [private, protected]"
    desc     = "description">
        <return
            type = "return value type, empty for ‘void‘"
            desc="description" />
         <!-- Para is the same to cpara, no This element for "void" parameter-->
         <para
            name="para_name"
            dtype="int"
            defval=""
            desc= "Updated ADC resolution in bits" />
            <code >
            <![CDATA[ "Predefined code" ]]>
            </code>
  </ufunc>
</module>
```


### Top Level ###
The toplevel module is enclosed in the child element `<toplevel>`. Toplevel is an optional element in the project. But only the first appearance in the project xml file will be parsed. It has no attribute. And contains the following subelement:
```xml
<toplevel>
  <!-- simulation configuration -->
  <sim_conf
      tstep ="1e-9 [ime step in second]"
      tsim="2e-4   [simulation time insecond]"
      tracefile="trace file formation 'vcd' or 'tablular' "/>
  <!-- instance of modules, can appear multi times -->
  <instance
      name="instance_name [no whitespace allowed]"
      module="adc [selection of module from module elements]">
      <para
          name= "[para_name] [must matches the parameter name of the selected module]"
          val = "1.0 [value pass to the parameter]"
          />
  </instance>
  <interconnect />   <!-- not implemented in this version -->
</toplevel>
```

## Example ##
### Generate an ADC Project ###
1. It is always easily to show usage through an example. consider the following xml file (example_adc.xml) which specifies an SystemC-AMS TDF module ADC with no toplevel:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<SCSCA_Project>
 	<info name="adc_autogen" author = "xiao pan" date="2018-04-05" toplevelOnly="false" desc= "Test project of [XML2AMS]"/>
	<module name="adc" 	type="sca" 		moc="tdf" 			desc="adc">
		<port_in  name="pos"  ptype="sca_tdf::sca_in" dtype ="double" desc="power supply voltage positive input" />
		<port_in  name="neg"  ptype="sca_tdf::sca_in" dtype ="double" desc="power supply voltage negative input" />
		<port_in  name="freq" ptype="sca_tdf::sca_in" dtype ="double" desc="sampling frequency" />
		<port_out name="conv" ptype="sca_tdf::sc_out" dtype ="unsigned int" desc="" />
		<cpara    name="vref" dtype="double" defval="1.0" desc= "ADC reference voltage" />
        <cpara    name="res" dtype="unsigned int" defval="12" desc= "ADC resolution, default is set to 12 [bits]" />
        <cpara    name="rate" dtype="int" defval="1" desc= "Output port p_rate, default is set to 1" />
        <ufunc    name="max_value" accessor="private" desc="return maximum converted value of ADC ">
        	<return   type = "long signed int" desc="" />
            <para name="value" dtype="long signed int" defval="" desc= "" />
            <code >
              <![CDATA[ // user defined code
                //if(abs(value) > max-1)
                //    value = value>0 ? max-1 : -max; // max is private variable of adc
                //return value;
                ]]>
            </code>
        </ufunc>
	</module>
</SCSCA_Project>
```

2. You can excute `XML2SCA` from any location to generate project with the following command:
```sh
./PathToProgram/XML2SCA -f /PathToProjectXMLFile/example_adc.xml
```

3. If no error occurs, the project is successfuly created and a folder with project name will appear in the same location :
```
adc_autogen
├── cmake-files             # CMake files handles the arguments to find SystemC and SystemC-AMS libraries
│   └── ...
├── CMakeLists.txt          # Top CMake configuration file
├── configs                 # Configuration files to the simulation
│   └── cfg.json            # Toplevel configration file (json)
├── src                     # Source files
│   ├── basic               # Utilitie files such as json parser
│   ├── CMakeLists          # CMake configration
│   ├── main.cpp
│   ├── sca_tdf_adc.cpp     # module implementation file (cpp file)
│   └── sca_tdf_adc.h       # module declaration file (header file)
└── tb                      # testbench for each module
    └── adc
        ├── main.cpp
        ├── sc_top.h        # toplevel component
        ├── cfg.json        # parameter configuration for testbench toplevel
        └── CMakeLists.txt
```

### Top level structure of the testbench  ###
As long as the project is automatically generted, a testbench for each individual module is created. Open toplevel module (`tb/adc/sc_top.h`), you will find a stimuli module (`sca_tdf_stimuli`) is automatically cretaed and binded to the adc modules.

![top](adc_tb.png)

Sincne the generated testbench is already executable, we try to build and simulate without any modifications:
1. Set an environment variable for SysremC and SystemC-AMS
```shell
export SYSTEMC_BASE = PathToInstallationSystemC/systemc-2.3.0
export SYSTEMCAMS_BASE = PathToInstallationSystemCAMS/systemc_ams
```

2. Build the auto-generateed ADC project :
```
> cd adc_autogen
> mkdir build
> cd build
> cmake ..
> make
```

3. Check the json configration file `/adc_autogen/tb/adc/cfg.json` if there is any empty filds. Change the simulation time smaller to 10 ns (10e-9):
```json
{
	"system":{
		"tstep"        : 1e-9,
		"tsim"         : 10e-9,
		"tracefile"    : "vcd",
		"_comment"     : "unit of time is second, trace file formate can be [vcd] or [tabular] "
	},
	"i_adc":{
		"vref"         : 1.0,
		"res"          : 12,
		"rate"         : 1
	}
}
```

4. Simulate the adc testbench:
```
> cd ../bin
> ./adc_tb -c ../tb/adc/cfg.json
```

5. The following results shows our example works!
```
Info: Simulation options:
         Simulation time:     1e-08
         Simulation timestep: 1e-09
Info: Simulation start.
...
readout adc-pout[conv] = 0
readout adc-pout[conv] = 0
...
Info: simulation of 1e-08 SEC complete. (took 0.000403 sec )
```
Since no behaviours has been implemented to adc module, the stimuli module prints out 0 from the readin of input port.  Let's have a look at the implementation of stimuli behavior:
```Cpp
void processing()
{
    //output port
    pout_pos.write(0);
    pout_neg.write(0);
    pout_freq.write(0);
    //input port
    std::cout<< " readout adc-pout[conv] = " << pin_conv.read()<<std::endl;
}
```

### Implement behavior of module ADC  ###
An analog-to-digital converter (ADC) converts continuous signals to N bit discrete digital numbers. Its behaviour is implemented in the module file `sca_tdf_adc.cpp`. Add following code into sac-methods `initialize()` and `processing()`, as well as user defined function `max_value()`:
```Cpp
void  sca_tdf_adc::initialize()
{
    // user defined code section
    max = (long)(pow(2.0, p_res ));
    lsb = p_vref / (max);
    // end of user defined code section
}

void sca_tdf_adc::processing()
{
    // user defined code section
    analog = pin_pos.read() - pin_neg.read();
    erg = lround(analog / lsb);
    erg = max_value (erg);
    bv_erg = erg;
    pout_conv.write(bv_erg);
    // end of user defined code section
}

long signed int sca_tdf_adc::max_value(long signed int _value)
{
    // user defined code section
    if(std::fabs(_value) > adc_max-1)
        _value = _value>0 ? adc_max-1 : -adc_max;
    return _value;
    // end of user defined code section
}
```
Add following code to the header file which enables using functions proviede by `cmath`, and add definitions of class(module) variables.

``` cpp
#include <cmath>

SCA_TDF_MODULE(sca_tdf_adc) {
  ...
  // user defined code section
   long adc_max;
   double adc_lsb;
   double analog;
   long erg;
   long bv_erg;
  // end of user defined code section
   ...}
```

### Add stimuli to generate a sine signal  ###
Now we add a sine wave generater function to the stimuli to test our ADC module.
``` cpp
SCA_TDF_MODULE(sca_tdf_stimuli)
{
  ...
  double sample_time;
  double  actval;
  double  stepsize;

  void initialize(){
    actval = 0;
    sample_time = pout_pos.get_timestep().to_seconds();
  }

  void processing()
  {
    sample_time = pout_pos.get_timestep().to_seconds();
    stepsize = sample_time*1e6*2.*M_PI;
    actval+=stepsize;
    //output port
    pout_pos.write(sin(actval));
    ...
    //input port
    std::cout<< " readout adc-pout[conv] = " << pin_conv.read()<<std::endl;
	}
};
```

### Tracefile analysis ###


## Contact ##
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
