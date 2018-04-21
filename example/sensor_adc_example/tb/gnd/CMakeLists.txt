ADD_EXECUTABLE(gnd_tb
../../src/sca_tdf_gnd.cpp
../../src/sca_tdf_gnd.h
sc_top.h
main.cpp
)
target_link_libraries( gnd_tb
 ${SYSTEMC_LIBRARIES}
${SYSTEMCAMS_LIBRARIES}
)
