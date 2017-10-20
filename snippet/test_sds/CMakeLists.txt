include_directories(${PROJECT_SOURCE_DIR}/thirdparty/sds)
add_executable(demo_sds demo_sds.c)
target_link_libraries(demo_sds sds)
