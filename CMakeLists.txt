cmake_minimum_required(VERSION 3.8)
project(ros2_custom_msgs)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 1. Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 2. Generate the custom message
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/RobotStatus.msg"
  DEPENDENCIES std_msgs
)

# 3. Define the executable
add_executable(status_publisher src/status_publisher.cpp)

# 4. Add standard dependencies
# Note: We add rosidl_default_generators to ensure the build order is correct
ament_target_dependencies(status_publisher 
  rclcpp 
  std_msgs
)

# 5. Link the generated message headers to the executable
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(status_publisher "${cpp_typesupport_target}")

# 6. Install instructions
install(TARGETS
  status_publisher
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
