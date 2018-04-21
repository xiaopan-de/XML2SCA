cmake_minimum_required (VERSION 2.8)
project (sensor_adc_example)  

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-files)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)  
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++0x -DSC_INCLUDE_DYNAMIC_PROCESSES") 

find_package(SystemC REQUIRED)
find_package(SystemCAMS REQUIRED)

include_directories(${SYSTEMC_INCLUDE_DIRS}  
                    ${SYSTEMCAMS_INCLUDE_DIRS}) 

ADD_SUBDIRECTORY(src bin)  
ADD_SUBDIRECTORY(tb/adc)
ADD_SUBDIRECTORY(tb/sine)
ADD_SUBDIRECTORY(tb/drain)
ADD_SUBDIRECTORY(tb/gnd)
