#include "pch.h"
#include "ReplayToData.h"
#include <iostream>


BAKKESMOD_PLUGIN(ReplayToData, "Convert Replays to Trainingdata for Neural Network", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void ReplayToData::onLoad()
{
	_globalCvarManager = cvarManager;
	b_gamewrapper = false;
	b_ingame = false;
	b_call_boost = false;
	w_car = NULL;
	w_boost = NULL;
	if (gameWrapper != NULL) {
		if (gameWrapper->IsInGame() || gameWrapper->IsInOnlineGame()) {
			LOG("ingame");
			w_car = gameWrapper->GetLocalCar();
			if (!w_car.IsNull()) {
				w_boost = w_car.GetBoostComponent();
				w_server = gameWrapper->GetCurrentGameState();
				if (!w_boost.IsNull()) {
					b_call_boost = true;
				}
			}
			w_server = gameWrapper->GetCurrentGameState();
			b_ingame = true;
		}
		b_gamewrapper = true;
	}


	const wchar_t* appPath = L"C:\\Users\\nicob\\source\\repos\\discord_test\\x64\\Release\\Test\\discord_test.exe";
	const wchar_t* startInPath = L"C:\\Users\\nicob\\source\\repos\\discord_test\\x64\\Release\\Test";

	if (StartProcessFromDLL(appPath, startInPath)) {
		std::cout << "This should appear in child process input." << std::endl;
		if (WriteToPipe("Pipe initiated")) {
			LOG("Pipe Write successful");
		}
		else {
			LOG("Pipe Error");
		}
	}
	else {
		LOG("Failed to start process.");
	}


	// Function TAGame.PlayerController_TA.HandleAddBoostComponent works, but we maybe should call it every tick? Function GameEvent_Soccar_TA.Active.Tick
	gameWrapper->HookEvent("Function Engine.Interaction.Tick", [this](std::string eventName) {
		// Averages out to around 1000 Ticks per Second on my machine
		b_gamewrapper = false;
		b_ingame = false;
		b_call_boost = false;
		w_car = NULL;
		w_boost = NULL;
		if (gameWrapper != NULL) {
			if (gameWrapper->IsInGame() || gameWrapper->IsInOnlineGame()) {
				w_car = gameWrapper->GetLocalCar();
				if (!w_car.IsNull()) {
					w_boost = w_car.GetBoostComponent();
					if (!w_boost.IsNull()) {
						b_call_boost = true;
					}
				}
				w_server = gameWrapper->GetCurrentGameState();
				log_replay();
				b_ingame = true;
			}
			b_gamewrapper = true;
		}
	});

	cvarManager->registerNotifier("double_boost", [&](std::vector<std::string> args) {
		if (b_call_boost) {
			w_boost.SetCurrentBoostAmount(w_boost.GetCurrentBoostAmount() * 2);
			Utils::write_to_file(w_boost.GetCurrentBoostAmount(), "replay-to-data_args.txt");
		} else {
			LOG("Command can only be called ingame!");
		}
	}, "Doubles the Boost", 0);

	cvarManager->registerNotifier("set_boost", [&](std::vector<std::string> args) {
		if (b_call_boost) {
			Utils::write_to_file(args, "replay-to-data_args.txt");
			float new_boost = std::stof(args.at(1)) / 100;
			Utils::write_to_file(new_boost, "replay-to-data_args.txt");
			w_boost.SetCurrentBoostAmount(new_boost);
		} else {
			LOG("Command can only be called ingame!");
		}
	}, "Sets the Boost amount to the passed Argument", 0);

	cvarManager->registerNotifier("get_boost", [&](std::vector<std::string> args) {
		if (b_ingame) {
			if (!w_server.IsNull()) {
				//LOG(std::to_string(w_server.GetLocalPrimaryPlayer().name) + " Players");
				ArrayWrapper<CarWrapper> w_enemies = w_server.GetCars();
				for (auto w_enemy : w_enemies) {
					BoostWrapper w_enemy_boost = w_enemy.GetBoostComponent();
					LOG(w_enemy.GetOwnerName()+": "+std::to_string(w_enemy_boost.GetCurrentBoostAmount() * 100));
				}
			}
		}
		else {
			LOG("Command can only be called ingame!");
		}
	}, "Doubles the Boost", 0);

	cvarManager->registerNotifier("log_replay", [&](std::vector<std::string> args) {
		ReplayToData::log_replay(true);
		}, "Logs the Replay Data to a CSV", 0);

	// LOG and DEBUGLOG use fmt format strings https://fmt.dev/latest/index.html
	//DEBUGLOG("1 = {}, 2 = {}, pi = {}, false != {}", "one", 2, 3.14, true);

	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	LOG("Hello notifier!");
	//}, "", 0);

	//auto cvar = cvarManager->registerCvar("template_cvar", "hello-cvar", "just a example of a cvar");
	//auto cvar2 = cvarManager->registerCvar("template_cvar2", "0", "just a example of a cvar with more settings", true, true, -10, true, 10 );

	//cvar.addOnValueChanged([this](std::string cvarName, CVarWrapper newCvar) {
	//	LOG("the cvar with name: {} changed", cvarName);
	//	LOG("the new value is: {}", newCvar.getStringValue());
	//});

	//cvar2.addOnValueChanged(std::bind(&ReplayToData::YourPluginMethod, this, _1, _2));

	// enabled decleared in the header
	//enabled = std::make_shared<bool>(false);
	//cvarManager->registerCvar("TEMPLATE_Enabled", "0", "Enable the TEMPLATE plugin", true, true, 0, true, 1).bindTo(enabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&TEMPLATE::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&ReplayToData::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(bind(&TEMPLATE::Render, this, std::placeholders::_1));


	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", [this](std::string eventName) {
	//	LOG("Your hook got called and the ball went POOF");
	//});
	// You could also use std::bind here
	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", std::bind(&ReplayToData::YourPluginMethod, this);
}
void ReplayToData::onUnload() {
	BOOL result = TerminateProcess(discord_window, 0);

	if (result == 0) {
		DWORD error = GetLastError();
	}
	CloseHandle(discord_window);
	LOG("UNLOADED PLUGIN");
}

// Find a way to bypass the Random Junk Values in Online Games by manipulating the b_onlinegame directly through memory? 
void ReplayToData::log_replay(bool activator) {
	ticks++;
	if(activator) b_logreplay = !b_logreplay;
	if (b_logreplay && !w_server.IsNull()) {
		if (ticks >= 500) { // 32x/sec -> set it to 31x for 32fps
			ticks = 0;
			ArrayWrapper<CarWrapper> w_enemies = w_server.GetCars();
			std::vector<Player> w_players;
			if(!w_enemies.IsNull()) {
				for (auto w_enemy : w_enemies) {
					if(!w_enemy.IsNull()) {
						BoostWrapper w_enemy_boost = w_enemy.GetBoostComponent();
						if(!w_enemy_boost.IsNull()) {
							Player player;
							player.name = w_enemy.GetOwnerName();
							player.team = w_enemy.GetTeamNum2();
							player.boost = w_enemy_boost.GetCurrentBoostAmount() * 100.0;
							player.x = w_enemy.GetLocation().X;
							player.y = w_enemy.GetLocation().Y;
							player.z = w_enemy.GetLocation().Z;
							player.yaw = w_enemy.GetRotation().Yaw;
							player.pitch = w_enemy.GetRotation().Pitch;
							player.roll = w_enemy.GetRotation().Roll;

							player.b_supersonic = w_enemy.GetbSuperSonic();
							//player.b_canJump = w_enemy.GetbCanJump(); // Always True??
							player.b_canJump = w_enemy.HasFlip(); // Has_Flip works tho
							player.b_jumped = w_enemy.GetbJumped(); 
							player.b_doubledJumped = w_enemy.GetbDoubleJumped();
							player.b_isdodging = w_enemy.IsDodging();

							player.time_offGround = w_enemy.GetTimeOffGround();
							player.time_onGround = w_enemy.GetTimeOnGround();
							
							player.velocity_X = w_enemy.GetVelocity().X;
							player.velocity_Y = w_enemy.GetVelocity().Y;
							player.velocity_Z = w_enemy.GetVelocity().Z;
							player.angular_velocity_X = w_enemy.GetAngularVelocity().X;
							player.angular_velocity_Y = w_enemy.GetAngularVelocity().Y;
							player.angular_velocity_Z = w_enemy.GetAngularVelocity().Z;

							w_players.push_back(player);
							if (!WriteToPipe(player.to_string())) {
								LOG("Pipe Error");
							}
						}
					}

					if (!WriteToPipe("- - - - - - - - -\n")) {
						LOG("Pipe Error");
					}
				}
			}
			Ball ball;
			ball.x = w_server.GetBall().GetLocation().X;
			ball.y = w_server.GetBall().GetLocation().Y;
			ball.z = w_server.GetBall().GetLocation().Z;
			ball.roll = w_server.GetBall().GetRotation().Roll;
			ball.yaw = w_server.GetBall().GetRotation().Yaw;
			ball.pitch = w_server.GetBall().GetRotation().Pitch;
			ball.velocity_X = w_server.GetBall().GetVelocity().X;
			ball.velocity_Y = w_server.GetBall().GetVelocity().Y;
			ball.velocity_Z = w_server.GetBall().GetVelocity().Z;
			ball.angular_velocity_X = w_server.GetBall().GetAngularVelocity().X;
			ball.angular_velocity_Y = w_server.GetBall().GetAngularVelocity().Y;
			ball.angular_velocity_Z = w_server.GetBall().GetAngularVelocity().Z;

			CSVObject csvObject(*gameWrapper, w_players, ball);
			csvObject.print_csv_line();
			if (!WriteToPipe(ball.to_string())) {
				LOG("Pipe Error");
			}
			if (!WriteToPipe("- - - - - - - - -\n")) {
				LOG("Pipe Error");
			}
		}
	}
}