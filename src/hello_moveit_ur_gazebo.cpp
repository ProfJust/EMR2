/* from https://github.com/LucaBross/simple_moveit2_universal_robots_movement

https://moveit.picknik.ai/humble/doc/examples/move_group_interface/move_group_interface_tutorial.html

USAGE
First set Parameters
moveit_configs_builder.py
Zeile 389 und 390
def planning_scene_monitor(
        self,
        publish_planning_scene: bool = True,
        publish_geometry_updates: bool = True,
        publish_state_updates: bool = True,
        publish_transforms_updates: bool = True,
        publish_robot_description: bool = True,  #OJU was False
        publish_robot_description_semantic: bool = True, #OJU was False
    ):



$1a ros2 param set /move_group publish_robot_description true
$1b ros2 param set /move_group publish_robot_description_semantic true
$1c ros2 launch ur_simulation_gz ur_sim_moveit.launch.py ur_type:=ur3e sim_time:=true

$3 ros2 run  emr2 hello_moveit_ur 


$4 show pose of the interactive Marker 
$ ros2 topic echo /rviz_moveit_motion_planning_display/robot_interaction_interactive_marker_topic/update


*/


#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

int main(int argc, char * argv[])
{
  // Initialize ROS 
  rclcpp::init(argc, argv);
  

  //create the Node
  /*auto const node = std::make_shared<rclcpp::Node>(
    "hello_moveit",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)

    //automatically_declare_parameters_from_overrides(true) is a method that modifies the node options
    //to automatically declare parameters that are overridden via the command line or launch file parameters.
  );*/

  auto node = rclcpp::Node::make_shared("move_group_node");
  auto move_group_if = moveit::planning_interface::MoveGroupInterface(node, "ur_manipulator");

  // Example usage: Planning to a pose goal
    geometry_msgs::msg::Pose target_pose;
    
    target_pose.position.x = -0.19964362641172745;
    target_pose.position.y =  0.32309863449505277;
    target_pose.position.z =  0.2890648877410565;
    
    target_pose.orientation.x = 0.9962273179306839;
    target_pose.orientation.y = 0.04057380158202765;
    target_pose.orientation.z = 0.00922585009362648;
    target_pose.orientation.w = 0.0761562953659663;

    
    move_group_if.setPoseTarget(target_pose);

    // Plan and execute the motion
    auto success = move_group_if.move();

    if (success) {
        RCLCPP_INFO(node->get_logger(), "Motion plan executed successfully.");
    } else {
        RCLCPP_WARN(node->get_logger(), "Failed to execute motion plan.");
    }

    // Shutdown ROS 2
    rclcpp::shutdown();
    return 0;
}
  // $ ros2 param set /hello_moveit use_sim_time true
  //rclcpp::Parameter simTime( "use_sim_time", rclcpp::ParameterValue( true ) );
  //rclcpp::Parameter robot_description( "robot_description", rclcpp::ParameterValue( true ) );
  //rclcpp::Parameter robot_description_semantic( "robot_description_semantic", rclcpp::ParameterValue( true ) );


  // Create a ROS logger
  //auto const logger = rclcpp::get_logger("hello_moveit");

  // Create the MoveIt MoveGroup Interface  (MGI)
  //using moveit::planning_interface::MoveGroupInterface; //set namespace

  //static const std::string PLANNING_GROUP = "ur_manipulator";
  // Initiiere C++-API für MoveIt! => MoveGroupInterface
  //auto mg_if = MoveGroupInterface(node, "ur_manipulator");
  //moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);
  //  This name must match a planning group defined in your robot's SRDF file. 
  // https://github.com/ros-planning/moveit2_tutorials/issues/528

  // Set a target Pose 
  // auto  => Automatic Type Deduction, erkennt von selber den notwendigen Variablentyp
  /*auto const target_pose = []{
    geometry_msgs::msg::Pose msg;
    msg.orientation.w = 1.0;
    msg.position.x = 0.28;
    msg.position.y = -0.2;
    msg.position.z = 0.5;
    return msg;
  }();*/

  // Send Target Pose  => MGI
   //move_grp_if.setPoseTarget(target_pose);

  // Create a plan to that target pose
  //------------------------------------------------
      // Modern C++
      // auto  => Automatic Type Deduction, erkennt von selber den notwendigen Variablentyp
      // Bsp: auto myVar ='c';   => char-Type
      //      auto x = 123;      => int-Type
      //
      // Modern C++ => Structured bindings
      // Einen oder mehrere Aliase für Ausdrücke definieren
      // https://devblogs.microsoft.com/oldnewthing/20201014-00/?p=104367
      // C++17 introduced a feature known as structured binding. It allows a single source object to be taken apart:
      // std::pair<int, double> p{ 42, 0.0 };
      // auto [i, d] = p;
      // https://de.linkedin.com/learning/c-plus-plus-17-grundkurs/structured-bindings-und-mehrere-werte-zuruckgeben


      // Auto Deducted   Structured Binding, definiert die Ausdrücke "success" und "plan" 
      // [&mv] {...} () => Funktionszeiger mit Inline-Definition der Funktion?????
  /*auto const [success, plan] = [&move_grp_if]{
       moveit::planning_interface::MoveGroupInterface::Plan msg;
       auto const ok = static_cast<bool>(move_grp_if.plan(msg));
       return std::make_pair(ok, msg);
      }();

  // Execute the plan
  if(success) {
      //Getting Basic Information
      RCLCPP_INFO(logger, "Planning frame: %s", move_grp_if.getPlanningFrame().c_str());     

      std::string inputString;
      std::cout << "\nGive input: Should the robot go to target pose now? (yes/no)";
      std::getline(std::cin, inputString);
      if(inputString.compare("yes") == 0){
          move_grp_if.execute(plan);
          RCLCPP_INFO(logger, "\n Target Pose 1 reached \n"); //logger-Definition s.o.

         //Fails to fetch current robot state => why???
          geometry_msgs::msg::PoseStamped current_pose; 
          current_pose = move_grp_if.getCurrentPose();
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
  } 
  else {
    RCLCPP_ERROR(logger, "Planing failed!");
  }

//======================= second target =================
  auto const target_pose2 = []{
    geometry_msgs::msg::Pose msg;
    msg.orientation.w = 1.0;
    msg.position.x = 0.78;
    msg.position.y = 0.2;
    msg.position.z = 0.5;
    return msg;
  }();

  move_grp_if.setPoseTarget(target_pose2);

  auto const [success2, plan2] = [&move_grp_if]{
       moveit::planning_interface::MoveGroupInterface::Plan msg;
       auto const ok = static_cast<bool>(move_grp_if.plan(msg));
       return std::make_pair(ok, msg);
      }();

  // Execute the plan
  if(success2) {
      std::string inputString;
      std::cout << "\n Give input: Should the robot go to target pose 2 now? (yes/no)";
      std::getline(std::cin, inputString);
      if(inputString.compare("yes") == 0){
          move_grp_if.execute(plan2);
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
*/

// [] kann auch ein Tuple sein
  // A tuple is an object that can hold a number of elements. The elements can be of different data types.
  // https://www.geeksforgeeks.org/tuples-in-c/
  // tuple <char, int, float> geek;
  // Assigning values to tuple using make_tuple()
  // geek = make_tuple('a', 10, 15.5);