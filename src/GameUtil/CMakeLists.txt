project(GameUtil)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
# link_libraries(soil)
add_library(${PROJECT_NAME} SHARED ${SRC})
target_link_libraries(${PROJECT_NAME} glfw3 glad opengl32)