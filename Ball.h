#pragma once
#include "pch.h"

struct Ball {
public:
	float x = 0.0, y = 0.0, z = 0.0;
	int roll = 0, yaw = 0, pitch = 0;

	float velocity_X = 0.0, velocity_Y = 0.0, velocity_Z = 0.0;
	float angular_velocity_X = 0.0, angular_velocity_Y = 0.0, angular_velocity_Z = 0.0;

	std::string to_string() {
		return "Ball\n" +
			std::to_string(x) + "|" + std::to_string(y) + "|" + std::to_string(z) + " (X|Y|Z)\n" +
			std::to_string(roll) + "|" + std::to_string(yaw) + "|" + std::to_string(pitch) + " (Roll|Yaw|Pitch)\n" +
			std::to_string(velocity_X) + "|" + std::to_string(velocity_Y) + "|" + std::to_string(velocity_Z) + " (Velocity X|Y|Z)\n" +
			std::to_string(angular_velocity_X) + "|" + std::to_string(angular_velocity_Y) + "|" + std::to_string(angular_velocity_Z) + " (Angular Velocity X|Y|Z)";
	}

	std::string to_csv_header(char seperator = ',') {
		return (std::string)"BallPos-X" + seperator + "BallPos-Y" + seperator + "BallPos-Z" + seperator +
			"BallRot-Roll" + seperator + "BallRot-Yaw" + seperator + "BallRot-Pitch" + seperator +
			"BallVel-X" + seperator + "BallVel-Y" + seperator + "BallVel-Z" + seperator +
			"BallAngVel-X" + seperator + "BallAngVel-Y" + seperator + "BallAngVel-Z" + seperator;
	}
	std::string to_csv(char seperator = ',') {
		return std::to_string(x) + seperator + std::to_string(y) + seperator + std::to_string(z) + seperator +
			std::to_string(roll) + seperator + std::to_string(yaw) + seperator + std::to_string(pitch) + seperator +
			std::to_string(velocity_X) + seperator + std::to_string(velocity_Y) + seperator + std::to_string(velocity_Z) + seperator +
			std::to_string(angular_velocity_X) + seperator + std::to_string(angular_velocity_Y) + seperator + std::to_string(angular_velocity_Z) + seperator;
	}
};
