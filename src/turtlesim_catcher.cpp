// Code gefunden in 
// https://github.com/siksal/turtle_control/blob/main/src/catch_turtle.cpp

// In TurtleSim spawnt eine zweite Turtle2 an einer zufälligen Stelle und 
// fährt an die Pose (Goal-Pose) der ersten Turtle1.
// Sobald die erste Turtle1 erreicht ist, wird sie gekillt
// Eine neue Turtle1 an einer anderen zufälligen Stelle wird gespawnt
// und die Turtle2 fährt dorthin und killt auch sie ...


// usage: $1 ros2 run turtlesim turtlesim_node
//        $2 ros2 run emr2 turtlesim_catcher    

#define _USE_MATH_FUNCTIONS
#include <random>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

// hier die vorgefertigten Nachrichten und Services der turtlesim einbinden
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/spawn.hpp"
#include "turtlesim/srv/kill.hpp"

using namespace std::chrono_literals;  //C++14 
// definiert 12 benutzerdefinierte Literale , 
// die Stunden, Minuten, Sekunden, Millisekunden, Mikrosekunden und Nanosekunden darstellen

using std::placeholders::_1;
// Platzhalter für den Kompier beim Binden von Funktionen,
// _1 ist das erste Argument im Funktionsaufruf
// wird für die Funktion bind() in create_subscrioption gebraucht 

//globals
unsigned int count = 0;  
double pose_x;
double pose_y;
double pose_theta;
const std::string turtle_prefix = "turtle_"; //Vorsilbe für alle Turtles
const double tolerance = 0.6;

bool spawn_first_flag = true;
bool stop_flag = false;

std::random_device rd;  //C++11 integer random number generator
std::mt19937 gen(rd()); 
//A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits
// https://www.sciencedirect.com/topics/computer-science/mersenne-twister

std::uniform_int_distribution<> dist1(1, 10);  
std::uniform_int_distribution<> dist2(0, 2*M_PI);
//C++11 Produces random integer values i, uniformly distributed on the closed interval [a,b]

//-----------------------------------------------------------------------------------
class TurtleCatcher : public rclcpp::Node
{
    public: //Konstruktor 
        TurtleCatcher() : Node("turtle_catcher")
        {
            // Subscriber für /turtle1/pose
            pose_sub = this->create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose", 10, std::bind(&TurtleCatcher::pose_cb, this, _1));

            // Publisher für /turtle1/cmd_vel
            vel_pub = this->create_publisher<geometry_msgs::msg::Twist>(
                "/turtle1/cmd_vel", 10);

            // Timer zum Treiben der Aktionen 
            timer = this->create_wall_timer(
                0.01s, std::bind(&TurtleCatcher::go_to_goal, this));
        }
    private:
        // Klassenvariablen instanzieren
        rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr spawn_srv;  // Spawn-Service-Client
        rclcpp::Client<turtlesim::srv::Kill>::SharedPtr kill_srv;    // Kill-Service-Client
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub;  // Pose - Msg Subscriber
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub; // Twist Msg Publishcer für cmd_vel
        rclcpp::TimerBase::SharedPtr timer; // der Timer
        geometry_msgs::msg::Twist vel_msg;  // eine Msg für cmd_vel
        turtlesim::msg::Pose gpose; // eine Msg für die Pose

        // Funktion um eine Turtle an einer gewünschten Pose (goal_pose) zu spawnen
        // call Spawn Service from C++
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

        // Funktion um eine Turtle zu killen
        // call Kill Service from C++
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
            request->name = turtle_prefix + std::to_string(count); // Name der Turtle zusammenbauen
            auto result = kill_srv->async_send_request(request);
        }

        // jedesmal wenn eine Pose (Turtle1) empfangen wird (Subscriber),
        // wird sie hier an die globalen Variablen übergeben
        void pose_cb(turtlesim::msg::Pose::SharedPtr pose)
        {
            pose_x = pose->x;
            pose_y = pose->y;
            pose_theta = pose->theta;
        }

        // Berechnet die Distanz der Turtle2 (goal_pose) zur Turtle1 (pose_x/y)
        double euclidean_dist(turtlesim::msg::Pose goal_pose)
        {
            return sqrt(pow((goal_pose.x - pose_x), 2) + pow((goal_pose.y - pose_y), 2));
        }

        // Berechnet die Geschwindigket der Turtle2 in Abhängigkeit vom Abstand zum Ziel (Turtle1)
        double linear_vel(turtlesim::msg::Pose goal_pose, double constant=1.5)
        {
            return constant * euclidean_dist(goal_pose);
        }
        // Berechnet den Winkel zum Ziel (Turtle1)
        double steering_angle(turtlesim::msg::Pose goal_pose)
        {
            return atan2(goal_pose.y - pose_y, goal_pose.x - pose_x);
        }
        // Berechnet die Dreh-Geschwindigket der Turtle2 in Abhängigkeit vom Abstand zum Ziel (Turtle1)
        double angular_vel(turtlesim::msg::Pose goal_pose, double constant=6)
        {
            return constant * (steering_angle(goal_pose) - pose_theta);
        }

        // Hier werden die Aktionen gestartet
        // go_to_goal ist eine Zustandautomat der regelmäßig vom Timer getriggert wird
        void go_to_goal()
        {   
            // Noch keine zweite Turtle gespawnt => spawne
            if(spawn_first_flag == true){
                count = 1;
                gpose.x = dist1(gen);
                gpose.y = dist1(gen);
                gpose.theta = dist2(gen);
                spawn_turtle(gpose);
                spawn_first_flag = false;
            }

            // zur Turtle1 laufen 
            if(euclidean_dist(gpose) >= tolerance && spawn_first_flag == false){
                vel_msg.linear.x = linear_vel(gpose);
                vel_msg.linear.y = 0.0;
                vel_msg.linear.z = 0.0;

                vel_msg.angular.x = 0.0;
                vel_msg.angular.y = 0.0;
                vel_msg.angular.z = angular_vel(gpose);

                vel_pub->publish(vel_msg);
            }

            //Turtle1 erreicht
            if(euclidean_dist(gpose) < tolerance && spawn_first_flag == false){
                vel_msg.linear.x = 0.0;
                vel_msg.angular.z = 0.0;
                vel_pub->publish(vel_msg);
                stop_flag = true;
            }
            // Kill Turtle1
            if(stop_flag == true){
                kill_turtle();
                gpose.x = dist1(gen);
                gpose.y = dist1(gen);
                gpose.theta = dist2(gen);
                count++;
                spawn_turtle(gpose);
                stop_flag = false;
            }
        }
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TurtleCatcher>());
    rclcpp::shutdown();
}

