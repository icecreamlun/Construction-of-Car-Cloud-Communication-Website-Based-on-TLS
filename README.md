# Construction of Car Cloud Communication Website Based on TLS

## Introduction

This project has established a website based on TLS encryption using the Songling autonomous driving vehicle [Scoutmini])(https://global.agilex.ai/products/scout-mini) as the server. By logging in to the website, various data of the vehicle, including camera data, can be viewed in real-time.

This software package has two parts, the ugv_sdk part comes from the https://github.com/agilexrobotics/ugv_sdk , I modify some files in it to send the data of the car to the mysql. The go_backend use golang to build the website backend, and use template to build the UI.

## Supported Platforms

### Robot bases

* Scout
* Scout Mini

### Software environments

* Architecture: x86_64/arm64
* OS: Ubuntu 18.04/20.04/22.04
* ROS: Melodic/Noetic/Foxy/Humble

It should also work in other similar Linux environments but only the above listed environments are regularly tested.

## User Guide

### UGV_SDK

#### Change 

I change the scout_demo file, add a new file named scout_robot_v3.cpp to add the data to the database, as well as change the CMakeLists.txt

#### Install dependencies

```
$ sudo apt-get update
$ sudo apt-get install build-essential git cmake libasio-dev
```

#### Build the package in a catkin workspace

```
$ cd <your-catkin-ws>/src
$ git clone ...(ugv_sdk)
$ cd ..
$ catkin_make
```
#### Run Scout_Robot_V3 Sample

First you need to build the MySQL table to hold the data
[the sql in car](/images/sql_in_car.png)

Second you need to open the chassis and video topic on the car

```
$ rosrun scout_bringup bringup_can2usb.bash
$ roslaunch scout_base scout_base.launch

$ roslaunch realsense2_camera rs_camera.launch
$ rosrun web_video_server web_video_server //install web_video_server before run
```


Third you need to find the build file and run the demo
```
$ ./bin/demo/scout_robot_v3 can0
```
[sql_state](/images/sdk_state.png)
[robotstate2details](/images/robotstate2details.png)

### GO_Backend

#### Run the project

Build User table in MySQL which include ID username password

Go into the cert file to generate server.key and server.crt, you can use openssl

Run the code

```
$ go mod init your_project_name
$ go mod tidy
$ go run main.go
```

Open the website, and now you will see the register and login UI

```
https://your_robot_id:8081/
```

[login_UI](/images/login_rigester.png)
[carinformation_UI](/images/car_information.png)
[video_UI](/images/video.png)

## Important Nodes

This project is built under the same LAN, using a self signed certificate for encryption, the client needs to load the self signed certificate.
In order to protect the website, I added a middleware, without login, you can't contect the server, and client does't have the rights to change database.

Any problem you met, you can email me at zl5583@nyu.edu, I will be happy to connect with you.