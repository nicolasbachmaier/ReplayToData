#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

class Utils {
public:
	static void write_to_file(const std::vector<std::string>& val, const std::string& filename);
	static void write_to_file(const float& val, const std::string& filename);

	static void write_to_csv(const std::string& val, const std::string& filename);
	
	static bool file_exists(const std::string& filename);
};