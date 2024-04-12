
# https://roboticscasual.com/tutorial-ros2-launch-files-all-you-need-to-know/
from launch import LaunchDescription
from launch_ros.actions import Node
 
def generate_launch_description():
    
    ld = LaunchDescription()  # Instanziere Launch-Description Objekt

    turtlesim_node = Node(    # Instanziere Node Objekt
        package="turtlesim",
        executable="turtlesim_node",   # mandatory = Pflichtangabe
        name='sim'                     # You will see the specified name if the action writes a terminal output. 
                                       # [sim]: Starting turtlesim with node name /sim
    )

# Additional Node options are:
#    namespace: Launch the Node in a namespace, e.g. turtlesim1
#    exec_name: Name of the process. Default is the same as name
#    parameters: List of parameter files that are loaded for the Node (e.g. parameters="my_params.yaml")
#    remappings: Remapping of node names, e.g. remappings=[('/input/pose', '/turtlesim1/turtle1/pose')]
#    ros_arguments: List of ROS arguments to hand over to the Node, e.g. ros_arguments=['__log_disable_rosout', 'true']
#    arguments: List of arguments to hand over to the Node, e.g. arguments=['-name', 'my_robot']



 
    ld.add_action(turtlesim_node)   # add action zum ld-Objekt
    # actions define what the launch file should execute



    return ld