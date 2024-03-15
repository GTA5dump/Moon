#include "MainScript.hpp"
#include "ScriptCallback.hpp"
#include "Lists.hpp"
#include "Natives.hpp"
#include "Timer.hpp"
#include "Functions.h"
#include "Translation.hpp"
#include "UI/UIManager.hpp"
#include "UI/BoolOption.hpp"
#include "UI/ChooseOption.hpp"
#include "UI/NumberOption.hpp"
#include "UI/RegularOption.hpp"
#include "UI/SubOption.hpp"
#include "UI/RegularSubmenu.hpp"
#include "UI/PlayerSubmenu.hpp"
#include "Hashes.h"
#include "YTDLoader.h"
#include <discord_rpc.h>
#include <discord_register.h>
namespace Big
{
	std::size_t WeatherTypePos = 0;
	std::vector<std::string> WeatherTypes =
	{
		"EXTRASUNNY",
		"CLEAR",
		"CLOUDS",
		"SMOG",
		"FOGGY",
		"OVERCAST",
		"RAIN",
		"THUNDER",
		"CLEARING",
		"NEUTRAL",
		"SNOW",
		"BLIZZARD",
		"SNOWLIGHT",
		"XMAS",
		"HALLOWEEN",
		"BLACK SCREEN"
	};
	void TPto(Vector3 Coords)
	{
		int Handle = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_IN_ANY_VEHICLE(Handle, 0))
		{
			ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_IN(Handle, false), Coords.x, Coords.y, Coords.z, 0, 0, 0, 1);
		}
		else
			ENTITY::SET_ENTITY_COORDS(Handle, Coords.x, Coords.y, Coords.z, 0, 0, 0, 1);
	}
	Ini* settingsIni = new Ini(DocumentsPath());
	std::string username = settingsIni->GetString("Login", "Username");
	std::string password = settingsIni->GetString("Login", "Password");
	static float damage = { 0.f };
	static float spread = { 0.f };
	static float recoil = { 0.f };
	static float range = { 0.f };
	static float reloadtime = { 0.f };
	static float muzzlevelocity = { 0.f };
	static int PlateIndexs = { 0 };
	static int EntityAlpha = { 0 };
	bool protex13 = false;
	bool protex14 = false;
	bool protex15 = false;
	bool protex16 = false;
	enum Submenu : std::uint32_t
	{
		SubmenuHome,
		SubmenuSettings,
		SubmenuExtra,
		Credits,
		Outfits,
		SubmenuSettingsHeader,
		AllPlayers,
		SubmenuSettingsHeaderStaticBackground,
		SubmenuSettingsHeaderGradientBackground,
		SubmenuSettingsHeaderText,
		SubmenuSettingsSubmenuBar,
		SubmenuSettingsOption,
		SubmenuSettingsFooter,
		SubmenuSettingsDescription,
		SubmenuSettingsInput,
		SubmenuSettingsLanguage,
		SubmenuSelectedPlayer,
		SubmenuPlayerList,
		SubmenuTest,
		LOCAL,
		MODELS,
		CONTACTS,
		NAMECHANGE,
		SELECTEDATTACH,
		PEDSPAWN,
		ENEMY,
		BODYGUARD,
		TELEPORTS,
		TELEPORTALL,
		SCRIPTSALL,
		TELEPORTSELF,
		SCRIPTS,
		INVITES,
		MESSAGES,
		VEHICLES,
		VEHICLESPAWN,
		WEAPONS,
		MOBILELSC,
		WEAPONSTATS,
		VEHICLECOLOR,
		VEHICLESTATS,
		IPLs,
		WEATHER,
		MISC,
		PROTECTIONS,
		ScriptHookV,
		CHATCOMMANDS,
		RECOVERY,
		UNLOCKS,
		TUNABLES,
		CASINO_TUNABLES,
		ONLINE,
		VIEW_REPORTS,
		DEV
	};

	bool MainScript::IsInitialized()
	{
		return m_Initialized;
	}
	ScriptType MainScript::GetType()
	{
		return ScriptType::Game;
	}
	void MainScript::Initialize()
	{
		m_Initialized = true;
		using namespace UserInterface;
		g_YtdLoader->init();
		g_YtdLoader->loadYtd(DocumentsPatha() + "\\Moonlight.ytd", "Moonlight.ytd");
		ScriptEventLog("~g~Successfully Injected");
		ScriptEventLog("~g~[OpenKey]: ~b~F4");
		ScriptEventLog("~g~Other keys can be found from the auth.ini file in Documents/Moonlight");
		settingsIni->WriteString(username, "Login", "Username");
		settingsIni->WriteString(password, "Login", "Password");
		g_UiManager->AddSubmenu<RegularSubmenu>("Home", SubmenuHome, [](RegularSubmenu* sub)
		{
			if (loggedin)
			{
				sub->AddOption<SubOption>("Local", nullptr, LOCAL);
				sub->AddOption<SubOption>("Players", nullptr, SubmenuPlayerList);
				sub->AddOption<SubOption>("Online", nullptr, ONLINE);
				sub->AddOption<SubOption>("Vehicles", nullptr, VEHICLES);
				sub->AddOption<SubOption>("Weapons", nullptr, WEAPONS);
				sub->AddOption<SubOption>("Teleports", nullptr, TELEPORTSELF);
				sub->AddOption<SubOption>("World", nullptr, WEATHER);
				if (true)
				{
					sub->AddOption<SubOption>("ScriptHookV", nullptr, ScriptHookV);
				}
				sub->AddOption<SubOption>("Recovery", nullptr, RECOVERY);
				sub->AddOption<SubOption>("Settings", nullptr, SubmenuExtra);
				//sub->AddOption<SubOption>("~r~Developer", nullptr, DEV);
			}
			else
			{
				is_authed(username, password);
				sub->AddOption<RegularOption>("Click to input Username", "Enter your Moonlight Username", [] {
					g_FiberScript->addTask([] {
						GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Enter Username", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
						while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0)
						{
							IsKeyboardActive = true;
							g_UiManager->ResetInput();
							g_FiberScript->Wait(0);
						}
						IsKeyboardActive = false;
						if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())
							return 0;
						username = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
					});

				});
				sub->AddOption<RegularOption>("Click to input Password", "Enter your Moonlight Password", [] {
					g_FiberScript->addTask([] {
						GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Enter Password", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
						while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0)
						{
							IsKeyboardActive = true;
							g_UiManager->ResetInput();
							g_FiberScript->Wait(0);
						}
						IsKeyboardActive = false;
						if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())
							return 0;
						password = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
						});

					});
				sub->AddOption<RegularOption>(std::move(RegularOption("Entered Username:").SetRightText(username.c_str())));
				sub->AddOption<RegularOption>(std::move(RegularOption("Entered Password:").SetRightText(password.c_str())));
				sub->AddOption<RegularOption>("Login", "Wait 4-6 seconds after pressing!", [] { is_authed(username, password); });

			}
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("~r~Developer", DEV, [](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Show IP Addresses", nullptr, &Features::ShowIPs, BoolDisplay::OnOff, false, [] { });
				sub->AddOption<RegularOption>("Instant BST", "", [] {globalHandle(2437022 + 1 + PLAYER::PLAYER_ID() * 406 + 196).As<bool>() = true;
					globalHandle(2437022).At(3876).As<int>() = NETWORK::GET_NETWORK_TIME(); });
				
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Local", LOCAL, [](RegularSubmenu* sub)
		{
			//sub->AddOption<SubOption>("Models", nullptr, MODELS);
			sub->AddOption<SubOption>("Outfits", nullptr, Outfits);
			sub->AddOption<BoolOption<bool>>("Godmode", nullptr, &Features::GodModeBool, BoolDisplay::OnOff, false, [] { Features::GodMode(); });
			sub->AddOption<BoolOption<bool>>("No Ragdoll", nullptr, &Features::RagdollBool, BoolDisplay::OnOff, false, [] { Features::NoRagdoll(); });
			sub->AddOption<BoolOption<bool>>("Never Wanted", nullptr, &Features::NeverWantedBool, BoolDisplay::OnOff, false, [] { Features::NeverGetWanted(); });
			sub->AddOption<BoolOption<bool>>("Slow Motion", nullptr, &Features::SlowMoBool, BoolDisplay::OnOff, false, [] { Features::SlowMo(); });
			sub->AddOption<BoolOption<bool>>("No Clip", nullptr, &Features::freecambool, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("Super Jump", nullptr, &Features::SuperJumpBool, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("Anti-AFK", nullptr, &Features::AFKNo, BoolDisplay::OnOff, false, [] { Features::AntiAFK(); });
			sub->AddOption<BoolOption<bool>>("Speed Run", nullptr, &Features::RunningBool, BoolDisplay::OnOff, false, [] { Features::RunningFast(); });
			sub->AddOption<BoolOption<bool>>("Fast Swim", nullptr, &Features::BackstrokePro, BoolDisplay::OnOff, false, [] { Features::BackstrokeSpeed(); });
			sub->AddOption<BoolOption<bool>>("Disable Phone", nullptr, &Features::DisableCell, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Invisibility", nullptr, &Features::InvisibilityBool, BoolDisplay::OnOff, false, [] { Features::Invisibility(); });
			sub->AddOption<BoolOption<bool>>("Mobile Radio", nullptr, &Features::mobileRadio, BoolDisplay::OnOff, false, [] { Features::mobiletunes(); });
			sub->AddOption<BoolOption<bool>>("Tiny Player", nullptr, &Features::TinyPlayerBool, BoolDisplay::OnOff, false, [] { Features::TinyPlayer(); });
			sub->AddOption<BoolOption<bool>>("Forcefield", nullptr, &Features::Forcefield, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("Superman", nullptr, &Features::superman, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<NumberOption<std::int32_t>>("Wanted Level", nullptr, &Features::WantedLevel, 0, 5, 1, 3, true, "", "", [] {
				Features::ChangeWanted(Features::WantedLevel);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {
						Features::WantedLevel = getInputs();
						if (Features::WantedLevel > 5)
							Features::WantedLevel = 5;
						if (Features::WantedLevel < 0)
							Features::WantedLevel = 0;
						Features::ChangeWanted(Features::WantedLevel);
					});
				}
			});
			sub->AddOption<RegularOption>("Heal Player", "", [] { Features::HealPlayer(); });
			sub->AddOption<RegularOption>("Suicide", "", [] { Memory::set_value<float>({ OFFSET_PLAYER , OFFSET_ENTITY_HEALTH }, 0); });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Name Options", NAMECHANGE, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Change Name", "", [] { 
					g_FiberScript->addTask([] {
						GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
						while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0)
						{
							IsKeyboardActive = true;
							g_UiManager->ResetInput();
							g_FiberScript->Wait(0);
						}
						IsKeyboardActive = false;
						if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())
							return 0;
						Features::SetName(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT());
					});
			
			});
			sub->AddOption<RegularOption>("~r~Red Name", "", [] { Features::SetName("~r~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~b~Blue Name", "", [] { Features::SetName("~b~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~g~Green Name", "", [] { Features::SetName("~g~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~y~Yellow Name", "", [] { Features::SetName("~y~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~p~Purple Name", "", [] { Features::SetName("~p~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~o~Orange Name", "", [] { Features::SetName("~o~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~c~Grey Name", "", [] { Features::SetName("~c~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~m~Dark Grey Name", "", [] { Features::SetName("~m~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~u~Black Name", "", [] { Features::SetName("~u~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~s~White Name", "", [] { Features::SetName("~s~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~d~Dark Blue Name", "", [] { Features::SetName("~d~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~f~Light Blue Name", "", [] { Features::SetName("~f~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~bold~Bold Name", "", [] { Features::SetName("~bold~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~italic~Italic Name", "", [] { Features::SetName("~italic~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~ws~Wanted Star Name", "", [] { Features::SetName("~ws~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
			sub->AddOption<RegularOption>("~n~Skip Line Name", "", [] { Features::SetName("~n~" + (string)PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())); });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Online", ONLINE, [](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Modder Protections", nullptr, PROTECTIONS);
				sub->AddOption<SubOption>("Online Requests", nullptr, CONTACTS);
				sub->AddOption<SubOption>("Chat Commands", nullptr, CHATCOMMANDS);
				sub->AddOption<SubOption>("Name Changer", nullptr, NAMECHANGE);
				sub->AddOption<BoolOption<bool>>("ESP", nullptr, &Features::espbool, BoolDisplay::OnOff, false, [] {});
				sub->AddOption<BoolOption<bool>>("Name ESP", nullptr, &Features::Nametag, BoolDisplay::OnOff, false, [] {});
				sub->AddOption<BoolOption<bool>>("Off The Radar", nullptr, &Features::OffRadarBool, BoolDisplay::OnOff, false, [] { Features::OffRadar(); });
				sub->AddOption<RegularOption>("R*ID Join", "", [] {
					g_FiberScript->addTask([] {
						GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
						while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0)
						{
							IsKeyboardActive = true;
							g_UiManager->ResetInput();
							g_FiberScript->Wait(0);
						}
						IsKeyboardActive = false;
						if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())
							return 0;
						Join = atoi(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT());
						Joiner = true;
						ScriptEventLog("~w~R*ID attached!");
						ScriptEventLog("~g~Now join anyone from your crew members, and you will be redirected to the R*ID player.");
						});

					});
				sub->AddOption<ChooseOption<std::string, std::size_t>>("Lobby Weather", nullptr, &WeatherTypes, &WeatherTypePos, false, [] { Features::SetWeather(WeatherTypePos); });
				sub->AddOption<BoolOption<bool>>("Lobby Time Spam", nullptr, &Features::TimeSpamBool, BoolDisplay::OnOff, false, [] {});
				sub->AddOption<NumberOption<std::int32_t>>("Lobby Time:", nullptr, &Features::TimeHour, 0, 23, 1, 1, true, "", "", [] { Features::SetLobbyTime(Features::TimeHour, 0, 0); });
			});
#include "SHVASILoader.h"
#include "SHVScriptEngine.h"

		g_UiManager->AddSubmenu<RegularSubmenu>("ScriptHookV", ScriptHookV, [](RegularSubmenu* sub)
		{
			namespace fs = std::filesystem;
			fs::directory_iterator dirIt{ DocumentsPatha() + "\\ScriptHookV\\" };
			for (auto&& dirEntry : dirIt)
			{
				if (dirEntry.is_regular_file())
				{
					auto path = dirEntry.path();
					if (path.has_filename())
					{
						if (path.extension() == ".asi")
						{
							sub->AddOption<RegularOption>(path.stem().u8string().c_str(), nullptr, [=]
							{
								Features::LoadPlugins(path);
							});
						}

					}
				}
			}
			//ScriptEngine::Initialize()

		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Models", MODELS, [](RegularSubmenu* sub)
		{
			for (auto model : pedModels)
			{
				sub->AddOption<RegularOption>(model, "", [model] { applyChosenSkin(model); });
			}
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Chat Commands", CHATCOMMANDS, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Enable", nullptr, &Features::ChatCommands, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/vehicle", nullptr, &Features::SlashVehicle, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/cash", nullptr, &Features::SlashCash, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/otr", nullptr, &Features::SlashOtr, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/day", nullptr, &Features::SlashDay, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/night", nullptr, &Features::SlashNight, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/snow", nullptr, &Features::SlashSnow, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/clear", nullptr, &Features::SlashClear, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/weapons", nullptr, &Features::SlashWeapons, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/neverwanted", nullptr, &Features::SlashNeverWanted, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("/clone", nullptr, &Features::SlashClone, BoolDisplay::OnOff, false, [] { });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Online Requests", CONTACTS, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Helicopter Backup", "", [] { globalHandle(2528542).At(4380).As<int>() = 1; });
			sub->AddOption<RegularOption>("Helicopter Pickup", "", [] { globalHandle(2528542).At(826).As<int>() = 1; });
			sub->AddOption<RegularOption>("Boat Pickup", "", [] { globalHandle(2528542).At(825).As<int>() = 1; });
			sub->AddOption<RegularOption>("Ammo Drop", "", [] { globalHandle(2528542).At(824).As<int>() = 1; });
			sub->AddOption<RegularOption>("Crate Drop", "", [] { globalHandle(2528542).At(834).As<int>() = 1; });
			sub->AddOption<RegularOption>("Airstrike", "", [] { globalHandle(2528542).At(4381).As<int>() = 1; });
		});
		g_UiManager->AddSubmenu<PlayerSubmenu>(&g_SelectedPlayer, SubmenuSelectedPlayer, [](PlayerSubmenu* sub)
		{
			Features::LoadPlayerInfo(g_SelectedPlayer);
			Ped SelectedCoords = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
			Vector3 SelectCoords = ENTITY::GET_ENTITY_COORDS(SelectedCoords, true);
			GRAPHICS::DRAW_MARKER(2, SelectCoords.x, SelectCoords.y, SelectCoords.z + 1.25, 0, 0, 0, 0, 180, 0, 0.25, 0.25, 0.25, 200, 94, 100, 255, 1, 1, 1, 0, 0, 0, 0);
			sub->AddOption<SubOption>("Attach Objects", nullptr, SELECTEDATTACH);
			sub->AddOption<SubOption>("Teleports", nullptr, TELEPORTS);
			sub->AddOption<SubOption>("Script Events", nullptr, SCRIPTS);
			sub->AddOption<SubOption>("Ped Spawns", nullptr, PEDSPAWN);
			sub->AddOption<ChooseOption<std::string, std::size_t>>("Player Weather", nullptr, &WeatherTypes, &WeatherTypePos, false, [] { Features::SetWeathers(WeatherTypePos); });
			sub->AddOption<NumberOption<std::int32_t>>("Wanted Level:", nullptr, &Features::TimeHour, 0, 5, 1, 3, true, "", "", [] { 
				Features::SetWanted(Features::TimeHour); 
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::TimeHour = getInputs();
						if (Features::TimeHour > 5)
							Features::TimeHour = 5;
						if (Features::TimeHour < 0)
							Features::TimeHour = 0;
						Features::SetWanted(Features::TimeHour);
					});
				}
			});
			sub->AddOption<BoolOption<bool>>("Spectate", nullptr, &Features::SpectateList[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::Spectate(Features::SpectateList[g_SelectedPlayer]); });
			sub->AddOption<BoolOption<bool>>("Fuck Their Cam", nullptr, &Features::FuckCamBool, BoolDisplay::OnOff, false, [] {  });
			sub->AddOption<BoolOption<bool>>("Explode", nullptr, &Features::ExplodeList[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::ExplodeLoopPlayer(Features::ExplodeList[g_SelectedPlayer]); });
			sub->AddOption<BoolOption<bool>>("Cash Drop", nullptr, &Features::CashDropRemote[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::CashDropV2(Features::CashDropRemote[g_SelectedPlayer]); });
			sub->AddOption<BoolOption<bool>>("Water Loop", nullptr, &Features::WaterList[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::WaterLoop(Features::WaterList[g_SelectedPlayer]); });
			sub->AddOption<BoolOption<bool>>("Fire Loop", nullptr, &Features::FireList[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::FireLoop(Features::FireList[g_SelectedPlayer]); });
			sub->AddOption<BoolOption<bool>>("Freeze", nullptr, &Features::FreezeList[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::Freeze(Features::FreezeList[g_SelectedPlayer]); });
			sub->AddOption<RegularOption>("Teleport To Player", "", [] { Features::TeleportToPlayer(g_SelectedPlayer); });
			sub->AddOption<RegularOption>("Crash", "", [] { 
				Features::Bodyguard((char*)"slod_human"); 
				});
			sub->AddOption<RegularOption>("Cage Player", "", [] { Features::TrapPlayer(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)); });
			sub->AddOption<RegularOption>("Blame Player", "", [] { Features::BlamePlayer(g_SelectedPlayer); });
			sub->AddOption<RegularOption>("Airstrike Player", "", [] { Features::AirStrikeKill(); });
			sub->AddOption<RegularOption>("Give Wanted", "", [] { Features::RemoteLevels(g_SelectedPlayer); });
			sub->AddOption<RegularOption>("Remove Weapons", "", [] { WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true); });
			sub->AddOption<RegularOption>("Give Weapons", "", [] { for (int i = 0; i < (sizeof(Weaponss) / 4); i++) { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), Weaponss[i], 9999, 1); }});
			sub->AddOption<RegularOption>("Clone Vehicle", "", [] { Features::CloneVehicle(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)); });
			sub->AddOption<RegularOption>("Teleport Into Vehicle", "", [] { Features::TeleportInVehicle(g_SelectedPlayer); });
			sub->AddOption<RegularOption>("Kick From Vehicle", "", [] { Features::KickFromVehicle(g_SelectedPlayer); });
			sub->AddOption<RegularOption>("Ram Player", "", [] { Features::RamPlayer(); });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Ped Manager", PEDSPAWN, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Spawn Enemy", nullptr, ENEMY);
			sub->AddOption<SubOption>("Spawn Bodyguard", nullptr, BODYGUARD);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Spawn Enemy", ENEMY, [](RegularSubmenu* sub)
		{
			for (auto enemies : pedModels)
			{
				sub->AddOption<RegularOption>(enemies, "", [enemies] { Features::Enemy((char*)enemies); });
			}
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Spawn Bodyguard", BODYGUARD, [](RegularSubmenu* sub)
		{
			for (auto bodyguard : pedModels)
			{
				sub->AddOption<RegularOption>(bodyguard, "", [bodyguard] { Features::Bodyguard((char*)bodyguard); });
			}
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Attach Objects", SELECTEDATTACH, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Detach Objects", "", [] { Features::ClearProps(); });
			for (auto objects1 : objects)
			{
				sub->AddOption<RegularOption>(objects1, "", [objects1] { Features::AttachObjects((char*)objects1); });
			}
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Teleports", TELEPORTS, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Eclipse Towers", "", [] { Features::Teleport1(); });
			sub->AddOption<RegularOption>("West Vinewood", "", [] { Features::Teleport2(); });
			sub->AddOption<RegularOption>("Las Lagunas", "", [] { Features::Teleport3(); });
			sub->AddOption<RegularOption>("Spanish Ave", "", [] { Features::Teleport4(); });
			sub->AddOption<RegularOption>("Power St", "", [] { Features::Teleport5(); });
			sub->AddOption<RegularOption>("Del Perro", "", [] { Features::Teleport6(); });
			sub->AddOption<RegularOption>("Milton Rd", "", [] { Features::Teleport7(); });
			sub->AddOption<RegularOption>("The Royale", "", [] { Features::Teleport8(); });
			sub->AddOption<RegularOption>("La Puerta", "", [] { Features::Teleport9(); });
			sub->AddOption<RegularOption>("Rockford Dr", "", [] { Features::Teleport10(); });
			sub->AddOption<RegularOption>("Dream Tower", "", [] { Features::Teleport11(); });
			sub->AddOption<RegularOption>("San Vitas", "", [] { Features::Teleport12(); });
			sub->AddOption<RegularOption>("Vespucci Blvd", "", [] { Features::Teleport13(); });
			sub->AddOption<RegularOption>("Cougar Ave", "", [] { Features::Teleport14(); });
			sub->AddOption<RegularOption>("Prosperity St", "", [] { Features::Teleport15(); });
			sub->AddOption<RegularOption>("Weazel Plaza", "", [] { Features::Teleport16(); });
			sub->AddOption<RegularOption>("Del Perro Office", "", [] { Features::Teleport17(); });
			sub->AddOption<RegularOption>("Maze Bank West Office", "", [] { Features::Teleport18(); });
			sub->AddOption<RegularOption>("Arcadius Office", "", [] { Features::Teleport19(); });
			sub->AddOption<RegularOption>("Maze Bank Office", "", [] { Features::Teleport20(); });
			sub->AddOption<NumberOption<std::int32_t>>("Custom Location", nullptr, &Features::WantedLevel, 1, 115, 1, 3, true, "", "", [] {
				Features::TeleportCustom(Features::WantedLevel);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::WantedLevel = getInputs();
						if (Features::WantedLevel > 5)
							Features::WantedLevel = 5;
						if (Features::WantedLevel < 0)
							Features::WantedLevel = 0;
						Features::TeleportCustom(Features::WantedLevel);
					});
				}
			
			});
		});
		static const char* actions[] = {
			"GBSAL_DRAW", "GBSAL_COLL", "GBFK_TEXP", "GR_VISTKR_SELL"
		};
		g_UiManager->AddSubmenu<RegularSubmenu>("Script Events", SCRIPTS, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Invites", nullptr, INVITES);
			sub->AddOption<SubOption>("Messages", nullptr, MESSAGES);
			sub->AddOption<BoolOption<bool>>("Sound Spam", nullptr, &Features::Spam[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::DoSoundSpam(Features::Spam[g_SelectedPlayer]); });
			sub->AddOption<BoolOption<bool>>("Remote OTR", nullptr, &Features::RemoteOTRBool[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::RemoteOTR(Features::RemoteOTRBool[g_SelectedPlayer]); });
			sub->AddOption<BoolOption<bool>>("Cops Blind Eye", nullptr, &Features::BlindEyeBool[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::BlindEye(Features::BlindEyeBool[g_SelectedPlayer]); });
			sub->AddOption<BoolOption<bool>>("Camera Spam", nullptr, &Features::CameraSpam[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::sendFaceCameraForward(Features::CameraSpam[g_SelectedPlayer]); });
			sub->AddOption<BoolOption<bool>>("Never Wanted", nullptr, &Features::RemoteNeverWanted[g_SelectedPlayer], BoolDisplay::OnOff, false, [] { Features::RemoveWanted(Features::RemoteNeverWanted[g_SelectedPlayer]); });
			sub->AddOption<RegularOption>("Send To Job", "", [] { Features::SendToJob(); });
			sub->AddOption<RegularOption>("Kick Player", "", [] { Features::KickPlayer(); });
			sub->AddOption<RegularOption>("CEO Kick", "", [] { Features::CEOKick(); });
			sub->AddOption<RegularOption>("CEO Ban", "", [] { Features::CEOBan(); });
			sub->AddOption<RegularOption>("Transaction Error", "", [] { Features::TransactionError(g_SelectedPlayer); });
			sub->AddOption<RegularOption>("Infinite Loading Screen", "", [] { Features::InfiniteLoading(); });
			sub->AddOption<RegularOption>("Remove From Vehicle", "", [] { Features::GentleKick(); });

		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Invites", INVITES, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Yacht", "", [] { Features::Invite2Place(g_SelectedPlayer, 1); });
			sub->AddOption<RegularOption>("Office", "", [] { Features::Invite2Place(g_SelectedPlayer, 2); });
			sub->AddOption<RegularOption>("Clubhouse", "", [] { Features::Invite2Place(g_SelectedPlayer, 3); });
			sub->AddOption<RegularOption>("Office Garage", "", [] { Features::Invite2Place(g_SelectedPlayer, 6); });
			sub->AddOption<RegularOption>("Apartment", "", [] { Features::Invite2Place(g_SelectedPlayer, 70); });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Messages", MESSAGES, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("1.5bill Banked", nullptr, &Features::AddSpam, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("1.5bill Remove", nullptr, &Features::RemoveSpam, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<RegularOption>("Troll Insurance Message", "", [] { Features::TrollMessage(); });
			sub->AddOption<RegularOption>("Spectating", "", [] { Features::MessageSpam(g_SelectedPlayer, RemoteMessageSpectating); });
			sub->AddOption<RegularOption>("Idle Kick", "", [] { Features::MessageSpam(g_SelectedPlayer, RemoteMessageIdleKick); });
			sub->AddOption<RegularOption>("Psycho", "", [] { Features::MessageSpam(g_SelectedPlayer, RemoteMessagePsycho); });
			sub->AddOption<RegularOption>("Goon Remove", "", [] { Features::MessageSpam(g_SelectedPlayer, RemoteMessageGoonRemove); });
			sub->AddOption<RegularOption>("Goon Leave", "", [] { Features::MessageSpam(g_SelectedPlayer, RemoteMessageGoonLeave); });
			sub->AddOption<RegularOption>("Goon Join", "", [] { Features::MessageSpam(g_SelectedPlayer, RemoteMessageGoonJoin); });
		});
		static const char* PlateRegions[] = {
			"Blue/White", "Yellow/Black","Yellow/Blue", "Blue/White2", "Blue/White3", "Yankton"
		};

		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle", VEHICLES, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Spawn Vehicle", nullptr, VEHICLESPAWN);
			sub->AddOption<SubOption>("Mobile LSC", nullptr, MOBILELSC);
			sub->AddOption<SubOption>("Vehicle RGB", nullptr, VEHICLECOLOR);
			sub->AddOption<SubOption>("Vehicle Stats", nullptr, VEHICLESTATS);
			sub->AddOption<NumberOption<std::int32_t>>("Vehicle Alpha", nullptr, &EntityAlpha, 0, 255, 1, 3,true, "", "", [] {
			ENTITY::SET_ENTITY_ALPHA(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), EntityAlpha, true);
			if (IsKeyPressed(VK_NUMPAD5))
			{
				g_FiberScript->addTask([] {

					EntityAlpha = getInputs();
					if (EntityAlpha > 255)
						EntityAlpha = 255;
					if (EntityAlpha < 0)
						EntityAlpha = 0;
					ENTITY::SET_ENTITY_ALPHA(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), EntityAlpha, true);
				});
			}

			});
			sub->AddOption<NumberOption<std::int32_t>>("Boost Level", nullptr, &Features::boostlevel, 0, 1000, 10, 3, true, "", "", [] { PrintBottomLeft((char*)"Press NUMPAD9 for forward boost"); PrintBottomLeft((char*)"Press NUMPAD3 for instant brake");
			if (IsKeyPressed(VK_NUMPAD5))
			{
				g_FiberScript->addTask([] {

					Features::boostlevel = getInputs();
					if (Features::boostlevel > 1000)
						Features::boostlevel = 1000;
					if (Features::boostlevel < 0)
						Features::boostlevel = 0;
				});
			}
			
			});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Plate Region", nullptr, &PlateRegions, &Lists::DemoListPos26, false, [] { 
				Vehicle vehicle; vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, Lists::DemoListPos26);
				});
			sub->AddOption<RegularOption>("Set Plate Text", "", [] { SetPlate(); });
			sub->AddOption<BoolOption<bool>>("Godmode", nullptr, &Features::cargodmodebool, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("Boost Charge", nullptr, &Features::Boost, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("Invisibility", nullptr, &Features::InvisibleCarBool, BoolDisplay::OnOff, false, [] { Features::InvisibleCar(); });
			sub->AddOption<BoolOption<bool>>("Vehicle Gravity", nullptr, &Features::vehiclegravitybool, BoolDisplay::OnOff, false, [] { Features::vehiclegravity(); });
			sub->AddOption<BoolOption<bool>>("Vehicle Rockets", nullptr, &Features::WeaponRockets, BoolDisplay::OnOff, false, [] { PrintBottomLeft((char*)"Press NUMPAD+ to fire"); });
			sub->AddOption<BoolOption<bool>>("Speedometer", nullptr, &Features::SpeedoBool, BoolDisplay::OnOff, false, [] {  });
			sub->AddOption<BoolOption<bool>>("Matrix Plates", nullptr, &Features::MatrixPlates, BoolDisplay::OnOff, false, [] {  });
			sub->AddOption<BoolOption<bool>>("Sticky Vehicle", nullptr, &Features::StickyBool, BoolDisplay::OnOff, false, [] {  });
			sub->AddOption<BoolOption<bool>>("Fix Loop", nullptr, &Features::FixLoop, BoolDisplay::OnOff, false, [] {  });
			//sub->AddOption<BoolOption<bool>>("Drive On Water", nullptr, &Features::dowbool, BoolDisplay::OnOff, false, [] {  });
			sub->AddOption<RegularOption>("Fix Vehicle", "", [] { Features::FixCar(); });
			sub->AddOption<RegularOption>("Max Vehicle", "", [] { Features::maxvehicle(); });
			sub->AddOption<RegularOption>("Open Doors", "", [] { Features::OpenDoors(); });\
			sub->AddOption<RegularOption>("Close Doors", "", [] { VEHICLE::SET_VEHICLE_DOORS_SHUT(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), true); });
			sub->AddOption<RegularOption>("Reset Placement", "", [] { Features::flip(); });
		});
		static float Acceleration = { 0.f };
		static float Brake = { 0.f };
		static float Traction = { 0.f };
		static float Deform = { 0.f };
		static float Suspensions = { 0.f };
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Stats", VEHICLESTATS, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Acceleration", nullptr, &Acceleration, 0.f, 1000.f, 0.1f, 1, true, "", "", [] { 
				Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x4C }, Acceleration);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Acceleration = (float)getInputs();
						if (Acceleration > 1000.f)
							Acceleration = 1000.f;
						if (Acceleration < 0.f)
							Acceleration = 0.f;
						Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x4C }, Acceleration);
					});
				}
			
			
			});
			sub->AddOption<NumberOption<float>>("Brake", nullptr, &Brake, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
				Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x6C }, Brake);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Brake = (float)getInputs();
						if (Brake > 1000.f)
							Brake = 1000.f;
						if (Brake < 0.f)
							Brake = 0.f;
						Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x6C }, Brake);
					});
				}
			
			});
			sub->AddOption<NumberOption<float>>("Traction", nullptr, &Traction, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
				Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x90 }, Traction);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Traction = (float)getInputs();
						if (Traction > 1000.f)
							Traction = 1000.f;
						if (Traction < 0.f)
							Traction = 0.f;
						Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x90 }, Traction);
					});
				}
			
			});
			sub->AddOption<NumberOption<float>>("Deform", nullptr, &Deform, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
				Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0xF8 }, Deform); 
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Deform = (float)getInputs();
						if (Deform > 1000.f)
							Deform = 1000.f;
						if (Deform < 0.f)
							Deform = 0.f;
						Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0xF8 }, Deform);
					});
				}
			
			});
			sub->AddOption<NumberOption<float>>("Suspension", nullptr, &Suspensions, 0.f, 1000.f, 0.1f, 1, true, "", "", [] { 
				Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0xBC }, Suspension); 
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Suspensions = (float)getInputs();
						if (Suspensions > 1000.f)
							Suspensions = 1000.f;
						if (Suspensions < 0.f)
							Suspensions = 0.f;
						Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0xBC }, Suspension);
					});
				}
			
			
			});
		});
		//
		g_UiManager->AddSubmenu<RegularSubmenu>("Mobile LSC [Beta]", MOBILELSC, [](RegularSubmenu* sub)
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
			VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Spoiler:", nullptr, &Spoiler, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SPOILER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SPOILER, Spoiler, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Front Bumper:", nullptr, &FBumper, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_FRONTBUMPER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_FRONTBUMPER, FBumper, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Rear Bumper:", nullptr, &RBumper, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_REARBUMPER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_REARBUMPER, RBumper, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Side Skirt:", nullptr, &SSkirt, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SIDESKIRT), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SIDESKIRT, SSkirt, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Exhaust:", nullptr, &Exhaust, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_EXHAUST), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_EXHAUST, Exhaust, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Grille:", nullptr, &Grille, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_GRILLE), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_GRILLE, Grille, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Chassis:", nullptr, &Chasis, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_CHASSIS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_CHASSIS, Chasis, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Hood:", nullptr, &Hood, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_HOOD), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_HOOD, Hood, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Fender:", nullptr, &Fender, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_FENDER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_FENDER, Fender, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Right Fender:", nullptr, &RFender, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_RIGHTFENDER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_RIGHTFENDER, RFender, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Roof:", nullptr, &Roof, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ROOF), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ROOF, Roof, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Engine:", nullptr, &Engine, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ENGINE), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ENGINE, Engine, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Brakes:", nullptr, &Brakes, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_BRAKES), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_BRAKES, Brakes, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Transmission:", nullptr, &Transmission, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRANSMISSION), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRANSMISSION, Transmission, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Horns:", nullptr, &Horns, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_HORNS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_HORNS, Horns, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Suspension:", nullptr, &Suspension, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SUSPENSION), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SUSPENSION, Suspension, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Armor:", nullptr, &Armor, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ARMOR), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ARMOR, Armor, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Turbo:", nullptr, &Turbo, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TURBO), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TURBO, Turbo, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Tire Smoke:", nullptr, &TSmoke, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TIRESMOKE), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TIRESMOKE, TSmoke, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Xenon:", nullptr, &Xenon, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_XENONLIGHTS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_XENONLIGHTS, Xenon, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Wheels:", nullptr, &Wheels, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_FRONTWHEELS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_FRONTWHEELS, Wheels, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Windows:", nullptr, &Windows, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_WINDOWS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_WINDOWS, Windows, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Plate Holder:", nullptr, &Plateholder, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_PLATEHOLDER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_PLATEHOLDER, Plateholder, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Vanity Plates:", nullptr, &Vanity, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_VANITY_PLATES), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_VANITY_PLATES, Vanity, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Trim:", nullptr, &Trims, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRIM), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRIM, Trims, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Ornaments:", nullptr, &Ornaments, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ORNAMENTS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ORNAMENTS, Ornaments, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Dashboard:", nullptr, &Dashboard, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_DASHBOARD), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_DASHBOARD, Dashboard, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Dial:", nullptr, &Dial, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_DIAL), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_DIAL, Dial, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Door Speaker:", nullptr, &DoorSpeaker, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_DOOR_SPEAKER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_DOOR_SPEAKER, DoorSpeaker, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Seats:", nullptr, &Seats, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SEATS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SEATS, Seats, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Steering Wheel:", nullptr, &SteeringWheel, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_STEERINGWHEEL), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_STEERINGWHEEL, SteeringWheel, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Shifting Levers:", nullptr, &Shifter, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SHIFTER_LEAVERS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SHIFTER_LEAVERS, Shifter, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Plaques:", nullptr, &Plaques, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_PLAQUES), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_PLAQUES, Plaques, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Speakers:", nullptr, &Speakers, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SPEAKERS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SPEAKERS, Speakers, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Trunk:", nullptr, &Trunk, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRUNK), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRUNK, Trunk, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Hydraulics:", nullptr, &Hydraulics, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_HYDRULICS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_HYDRULICS, Hydraulics, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Engine Block:", nullptr, &EngineBlock, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ENGINE_BLOCK), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ENGINE_BLOCK, EngineBlock, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Air Filter:", nullptr, &Filter, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_AIR_FILTER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_AIR_FILTER, Filter, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Struts:", nullptr, &Struts, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_STRUTS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_STRUTS, Struts, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Arch Cover:", nullptr, &Arch, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ARCH_COVER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ARCH_COVER, Arch, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Aerials:", nullptr, &Aerials, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_AERIALS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_AERIALS, Aerials, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Trim 2:", nullptr, &TrimTwo, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRIM_2), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRIM_2, TrimTwo, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Tank:", nullptr, &Tank, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TANK), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TANK, Tank, false); });
			sub->AddOption<NumberOption<std::int32_t>>("Livery:", nullptr, &Livery, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_LIVERY), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_LIVERY, Livery, false); });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Color", VEHICLECOLOR, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Smooth Rainbow", nullptr, &Features::rainbowbool, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Rapid Rainbow", nullptr, &Features::RainbowPaintLoop, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<NumberOption<std::int32_t>>("Primary R:", nullptr, &Features::primarycolorintr, 0, 255, 1, 3, true, "", "", [] { 
				Features::PrimaryColor();
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::primarycolorintr = getInputs();
						if (Features::primarycolorintr > 255)
							Features::primarycolorintr = 255;
						if (Features::primarycolorintr < 0)
							Features::primarycolorintr = 0;
						Features::PrimaryColor();
					});
				}
			
			});
			sub->AddOption<NumberOption<std::int32_t>>("Primary G:", nullptr, &Features::primarycolorintg, 0, 255, 1, 3, true, "", "", [] {
				Features::PrimaryColor();
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::primarycolorintg = getInputs();
						if (Features::primarycolorintg > 255)
							Features::primarycolorintg = 255;
						if (Features::primarycolorintg < 0)
							Features::primarycolorintg = 0;
						Features::PrimaryColor();
					});
				}
			
			});
			sub->AddOption<NumberOption<std::int32_t>>("Primary B:", nullptr, &Features::primarycolorintb, 0, 255, 1, 3, true, "", "", [] { 
				Features::PrimaryColor(); 
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::primarycolorintb = getInputs();
						if (Features::primarycolorintb > 255)
							Features::primarycolorintb = 255;
						if (Features::primarycolorintb < 0)
							Features::primarycolorintb = 0;
						Features::PrimaryColor();
					});
				}
			});
			sub->AddOption<NumberOption<std::int32_t>>("Secondary R:", nullptr, &Features::secondarycolorintr, 0, 255, 1, 3, true, "", "", [] { 
				Features::SecondaryColor1(); 
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::secondarycolorintr = getInputs();
						if (Features::secondarycolorintr > 255)
							Features::secondarycolorintr = 255;
						if (Features::secondarycolorintr < 0)
							Features::secondarycolorintr = 0;
						Features::SecondaryColor1();
					});
				}
			
			});
			sub->AddOption<NumberOption<std::int32_t>>("Secondary G:", nullptr, &Features::secondarycolorintg, 0, 255, 1, 3, true, "", "", [] { 
				Features::SecondaryColor1(); 
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::secondarycolorintg = getInputs();
						if (Features::secondarycolorintg > 255)
							Features::secondarycolorintg = 255;
						if (Features::secondarycolorintg < 0)
							Features::secondarycolorintg = 0;
						Features::SecondaryColor1();
					});
				}
			});
			sub->AddOption<NumberOption<std::int32_t>>("Secondary B:", nullptr, &Features::secondarycolorintb, 0, 255, 1, 3, true, "", "", [] { 
				Features::SecondaryColor1(); 
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::secondarycolorintb = getInputs();
						if (Features::secondarycolorintb > 255)
							Features::secondarycolorintb = 255;
						if (Features::secondarycolorintb < 0)
							Features::secondarycolorintb = 0;
						Features::SecondaryColor1();
					});
				}
			});
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Spawner", VEHICLESPAWN, [](RegularSubmenu* sub)
		{
			Features::listPresets();
			sub->AddOption<BoolOption<bool>>("Spawn In Vehicle", nullptr, &Features::SpawnInCar, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("Spawn Maxed", nullptr, &Features::SpawnMaxed, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<RegularOption>("Spawn vehicle by name", "", [] {
				g_FiberScript->addTask([] {
					GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
					while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0)
					{
						IsKeyboardActive = true;
						g_UiManager->ResetInput();
						g_FiberScript->Wait(0);
					}
					IsKeyboardActive = false;
					if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())
						return 0;
					spawn_vehicle(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT(), PLAYER::PLAYER_PED_ID());
					});

				});
			/*sub->AddOption<ChooseOption<std::string, std::size_t>>("Ini Vehicles", nullptr, &Features::list, &Lists::DemoListPos25, false, [] {
 Features::LoadPreset(Features::list[Lists::DemoListPos25]);	 });*/
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Super", nullptr, &Super1, &Lists::DemoListPos1, false, [] { spawn_vehicle((char*)Super1[Lists::DemoListPos1]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Sports", nullptr, &Sports1, &Lists::DemoListPos2, false, [] { spawn_vehicle((char*)Sports1[Lists::DemoListPos2]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Sport Classic", nullptr, &SportsClassics1, &Lists::DemoListPos3, false, [] { spawn_vehicle((char*)SportsClassics1[Lists::DemoListPos3]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Offroad", nullptr, &OffRoad1, &Lists::DemoListPos4, false, [] { spawn_vehicle((char*)OffRoad1[Lists::DemoListPos4]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Sedans", nullptr, &Sedans1, &Lists::DemoListPos5, false, [] { spawn_vehicle((char*)Sedans1[Lists::DemoListPos5]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Coupes", nullptr, &Coupes1, &Lists::DemoListPos6, false, [] { spawn_vehicle((char*)Coupes1[Lists::DemoListPos6]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Muscle", nullptr, &Muscle1, &Lists::DemoListPos7, false, [] { spawn_vehicle((char*)Muscle1[Lists::DemoListPos7]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Boats", nullptr, &Boats1, &Lists::DemoListPos8, false, [] { spawn_vehicle((char*)Boats1[Lists::DemoListPos8]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Commercial", nullptr, &Commercial1, &Lists::DemoListPos9, false, [] { spawn_vehicle((char*)Commercial1[Lists::DemoListPos9]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Compacts", nullptr, &Compacts1, &Lists::DemoListPos10, false, [] { spawn_vehicle((char*)Compacts1[Lists::DemoListPos10]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Cycles", nullptr, &Cycles1, &Lists::DemoListPos11, false, [] { spawn_vehicle((char*)Cycles1[Lists::DemoListPos11]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Emergency", nullptr, &Emergency1, &Lists::DemoListPos12, false, [] { spawn_vehicle((char*)Emergency1[Lists::DemoListPos12]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Helicopters", nullptr, &Helicopters1, &Lists::DemoListPos14, false, [] { spawn_vehicle((char*)Helicopters1[Lists::DemoListPos14]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Industrial", nullptr, &Industrial1, &Lists::DemoListPos15, false, [] { spawn_vehicle((char*)Industrial1[Lists::DemoListPos15]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Military", nullptr, &Military1, &Lists::DemoListPos16, false, [] { spawn_vehicle((char*)Military1[Lists::DemoListPos16]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Motorcycles", nullptr, &Motorcycles1, &Lists::DemoListPos17, false, [] { spawn_vehicle((char*)Motorcycles1[Lists::DemoListPos17]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Planes", nullptr, &Planes1, &Lists::DemoListPos18, false, [] { spawn_vehicle((char*)Planes1[Lists::DemoListPos18]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Service", nullptr, &Service1, &Lists::DemoListPos19, false, [] { spawn_vehicle((char*)Service1[Lists::DemoListPos19]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("SUV", nullptr, &SUVs1, &Lists::DemoListPos20, false, [] { spawn_vehicle((char*)SUVs1[Lists::DemoListPos20]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Trailer", nullptr, &Trailer1, &Lists::DemoListPos21, false, [] { spawn_vehicle((char*)Trailer1[Lists::DemoListPos21]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Trains", nullptr, &Trains1, &Lists::DemoListPos22, false, [] { spawn_vehicle((char*)Trains1[Lists::DemoListPos22]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Utility", nullptr, &Utility1, &Lists::DemoListPos23, false, [] { spawn_vehicle((char*)Utility1[Lists::DemoListPos23]);	});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Vans", nullptr, &Vans1, &Lists::DemoListPos24, false, [] { spawn_vehicle((char*)Vans1[Lists::DemoListPos24]);	});
			sub->AddOption<RegularOption>("Delete Vehicle", "", [] { Features::DeleteVeh(); });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Weapons", WEAPONS, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Weapon Stats", nullptr, WEAPONSTATS);
			sub->AddOption<BoolOption<bool>>("Infinite Ammo", nullptr, &Features::InfiniteAmmoBool, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Explosive Ammo", nullptr, &Features::ExplosiveAmmoFlag, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Flame Ammo", nullptr, &Features::FireShoot, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Water Ammo", nullptr, &Features::WaterShoot, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Firework Ammo", nullptr, &Features::FireworkBool, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Shoot Vehicle", nullptr, &Features::ShootVehicle, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("AirStrike Ammo", nullptr, &Features::AirStrike, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Rainbow Weapon", nullptr, &Features::RainbowWeaponsBool, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Shoot To Delete", nullptr, &Features::AimBool, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Teleport Gun", nullptr, &Features::teleportbool, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Object Gun", nullptr, &Features::ObjectGunBool, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Gravity Gun", nullptr, &Features::GravityGun, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Valkyrie Rocket", nullptr, &Features::VALKYIREROCKET, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Aimbot", nullptr, &Features::AimbotBool, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("1 Hit Kill", nullptr, &Features::DamageBool, BoolDisplay::OnOff, false, [] {	Features::DamageMod();	});
			sub->AddOption<BoolOption<bool>>("No Recoil", nullptr, &Features::Recoil, BoolDisplay::OnOff, false, [] {	Features::NoRecoil();	});
			sub->AddOption<BoolOption<bool>>("Rapid Fire", nullptr, &Features::RapidFireBool, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Orbital Cooldown", nullptr, &Features::OrbitalCooldownBool, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<RegularOption>("Give All Weapons", "", [] {
				for (Hash hash : Weaponss)
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 9999, 0);
			});
			sub->AddOption<RegularOption>("Give MK1 Weapons", "", [] {
				for (Hash hash : MK1Weapons)
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 9999, 0);
			});
			sub->AddOption<RegularOption>("Give MK2 Weapons", "", [] {
				for (Hash hash : MK2Weapons)
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 9999, 0);
			});
			sub->AddOption<RegularOption>("Give All Attachments", "", [] { Hash hash; WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &hash, 1);  Features::BruteForceWeaponAddons(PLAYER::PLAYER_PED_ID(), hash); });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Weapon Stats", WEAPONSTATS, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Damage", nullptr, &damage, 0.f, 1000.f, 0.1f, 1, true,"","/1000.f", [] { 
				Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_BULLET_DMG }, damage);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						damage = (float)getInputs();
						if (damage > 1000)
							damage = 1000;
						if (damage < 0)
							damage = 0;
						Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_BULLET_DMG }, damage);
					});
				}
			});
			sub->AddOption<NumberOption<float>>("Spread", nullptr, &spread, 0.f, 1000.f, 0.1f, 1, true, "", "", [] { 
				Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_SPREAD }, spread);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						spread = (float)getInputs();
						if (spread > 1000)
							spread = 1000;
						if (spread < 0)
							spread = 0;
						Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_SPREAD }, spread);
					});
				}
			});
			sub->AddOption<NumberOption<float>>("Recoil", nullptr, &recoil, 0.f, 1000.f, 0.1f, 1, true, "", "", [] { 
				Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_RECOIL }, recoil);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						recoil = (float)getInputs();
						if (recoil > 1000)
							recoil = 1000;
						if (recoil < 0)
							recoil = 0;
						Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_RECOIL }, recoil);
					});
				}
			});
			sub->AddOption<NumberOption<float>>("Range", nullptr, &range, 0.f, 1000.f, 0.1f, 1, true, "", "", [] { 
				Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_RANGE }, range);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						range = (float)getInputs();
						if (range > 1000)
							range = 1000;
						if (range < 0)
							range = 0;
						Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_RANGE }, range);
					});
				}
			});
			sub->AddOption<NumberOption<float>>("Reload", nullptr, &reloadtime, 0.f, 1000.f, 0.1f, 1, true, "", "", [] { 
				Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_RELOAD_MULTIPLIER }, reloadtime);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						reloadtime = (float)getInputs();
						if (reloadtime > 1000)
							reloadtime = 1000;
						if (reloadtime < 0)
							reloadtime = 0;
						Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_RELOAD_MULTIPLIER }, reloadtime);
					});
				}
			});
			sub->AddOption<NumberOption<float>>("Muzzle Velocity", nullptr, &muzzlevelocity, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
				Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_MUZZLE_VELOCITY }, muzzlevelocity); 
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						muzzlevelocity = (float)getInputs();
						if (muzzlevelocity > 1000)
							muzzlevelocity = 1000;
						if (muzzlevelocity < 0)
							muzzlevelocity = 0;
						Memory::set_value<float>({ OFFSET_PLAYER, OFFSET_WEAPON_MANAGER, OFFSET_WEAPON_CURRENT, OFFSET_WEAPON_MUZZLE_VELOCITY }, muzzlevelocity);
					});
				}
			});
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Teleports", TELEPORTSELF, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("IPLs", nullptr, IPLs);
			sub->AddOption<RegularOption>("Teleport To Waypoint", "", [] { teleport_to_marker(); });
			sub->AddOption<RegularOption>("Strip Club", "", [] { Vector3 Coords; Coords.x = 126.135f; Coords.y = -1278.583f; Coords.z = 29.270f; TPto(Coords); });
			sub->AddOption<RegularOption>("Police Station", "", [] { Vector3 Coords; Coords.x = 436.491f; Coords.y = -982.172f; Coords.z = 30.699f; TPto(Coords); });
			sub->AddOption<RegularOption>("Burnt FIB", "", [] { Vector3 Coords; Coords.x = 160.868f; Coords.y = -745.831f; Coords.z = 250.063f; TPto(Coords); });
			sub->AddOption<RegularOption>("Pacific Standard Vault", "", [] { Vector3 Coords; Coords.x = 255.851f; Coords.y = 217.030f; Coords.z = 101.683f; TPto(Coords); });
			sub->AddOption<RegularOption>("Lester's House", "", [] { Vector3 Coords; Coords.x = 1273.898f; Coords.y = -1719.304f; Coords.z = 54.771f; TPto(Coords); });
			sub->AddOption<RegularOption>("IAA Office", "", [] { Vector3 Coords; Coords.x = 117.220f; Coords.y = -620.938f; Coords.z = 206.047f; TPto(Coords); });
			sub->AddOption<RegularOption>("Torture Room", "", [] { Vector3 Coords; Coords.x = 147.170f; Coords.y = -2201.804f; Coords.z = 4.688f; TPto(Coords); });
			sub->AddOption<RegularOption>("Hippy Camp", "", [] { Vector3 Coords; Coords.x = 2476.712f; Coords.y = 3789.645f; Coords.z = 41.226f; TPto(Coords); });
			sub->AddOption<RegularOption>("Weed Farm", "", [] { Vector3 Coords; Coords.x = 2208.777f; Coords.y = 5578.235f; Coords.z = 53.735f; TPto(Coords); });
			sub->AddOption<RegularOption>("Casino", "", [] { Vector3 Coords; Coords.x = 925.329f; Coords.y = 46.152f; Coords.z = 80.908f; TPto(Coords); });
			sub->AddOption<RegularOption>("Prison", "", [] { Vector3 Coords; Coords.x = 1679.049f; Coords.y = 2513.711f; Coords.z = 45.565f; TPto(Coords); });
			sub->AddOption<RegularOption>("Mount Chiliad", "", [] { Vector3 Coords; Coords.x = 496.75f; Coords.y = 5591.17f; Coords.z = 795.03f; TPto(Coords); });
			sub->AddOption<RegularOption>("Maze Bank", "", [] { Vector3 Coords; Coords.x = -74.94243f; Coords.y = -818.63446f; Coords.z = 326.174347f; TPto(Coords); });
			sub->AddOption<RegularOption>("Military Base", "", [] { Vector3 Coords; Coords.x = -2012.8470f; Coords.y = 2956.5270f; Coords.z = 32.8101f; TPto(Coords); });
			sub->AddOption<RegularOption>("Zancudo Tower", "", [] { Vector3 Coords; Coords.x = -2356.0940; Coords.y = 3248.645; Coords.z = 101.4505; TPto(Coords); });
			sub->AddOption<RegularOption>("Mask Shop", "", [] { Vector3 Coords; Coords.x = -1338.16; Coords.y = -1278.11; Coords.z = 4.87; TPto(Coords); });
			sub->AddOption<RegularOption>("LS Customs", "", [] { Vector3 Coords; Coords.x = -373.01; Coords.y = -124.91; Coords.z = 38.31; TPto(Coords); });
			sub->AddOption<RegularOption>("Ammunation", "", [] { Vector3 Coords; Coords.x = 247.3652; Coords.y = -45.8777; Coords.z = 69.9411; TPto(Coords); });
			sub->AddOption<RegularOption>("Airport", "", [] { Vector3 Coords; Coords.x = -1102.2910f; Coords.y = -2894.5160f; Coords.z = 13.9467f; TPto(Coords); });
			sub->AddOption<RegularOption>("Clothes Store", "", [] { Vector3 Coords; Coords.x = -718.91; Coords.y = -158.16; Coords.z = 37.00; TPto(Coords); });
			sub->AddOption<RegularOption>("Waterfall", "", [] { Vector3 Coords; Coords.x = -597.9525f; Coords.y = 4475.2910f; Coords.z = 25.6890f; TPto(Coords); });
			sub->AddOption<RegularOption>("FIB", "", [] { Vector3 Coords; Coords.x = 135.5220f; Coords.y = -749.0003f; Coords.z = 260.0000f; TPto(Coords); });
			sub->AddOption<RegularOption>("Humane Labs", "", [] { Vector3 Coords; Coords.x = 3617.231f; Coords.y = 3739.871f; Coords.z = 28.6901f; TPto(Coords); });
			sub->AddOption<RegularOption>("MMI", "", [] { Vector3 Coords; Coords.x = -222.1977; Coords.y = -1185.8500; Coords.z = 23.0294; TPto(Coords); });
			sub->AddOption<RegularOption>("Sandy Shores Airfield", "", [] { Vector3 Coords; Coords.x = 1741.4960f; Coords.y = 3269.2570f; Coords.z = 41.6014f; TPto(Coords); });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("IPLs", IPLs, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("North Yankton", "", [] { Features::Yankton(); });
			sub->AddOption<RegularOption>("Yacht", "", [] { Features::Yacht(); });
			sub->AddOption<RegularOption>("Aircraft Carrier", "", [] { Features::Carrier(); });
			sub->AddOption<RegularOption>("Sunken Cargo Ship", "", [] { Features::SunkenShip(); });
			sub->AddOption<RegularOption>("Destroyed Hospital", "", [] { Features::Hospital(); });
			sub->AddOption<RegularOption>("ONeil Farm", "", [] { Features::Farm(); });
			sub->AddOption<RegularOption>("Life Invader", "", [] { Features::Invader(); });
			sub->AddOption<RegularOption>("Jewelry Store", "", [] { Features::Jewelry(); });
			sub->AddOption<RegularOption>("Morgue", "", [] { Features::Morgue(); });
		});
		static const char* Weathers[] = {
	"Clear", "Clearing","Snowlight", "Thunder", "Blizzard", "Overcast", "Foggy", "Smog", "Clouds"
		};
		g_UiManager->AddSubmenu<RegularSubmenu>("World", WEATHER, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Moon Gravity", nullptr, &Features::GravityBool, BoolDisplay::OnOff, false, [] {	Features::Gravity();	});
			//sub->AddOption<RegularOption>("Sync w/ System Time", "", [] { Features::IrlTime(); });
			//sub->AddOption<RegularOption>("Crash?", "", [] { Features::SetWeathers(-1); });
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Local Weather", nullptr, &Weathers, &Lists::DemoListPos27, false, [] { GAMEPLAY::SET_OVERRIDE_WEATHER((char*)Weathers[Lists::DemoListPos27]); });
			sub->AddOption<NumberOption<std::int32_t>>("Wave Intensity:", nullptr, &Features::WaveIntensityValue, 0, 5000, 1, 3, true, "", "", [] { 
				Features::SetWaveIntensity();
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::WaveIntensityValue = getInputs();
						if (Features::WaveIntensityValue > 5000)
							Features::WaveIntensityValue = 5000;
						if (Features::WaveIntensityValue < 0)
							Features::WaveIntensityValue = 0;
						Features::SetWaveIntensity();
					});
				}
			});
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Protections", PROTECTIONS, [](RegularSubmenu* sub)//
		{//
			sub->AddOption<BoolOption<bool>>("Notify Blocked", nullptr, &Features::EventNotif, BoolDisplay::OnOff, false, [] {});
			//sub->AddOption<BoolOption<bool>>("Notify R* Admin", nullptr, &Features::RockstarCheck, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("~r~All Script Events", nullptr, &Features::BlockAllProtex, BoolDisplay::OnOff, false, [] {		});
			sub->AddOption<BoolOption<bool>>("Money Drop (Pickups)", nullptr, &Features::AntiMoneyDrop, BoolDisplay::OnOff, false, [] {		});
			sub->AddOption<BoolOption<bool>>("Kick", nullptr, &protex1, BoolDisplay::OnOff, false, [] {		});
			sub->AddOption<BoolOption<bool>>("CEO Kick", nullptr, &protex2, BoolDisplay::OnOff, false, [] {	 });
			sub->AddOption<BoolOption<bool>>("CEO Ban", nullptr, &protex3, BoolDisplay::OnOff, false, [] {	 });
			sub->AddOption<BoolOption<bool>>("Remote OTR", nullptr, &protex4, BoolDisplay::OnOff, false, [] {	 });
			sub->AddOption<BoolOption<bool>>("Send To Job", nullptr, &protex6, BoolDisplay::OnOff, false, [] {	 });
			sub->AddOption<BoolOption<bool>>("Teleport", nullptr, &protex7, BoolDisplay::OnOff, false, [] {	 });
			sub->AddOption<BoolOption<bool>>("Sound Spam", nullptr, &protex8, BoolDisplay::OnOff, false, [] {	 });
			sub->AddOption<BoolOption<bool>>("Transaction Error", nullptr, &protex9, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("Bounty", nullptr, &protex11, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("Wanted Removal", nullptr, &protex12, BoolDisplay::OnOff, false, [] {	 });
			sub->AddOption<BoolOption<bool>>("Weapon Removal", nullptr, &Features::REMOVE_WEAPON_EVENT, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Camera Spam", nullptr, &protex17, BoolDisplay::OnOff, false, [] {	 });
			/*sub->AddOption<BoolOption<bool>>("Explosion", nullptr, &Features::EXPLOSION_EVENT, BoolDisplay::OnOff, false, [] { 	 });
			sub->AddOption<BoolOption<bool>>("Weather", nullptr, &Features::GAME_WEATHER_EVENT, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Time", nullptr, &Features::GAME_CLOCK_EVENT, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Fire", nullptr, &Features::FIRE_EVENT, BoolDisplay::OnOff, false, [] { 	 });*/
			sub->AddOption<BoolOption<bool>>("Freeze", nullptr, &Features::NETWORK_CLEAR_PED_TASKS_EVENT, BoolDisplay::OnOff, false, [] {  });
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Recovery", RECOVERY, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Stats/Unlocks", nullptr, UNLOCKS);
			sub->AddOption<SubOption>("Tunables", nullptr, TUNABLES);
			sub->AddOption<SubOption>("View reports", nullptr, VIEW_REPORTS);
			sub->AddOption<SubOption>("Casino Tunables ~r~[Risky]", nullptr, CASINO_TUNABLES);
			sub->AddOption<RegularOption>("~g~Clear reports", "", [] {
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_BECAME_BADSPORT_NUM"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BAD_SPORT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("CHEAT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), 32, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_FRIENDLY"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_HELPFUL"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GRIEFING"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_LANGUAGE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_UGC"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_VC_HATE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_ISPUNISHED"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_BECAME_BADSPORT_NUM"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BAD_SPORT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("CHEAT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), 32, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_FRIENDLY"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_HELPFUL"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GRIEFING"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_LANGUAGE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_UGC"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("_MPPLY_VC_HATE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_ISPUNISHED"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_BECAME_BADSPORT_NUM"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BAD_SPORT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("CHEAT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), 32, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_FRIENDLY"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_HELPFUL"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GRIEFING"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_LANGUAGE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_UGC"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_VC_HATE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_ISPUNISHED"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_BECAME_BADSPORT_NUM"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BAD_SPORT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("CHEAT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), 32, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_FRIENDLY"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_HELPFUL"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GRIEFING"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_LANGUAGE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_UGC"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_VC_HATE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_ISPUNISHED"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_BECAME_BADSPORT_NUM"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BAD_SPORT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("CHEAT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), 32, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_FRIENDLY"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_HELPFUL"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GRIEFING"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_LANGUAGE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_UGC"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_VC_HATE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_ISPUNISHED"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_BECAME_BADSPORT_NUM"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BAD_SPORT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("CHEAT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), 32, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_FRIENDLY"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_HELPFUL"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GRIEFING"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_LANGUAGE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_OFFENSIVE_UGC"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_VC_HATE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_ISPUNISHED"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_BECAME_BADSPORT_NUM"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BAD_SPORT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHEAT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_REPORT_STRENGTH"), 32, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_COMMEND_STRENGTH"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_FRIENDLY"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_HELPFUL"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GRIEFING"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_LANGUAGE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_UGC"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_VC_HATE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GAME_EXPLOITS"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_ISPUNISHED"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_BECAME_BADSPORT_NUM"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BAD_SPORT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHEAT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_REPORT_STRENGTH"), 32, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_COMMEND_STRENGTH"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_FRIENDLY"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_HELPFUL"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GRIEFING"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_LANGUAGE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_UGC"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_VC_HATE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GAME_EXPLOITS"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_ISPUNISHED"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_BECAME_BADSPORT_NUM"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BAD_SPORT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHEAT_BITSET"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_REPORT_STRENGTH"), 32, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_COMMEND_STRENGTH"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_FRIENDLY"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_HELPFUL"), 100, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GRIEFING"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_LANGUAGE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_UGC"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_VC_HATE"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GAME_EXPLOITS"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_ISPUNISHED"), 0, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_BECAME_BADSPORT_NUM"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BAD_SPORT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHEAT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_REPORT_STRENGTH"), 32, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_COMMEND_STRENGTH"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_FRIENDLY"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_HELPFUL"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GRIEFING"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_LANGUAGE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_UGC"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_VC_HATE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GAME_EXPLOITS"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_ISPUNISHED"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_BECAME_BADSPORT_NUM"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BAD_SPORT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHEAT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_REPORT_STRENGTH"), 32, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_COMMEND_STRENGTH"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_FRIENDLY"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_HELPFUL"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GRIEFING"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_LANGUAGE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_UGC"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_VC_HATE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GAME_EXPLOITS"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_ISPUNISHED"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_BECAME_BADSPORT_NUM"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BAD_SPORT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHEAT_BITSET"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_REPORT_STRENGTH"), 32, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_COMMEND_STRENGTH"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_FRIENDLY"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_HELPFUL"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GRIEFING"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_LANGUAGE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_UGC"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_VC_HATE"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GAME_EXPLOITS"), 0, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_ISPUNISHED"), 0, 1);
				});
			sub->AddOption<NumberOption<std::int32_t>>("Set Level:", nullptr, &Features::levelrp, 0, 8000, 1, 3, true, "", "", [] { 
				Features::SetRank(Features::levelrp); PrintBottomLeft((char*)"~g~Now switch online session for the level to take effect!");
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::levelrp = getInputs();
						if (Features::levelrp > 8000)
							Features::levelrp = 8000;
						if (Features::levelrp < 0)
							Features::levelrp = 0;
						Features::SetRank(Features::levelrp);
					});
				}
			});
			
			sub->AddOption<NumberOption<std::int32_t>>("Stealth Money Amount:", nullptr, &Features::StealthAmount, 10000, 3610000, 10000, 3, true, "", "",[] { 
				//Features::doStealth(Features::StealthAmount);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::StealthAmount = getInputs();
						if (Features::StealthAmount > 3610000)
							Features::StealthAmount = 3610000;
						if (Features::StealthAmount < 10000)
							Features::StealthAmount = 10000;
						Features::doStealth(Features::StealthAmount);
					});
				}
			
			});
			sub->AddOption<BoolOption<bool>>("Stealth Money Loop", nullptr, &Features::StealthLoop, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Shoot Cash", nullptr, &Features::shootcashbool, BoolDisplay::OnOff, false, [] { 	 });
			sub->AddOption<BoolOption<bool>>("Self Money Drop", nullptr, &Features::savenewdrop, BoolDisplay::OnOff, false, [] { 	 });
			sub->AddOption<BoolOption<bool>>("Money Drop Rain", nullptr, &Features::moneyrain2k, BoolDisplay::OnOff, false, [] { 	 });
			sub->AddOption<NumberOption<std::int32_t>>("Spoofed Level:", nullptr, &Features::SpoofLevelsAmount, 1, 9999, 1, 3, true, "", "", [] {
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						Features::SpoofLevelsAmount = getInputs();
						if (Features::SpoofLevelsAmount > 9999)
							Features::SpoofLevelsAmount = 9999;
						if (Features::SpoofLevelsAmount < 0)
							Features::SpoofLevelsAmount = 0;
						Features::SpoofLevels(Features::SpoofLevelsAmount);
						});
				}
				});
			sub->AddOption<BoolOption<bool>>("~g~Enable ~w~Spoof Level", nullptr, &Features::SpoofLevel, BoolDisplay::OnOff, false, [] { 	 });

		});
		g_UiManager->AddSubmenu<RegularSubmenu>("View reports", VIEW_REPORTS, [](RegularSubmenu* sub)//
			{
				int reportkek2;
				STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), &reportkek2, -1);
				const auto& reportstringkek2 = "~y~Report Strenght: ~w~"s + std::to_string(reportkek2);
				sub->AddOption<RegularOption>(reportstringkek2.c_str(), "", [] {});

				int reportkek;
				STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), &reportkek, -1);
				const auto& reportstringkek = "~r~Gameplay Exploits: ~w~"s + std::to_string(reportkek);
				sub->AddOption<RegularOption>(reportstringkek.c_str(), "", [] {});

				int reportkek4;
				STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), &reportkek4, -1);
				const auto& reportstringkek4 = "~r~Badsport: ~w~"s + std::to_string(reportkek4);
				sub->AddOption<RegularOption>(reportstringkek4.c_str(), "", [] {});

				int reportkek7;
				STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_GRIEFING"), &reportkek7, -1);
				const auto& reportstringkek7 = "~r~Griefing: ~w~"s + std::to_string(reportkek7);
				sub->AddOption<RegularOption>(reportstringkek7.c_str(), "", [] {});

				int reportkek3;
				STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), &reportkek3, -1);
				const auto& reportstringkek3 = "~g~Commend Strenght: ~w~"s + std::to_string(reportkek3);
				sub->AddOption<RegularOption>(reportstringkek3.c_str(), "", [] {});

				int reportkek5;
				STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_FRIENDLY"), &reportkek5, -1);
				const auto& reportstringkek5 = "~g~Friendly Stuff: ~w~"s + std::to_string(reportkek5);
				sub->AddOption<RegularOption>(reportstringkek5.c_str(), "", [] {});

				int reportkek6;
				STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_HELPFUL"), &reportkek6, -1);
				const auto& reportstringkek6 = "~g~Helpful Stuff: ~w~"s + std::to_string(reportkek6);
				sub->AddOption<RegularOption>(reportstringkek6.c_str(), "", [] {});
					

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Stats/Unlocks", UNLOCKS, [](RegularSubmenu* sub)
		{
			//sub->AddOption<BoolOption<bool>>("R* Dev", nullptr, &Features::DevMode, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<RegularOption>("Unlock All", "", [] { Features::UnlockAll(); });
			sub->AddOption<RegularOption>("Max Snacks", "", [] { Features::UnlockSnacks(); });
			sub->AddOption<RegularOption>("Max Stats", "", [] { Features::UnlockStats(); });
			sub->AddOption<RegularOption>("Unlock Tattoos", "", [] { Features::UnlockTattoos(); });
			sub->AddOption<RegularOption>("Unlock Rims", "", [] { Features::UnlockRims(); });
			sub->AddOption<RegularOption>("Unlock Vehicles", "", [] { Features::UnlockVehicles(); });
			sub->AddOption<RegularOption>("Unlock Achievements", "", [] { Features::UnlockAchievements(); });
			sub->AddOption<RegularOption>("Unlock Hairstyles", "", [] { Features::UnlockHairstyles(); });
			sub->AddOption<RegularOption>("Unlock Weapons", "", [] { Features::UnlockWeapons(); });
			sub->AddOption<RegularOption>("Unlock Clothes", "", [] { Features::UnlockClothes(); });
			sub->AddOption<RegularOption>("SecuroServ Office Cash", "", [] { Features::OfficeMoney(); });
			sub->AddOption<RegularOption>("SecuroServ Office Statue", "", [] { Features::OfficeStatue(); });

		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Casino Tunables ~r~[Risky]", CASINO_TUNABLES, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Reset Chip Cooldown", "", [] {
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_CASINO_CHIPS_PUR_GD"), 0, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_CASINO_CHIPS_PURTIM"), 0, 1);
					});
				sub->AddOption<RegularOption>("Reset Lucky Wheel Usage", "", [] {
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_LUCKY_WHEEL_USAGE"), 0, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_LUCKY_WHEEL_USAGE"), 0, 1);
					});
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Tunables", TUNABLES, [](RegularSubmenu* sub)
		{
			if (true)
			{
				sub->AddOption<BoolOption<bool>>("Unlock Bunker Research", nullptr, &ResearchUnlocks, BoolDisplay::OnOff, false, [] {	globalHandle(0x40001 + 21252).As<bool>() = ResearchUnlocks; globalHandle(262145).At(21252).As<bool>() = ResearchUnlocks; });
			}
			sub->AddOption<BoolOption<bool>>("Snow", nullptr, &Features::SnowBool, BoolDisplay::OnOff, false, [] {	globalHandle(0x40001 + 0x1271).As<bool>() = Features::SnowBool;});
			sub->AddOption<BoolOption<bool>>("Snow 2", nullptr, &Features::SnowBool, BoolDisplay::OnOff, false, [] {	globalHandle(0x40001 + 0xCF0758BE).As<bool>() = Features::SnowBool; });
			sub->AddOption<BoolOption<bool>>("Christmas", nullptr, &Features::XmasShitBool, BoolDisplay::OnOff, false, [] {	globalHandle(0x40001 + 0x127C).As<bool>() = Features::XmasShitBool;});
			sub->AddOption<BoolOption<bool>>("Halloween", nullptr, &Features::HalloweenShit, BoolDisplay::OnOff, false, [] { Features::HalloweenEnable(); });
			sub->AddOption<BoolOption<bool>>("Christmas Clothing", nullptr, &Features::ChristmasClothingBool, BoolDisplay::OnOff, false, [] { globalHandle(0x40001 + 0x2080).As<bool>() = Features::ChristmasClothingBool;});
			sub->AddOption<BoolOption<bool>>("Christmas Masks", nullptr, &Features::ChristmasMaskBool, BoolDisplay::OnOff, false, [] {	globalHandle(0x40001 + 0x2081).As<bool>() = Features::ChristmasMaskBool; });
			sub->AddOption<BoolOption<bool>>("Christmas Vehicles", nullptr, &Features::ChristmasVehicleBool, BoolDisplay::OnOff, false, [] {	globalHandle(0x40001 + 0x2082).As<bool>() = Features::ChristmasVehicleBool; });
			sub->AddOption<BoolOption<bool>>("Independence Day", nullptr, &Features::IndpendenceDayBool, BoolDisplay::OnOff, false, [] {	globalHandle(0x40001 + 7681).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7707).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7699).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7697).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7698).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 6221).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7700).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7707).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7708).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7709).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7710).As<bool>() = Features::IndpendenceDayBool; globalHandle(0x40001 + 7711).As<bool>() = Features::IndpendenceDayBool;globalHandle(0x40001 + 7701).As<bool>() = Features::IndpendenceDayBool; });
			sub->AddOption<BoolOption<bool>>("Beer Hats", nullptr, &Features::BeerHat1Bool, BoolDisplay::OnOff, false, [] {	Features::BeerHats(); });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Demo_sub", SubmenuTest, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("RegularOption_demo", "A regular option.", []
			{
				g_Logger->Info("You pressed the test option");
			});

			static bool testBool1{};
			sub->AddOption<BoolOption<bool>>("BoolOnOff_demo", nullptr, &testBool1, BoolDisplay::OnOff);
			static bool testBool2{};
			sub->AddOption<BoolOption<bool>>("BoolYesNo_demo", nullptr, &testBool2, BoolDisplay::YesNo);

			static std::int32_t int32Test{ 69 };
			sub->AddOption<NumberOption<std::int32_t>>("Int32", nullptr, &int32Test, 0, 100);

			static std::int64_t int64Test{ 420 };
			sub->AddOption<NumberOption<std::int64_t>>("Int64", nullptr, &int64Test, 0, 1000, 10);

			static float floatTest{ 6.9f };
			sub->AddOption<NumberOption<float>>("Float", nullptr, &floatTest, 0.f, 10.f, 0.1f, 1);

			static std::vector<std::uint64_t> vector{ 1, 2, 3 };
			static std::size_t vectorPos{};

			sub->AddOption<ChooseOption<const char*, std::size_t>>("Array", nullptr, &Lists::DemoList, &Lists::DemoListPos);
			sub->AddOption<ChooseOption<std::uint64_t, std::size_t>>("Vector", nullptr, &vector, &vectorPos);
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Settings", SubmenuExtra, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("FPS", nullptr, &Features::DisplayFPS, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("MPH", nullptr, &Features::MPH, BoolDisplay::OnOff);
			sub->AddOption<SubOption>("Credits", nullptr, Credits);
			sub->AddOption<SubOption>("UI Settings", nullptr, SubmenuSettings);
			sub->AddOption<RegularOption>("~r~Unload Menu", "", [] { g_Running = false; });
			sub->AddOption<RegularOption>("~r~Close Game", "", [] { exit(0); });

		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Credits", Credits, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Mistyy", "Developer", [] { if (Features::Hidden) Features::Hidden = false; if (!Features::Hidden) Features::Hidden = true; });
			sub->AddOption<RegularOption>("Beebey", "Helper", [] { });
			sub->AddOption<RegularOption>("Klexxon", "Big Helper :D", [] { });

		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Outfits", Outfits, [](RegularSubmenu* sub)
		{
			//Features::listoutfit();
			//sub->AddOption<RegularOption>("Save Outfit", "", [] { SaveOutfit();  });
			//sub->AddOption<ChooseOption<std::string, std::size_t>>("Load Outfit", nullptr, &Features::ListOutfit, &Lists::DemoListPos30, false, [] { Features::LoadOutfit(Features::ListOutfit[Lists::DemoListPos30]);	 });
			sub->AddOption<NumberOption<std::int32_t>>("Face:", nullptr, &testa, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 0), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 0, testa, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Head:", nullptr, &testb, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 1), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 1, testb, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Hair:", nullptr, &testc, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 2), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 2, testc, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Torso:", nullptr, &testd, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 3), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 3, testd, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Legs:", nullptr, &teste, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 4), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 4, teste, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Hands:", nullptr, &testf, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 5), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 5, testf, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Feet:", nullptr, &testg, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 6), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 6, testg, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Eyes:", nullptr, &testh, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 7), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 7, testh, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Accessories:", nullptr, &testi, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 8), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 8, testi, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Tasks:", nullptr, &testj, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 9), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 9, testj, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Textures:", nullptr, &testk, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 10), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 10, testk, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Torso2:", nullptr, &testl, 0, PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), 11), 1, 3, true, "", "", [] { PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 11, testl, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Head Props:", nullptr, &testm, 0, 255, 1, 3, true, "", "", [] { PED::SET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 1, testm, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Eye Props:", nullptr, &testn, 0, 255, 1, 3, true, "", "", [] { PED::SET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 2, testn, 0, 0); });
			sub->AddOption<NumberOption<std::int32_t>>("Ear Props:", nullptr, &testo, 0, 255, 1, 3, true, "", "", [] { PED::SET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 3, testo, 0, 0); });

		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Settings", SubmenuSettings, [](RegularSubmenu* sub)
		{
			Features::LoadPlayerInfo(g_SelectedPlayer);
			sub->AddOption<BoolOption<bool>>("Alt Header Image", nullptr, &Features::HeaderImage, BoolDisplay::OnOff);
			sub->AddOption<SubOption>("Header", nullptr, SubmenuSettingsHeader);
			sub->AddOption<SubOption>("Infobar", nullptr, SubmenuSettingsSubmenuBar);
			sub->AddOption<SubOption>("Options", nullptr, SubmenuSettingsOption);
			sub->AddOption<SubOption>("Footer", nullptr, SubmenuSettingsFooter);
		//	sub->AddOption<SubOption>("Description", nullptr, SubmenuSettingsDescription);
			sub->AddOption<SubOption>("Input", nullptr, SubmenuSettingsInput);
			sub->AddOption<NumberOption<float>>("X Position", nullptr, &g_UiManager->m_PosX, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("Y Position", nullptr, &g_UiManager->m_PosY, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("A Position", nullptr, &g_UiManager->m_PosA, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("B Position", nullptr, &g_UiManager->m_PosB, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("C Position", nullptr, &g_UiManager->m_PosC, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("D Position", nullptr, &g_UiManager->m_PosD, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("E Position", nullptr, &g_UiManager->m_PosE, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("F Position", nullptr, &g_UiManager->m_PosF, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("G Position", nullptr, &g_UiManager->m_PosG, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("H Position", nullptr, &g_UiManager->m_PosH, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("I Position", nullptr, &g_UiManager->m_PosI, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("J Position", nullptr, &g_UiManager->m_PosJ, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("K Position", nullptr, &g_UiManager->m_PosK, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("L Position", nullptr, &g_UiManager->m_PosL, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("M Position", nullptr, &g_UiManager->m_PosM, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("N Position", nullptr, &g_UiManager->m_PosN, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("O Position", nullptr, &g_UiManager->m_PosO, 0.f, 1.f, 0.01f, 2);
		});
#include "SHVASILoader.h"
		g_UiManager->AddSubmenu<RegularSubmenu>("Language", SubmenuSettingsLanguage, [](RegularSubmenu* sub)
		{
			namespace fs = std::filesystem;
			fs::directory_iterator dirIt{ GetModulePath(GetModuleHandleA(NULL)) };
			for (auto&& dirEntry : dirIt)
			{
				if (dirEntry.is_regular_file())
				{
					auto path = dirEntry.path();
					if (path.has_filename())
					{
						if (path.extension() == ".asi")
						{
						sub->AddOption<RegularOption>(path.stem().u8string().c_str(), nullptr, [=]
						{
							//ASILoader::LoadPlugins(path.stem().u8string().c_str());
						});
						}

					}
				}
			}

		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Infobar", SubmenuSettingsSubmenuBar, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_SubmenuBarHeight, 0.01f, 0.1f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Text Size", nullptr, &g_UiManager->m_SubmenuBarTextSize, 0.01f, 1.f, 0.01f, 2);

			sub->AddOption<NumberOption<std::uint8_t>>("Background R", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background G", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background B", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background A", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text R", nullptr, &g_UiManager->m_SubmenuBarTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text G", nullptr, &g_UiManager->m_SubmenuBarTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text B", nullptr, &g_UiManager->m_SubmenuBarTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text A", nullptr, &g_UiManager->m_SubmenuBarTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Options", SubmenuSettingsOption, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_OptionHeight, 0.01f, 0.1f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Text Size", nullptr, &g_UiManager->m_OptionTextSize, 0.01f, 1.f, 0.01f, 2);

			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background R", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background G", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background B", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background A", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background R", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background G", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background B", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background A", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));

			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text R", nullptr, &g_UiManager->m_OptionSelectedTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text G", nullptr, &g_UiManager->m_OptionSelectedTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text B", nullptr, &g_UiManager->m_OptionSelectedTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text A", nullptr, &g_UiManager->m_OptionSelectedTextColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text R", nullptr, &g_UiManager->m_OptionUnselectedTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text G", nullptr, &g_UiManager->m_OptionUnselectedTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text B", nullptr, &g_UiManager->m_OptionUnselectedTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text A", nullptr, &g_UiManager->m_OptionUnselectedTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Footer", SubmenuSettingsFooter, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_FooterHeight, 0.01f, 0.1f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Sprite Size", nullptr, &g_UiManager->m_FooterSpriteSize, 0.01f, 1.f, 0.001f, 3);

			sub->AddOption<NumberOption<std::uint8_t>>("Background R", nullptr, &g_UiManager->m_FooterBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background G", nullptr, &g_UiManager->m_FooterBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background B", nullptr, &g_UiManager->m_FooterBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background A", nullptr, &g_UiManager->m_FooterBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite R", nullptr, &g_UiManager->m_FooterSpriteColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite G", nullptr, &g_UiManager->m_FooterSpriteColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite B", nullptr, &g_UiManager->m_FooterSpriteColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite A", nullptr, &g_UiManager->m_FooterSpriteColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header", SubmenuSettingsHeader, [](RegularSubmenu* sub)
		{
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Type", nullptr, &Lists::HeaderTypesFrontend, &Lists::HeaderTypesPosition, true, []
			{
				g_UiManager->m_HeaderType = Lists::HeaderTypesBackend[Lists::HeaderTypesPosition];
			});

			switch (g_UiManager->m_HeaderType)
			{
			case HeaderType::Static:
				sub->AddOption<SubOption>("Background", nullptr, SubmenuSettingsHeaderStaticBackground);
				break;
			case HeaderType::Gradient:
				sub->AddOption<SubOption>("Sprite", nullptr, SubmenuSettingsHeaderGradientBackground);
				break;
			}

			sub->AddOption<SubOption>("Text", nullptr, SubmenuSettingsHeaderText);
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_HeaderHeight, 0.01f, 0.2f, 0.001f, 3);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Background", SubmenuSettingsHeaderStaticBackground, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<std::uint8_t>>("R", nullptr, &g_UiManager->m_HeaderBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G", nullptr, &g_UiManager->m_HeaderBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B", nullptr, &g_UiManager->m_HeaderBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A", nullptr, &g_UiManager->m_HeaderBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Gradient", SubmenuSettingsHeaderGradientBackground, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Transparent", nullptr, &g_UiManager->m_HeaderGradientTransparent, BoolDisplay::YesNo);
			sub->AddOption<BoolOption<bool>>("Flip", nullptr, &g_UiManager->m_HeaderGradientFlip, BoolDisplay::YesNo);

			sub->AddOption<NumberOption<std::uint8_t>>("R1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("R2", nullptr, &g_UiManager->m_HeaderGradientColorRight.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G2", nullptr, &g_UiManager->m_HeaderGradientColorRight.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B2", nullptr, &g_UiManager->m_HeaderGradientColorRight.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A2", nullptr, &g_UiManager->m_HeaderGradientColorRight.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Text", SubmenuSettingsHeaderText, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Enable", nullptr, &g_UiManager->m_HeaderText, BoolDisplay::OnOff);
			sub->AddOption<NumberOption<float>>("Size", nullptr, &g_UiManager->m_HeaderTextSize, 0.1f, 2.f, 0.01f, 2);
			sub->AddOption<NumberOption<std::uint8_t>>("R", nullptr, &g_UiManager->m_HeaderTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G", nullptr, &g_UiManager->m_HeaderTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B", nullptr, &g_UiManager->m_HeaderTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A", nullptr, &g_UiManager->m_HeaderTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Description", SubmenuSettingsDescription, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Padding", "Padding before the description rect.", &g_UiManager->m_DescriptionHeightPadding, 0.0001f, 10.f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Height", "Size of the description rect.", &g_UiManager->m_DescriptionHeight, 0.01f, 10.f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Text Size", "Size of the description text.", &g_UiManager->m_DescriptionTextSize, 0.1f, 20.f, 0.01f, 2);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Input", SubmenuSettingsInput, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<std::int32_t>>("Open Delay", nullptr, &g_UiManager->m_OpenDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Back Delay", nullptr, &g_UiManager->m_BackDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Enter Delay", nullptr, &g_UiManager->m_EnterDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Vertical Delay", nullptr, &g_UiManager->m_VerticalDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Horizontal Delay", nullptr, &g_UiManager->m_HorizontalDelay, 10, 1000, 10, 0);
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Players", SubmenuPlayerList, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("All Players", nullptr, AllPlayers);
			for (std::uint32_t i = 0; i < 32; ++i)
			{
				if (sub->GetSelectedOption() == sub->GetNumOptions()) {
					Features::LoadPlayerInfo(i);
				}
				if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
				{
					sub->AddOption<SubOption>(PlayerName(i).c_str(), nullptr, SubmenuSelectedPlayer, [=]
					{
						g_SelectedPlayer = i;
					});
				}
			}
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("All Players", AllPlayers, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Teleports", nullptr, TELEPORTALL);
			sub->AddOption<SubOption>("Script Events", nullptr, SCRIPTSALL);
			sub->AddOption<BoolOption<bool>>("Explode", nullptr, &Features::ExplodeAll, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Drop Money", nullptr, &Features::CashDropAll, BoolDisplay::OnOff, false, [] { });
			sub->AddOption<BoolOption<bool>>("Water Loop", nullptr, &Features::WaterAll, BoolDisplay::OnOff, false, [] {  });
			sub->AddOption<BoolOption<bool>>("Fire Loop", nullptr, &Features::FireAll, BoolDisplay::OnOff, false, [] {  });
			sub->AddOption<BoolOption<bool>>("Freeze", nullptr, &Features::FreezeAll, BoolDisplay::OnOff, false, [] {  });
			sub->AddOption<RegularOption>("Remove Weapons", "", [] {
				for (int i = 0; i <= 32; i++)
				{
					int Handle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
					WEAPON::REMOVE_ALL_PED_WEAPONS(Handle, 1);
				}
			});
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Script Events", SCRIPTSALL, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Sound Spam", nullptr, &Features::SoundSpamAll, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Remote OTR", nullptr, &Features::RemoteOTRAll, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Cops Blind Eye", nullptr, &Features::BlindEyeAll, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Camera Spam", nullptr, &Features::CameraSpamAll, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<BoolOption<bool>>("Remote Never Wanted", nullptr, &Features::NeverWantedAll, BoolDisplay::OnOff, false, [] {});
			sub->AddOption<RegularOption>("Send To Job", "", [] { Features::SendToJob(true); });
			sub->AddOption<RegularOption>("Kick Session", "", [] { Features::KickPlayer(true); });
			sub->AddOption<RegularOption>("CEO Kick", "", [] { Features::CEOKick(true); });
			sub->AddOption<RegularOption>("CEO Ban", "", [] { Features::CEOBan(true); });
			sub->AddOption<RegularOption>("Kick From Vehicle", "", [] { Features::GentleKick(true); });
		});

//Put Back here
		g_UiManager->AddSubmenu<RegularSubmenu>("Teleports", TELEPORTALL, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Eclipse Towers", "", [] { Features::Teleport1(true); });
			sub->AddOption<RegularOption>("West Vinewood", "", [] { Features::Teleport2(true); });
			sub->AddOption<RegularOption>("Las Lagunas", "", [] { Features::Teleport3(true); });
			sub->AddOption<RegularOption>("Spanish Ave", "", [] { Features::Teleport4(true); });
			sub->AddOption<RegularOption>("Power St", "", [] { Features::Teleport5(true); });
			sub->AddOption<RegularOption>("Del Perro", "", [] { Features::Teleport6(true); });
			sub->AddOption<RegularOption>("Milton Rd", "", [] { Features::Teleport7(true); });
			sub->AddOption<RegularOption>("The Royale", "", [] { Features::Teleport8(true); });
			sub->AddOption<RegularOption>("La Puerta", "", [] { Features::Teleport9(true); });
			sub->AddOption<RegularOption>("Rockford Dr", "", [] { Features::Teleport10(true); });
			sub->AddOption<RegularOption>("Dream Tower", "", [] { Features::Teleport11(true); });
			sub->AddOption<RegularOption>("San Vitas", "", [] { Features::Teleport12(true); });
			sub->AddOption<RegularOption>("Vespucci Blvd", "", [] { Features::Teleport13(true); });
			sub->AddOption<RegularOption>("Cougar Ave", "", [] { Features::Teleport14(true); });
			sub->AddOption<RegularOption>("Prosperity St", "", [] { Features::Teleport15(true); });
			sub->AddOption<RegularOption>("Weazel Plaza", "", [] { Features::Teleport16(true); });
			sub->AddOption<RegularOption>("Del Perro Office", "", [] { Features::Teleport17(true); });
			sub->AddOption<RegularOption>("Maze Bank West Office", "", [] { Features::Teleport18(true); });
			sub->AddOption<RegularOption>("Arcadius Office", "", [] { Features::Teleport19(true); });
			sub->AddOption<RegularOption>("Maze Bank Office", "", [] { Features::Teleport20(true); });
		});

		g_UiManager->AddSubmenu<PlayerSubmenu>(&g_SelectedPlayer, SubmenuSelectedPlayer, [](PlayerSubmenu* sub)
		{
			sub;
		});
	}

	void MainScript::Destroy()
	{
		g_UiManager.reset();
	}

	void MainScript::Tick()
	{
		g_UiManager->OnTick();
	}
}
