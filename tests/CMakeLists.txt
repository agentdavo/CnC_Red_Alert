add_executable(ipx_stub_test ipx_stub_test.c ../src/ipx_stub.c)
target_include_directories(ipx_stub_test PRIVATE ../CODE ../src)
if(WIN32)
    target_link_libraries(ipx_stub_test PRIVATE ws2_32)
endif()
add_test(NAME ipx_stub_test COMMAND ipx_stub_test)

add_executable(fixed_math_test fixed_math_test.cpp)
target_include_directories(fixed_math_test PRIVATE ../CODE ../src)
add_test(NAME fixed_math_test COMMAND fixed_math_test)

add_executable(vqa_video_player vqa_video_player.c)
target_include_directories(vqa_video_player PRIVATE
    ../CODE
    ../src
    ../VQ/LVGL
    ../VQ/LVGL/INCLUDE
    ../src/lvgl/src
)
target_link_libraries(vqa_video_player PRIVATE vqa32_lvgl lvgl)
