project(Game)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
# set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
# link_libraries(soil)
add_library(${PROJECT_NAME} STATIC ${SRC})
target_link_libraries(${PROJECT_NAME} GameUtil)