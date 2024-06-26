cmake_minimum_required(VERSION 3.8)
project(emr2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# ===>>>>> Add used Librarys ===>>>>
# fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(moveit_visual_tools REQUIRED)
find_package(graph_msgs REQUIRED)

find_package(turtlesim REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(open_cv_cpp_sw00_hello_world  src/open_cv_cpp_sw00_hello_world.cpp)
ament_target_dependencies(open_cv_cpp_sw00_hello_world rclcpp std_msgs sensor_msgs cv_bridge image_transport OpenCV)

add_executable(open_cv_cpp_sw01_read_camera  src/open_cv_cpp_sw01_read_camera.cpp)
ament_target_dependencies(open_cv_cpp_sw01_read_camera rclcpp std_msgs sensor_msgs cv_bridge image_transport OpenCV)

add_executable(open_cv_cpp_sw02_track_object  src/open_cv_cpp_sw02_track_object.cpp)
ament_target_dependencies(open_cv_cpp_sw02_track_object rclcpp std_msgs sensor_msgs cv_bridge image_transport OpenCV)

add_executable(tf_broadcaster  src/tf_broadcaster.cpp)
ament_target_dependencies(tf_broadcaster geometry_msgs rclcpp tf2 tf2_ros turtlesim)

# ros2 pkg create --build-type ament_cmake --license Apache-2.0 --dependencies geometry_msgs rclcpp tf2 tf2_ros turtlesim -- learning_tf2_cpp


add_executable(talkerNode   src/minimal_publisher.cpp)
ament_target_dependencies(talkerNode rclcpp std_msgs)

# ===>>>>> Add Executable and Dependencies ===>>>>
add_executable(listenerNode src/minimal_subscriber.cpp)
ament_target_dependencies(listenerNode rclcpp std_msgs)

add_executable(turtlesim_taste   src/turtlesim_taste.cpp)
ament_target_dependencies(turtlesim_taste rclcpp std_msgs geometry_msgs)

add_executable(turtlesim_catcher   src/turtlesim_catcher.cpp)
ament_target_dependencies(turtlesim_catcher rclcpp std_msgs geometry_msgs turtlesim)

add_executable(hello_moveit_ur_gazebo  src/hello_moveit_ur_gazebo.cpp)
ament_target_dependencies(hello_moveit_ur_gazebo moveit_ros_planning_interface)

add_executable(hello_moveit_ur_realbot src/hello_moveit_ur_realbot.cpp)
ament_target_dependencies(hello_moveit_ur_realbot moveit_ros_planning_interface)

add_executable(moveit_ur_gazebo_scene   src/moveit_ur_gazebo_scene.cpp)
ament_target_dependencies(moveit_ur_gazebo_scene rclcpp std_msgs moveit_ros_planning_interface moveit_visual_tools graph_msgs )


target_include_directories(talkerNode PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_compile_features(talkerNode PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

# ===>>>>> Add target ===>>>>
install(TARGETS talkerNode
                listenerNode
                turtlesim_taste
                turtlesim_catcher
                hello_moveit_ur_gazebo
                hello_moveit_ur_realbot
                tf_broadcaster 
                open_cv_cpp_sw00_hello_world
                open_cv_cpp_sw01_read_camera
                open_cv_cpp_sw02_track_object
                moveit_ur_gazebo_scene
  DESTINATION lib/${PROJECT_NAME})

# ===>>>>> Add Folder for Launch-Files ===>>>>
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME}/)
# install(DIRECTORY launch/my_first_launch DESTINATION share/${PROJECT_NAME}/)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
