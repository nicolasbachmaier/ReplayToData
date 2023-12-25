#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include <Windows.h>
#include <iostream>
#include <vector>

#include "version.h"
#include "Utils.h"
#include "Player.h"
#include "Ball.h"
#include "CSVObject.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class ReplayToData: public BakkesMod::Plugin::BakkesModPlugin
	//,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	void onLoad() override;
	void onUnload() override; // Uncomment and implement if you need a unload method
	
	//Discord
    HANDLE discord_window;
    HANDLE g_hChildStd_IN_Wr = NULL; // This will be the write end of the pipe to capture parent's std::cout.

    bool StartProcessFromDLL(const wchar_t* applicationName, const wchar_t* currentDirectory) {
        SECURITY_ATTRIBUTES saAttr;
        HANDLE hChildStd_IN_Rd = NULL;  // Read end of the pipe (should be hChildStd_IN_Rd)
        HANDLE hChildStd_IN_Wr = NULL;  // Write end of the pipe (should be hChildStd_IN_Wr)

        // Set the bInheritHandle flag so pipe handles are inherited.
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;  // Inheritable handle
        saAttr.lpSecurityDescriptor = NULL;

        // Create a pipe for the child process's STDIN.
        if (!CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, 0)) {
            return false;
        }

        // Ensure the write handle to the pipe is not inherited.
        if (!SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) {
            CloseHandle(hChildStd_IN_Rd);
            CloseHandle(hChildStd_IN_Wr);
            return false;
        }

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.hStdInput = hChildStd_IN_Rd; // Corrected to the read end of the pipe
        si.dwFlags |= STARTF_USESTDHANDLES;

        ZeroMemory(&pi, sizeof(pi));

        // Start the child process with handle inheritance.
        if (!CreateProcess(
            applicationName,   // No module name (use command line)
            NULL,              // Command line
            NULL,              // Process handle not inheritable
            NULL,              // Thread handle not inheritable
            TRUE,              // Set handle inheritance to TRUE
            0,                 // No creation flags
            NULL,              // Use parent's environment block
            currentDirectory,  // Set current directory
            &si,               // Pointer to STARTUPINFO structure
            &pi)               // Pointer to PROCESS_INFORMATION structure
            ) {
            CloseHandle(hChildStd_IN_Rd);
            CloseHandle(hChildStd_IN_Wr);
            return false; // Correctly closing both handles on failure to start process
        }

        // Close the parent's copy of the read end of the pipe.
        CloseHandle(hChildStd_IN_Rd);

        // Save the write handle to the pipe, so it can be used for writing to child's stdin.
        g_hChildStd_IN_Wr = hChildStd_IN_Wr;  // Variable name changed for clarity

        discord_window = pi.hProcess;
        CloseHandle(pi.hThread);  // Correctly closing the thread handle

        return true;
    }

    bool WriteToPipe(const std::string& message) {
        if (g_hChildStd_IN_Wr == NULL) {
            LOG("PIPE NULL");
            return false; // The pipe is not valid
        }

        const std::string msg = message + "\n";

        DWORD written;
        BOOL success = WriteFile(
            g_hChildStd_IN_Wr,          // Handle to the write end of the pipe
            msg.c_str(),             // Buffer to write from
            static_cast<DWORD>(msg.size()), // Number of bytes to write
            &written,                    // Number of bytes that were written
            NULL                         // Not overlapped I/O
        );


        if (!success) {
            DWORD error = GetLastError();
            LOG("Child Std OUT Write Handle: " + std::to_string(reinterpret_cast<uintptr_t>(g_hChildStd_IN_Wr)));

            LOG("WriteFile to pipe failed with error " + std::to_string(error));

            DWORD exitCode;
            if (GetExitCodeProcess(discord_window, &exitCode)) {
                if (exitCode == STILL_ACTIVE) {
                    LOG("Child process is still active.");
                }
                else {
                    LOG("Child process exited with code " + std::to_string(exitCode));
                }
            }
            else {
                LOG("GetExitCodeProcess failed with error " + std::to_string(GetLastError()));
            }
            
        }
        //CloseHandle(g_hChildStd_IN_Wr);
        return success != 0;
    }

	
	// Variables
	CarWrapper w_car = NULL;
	CarWrapper w_enemy = NULL;
	BoostWrapper w_boost = NULL;
	bool b_gamewrapper = false;
	bool b_ingame = false;
	bool b_call_boost = false;
	ServerWrapper w_server = NULL;
	ArrayWrapper<CarWrapper> w_enemies = NULL;
	int ticks = 0;
	bool b_logreplay = false;


	// Own Functions
	void log_replay(bool activator = false);

public:
	//void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
	//void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
