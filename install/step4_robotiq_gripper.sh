# Installieren der Gripper-Pakete
cd ~/ur3_ws/src
git clone https://github.com/PickNikRobotics/ros2_robotiq_gripper.git
build
ros2 launch robotiq_description view_gripper.launch.py