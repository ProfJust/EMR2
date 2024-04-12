# hand over command line arguments to a ROS2 launch file
# https://roboticscasual.com/tutorial-ros2-launch-files-all-you-need-to-know/

# usage: $ ros2 launch my_first_launch_file7.launch.py new_background_r:=200


# How to find out the launch arguments of a ROS2 launch file
# ros2 launch package_name <launch_file_name>.launch.py -s
# $ ros2 launch my_first_launch_file7.launch.py -s

from launch_ros.actions import Node
 
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration
 
def generate_launch_description():
    new_background_r_value = LaunchConfiguration('new_background_r')
 
    new_background_r_launch_arg = DeclareLaunchArgument(
        'new_background_r',
        default_value='200'
    )
 
    turtlesim_node = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='sim'
    )
 
    change_background_r_conditioned = ExecuteProcess(
        cmd=[[
            'ros2 param set ',
            '/sim background_r ',
            new_background_r_value
        ]],
        shell=True
    )
 
    return LaunchDescription([
        new_background_r_launch_arg,
        turtlesim_node,
        change_background_r_conditioned
    ])