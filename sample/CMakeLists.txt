
add_definitions(-std=c++11)

file(GLOB app_list RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.cpp)
foreach(app_filename ${app_list})
    get_filename_component(app ${app_filename} NAME_WE)
    add_executable(${app} ${app_filename})
    set_target_properties(${app} PROPERTIES COMPILE_FLAGS "-pthread")
    target_link_libraries(${app} ${MPI_LIBRARIES} ${LIBRARY})
endforeach()
