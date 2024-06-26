
// The following program takes video stream from default camera and send it to ROS2

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/header.hpp"
#include <chrono>
#include <cv_bridge/cv_bridge.h> // cv_bridge converts between ROS 2 image messages and OpenCV image representations.
#include <image_transport/image_transport.hpp> // Using image_transport allows us to publish and subscribe to compressed image streams in ROS2
#include <opencv2/opencv.hpp> // We include everything about OpenCV as we don't care much about compilation time at the moment.

using namespace std::chrono_literals;

class MinimalImagePublisher : public rclcpp::Node {
public:
  MinimalImagePublisher() : Node("opencv_image_publisher"), count_(0) {
    publisher_ =
        this->create_publisher<sensor_msgs::msg::Image>("random_image", 10);
    timer_ = this->create_wall_timer(500ms, 
                                     std::bind(&MinimalImagePublisher::timer_callback,
                                     this));
  }

private:
  void timer_callback() {

    cv::Mat myImage;//Declaring a matrix to load the frames//
    cv::namedWindow("Video Player");//Declaring the video to show the video//
    cv::VideoCapture cap(0);//Declaring an object to capture stream of frames from default camera//
    if (!cap.isOpened()){ //This section prompt an error message if no video stream is found//
      std::cout << "No video stream detected" << std::endl;
      //system("pause");
      return;
    }
    cap >> myImage;

    // Write message to be sent. Member function toImageMsg() converts a CvImage
    // into a ROS image message
    msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", myImage)
               .toImageMsg();

    // Publish the image to the topic defined in the publisher
    publisher_->publish(*msg_.get());
    RCLCPP_INFO(this->get_logger(), "Image %ld published", count_);
    count_++;
  }
  rclcpp::TimerBase::SharedPtr timer_;
  sensor_msgs::msg::Image::SharedPtr msg_;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  // create a ros2 node
  auto node = std::make_shared<MinimalImagePublisher>();

  // process ros2 callbacks until receiving a SIGINT (ctrl-c)
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}


// https://www.tutorialspoint.com/how-to-capture-video-from-default-camera-in-opencv-using-cplusplus
// #include<opencv2/opencv.hpp>//OpenCV header to use VideoCapture class//
// #include<iostream>
// using namespace std;
// using namespace cv;
// int main() {
//    Mat myImage;//Declaring a matrix to load the frames//
//    namedWindow("Video Player");//Declaring the video to show the video//
//    VideoCapture cap(0);//Declaring an object to capture stream of frames from default camera//
//    if (!cap.isOpened()){ //This section prompt an error message if no video stream is found//
//       cout << "No video stream detected" << endl;
//       system("pause");
//       return-1;
//    }
//    while (true){ //Taking an everlasting loop to show the video//
//       cap >> myImage;
//       if (myImage.empty()){ //Breaking the loop if no video frame is detected//
//          break;
//       }
//       imshow("Video Player", myImage);//Showing the video//
//       char c = (char)waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
//       if (c == 27){ //If 'Esc' is entered break the loop//
//          break;
//       }
//    }
//    cap.release();//Releasing the buffer memory//
//    return 0;
// }


// https://www.theconstruct.ai/how-to-integrate-opencv-with-a-ros2-c-node/

// Test if opneCV is installed 
// $ pkg-config --modversion opencv4
// => 4.5.4   
// ros2 pkg create my_opencv_demo --dependencies rclcpp std_msgs sensor_msgs cv_bridge image_transport OpenCV

