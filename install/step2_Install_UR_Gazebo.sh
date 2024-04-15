# step2_Install_UR_Gazebo.sh
# am 13.3.24 geändert
# basiert auf https://github.com/UniversalRobots/Universal_Robots_ROS2_Gazebo_Simulation

# export COLCON_WS=~/ur3_ws
cd ~/ur3_ws/src

git clone https://github.com/UniversalRobots/Universal_Robots_ROS2_Description.git -b humble
git clone https://github.com/UniversalRobots/Universal_Robots_ROS2_Gazebo_Simulation.git -b humble
git clone https://github.com/UniversalRobots/Universal_Robots_ROS2_Driver.git -b humble

# rqt - Tools für Controller
sudo apt install ros-humble-rqt-joint-trajectory-controller -y
sudo apt install ros-humble-rqt-joint-trajectory-controller -y
sudo apt install ros-humble-joint-trajectory-controller -y
sudo apt install ros-humble-control-toolbox -y


# falls es nicht in rqt zu finden ist 
# ggf. ros2 run rqt_joint_trajectory_controller rqt_joint_trajectory_controller --force-discover

cd ..
rosdep install --ignore-src --from-paths src -y
rosdep update

colcon build 
source install/local_setup.sh 

ros2 launch ur_simulation_gazebo ur_sim_control.launch.py ur_type=:=ur3e
