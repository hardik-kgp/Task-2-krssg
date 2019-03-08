#include <bits/stdc++.h>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Kill.h>

using namespace std;

struct point{
    float x;
    float y;
};

turtlesim::Pose pos;



void position(const turtlesim::Pose::ConstPtr& msg){
    pos.x = msg->x;
    pos.y = msg->y;
    pos.theta = msg->theta;
}


int main(int argc, char** argv){
    ros::init(argc, argv, "path_tracer");
    vector<point> points;
ros::NodeHandle n;
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
    ros::Subscriber sub =  n.subscribe("/turtle1/pose", 10, &position);
    
    ifstream myfile;
    myfile.open("/home/hardik/RRT/points.txt",std::ios::app);
    float value;
    if(myfile.is_open()){
        while(myfile>>value){
            point tmp;
            tmp.x = value*11.0/512.0;
            myfile>>value;
            tmp.y = value*11.0/512.0;
            points.push_back(tmp);
        }
    }
    myfile.close();

    ros::ServiceClient clientkill = n.serviceClient<turtlesim::Kill>("kill");
    turtlesim::Kill::Request req1;
    turtlesim::Kill::Response res1;
    req1.name = "turtle1";
    clientkill.call(req1, res1);

    ros::ServiceClient clientspawn = n.serviceClient<turtlesim::Spawn>("spawn");
    turtlesim::Spawn::Request req;
    turtlesim::Spawn::Response res;
    req.x = 0.0;
    req.y = 0.0;
    req.theta = 0;
    req.name = "turtle1";
    clientspawn.call(req, res);
       int j=1, i=0;
    ros::Rate loop_rate(5);
    while(ros::ok()){
        if(i<3){
            i++;
            continue;
        }
        geometry_msgs::Twist msg;
        while(j<points.size()){
            float theta = (points[j].x-pos.x!=0)? atan((float)(points[j].y-pos.y)/(float)(points[j].x-pos.x)) : 1.570793;
            //if(theta>0&&j>2)
            cout<<"Current position"<<pos.x<<" "<<pos.y<<" "<<pos.theta<<'\n';
            if(pos.theta<0&&theta>0&&(theta-pos.theta)>=1.57)
            {   while(abs(-3.14+theta-pos.theta)>0.001)
                {msg.linear.x=0;
                msg.angular.z=5*(-3.14+theta-pos.theta);
                pub.publish(msg);
                ros::spinOnce();
                }
            }
            //theta=-3.14159+theta;
            else
            { while(abs(theta-pos.theta)>0.001){
                msg.linear.x = 0;
                msg.angular.z = 5*(theta-pos.theta);
                pub.publish(msg);
                ros::spinOnce();            }
            }
            while(sqrt((points[j].x-pos.x)*(points[j].x-pos.x)+(points[j].y-pos.y)*(points[j].y-pos.y))>=0.1){
                msg.linear.x = 5*sqrt((points[j].x-pos.x)*(points[j].x-pos.x)+(points[j].y-pos.y)*(points[j].y-pos.y));
                msg.angular.z = 0;
                pub.publish(msg);
                
                ros::spinOnce();
            }
            j++;
        }   
        msg.linear.x = 0;
        msg.angular.z = 0;
        pub.publish(msg);
        ros::spinOnce();
    }
    
}




