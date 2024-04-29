#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
// https://docs.ros2.org/beta3/api/rclcpp/classrclcpp_1_1node_1_1Node.html
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
      // Parameter ------------------
      // https://foxglove.dev/blog/how-to-use-ros2-parameters 
      // https://roboticsbackend.com/rclcpp-params-tutorial-get-set-ros2-params-with-cpp/
      declare_parameter("speed", 0.2);  // default to 0.2 (double)   
      //get_parameter("speed", speed_param_);  
      //std::cout << "\n turtlespeed_param is set to "<< speed_param_; 
      // Timer for updating parameter 
      timer_param_ = this->create_wall_timer(200ms, std::bind(&TurtleSimPublisher::timer_param_callback, this));
     
      // run and set param
      // $1 ros2 run turtlesim turtlesim_node 
      // $2 ros2 run emr2 turtlesim_taste --ros-args -p speed:=0.4
      // ros2 param list  => Exception while calling service of node '/turtlesim_publisher': None
      // get value of param: $ ros2 param get /turtlesim_publisher speed   
      // set value of param: $ ros2 param set /turtlesim_publisher speed 0.3    

      cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
      timer_pub_ = this->create_wall_timer(500ms, std::bind(&TurtleSimPublisher::timer_pub_callback, this));
    }

  private:
    void timer_param_callback()  //Wird vom Timer alle 500ms aufgerufen
    {
      get_parameter("speed", speed_param_);  
      std::cout << "\n turtlespeed_param is set to "<< speed_param_;    
    }

    void timer_pub_callback()  //Wird vom Timer alle 500ms aufgerufen
    {
      // auto => automatischen Speicherklasse, d. h. eine Variable mit lokaler Lebensdauer
      auto msg = geometry_msgs::msg::Twist();  // instanziert Twist - Object 

      // Hole Taste
      char key = 0; //getch();
      double speed_val = speed_param_;  //0.2
      

      switch(key){
        case 'w':
          msg.linear.set__x(speed_val); break;
        case 's':
         msg.linear.set__x(-speed_val); break;
        case 'a':
          msg.angular.set__z(speed_val); break;
        case 'd':
          msg.angular.set__z(-speed_val); break;
      }

      cmd_pub_->publish(msg);   // Sende Nachricht ins ROS2
      RCLCPP_INFO(this->get_logger(), "Publishing linear X : '%f'", msg.linear.x); //Konsolenausgabe
      RCLCPP_INFO(this->get_logger(), "Publishing angular Z : '%f'", msg.angular.z); //Konsolenausgabe
    }
  
  private:  //Klassenvariablen
    rclcpp::TimerBase::SharedPtr timer_pub_;
    rclcpp::TimerBase::SharedPtr timer_param_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    size_t count_;    
    double speed_param_ = 0.0;
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