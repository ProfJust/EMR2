// emr2 - P02 - TurtleSim WASD-Steurung
// Vorgabe - zu vervollständigen
// OJ am 11.03.2024
//---------------------------------------------

#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"  //Type of cmd_vel

using namespace std::chrono_literals;

int getch(void); // hole Taste (s.u.)

class TurtleSimPublisher : public rclcpp::Node
{
   public: 
   //Konstruktor
    TurtleSimPublisher(): Node("turtlesim_publisher"), count_(0)
    {   
      cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
      timer_ = this->create_wall_timer(500ms, std::bind(&TurtleSimPublisher::timer_callback, this));
    }

  private:
    void timer_callback()  //Wird vom Timer alle 500ms aufgerufen
    {
      // auto => automatischen Speicherklasse, d. h. eine Variable mit lokaler Lebensdauer
      auto msg = geometry_msgs::msg::Twist();  // instanziert Twist - Object 

      // Hole Taste
      char key = getch();
      double speed_val = 0.0;

      // Setze Werte für die Message
      double speed_val = 0.2;
      msg.linear.set__x(speed_val);

      /****** HIER CODE EINFÜGEN ******/

      cmd_pub_->publish(msg);   // Sende Nachricht ins ROS2
      RCLCPP_INFO(this->get_logger(), "Publishing linear X : '%f'", msg.linear.x); //Konsolenausgabe
      RCLCPP_INFO(this->get_logger(), "Publishing angular Z : '%f'", msg.angular.z); //Konsolenausgabe
    }
  
  private:  //Klassenvariablen
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    size_t count_;
};



int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  std::cout<<  " Tasten-Steurung für die TurtleSim - Druecken Sie 'W' 'A' 'S' 'D' "; //Konsolenausgabe
  rclcpp::spin(std::make_shared<TurtleSimPublisher>());
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