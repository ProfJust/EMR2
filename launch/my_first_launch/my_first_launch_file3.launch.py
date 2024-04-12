
# https://roboticscasual.com/tutorial-ros2-launch-files-all-you-need-to-know/
from launch import LaunchDescription
from launch.substitutions import EnvironmentVariable
from launch_ros.actions import Node
 
def generate_launch_description():
   return LaunchDescription([
      Node(
            package='turtlesim',
            executable='turtlesim_node',
            name=[EnvironmentVariable('USER'), '_turtlesim'],
      ),
   ])