ADD_EXECUTABLE(drain_tb
../../src/sca_tdf_drain.cpp
../../src/sca_tdf_drain.h
sc_top.h
main.cpp
)
target_link_libraries( drain_tb
 ${SYSTEMC_LIBRARIES}
${SYSTEMCAMS_LIBRARIES}
)
