// rosserial_hello_world.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

//#include <iostream>

//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln

// rosserial_hello_world.cpp : Example of sending command velocities from Windows using rosserial
//
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include "ros.h"
#include <ros/time.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseArray.h>
#include <std_msgs/String.h>
#include <std_msgs/Time.h>
#include <tf/tf.h>
#include <tchar.h>
#include <windows.h>
#include "TrajectoryGenerator.h"

using std::string;

void time_callback(const std_msgs::Time& msg)
{
	std::cout << " -> Received: " << std::to_string(msg.data.sec) <<"." << std::to_string(msg.data.nsec);
}

int _tmain(int argc, _TCHAR* argv[])
{
	ros::NodeHandle nh;
	char* ros_master = const_cast<char*>("192.168.0.15:11411");

	printf("Connecting to server at %s\n", ros_master);
	nh.initNode(ros_master);

	/*SENDING*/
	printf("Advertising cmd_vel message\n");
	RobotPosition robotPosition;
	geometry_msgs::Pose pose;
	geometry_msgs::PoseArray poseArray;
	std_msgs::String msg;
	//ros::Publisher cmd_vel_pub("husky_velocity_controller/cmd_vel", &box_marker);
	//ros::Subscriber <std_msgs::Time> poseSub("estimated_pose", &time_callback);
	ros::Publisher msgPublisher("chatter", &msg);
	ros::Subscriber <std_msgs::Time> msgSub("callback", &time_callback);
	ros::Publisher cmd_vel_pub("position", &poseArray);
	nh.subscribe(msgSub);
	//ros::Publisher cmd_vel_pub("cmd_vel", &twist_msg);
	nh.advertise(cmd_vel_pub);
	nh.advertise(msgPublisher);
	printf("Go robot go!\n");
	int sleepTimeMillis = 500;
	double timeElapsed = 0;
	poseArray.poses_length = 1;
	int i = 0;
	while (1)
	{
		robotPosition = getTrajectoryPoint(timeElapsed);
		timeElapsed += sleepTimeMillis;
		pose.position.x = robotPosition.x;
		pose.position.y = robotPosition.y;
		pose.position.z = robotPosition.z;
		pose.orientation = tf::createQuaternionFromYaw(robotPosition.yaw);
		poseArray.header.frame_id = "map";
		geometry_msgs::Pose* poses = new geometry_msgs::Pose[1];
		poses[0] = pose;
		poseArray.poses = poses;
		const auto p1 = std::chrono::system_clock::now();
		auto sendTime = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
		auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(p1.time_since_epoch()).count();
		std::string sec = std::to_string(sendTime);
		std::string ns = std::to_string(nano);
		std::string data = ns;
		//msg.data = "Message";
		msg.data = data.c_str();
		cmd_vel_pub.publish(&poseArray);
		msgPublisher.publish(&msg);
		std::cout << std::endl;
		std::cout << "Sending: ";
		std::cout << nano;
		//printf("Spinning");
		nh.spinOnce();
		Sleep(sleepTimeMillis);
		i++;
	}

	printf("All done!\n");
	return 0;
}