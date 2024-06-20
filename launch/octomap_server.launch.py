from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
   return LaunchDescription([
     Node(
       package='octomap_server',
       executable='octomap_server_node',
       name='octomap_server',
       output='screen',
       parameters=[
         {'resolution':0.05},
         {'frame_id': 'world'},
         {'queue_size':100}
       ],
       remappings=[
         ('cloud_in','camera/camera/depth/color/points')
       ],
     ),
   ])

