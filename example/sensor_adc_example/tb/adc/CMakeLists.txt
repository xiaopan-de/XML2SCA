ADD_EXECUTABLE(adc_tb
../../src/sca_tdf_adc.cpp
../../src/sca_tdf_adc.h
sc_top.h
main.cpp
)
target_link_libraries( adc_tb
 ${SYSTEMC_LIBRARIES}
${SYSTEMCAMS_LIBRARIES}
)
