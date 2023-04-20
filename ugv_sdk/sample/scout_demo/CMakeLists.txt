add_executable(demo_scout_robot scout_robot_demo.cpp)
target_link_libraries(demo_scout_robot ugv_sdk)

add_executable(demo_scout_mini_omni_robot scout_mini_omni_demo.cpp)
target_link_libraries(demo_scout_mini_omni_robot ugv_sdk)

add_executable(scout_robot_v3 scout_robot_v3.cpp)
target_link_libraries(scout_robot_v3  
   PRIVATE
   ugv_sdk
   mysqlclient
)