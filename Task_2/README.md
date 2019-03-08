# RRT* -HARDIK AGGARWAL
first run the roscore
build package named gotogoal
now run rrt-star.cpp using COMMAND
 
g++ rrt-star.cpp -o app `pkg-config --cflags --libs opencv`
./app

YOU HAVE YOUR PATH !!!!!

now change the filepath of points.txt in commander.cpp of gotogoal according to your computer.

now run the turtlesim file commander

rosrun gotogoal commander

HURRAY!!!!
YOUR TURTLE IS TRACING THE PATH!!!
