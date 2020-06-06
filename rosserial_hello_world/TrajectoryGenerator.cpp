#include "TrajectoryGenerator.h"
#include <math.h>
#include <iostream>

RobotPosition getTrajectoryPoint(float t) {
	float R = 5;
	double u = 0.001;
	float x_0 = 0.0;
	float y_0 = 0.0;

	RobotPosition rpos;
	float x = std::ceil((R * cos(u * t) + x_0) * 10000) / 10000;
	float y = std::ceil((R * sin(u * t) + y_0) * 10000) / 10000;
	rpos.x = x;
	rpos.y = y;
	rpos.z = 0;
	float dx = std::ceil((-R * sin(u * t) * u) * 10000) / 10000;
	float dy = std::ceil((R * cos(u * t) * u) * 10000) / 10000;
	rpos.yaw = atan2(dy, dx);
	return rpos;
}