ADD_EXECUTABLE(sensor_adc_example
sca_tdf_adc.cpp
sca_tdf_adc.h
sca_tdf_sine.cpp
sca_tdf_sine.h
sca_tdf_drain.cpp
sca_tdf_drain.h
sca_tdf_gnd.cpp
sca_tdf_gnd.h
sc_top.h
main.cpp)

target_link_libraries( sensor_adc_example
 ${SYSTEMC_LIBRARIES}
${SYSTEMCAMS_LIBRARIES}
)
