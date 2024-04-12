# https://roboticscasual.com/tutorial-ros2-launch-files-all-you-need-to-know/


# sende Argumente an Gazebo
# hier spawnen eines UR3e- Roboters 
# dafür vorher  Gazebo starten
# gazebo

# usage
# ros2 launch ur_simulation_gazebo ur_sim_control.launch.py ur_type=:=ur3e
# ros2 launch my_first_launch_file8.launch.py 


from launch import LaunchDescription
from launch.substitutions import EnvironmentVariable
from launch_ros.actions import Node
 
def generate_launch_description():
   return LaunchDescription([
        Node(
             package='gazebo_ros',
             executable='spawn_entity.py',
             output='screen',
             arguments=['-topic', 'robot_description',
                      '-entity', 'ur3e',
                      '-z', '0.1',
                      '-x', '0.5',
                      ]
        ),
   ])