# Installieren der Gripper-Pakete
cd ~/ur3_ws/src
git clone https://github.com/PickNikRobotics/ros2_robotiq_gripper.git
build
ros2 launch robotiq_description view_gripper.launch.py

###  von Hand ausführen
# editiere
# ~/ur3_ws/src/Universal_Robots_ROS2_Description/urdf/ur.urdf.xacro

# ergänze am Ende

# <!--========================= OJU added Gripper =================== -->
#   <xacro:include filename="$(find robotiq_description)/urdf/robotiq_2f_85_gripper.urdf.xacro" />
  
# <!--END ========================= OJU added Gripper ===================-->

in robotiq_2f_85_gripper.urdf.xacro ändern:

 <!--<link name="world" />-->
    <xacro:robotiq_gripper name="RobotiqGripperHardwareInterface" prefix="" parent="tool0" use_fake_hardware="$(arg use_fake_hardware)">
        <origin xyz="0 0 0" rpy="0 0 0" />
    </xacro:robotiq_gripper>

# </robot>





# editiere  robotiq_2f_85_gripper.urdf.xacro


# change parent to tool0


# <!-- link name="world" /   already existst-->
#     <xacro:robotiq_gripper name="RobotiqGripperHardwareInterface" prefix="" parent="tool0" use_fake_hardware="$(arg use_fake_hardware)">
#         <origin xyz="0 0 0" rpy="0 0 0" />
#     </xacro:robotiq_gripper>

# build
# ros2 launch ur_simulation_gazebo ur_sim_control.launch.py ur_type:=ur3e
