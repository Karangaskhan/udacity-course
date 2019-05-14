#!/bin/sh

xterm -e "roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/export/bulk/local-home/kpurohit/projects/udacity-course/homeServiceRobot/catkin_ws/src/map/karanWorld.world" &
sleep 5
xterm -e "roslaunch turtlebot_gazebo amcl_demo.launch map_file:=/export/bulk/local-home/kpurohit/projects/udacity-course/homeServiceRobot/catkin_ws/src/map/karanWorldMap.yaml" &
sleep 2
xterm -e "roslaunch turtlebot_rviz_launchers view_navigation.launch config_file:=/export/bulk/local-home/kpurohit/projects/udacity-course/homeServiceRobot/catkin_ws/src/rvizConfig/configWithMarkers.rviz" &
sleep 3
xterm -e "rosrun pick_objects pick_objects" &
sleep 1
xterm -e "rosrun add_markers add_markers"