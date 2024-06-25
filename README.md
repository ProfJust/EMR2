# EMR2
Embedded Robotics, UR3, UR3e, UR5e  mit  ROS2 steuern

# EMR2 - ROS2 und MoveIt!2
Repsitorium zum Modul "Embedded Robotics" - EMR2 ab dem SS24.
Stichworte:  ROS2 - Humble, openCV, Gazebo-Classic, MoveIt!2, UR3, UR3e, UR5e, robtiq Gripper

![rviz_plugin](https://jaspereb.github.io/UR5_With_ROS_Moveit_Tutorial/media/frontImg.png)


## Usage - Start der Umgebung mit UR5 mit Gripper ##
>$1 roslaunch ur5_moveit_config demo_gazebo.launch 



## Installation ##

Nachdem Ubuntu Jammy Jellyfish 22.4 (LTS) installiert wurde 
als erstes einen Workspace Ordner erstellen

1. Terminal öffnen mit STRG+ALT+T. Hinter dem Prompt $ die Befehle eingeben

>$ cd ~

>$ mkdir ur3_ws

>$ cd ur3_ws

>$ mkdir src



2. Klonen dieses Repositoriums nach ur3_ws/src

>$ sudo apt install git
>$ cd ~/ws_moveit/src/

>$ git clone https://github.com/ProfJust/emr2.git -b humble

Jetzt sollte der Ordner emr2 geclont worden sein.

3. Erstellen des Moveit-Workspace mit Shellskript
>$ cd ~/ur3_ws/src/emr2/install

Nun ggf. erstmal ROS-Noetic installieren, dazu dem Skript 
die Ausführungsrechte geben und dann ausführen
>$ chmod +x step1_.. .sh

>$ ./step1_....sh

4. Danach sämtliche für unseren Roboter benötigte Software-
Pakete installieren

>$ chmod +x  step2_....sh

>$ ./step2_....sh

5. Das Shell-Script sollte die die .bashrc - Datei konfiguriert haben, ansonsten
>$ gedit ~/.bashrc

6. Jetzt sollte das Kompilieren mit dem colcon-Build-System funktionieren
>$ buildslow


  Das Kompilieren des MoveIt-Paketes kann schonmal 30 Minuten oder länger dauern (je nach PC-Leistung, bei mir 34min 39sec)
  Falls der PC während der Kompilation abstürzt, prüfen Sie bitte, ob Ihr Speicher bzw. Sawp-Speicher ausreicht,
  da die Kompilation von MoveIt hier sehr anspruchsvoll ist. Ggf. den Swap Buffer vergrößern.
  
7. Nun sollte man die UR5 Gazebo Simulation + RViZ starten können
  >$ ros2 launch ur_simulation_gazebo ur_sim_moveit.launch.py ur_type:=ur3e



