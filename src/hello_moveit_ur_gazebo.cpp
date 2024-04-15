/* from https://github.com/LucaBross/simple_moveit2_universal_robots_movement

USAGE
$1 ros2 launch ur_simulation_gazebo ur_sim_moveit.launch.py
# nur realbot??  §2 ros2 launch emr2 hello_moveit_ur_launch.py 
$3 ros2 run  emr2 hello_moveit_ur 
*/

/* Ergänze im File  
 ~/ur3_ws/src/Universal_Robots_ROS2_Drivere/ur_moveit_config/launch/ur_moveit.launch.py
 ca. ab Zeile 184
 planning_scene_monitor_parameters = {
        "publish_planning_scene": True,
        "publish_geometry_updates": True,
        "publish_state_updates": True,
        "publish_transforms_updates": True,
        "publish_robot_description": True,
        "publish_robot_description_semantic": True,
    }

  Don't forget to save, rebuild ans source !!!!  
*/


#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

int main(int argc, char * argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
    "hello_moveit",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("hello_moveit");

  // Create the MoveIt MoveGroup Interface
using moveit::planning_interface::MoveGroupInterface;
auto move_group_interface = MoveGroupInterface(node, "ur_manipulator");
// https://github.com/ros-planning/moveit2_tutorials/issues/528

// Set a target Pose
auto const target_pose = []{
  geometry_msgs::msg::Pose msg;
  msg.orientation.w = 1.0;
  msg.position.x = 0.28;
  msg.position.y = -0.2;
  msg.position.z = 0.5;
  return msg;
}();
move_group_interface.setPoseTarget(target_pose);

// Create a plan to that target pose
auto const [success, plan] = [&move_group_interface]{
  moveit::planning_interface::MoveGroupInterface::Plan msg;
  auto const ok = static_cast<bool>(move_group_interface.plan(msg));
  return std::make_pair(ok, msg);
}();

// Execute the plan
if(success) {
  move_group_interface.execute(plan);
} else {
  RCLCPP_ERROR(logger, "Planing failed!");
}

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
