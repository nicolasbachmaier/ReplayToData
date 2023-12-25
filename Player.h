#pragma once
#include "pch.h"

struct Player {
public:
	std::string name = "";
	unsigned char team = 0; // 0 = Blue | 1 = Orange 
	float boost = 0.0;

	float x = 0.0, y = 0.0, z = 0.0;
	int roll = 0, yaw = 0, pitch = 0;

	bool b_supersonic = false, b_canJump = true, b_jumped = false, b_doubledJumped = false, b_isdodging = false;
	float time_offGround = 0.0, time_onGround = 0.0;
	float velocity_X = 0.0, velocity_Y = 0.0, velocity_Z = 0.0;
	float angular_velocity_X = 0.0, angular_velocity_Y = 0.0, angular_velocity_Z = 0.0;
	//Team Info? 

	std::string to_string() {
		return name + ": " + std::to_string(boost) + " Boost | Team: " + std::to_string(team) + "\n" + 
			std::to_string(x) + "|" + std::to_string(y) + "|" + std::to_string(z) + " (X|Y|Z)\n" + 
			std::to_string(roll) + "|" + std::to_string(yaw) + "|" + std::to_string(pitch) + " (Roll|Yaw|Pitch)\n" +
			std::to_string(b_supersonic) + "|" + std::to_string(b_canJump) + "|" + std::to_string(b_jumped) + "|" + std::to_string(b_doubledJumped) + "|" + std::to_string(b_isdodging) + " (Supersonic|CanJump|Jumped|DoubleJumped|IsDodging)\n" +
			std::to_string(time_offGround) + "|" + std::to_string(time_onGround) + " (Time offGround|onGround)\n" +
			std::to_string(velocity_X) + "|" + std::to_string(velocity_Y) + "|" + std::to_string(velocity_Z) + " (Velocity X|Y|Z)";
	}

	std::string to_csv_header(int& iterator, char seperator = ',') {
		std::string player_id = "Player" + std::to_string(iterator); // ITERATOR = ORDER OF PLAYERS CHANGES ON NEW PLAYER SPAWN
		return player_id + "Pos-X" + seperator + player_id + "Pos-Y" + seperator + player_id + "Pos-Z" + seperator +
			player_id + "Rot-Roll" + seperator + player_id + "Rot-Yaw" + seperator + player_id + "Rot-Pitch" + seperator +
			player_id + "B-Supersonic" + seperator + player_id + "B-CanJump" + seperator + player_id + "B-Jumped" +
			seperator + player_id + "B-DoubleJumped" + seperator + player_id + "B-IsDodging" + seperator +
			player_id + "Time-OffGround" + seperator + player_id + "Time-OnGround" + seperator + player_id + "Vel-X" +
			seperator + player_id + "Vel-Y" + seperator + player_id + "Vel-Z" + seperator + player_id + "AngVel-X" +
			seperator + player_id + "AngVel-Y" + seperator + player_id + "AngVel-Z" + seperator + 
			player_id + "Team" + seperator;
	}
	std::string to_csv(char seperator = ',') {
		return std::to_string(x) + seperator + std::to_string(y) + seperator + std::to_string(z) + seperator +
			std::to_string(roll) + seperator + std::to_string(yaw) + seperator + std::to_string(pitch) + seperator +
			std::to_string(b_supersonic) + seperator + std::to_string(b_canJump) + seperator + std::to_string(b_jumped) +
			seperator + std::to_string(b_doubledJumped) + seperator + std::to_string(b_isdodging) + seperator +
			std::to_string(time_offGround) + seperator + std::to_string(time_onGround) + seperator + std::to_string(velocity_X) +
			seperator + std::to_string(velocity_Y) + seperator + std::to_string(velocity_Z) + seperator + std::to_string(angular_velocity_X) +
			seperator + std::to_string(angular_velocity_Y) + seperator + std::to_string(angular_velocity_Z) + seperator +
			std::to_string(team) + seperator;
	}
};