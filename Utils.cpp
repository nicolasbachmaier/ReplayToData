#include "pch.h"
#include "Utils.h"

void Utils::write_to_file(const std::vector<std::string>& val, const std::string& filename) {
	std::ofstream file("C:\\Users\\nicob\\Downloads\\" + filename, std::ios::app);
	if (file.is_open() && val.empty() == false) {
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm;
		localtime_s(&now_tm, &now_c);

		for (const std::string& arg : val) {
			file << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "] " << arg << std::endl;
		}
		file.close();
	} 
	// Error Logging later
}

void Utils::write_to_file(const float& val, const std::string& filename) {
	std::ofstream file("C:\\Users\\nicob\\Downloads\\" + filename, std::ios::app);
	if (file.is_open()) {
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm;
		localtime_s(&now_tm, &now_c);
		file << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "] " << val << std::endl;
		file.close();
	}
	// Error Logging later
}

void Utils::write_to_csv(const std::string& val, const std::string& filename) {
	std::ofstream file("C:\\Users\\nicob\\Downloads\\" + filename, std::ios::app);
	if (file.is_open()) {
		file << val << "\n";
	}
}

bool Utils::file_exists(const std::string& filename) {
	std::ifstream file("C:\\Users\\nicob\\Downloads\\" + filename);
	return file.good();
}