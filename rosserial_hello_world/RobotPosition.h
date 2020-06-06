#pragma once
class RobotPosition {
public:
	float x;
	float y;
	float z;
	float yaw;
	RobotPosition(float x, float y, float z, float yaw);
	RobotPosition() {
		x = 0;
		y = 0;
		z = 0;
		yaw = 0;
	};
};