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
#include <stdio.h>
#include "ros.h"
#include <geometry_msgs/Twist.h>
#include <tchar.h>
#include <windows.h>

using std::string;

int _tmain(int argc, _TCHAR* argv[])
{
    ros::NodeHandle nh;
    char* ros_master = const_cast<char*>("192.168.0.15:11411");

    printf("Connecting to server at %s\n", ros_master);
    nh.initNode(ros_master);

    printf("Advertising cmd_vel message\n");
    geometry_msgs::Twist twist_msg;
    ros::Publisher cmd_vel_pub("cmd_vel", &twist_msg);
    nh.advertise(cmd_vel_pub);

    printf("Go robot go!\n");
    while (1)
    {
        twist_msg.linear.x = 5.1;
        twist_msg.linear.y = 0;
        twist_msg.linear.z = 0;
        twist_msg.angular.x = 0;
        twist_msg.angular.y = 0;
        twist_msg.angular.z = -1.8;
        cmd_vel_pub.publish(&twist_msg);

        nh.spinOnce();
        Sleep(100);
    }

    printf("All done!\n");
    return 0;
}