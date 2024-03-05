# rtc2
Ruhr Turtlebot Competition mit ROS2

# installation
erstelle den Ordner  ~/ur3_ws/src
  $ cd ~
  $ mkdir ur3_ws
  $ cd ur3_ws
  $ mkdir src
  $ cd src

clone emr2 dorthin
  $ git clone https://github.com/ProfJust/emr2
  

mache step1....sh und step2....sh ausführbar
und führe sie aus

$ cd ~/ur3_ws/src
$ ./step1...
$ ./step2...

# usage
to be done

# nano ~/.bashrc

In der .bashrc muss folgendes stehen (SimBot)
 => Gazebo mit mehreren Tln in einem Netz:  export ROS_LOCALHOST_ONLY=1  

??export ROS_DOMAIN_ID=30 #TURTLEBOT3
??export TURTLEBOT3_MODEL=burger
export ROS_LOCALHOST_ONLY=0  # 0 = Communication allowed
# this environment variable allows you to limit ROS 2 communication to localhost only.
source /opt/ros/humble/setup.bash
source ~/ur3_ws/install/setup.bash



