#include <ros/ros.h>
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services 
ros::ServiceClient srvClient;

// This function calls the command_robot service to drive the robot in the specified direction 


void drive_robot(float lin_x, float ang_z)
{
  
  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = lin_x;
  srv.request.angular_z = ang_z;
  
  
  if (!srvClient.call(srv)) {
    ROS_ERROR("Failed, error!");
  }
  
}



void process_image_callback(const sensor_msgs::Image img)
{
    int whitepixelcoord;
    int pos = 0; // track where we are in image 
    int whitepixel = 0;
    int white_pixel_thres = (2/3)* 255;
    float x,y;
    
    
    for (int i = 0; i + 2 < img.data.size(); i = i + 3)


    {//ROS_INFO("Pixel value %1.2f", (float)(img.data[i]+img.data[i+1]+img.data[i+2]));
        if (img.data[i] + img.data[i+1] + img.data[i+2] >=  white_pixel_thres)
        {
            pos += (i % (img.width * 3)) / 3;
            whitepixel++;
        }
    }
    
    if (whitepixel == 0)
    {
// no white pixels detected stay in position
        drive_robot(0, 0);
    }
    else

    { 
// white pixels detected, determine in which direction, left is pos direction, right is neg direction
        whitepixelcoord = pos / whitepixel;
        x = 0.25;
        
        if(whitepixelcoord < img.width / 3)
        {
// white pixels detected, to the left 
            y= 0.5;
        }
        else if(whitepixelcoord > img.width * 2 / 3)
        {
// white pixels detected, to the right 
            y = -0.5;
        }
        else
        {
// white pixels detected, straight ahead 
            y= 0;
        }
    drive_robot(x, y);
    }

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "process_image");
  ros::NodeHandle nh;

  srvClient = nh.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

  ros::Subscriber sub = nh.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

  ros::spin();

  return 0;

}
