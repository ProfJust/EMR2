# turtlesim_launch.py
#---------------------------------------------------------------------
# 16.10.23 by OJ 
# Westfälische Hochschule, Campus Bocholt, Ruhr TurtleBot Competition
#---------------------------------------------------------------------
# launches turtlesim and our move_node at the same time
# usage:
# $ ros2 launch rtc2 turtlesim_launch.py
# --------------------------------------------------------------------

from launch import LaunchDescription
from launch.actions import RegisterEventHandler
from launch_ros.actions import Node
# from launch.event_handlers import OnExecutionComplete
from launch.event_handlers import OnProcessStart
  
def generate_launch_description():
    Node_turtlesim1 = Node(
        package="turtlesim",
        executable="turtlesim_node",
        name='turtlesim1'
    )

    Node_teleop = Node(
        package="emr2",
        executable="turtlesim_taste",   
        name='teleop_node',        
        # teleop node needs to have an access to keyboard capture and cannot be used in launch file ????
        #[ERROR] [turtle_teleop_key-2]: process has died [pid 7973, exit code -6, 
        #        cmd '/opt/ros/humble/lib/turtlesim/turtle_teleop_key --ros-args -r __node:=move_node'].
        # Workaround: #https://github.com/ros2/teleop_twist_keyboard/issues/21
        # dafür xterm installieren => $sudo apt install xterm
        prefix = 'xterm -e',
        output='screen',          
    )
 
    return LaunchDescription([
        RegisterEventHandler(
            event_handler=OnProcessStart(
                target_action=Node_turtlesim1,
                on_start=[Node_teleop],
            )
        ),
        Node_turtlesim1
    ])

# for more information on launch files see
# https://docs.ros.org/en/humble/Tutorials/Intermediate/Launch/Creating-Launch-Files.html
# https://roboticscasual.com/tutorial-ros2-launch-files-all-you-need-to-know/#a-simple-launch-file
# https://roboticscasual.com/tutorial-ros2-launch-files-all-you-need-to-know/
# https://roboticscasual.com/tutorial-ros2-launch-files-all-you-need-to-know/#launch-event-handler-list