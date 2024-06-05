/* from https://github.com/LucaBross/simple_moveit2_universal_robots_movement

USAGE
$1 ros2 launch ur_robot_driver ur3e.launch.py robot_ip:=192.168.00.50 target_filename:="${HOME}/ur3e_robot_calibration.yaml" ur_type:=ur3e
§2 ros2 launch ur_moveit_config ur_moveit.launch.py ur_type:=ur3e
$3 ros2 run emr2 hello_moveit_ur_realbot 

Show Pose of the Interactive Marker
$4 ros2 topic echo /rviz_moveit_motion_planning_display/robot_interaction_interactive_marker_topic/update 

Add Scene Object GroundPlane 2m x 2m x 0.05m
Position 0 0 -0.1
Rotation 0 0 0 

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

// Set a target Pose
auto const target_pose = []{
  geometry_msgs::msg::Pose msg;
  msg.position.x = 0.1;
  msg.position.y = 0.25;
  msg.position.z = 0.6;
  msg.orientation.x = -0.5;
  msg.orientation.y =  0.5;
  msg.orientation.z =  0.5;
  msg.orientation.w =  0.5;
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


      //Getting Basic Information
      RCLCPP_INFO(logger, "Planning frame: %s", move_group_interface.getPlanningFrame().c_str());     

      std::string inputString;
      std::cout << "\nGive input: Should the robot go to target pose now? (yes/no)";
      std::getline(std::cin, inputString);
      if(inputString.compare("yes") == 0){
          move_group_interface.execute(plan);
          RCLCPP_INFO(logger, "\n Target Pose 1 reached \n"); //logger-Definition s.o.

         //Fails to fetch current robot state => why???
          geometry_msgs::msg::PoseStamped current_pose; 
          current_pose = move_group_interface.getCurrentPose();
          RCLCPP_INFO(logger, "Current Pose X: '%f'", current_pose.pose.position.x);
          RCLCPP_INFO(logger, "Current Pose Y: '%f'", current_pose.pose.position.y);
          RCLCPP_INFO(logger, "Current Pose Z: '%f'", current_pose.pose.position.z);     

          //moveit::core::RobotStatePtr current_state = move_grp_if.getCurrentState(10);   //Wartezeit in sec
          // Didn't receive robot state (joint angles) with recent timestamp within 10.000000 seconds  
          //std::cout << current_state;     

         // $4 show pose of the interactive Marker 
         //$ ros2 topic echo /rviz_moveit_motion_planning_display/robot_interaction_interactive_marker_topic/update     
      }
      else{  rclcpp::shutdown();
         RCLCPP_INFO(logger, "\n Exit programm from user request \n");
         return 0; 
      }

} else {
  RCLCPP_ERROR(logger, "Planing failed!");
}

//======================= second target =================
  auto const target_pose2 = []{
    geometry_msgs::msg::Pose msg;
    msg.orientation.w = 1.0;
    msg.position.x = 0.38;
    msg.position.y = -0.3;
    msg.position.z = 0.6;
    return msg;
  }();

  move_group_interface.setPoseTarget(target_pose2);

  auto const [success2, plan2] = [&move_group_interface]{
       moveit::planning_interface::MoveGroupInterface::Plan msg;
       auto const ok = static_cast<bool>(move_group_interface.plan(msg));
       return std::make_pair(ok, msg);
      }();

  // Execute the plan
  if(success2) {
      std::string inputString;
      std::cout << "\n Give input: Should the robot go to target pose 2 now? (yes/no)";
      std::getline(std::cin, inputString);
      if(inputString.compare("yes") == 0){
          move_group_interface.execute(plan2);
          RCLCPP_INFO(logger, "\n Target Pose 2 reached \n"); //logger-Definition s.o.
      }
      else{  rclcpp::shutdown();
         RCLCPP_INFO(logger, "\n Exit programm from user request \n");
         return 0; 
      }
  } 
  else {
    RCLCPP_ERROR(logger, "Planing failed!");
  }

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
