#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"

// This header is generated from your msg/RobotStatus.msg file during build
#include "ros2_custom_msgs/msg/robot_status.hpp"

using namespace std::chrono_literals;

/**
 * A ROS2 Node that publishes the status of a robot at 1Hz.
 */
class StatusPublisher : public rclcpp::Node
{
public:
    StatusPublisher()
        : Node("status_publisher"), battery_level_(100.0), mission_count_(0)
    {
        // Initialize the publisher on the "/robot_status" topic
        publisher_ = this->create_publisher<ros2_custom_msgs::msg::RobotStatus>("/robot_status", 10);

        // Create a timer that triggers the callback every 1000ms (1 second)
        timer_ = this->create_wall_timer(
            1000ms, std::bind(&StatusPublisher::timer_callback, this));
            
        RCLCPP_INFO(this->get_logger(), "Status Publisher Node has started.");
    }

private:
    void timer_callback()
    {
        // 1. Create a new message instance
        auto message = ros2_custom_msgs::msg::RobotStatus();

        // 2. Assign values based on assignment requirements
        message.robot_name = "Explorer1";
        message.battery_level = battery_level_;
        message.is_active = true;
        message.mission_count = mission_count_;

        // 3. Log the status to the console (matches expected output format)
        RCLCPP_INFO(this->get_logger(), "Publishing: robot=%s, battery=%.1f, active=%s, missions=%d",
                    message.robot_name.c_str(), 
                    message.battery_level,
                    message.is_active ? "true" : "false", 
                    message.mission_count);

        // 4. Publish the message
        publisher_->publish(message);

        // 5. Update state for the next tick
        battery_level_ -= 0.5;  // Decreasing by 0.5 each tick
        mission_count_++;       // Incrementing from 0
    }

    // Member variables
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<ros2_custom_msgs::msg::RobotStatus>::SharedPtr publisher_;
    
    double battery_level_;
    int32_t mission_count_;
};

int main(int argc, char *argv[])
{
    // Initialize ROS2 communication
    rclcpp::init(argc, argv);
    
    // Spin the node so callbacks can be processed
    rclcpp::spin(std::make_shared<StatusPublisher>());
    
    // Shutdown cleanly
    rclcpp::shutdown();
    return 0;
}
