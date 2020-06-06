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
#include <iostream>
#include <stdio.h>
#include <chrono>
#include "ros.h"
#include <ros/time.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf/tf.h>
#include <tchar.h>
#include <windows.h>
#include "TrajectoryGenerator.h"

using std::string;

void estimated_pose_callback(const geometry_msgs::PoseWithCovarianceStamped& pose)
{
	printf("Received pose %f, %f, %f\n", pose.pose.pose.position.x,
		pose.pose.pose.position.y, pose.pose.pose.position.z);
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
	geometry_msgs::PoseStamped poseStamped;
	geometry_msgs::Pose pose;
	geometry_msgs::PoseArray poseArray;
	//ros::Publisher cmd_vel_pub("husky_velocity_controller/cmd_vel", &box_marker);
	ros::Publisher cmd_vel_pub("position", &poseArray);
	ros::Subscriber <geometry_msgs::PoseWithCovarianceStamped> poseSub("estimated_pose", &estimated_pose_callback);
	nh.subscribe(poseSub);
	//ros::Publisher cmd_vel_pub("cmd_vel", &twist_msg);
	nh.advertise(cmd_vel_pub);
	printf("Go robot go!\n");
	int sleepTimeMillis = 10;
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

		cmd_vel_pub.publish(&poseArray);
		printf("Spinning");
		nh.spinOnce();
		Sleep(sleepTimeMillis);
		i++;
	}

	printf("All done!\n");
	return 0;
}