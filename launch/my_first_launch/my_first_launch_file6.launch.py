# Launch-with Arguments
# https://roboticscasual.com/tutorial-ros2-launch-files-all-you-need-to-know/

# usage: ros2 launch my_first_launch_file6.launch.py launch_arg:=2

from launch import LaunchDescription
from launch.actions import LogInfo, DeclareLaunchArgument
from launch.conditions import LaunchConfigurationEquals, LaunchConfigurationNotEquals
from launch.substitutions import TextSubstitution
  
def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            'launch_arg', default_value=TextSubstitution(text='default')
        ),
 
        LogInfo(condition=LaunchConfigurationEquals('launch_arg', 'default'),
        msg=['Okay, no launch argument.']
        ),
         
        LogInfo(condition=LaunchConfigurationNotEquals('launch_arg', 'default'),
        msg=['Yay, a launch argument!']
        )
    ])