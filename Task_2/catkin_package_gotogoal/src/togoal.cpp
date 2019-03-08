#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>


ros:: Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;

const double x_min=0.0;
const double y_min=0.0;
const double x_max=11.0;
const double y_max=11.0;
const double PI = 3.14159265359


void move(double speed,double distance,bool isForward);
void rotate(double angular_speed,double angle,bool clockwise);

int main(int argc,char **argc)
{
    ros::init(argc,argv,"robot_cleaner");
    ros::NodeHandle n;
    double speed,angular_speed,distance,angle;
    bool isForward,clockwise;

    velocity_publisher=n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10);

    ros::Rate loop_rate(10);
    ROS_INFO("\n\n\n*******START TESTING***************\n");
    cout<<"enter speed :";
    cin>>speed;
    cout<<"enter distance :";
    cin>>distance;
    cout<<"isForward :";
    cin>>isForward;
    move(speed,distance,isForward);
    
    ros::spin();

    return 0;


}

void move(double speed,double distance,bool isForward)
{
    geometry_msgs::Twist vel_msg;

    if(isForward)
        vel_msg.linear.x=abs(speed);
    else
        vel_msg.linear.x=-abs(speed);

    vel_msg.linear.y=0;
    vel_msg.linear.z=0;

    vel_msg.angular.x=0;
    vel_msg.angular.y=0;
    vel_msg.angular.z=0;

    //loop_rate.sleep();

    double t0=ros::Time::now().tosec();
    do{

        velocity_publisher.publish(vel_msg);
        current_distance=speed*(t1-t0);
    }while(current_distance<distance);


        vel_msg.linear.x=0;
        velocity_publisher.publish(vel_msg);


}

void rotate(double angular_speed,double relative_angle,bool clockwise)
{
    geometry_msgs::Twist vel_msg;

    vel_msg.linear.x=0;
    vel_msg.linear.y=0;
    vel_msg.linear.z=0;
    vel_msg.angular.x=0;
    vel_msg.angular.y=0;

    if(clockwise)
        vel_msg.angular.z=-abs(angular_speed);
    else
        vel_msg.angular.z=abs(angular_speed);

    double current_angle=0.0;
    double t0=ros::Time::now().toSec();
    {
        velocity_publisher.publish(vel_msg);
        double t1=ros::Time::now().toSec();
        current_angle=angular_speed*(t1-t0);
        ros::spinOnce();
        //loop_rate.sleep();

    }while(current_angle<relative_angle);

    vel_msg.angular.z=0;
    velocity_publisher.publish(vel_msg);

}

/*double setDesiredOrientation (double desired_angle_radians)
{
    double relative_angle_radians =desired_angle_radians-tur
}*/

