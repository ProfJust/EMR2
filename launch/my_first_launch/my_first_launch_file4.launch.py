
# https://roboticscasual.com/tutorial-ros2-launch-files-all-you-need-to-know/
from launch import LaunchDescription  # fehlt auf der Webseite => Error
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
  
def generate_launch_description():
 
   rviz_config_file = PathJoinSubstitution(
           [FindPackageShare("ur_description"), "rviz", "view_robot.rviz"]
   )
    
   return LaunchDescription([
        Node(
            package="rviz2",
            executable="rviz2",
            name="rviz2",
            output="log",
            arguments=["-d", rviz_config_file],
        ),
   ])