#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>

class PubSub
{
public:
  PubSub()
  {
    //Topic you want to publish
    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    odom_sub = n.subscribe("odom", 1, &PubSub::odomCallback, this);

    robotHoldingObject = false;

    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "add_markers";
    marker.id = 0;

    // Set our shape type to be a cylinder
    marker.type = visualization_msgs::Marker::CYLINDER;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = 6.0;
    marker.pose.position.y = 0.0;
    marker.pose.position.z = 0.1;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.2;
    marker.scale.y = 0.2;
    marker.scale.z = 0.2;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.3f;
    marker.color.g = 0.1f;
    marker.color.b = 0.8f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();
  }

  bool update_and_publish_marker();

private:
  ros::NodeHandle n; 
  ros::Publisher marker_pub;
  ros::Subscriber odom_sub;
  visualization_msgs::Marker marker;
  bool robotHoldingObject;

  void odomCallback(const nav_msgs::Odometry::ConstPtr& msg);

}; // End of PubSub class

bool PubSub::update_and_publish_marker()
{
    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return false;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);
    if(marker.color.a == 0.0 && robotHoldingObject == false){
      sleep(5.0);
      robotHoldingObject = true;
      marker.pose.position.x = 0.0;
      marker.pose.position.y = -4.0;
    }
    return true;
}

void PubSub::odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  float x = msg->pose.pose.position.x;
  float y = msg->pose.pose.position.y;
  ROS_INFO("X: %f\nY: %f",x,y);
  float ans = fabs(x-marker.pose.position.x);
  ROS_INFO("X DIFF = %f", ans);
  if(fabs(x-marker.pose.position.x) < 0.15f && fabs(y-marker.pose.position.y) < 0.15f && robotHoldingObject == false){
    marker.color.a = 0.0;
    ROS_INFO("Picked up object");
  }else if(fabs(x-marker.pose.position.x) < 0.15f && fabs(y-marker.pose.position.y) < 0.15f){
    marker.color.a = 1.0;
    ROS_INFO("Dropped off object");
  }
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  PubSub pubSub;

  ros::Rate r(1);
  while (ros::ok())
  {
    ros::spinOnce();
     if(!pubSub.update_and_publish_marker()){
       return 0;
     }
    r.sleep();
  }
}