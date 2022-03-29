project(core-food-dispensary)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES "food_disp.cxx")

add_executable(fdisp ${SOURCE_FILES})

install(TARGETS fdisp
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)