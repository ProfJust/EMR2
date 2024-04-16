// emr2 - P02 - TurtleSim Follower
// Turtle2 folgt der Turtle1
// turtle1 fahren lassen
// turtle2 spawnen lassen, an der selben Stelle wie Turtle1 und dann dem weissen Pfad hinterher fahren lassen
// OJ am 15.04.2024
//---------------------------------------------
// usage:
// $ros2 launch emr2 turtlesim_teleop.launch.py 
// $ rqt   => call spawn service turtle2
// Spawning turtle [turtle2] at x=[5.544445], y=[5.544445], theta=[0.000000]
// start this file: $ros2 run emr2 turtlesim_follower
//
// show sub andf pub: ros2 node info /turtlesim1



#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"  //Type of cmd_vel
#include <turtlesim/msg/pose.hpp>
// CMakeList add_executable(turtlesim_follower   src/turtlesim_follower.cpp)
//           ament_target_dependencies(turtlesim_follower rclcpp std_msgs geometry_msgs turtlesim)
//#include "/opt/ros/humble/include/turtlesim/turtlesim/msg/pose.hpp"
// See Directory /opt/ros/humble/include/turtlesim/turtlesim/msg

#include <turtlesim/msg/color.hpp> //color_sensor

// #include "turtlesim/srv/spawn.hpp"
// https://github.com/siksal/turtle_control/blob/main/src/catch_turtle.cpp

using namespace std::chrono_literals;

int getch(void); // hole Taste (s.u.)

class TurtleSimNode: public rclcpp::Node
{
   public: 
   //Konstruktor
    TurtleSimNode(): Node("turtlesim_follower"), count_(0)
    {   
      cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle2/cmd_vel", 10);
      color_subscriber_ = this->create_subscription<turtlesim::msg::Color>("turtle2/color", 10, std::bind(&TurtleSimNode::color_callback,    this, std::placeholders::_1));
      pose_subscriber_  = this->create_subscription<turtlesim::msg::Pose> ("turtle1/pose",  10, std::bind(&TurtleSimNode::callbackTurtlePose, this, std::placeholders::_1));
      timer_ = this->create_wall_timer(500ms, std::bind(&TurtleSimNode::timer_callback, this));
    }

  private:
    void timer_callback()  //Wird vom Timer alle 500ms aufgerufen
    {
      // auto => automatischen Speicherklasse, d. h. eine Variable mit lokaler Lebensdauer
      auto msg = geometry_msgs::msg::Twist();  // instanziert Twist - Object 

      // Hole Taste
      //char key = getch();
      double speed_val = 0.0;

      // Setze Werte für die Message
      speed_val = 0.2;
      msg.linear.set__x(speed_val);

      /****** HIER CODE EINFÜGEN ******/

      cmd_pub_->publish(msg);   // Sende Nachricht ins ROS2
      RCLCPP_INFO(this->get_logger(), "Publishing linear X : '%f'", msg.linear.x); //Konsolenausgabe
      RCLCPP_INFO(this->get_logger(), "Publishing angular Z : '%f'", msg.angular.z); //Konsolenausgabe
    }

    void callbackTurtlePose(const turtlesim::msg::Pose::SharedPtr msg)
    {
        this->pose_ = *msg;
    }

    void color_callback(const turtlesim::msg::Color::SharedPtr msg)
    {
        this->color_ = *msg;
    }
  
  private:  //Klassenvariablen
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscriber_;
    rclcpp::Subscription<turtlesim::msg::Color>::SharedPtr color_subscriber_;
    size_t count_;
    turtlesim::msg::Pose pose_;
    turtlesim::msg::Color color_;


};



int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  std::cout <<  " Tasten-Steurung für die TurtleSim - Druecken Sie 'W' 'A' 'S' 'D' "; //Konsolenausgabe
  rclcpp::spin(std::make_shared<TurtleSimNode>());
  rclcpp::shutdown();
  return 0;
}




// https://raw.githubusercontent.com/aarsht7/teleop_cpp_ros2/main/src/teleop_cpp_ros2.cpp
// For non-blocking keyboard inputs
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
int getch(void)
{
  int ch;
  struct termios oldt;
  struct termios newt;

  // Store old settings, and copy to new settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  // Make required changes and apply the settings
  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_iflag |= IGNBRK;
  newt.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
  newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  newt.c_cc[VMIN] = 1;
  newt.c_cc[VTIME] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &newt);

  // Get the current character
  ch = getchar();

  // Reapply old settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}