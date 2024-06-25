# https://moveit.picknik.ai/humble/doc/tutorials/getting_started/getting_started.html#download-source-code-of-moveit-and-the-tutorials

sudo apt install python3-rosdep

# Install Colcon the ROS 2 build system with mixin:

sudo apt install python3-colcon-common-extensions -y
sudo apt install python3-colcon-mixin -y
colcon mixin add default https://raw.githubusercontent.com/colcon/colcon-mixin-repository/master/index.yaml
colcon mixin update default

sudo apt install python3-vcstool -y

cd ~/ur3_ws/src
git clone --branch humble https://github.com/ros-planning/moveit2_tutorials

vcs import < moveit2_tutorials/moveit2_tutorials.repos

sudo apt update && rosdep install -r --from-paths . --ignore-src --rosdistro $ROS_DISTRO -y

cd ~/ur3_ws
colcon build --mixin release   # nur so compiliert es, ohne mixin => fail

# Ergänze .bashrc
# echo 'alias so='source ~/ur3_ws/install/local_setup.bash'' >> ~/.bashrc
# echo 'alias co='colcon build --mixin release''             >> ~/.bashrc
# echo 'alias ce='colcon build --packages-select emr2' >> ~/.bashrc

# change to Cyclone DDS 
# sudo apt install ros-humble-rmw-cyclonedds-cpp -y
sudo apt install ros-humble-moveit-msgs -y
sudo apt install ros-humble-ros-testing -y
sudo apt install ros-humble-eigen-stl-containers -y
sudo apt install ros-humble-geometric-shapes -y
sudo apt install ros-humble-ros2-control-test-assets -y
sudo apt install ros-humble-ruckig -y
sudo apt install ros-humble-ur-msgs -y
sudo apt install ros-humble-controller-manager -y
sudo apt install ros-humble-ur-client-library -y
sudo apt install ros-humble-graph-msgs -y
sudo apt install ros-humble-ompl -y
sudo apt install ros-humble-warehouse-ros -y
sudo apt install ros-humble-rviz-visual-tools -y

sudo apt update -y
sudo apt dist-upgrade -y
sudo rosdep init
rosdep update



#echo 'export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp' >> ~/.bashrc  # Ergänze .bashrc

# ros2 launch moveit2_tutorials demo.launch.py rviz_config:=your_rviz_config.rviz
