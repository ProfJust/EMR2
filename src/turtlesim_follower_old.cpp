// emr2 - P02 - TurtleSim Follower
// Turtle2 folgt der Turtle1
// turtle1 fahren lassen
// turtle2 spawnen lassen, an der selben Stelle wie Turtle1 und dann der Turtle1 hinterher fahren lassen
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
#include "turtlesim/srv/spawn.hpp"
#include "turtlesim/srv/kill.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;
#define _USE_MATH_FUNCTIONS
#include <random>
#include <cmath>
#include <ctime> 
std::uniform_int_distribution<> dist1(1, 10);
std::uniform_int_distribution<> dist2(0, 2*M_PI);
std::mt19937 gen(time(nullptr));   // std::mt19937(since C++11) class is a very efficient pseudo-random number generator
const std::string turtle_prefix = "turtle_";
const double tolerance = 0.6;

class TurtleSimNode: public rclcpp::Node
{
   public: 
   //Konstruktor
    TurtleSimNode(): Node("turtlesim_follower"), count_(0)
    {   
      cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle2/cmd_vel", 10);
      color_subscriber_ = this->create_subscription<turtlesim::msg::Color>("turtle2/color", 10, std::bind(&TurtleSimNode::color_callback,    this, std::placeholders::_1));
      pose_subscriber_  = this->create_subscription<turtlesim::msg::Pose> ("turtle1/pose",  10, std::bind(&TurtleSimNode::callbackTurtlePose, this, std::placeholders::_1));
      //timer_ = this->create_wall_timer(500ms, std::bind(&TurtleSimNode::timer_callback, this));
      timer_ = this->create_wall_timer(500ms, std::bind(&TurtleSimNode::go_to_goal, this));
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
  
    double euclidean_dist(turtlesim::msg::Pose goal_pose)
    {
      return sqrt(pow((goal_pose.x - pose_.x), 2) + pow((goal_pose.y - pose_.y), 2));
    }

    double linear_vel(turtlesim::msg::Pose goal_pose, double constant=1.5)
    {
      return constant * euclidean_dist(goal_pose);
    }

    double steering_angle(turtlesim::msg::Pose goal_pose)
    {
      return atan2(goal_pose.y - pose_.y, goal_pose.x - pose_.x);
    }

    double angular_vel(turtlesim::msg::Pose goal_pose, double constant=6)
    {
      return constant * (steering_angle(goal_pose) - pose_.theta);
    }
    
    void spawn_turtle(turtlesim::msg::Pose goal_pose)
    {
            spawn_srv = this->create_client<turtlesim::srv::Spawn>("/spawn");
            while (!spawn_srv->wait_for_service(1s)) {
                if (!rclcpp::ok()) {
                  RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Service not available, waiting again...");
            }

            auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
            request->x = goal_pose.x;
            request->y = goal_pose.y;
            request->theta = goal_pose.theta;
            request->name = turtle_prefix + std::to_string(count);

            auto result = spawn_srv->async_send_request(request);
    }

    void kill_turtle()
    {
            kill_srv = this->create_client<turtlesim::srv::Kill>("/kill");
            while (!kill_srv->wait_for_service(1s)) {
                if (!rclcpp::ok()) {
                  RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Service not available, waiting again...");
            }

            auto request = std::make_shared<turtlesim::srv::Kill::Request>();
            request->name = turtle_prefix + std::to_string(count);
            auto result = kill_srv->async_send_request(request);
    }

    void go_to_goal()
    {
            if(spawn_first == true){
                count = 1;
                gpose.x = dist1(gen);
                gpose.y = dist1(gen);
                gpose.theta = dist2(gen);
                spawn_turtle(gpose);
                spawn_first = false;
            }

            if(euclidean_dist(gpose) >= tolerance && spawn_first == false){
                vel_msg.linear.x = linear_vel(gpose);
                vel_msg.linear.y = 0.0;
                vel_msg.linear.z = 0.0;

                vel_msg.angular.x = 0.0;
                vel_msg.angular.y = 0.0;
                vel_msg.angular.z = angular_vel(gpose);

                vel_pub->publish(vel_msg);
            }

            if(euclidean_dist(gpose) < tolerance && spawn_first == false){
                vel_msg.linear.x = 0.0;
                vel_msg.angular.z = 0.0;
                vel_pub->publish(vel_msg);
                stop = true;
            }
    }

  private:  //Klassenvariablen
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscriber_;
    rclcpp::Subscription<turtlesim::msg::Color>::SharedPtr color_subscriber_;
    size_t count_;
    turtlesim::msg::Pose pose_;  // Turtle1_pose
    turtlesim::msg::Pose gpose;  // Goal Pose
    turtlesim::msg::Color color_;
    bool spawn_first = true;
    bool stop = false;
    unsigned int count = 0;
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr spawn_srv;
    rclcpp::Client<turtlesim::srv::Kill>::SharedPtr kill_srv;
    geometry_msgs::msg::Twist vel_msg;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub;
};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  std::cout <<  " Tasten-Steurung für die TurtleSim - Druecken Sie 'W' 'A' 'S' 'D' "; //Konsolenausgabe
  rclcpp::spin(std::make_shared<TurtleSimNode>());
  rclcpp::shutdown();
  return 0;
}




