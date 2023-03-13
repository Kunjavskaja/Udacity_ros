#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "ball_chaser/DriveToTarget.h"



ros::Publisher motor_commander;

// TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities


bool drive_request_callback(ball_chaser::DriveToTarget::Request& req,
                            ball_chaser::DriveToTarget::Response& res)
{
  
  geometry_msgs::Twist velocity;

  velocity.linear.x = req.linear_x; // Set linear velocity 
  velocity.angular.z = req.angular_z; // Set angular velocity 
  
  motor_commander.publish(velocity);

  
  res.msg_feedback = "Angular velocity is " + std::to_string(velocity.linear.x) + " and angular velocity is " + std::to_string(velocity.angular.z);

  ROS_INFO_STREAM(res.msg_feedback);
  

  return true;

}

int main(int argc, char** argv)
{

  ros::init(argc, argv, "drive_bot");

  ros::NodeHandle n;

  motor_commander = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

  
  ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", drive_request_callback);

  ros::spin();


  return 0;

} 
