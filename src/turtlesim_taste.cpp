#include <cstdio>

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"  //Type of cmd_vel

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

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
      
      // Setze Werte für die Message
      double speed_val = 0.2;
      msg.linear.set__x(speed_val);

      cmd_pub_->publish(msg);   // Sende Nachricht ins ROS2
      RCLCPP_INFO(this->get_logger(), "Publishing linear X : '%f'", msg.linear.x); //Konsolenausgabe
    }
  
  private:  //Klassenvariablen
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TurtleSimPublisher>());
  rclcpp::shutdown();
  return 0;
}