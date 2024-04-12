from launch import LaunchDescription
from launch.actions import LogInfo
from launch.conditions import IfCondition
from launch.substitutions import PythonExpression, EnvironmentVariable
  
def generate_launch_description():
    return LaunchDescription([
        LogInfo(condition=IfCondition(
            PythonExpression(["'",
                EnvironmentVariable('USER'),       # get current User from Environent
                "' == 'robolab'"
            ])
        ),
        msg='Welcome back in the roboLab'
        )
    ])