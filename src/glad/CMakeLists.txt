project(glad)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)
include_directories(${CMAKE_SOURCE_DIR}/include)
# link_directories(${CMAKE_SOURCE_DIR}/lib)
add_library(${PROJECT_NAME} STATIC ${SRC})

