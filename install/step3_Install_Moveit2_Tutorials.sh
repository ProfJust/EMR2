# https://moveit.picknik.ai/humble/doc/tutorials/getting_started/getting_started.html#download-source-code-of-moveit-and-the-tutorials

sudo apt install python3-rosdep

sudo rosdep init
rosdep update
sudo apt update -y
sudo apt dist-upgrade -y

# Install Colcon the ROS 2 build system with mixin:

sudo apt install python3-colcon-common-extensions
sudo apt install python3-colcon-mixin
colcon mixin add default https://raw.githubusercontent.com/colcon/colcon-mixin-repository/master/index.yaml
colcon mixin update default

sudo apt install python3-vcstool

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
sudo apt install ros-humble-rmw-cyclonedds-cpp -y
echo 'export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp' >> ~/.bashrc  # Ergänze .bashrc

ros2 launch moveit2_tutorials demo.launch.py rviz_config:=your_rviz_config.rviz