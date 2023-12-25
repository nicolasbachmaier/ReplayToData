#pragma once
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "Player.h"
#include "Ball.h"
#include "Utils.h"
#include <vector>

class CSVObject {
	GameWrapper w_game;
	std::vector<Player> players;
	Ball ball;
public:
	CSVObject(GameWrapper w_game, std::vector<Player> players, Ball ball) : w_game(w_game), players(players), ball(ball) {
		// Header
		std::string filename = "replay1.csv";
		if (!Utils::file_exists(filename)) {
			std::string value = "";
			value += ball.to_csv_header();
			int iterator = 1; // Use Array Map later for order
			for (auto player : players) {
				value += player.to_csv_header(iterator); // Include Iterator
				iterator++;
			}
			Utils::write_to_csv(value, "replay1.csv");
		}
	}

	void print_csv_line() {
		std::string value = "";
		value += ball.to_csv();
		int iterator = 1; // Use Array Map later for order
		for (auto player : players) {
			value+=player.to_csv(); // Include Iterator
			iterator++;
		}
		Utils::write_to_csv(value, "replay1.csv");
	}
};