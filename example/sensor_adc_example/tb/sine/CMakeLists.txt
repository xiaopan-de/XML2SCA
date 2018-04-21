ADD_EXECUTABLE(sine_tb
../../src/sca_tdf_sine.cpp
../../src/sca_tdf_sine.h
sc_top.h
main.cpp
)
target_link_libraries( sine_tb
 ${SYSTEMC_LIBRARIES}
${SYSTEMCAMS_LIBRARIES}
)
