// Dll entry point
#include "Game.hpp"
#include "Hooking.hpp"
#include "ScriptManager.hpp"
#include "LogScript.hpp"
#include "MainScript.hpp"
#include "ScriptCallback.hpp"
#include "Invoker.hpp"
#include "CustomText.hpp"
#include "D3DRenderer.hpp"
#include "UI/UIManager.hpp"
#include "Translation.hpp"
#include <KnownFolders.h>
#include <ShlObj.h>
#include <comutil.h>
#include <discord_rpc.h>
#include <discord_register.h>
#include "Functions.h"
#include "Fibers.hpp"
#include "YTDLoader.h"
#pragma comment(lib, "discord-rpc.lib")
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "SHVUtility.h"
#include "SHVScriptEngine.h"
bool Dev = false;
bool times = false;
void Features::UpdatePresence()
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = "Playing with Moonlight";
	discordPresence.details = "Grand Theft Auto V";
	discordPresence.startTimestamp = time(0);
	discordPresence.largeImageKey = "dc";
	discordPresence.largeImageText = "https://moonlightgta.xyz";
	discordPresence.smallImageKey = "logo2";
	discordPresence.smallImageText = "Version: 3.0";
	Discord_UpdatePresence(&discordPresence);

}
std::string RPCCheck = "1"; //std::string RPCCheck;
HANDLE g_main_thread{};
BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID)
{
	using namespace Big;

	if (reason == DLL_PROCESS_ATTACH)
	{
		g_Module = hInstance;
		g_main_thread = CreateThread(nullptr, 0, [](LPVOID) -> DWORD
		{
			Ini *settingsIni = new Ini(DocumentsPath());
			//Features::Token = settingsIni->GetString("Login", "Token");
			//RPCCheck = settingsIni->GetString("Login", "DiscordRPC");
			m_Controller = settingsIni->GetString("Login", "Controller");
			//Features::ConsoleLogger = settingsIni->GetString("Login", "ConsoleLog");
			/*if (Features::Token == "NULL") {
				settingsIni->WriteString("TOKENHERE", "Login", "Token");
			}*/
			/*std::string username = "USERNAMEHERE";
			std::string password = "PASSWORDHERE";
			settingsIni->WriteString(username, "Login", "Username");
			settingsIni->WriteString(password, "Login", "Password");*/
			if (Features::ConsoleLogger == "NULL")  settingsIni->WriteString("false", "Login", "ConsoleLog");
			if (Features::ConsoleLogger == "1")  settingsIni->WriteString("true", "Login", "ConsoleLog");
			if (Features::ConsoleLogger == "0")  settingsIni->WriteString("false", "Login", "ConsoleLog");
			if (m_Controller == "NULL")  settingsIni->WriteString("false", "Login", "Controller");
			if (m_Controller == "1")  settingsIni->WriteString("true", "Login", "Controller");
			if (m_Controller == "0")  settingsIni->WriteString("false", "Login", "Controller");
			g_Logger = std::make_unique<Logger>();
			g_Logger->Info("Moonlight VIP Injected.");
			g_Logger->Info("https://moonlightgta.xyz/");
			m_OpenKey = settingsIni->GetInt("Controls", "Open");
			m_BackKey = settingsIni->GetInt("Controls", "Back");
			m_EnterKey = settingsIni->GetInt("Controls", "Enter");
			m_UpKey = settingsIni->GetInt("Controls", "Up");
			m_DownKey = settingsIni->GetInt("Controls", "Down");
			m_LeftKey = settingsIni->GetInt("Controls", "Left");
			m_RightKey = settingsIni->GetInt("Controls", "Right");
			m_ForwardBoost = settingsIni->GetInt("Controls", "ForwardBoost");
			m_BackwardBoost = settingsIni->GetInt("Controls", "BackwardBoost");
			std::string link = settingsIni->GetString("Controls", "KeyLink");
			if (link == "NULL")
			{
				settingsIni->WriteString("http://cherrytree.at/misc/vk.htm", "Controls", "KeyLink");
			}
			if (m_OpenKey == 90210) {
				settingsIni->WriteInt(106, "Controls", "Open");
				m_OpenKey = 106;
				g_Logger->Info("[Open] Control written to config");
			}
			if (m_BackKey == 90210) {
				settingsIni->WriteInt(96, "Controls", "Back");
				m_BackKey = 96;
				g_Logger->Info("[Back] Control written to config");
			}
			if (m_EnterKey == 90210) {
				settingsIni->WriteInt(101, "Controls", "Enter");
				m_EnterKey = 101;
				g_Logger->Info("[Enter] Control written to config");
			}
			if (m_UpKey == 90210) {
				settingsIni->WriteInt(104, "Controls", "Up");
				m_UpKey = 104;
				g_Logger->Info("[Up] Control written to config");
			}
			if (m_DownKey == 90210) {
				settingsIni->WriteInt(98, "Controls", "Down");
				m_DownKey = 98;
				g_Logger->Info("[Down] Control written to config");
			}
			if (m_LeftKey == 90210) {
				settingsIni->WriteInt(100, "Controls", "Left");
				m_LeftKey = 100;
				g_Logger->Info("[Left] Control written to config");
			}
			if (m_RightKey == 90210) {
				settingsIni->WriteInt(102, "Controls", "Right");
				m_RightKey = 102;
				g_Logger->Info("[Right] Control written to config");
			}
			if (m_ForwardBoost == 90210) {
				settingsIni->WriteInt(105, "Controls", "ForwardBoost");
				m_ForwardBoost = 105;
				g_Logger->Info("[Forward Boost] Control written to config");
			}
			if (m_BackwardBoost == 90210) {
				settingsIni->WriteInt(99, "Controls", "BackwardBoost");
				m_BackwardBoost = 99;
				g_Logger->Info("[Backward Boost] Control written to config");
			}
			




			if (RPCCheck == "NULL")  settingsIni->WriteString("true", "Login", "DiscordRPC");
			if(RPCCheck == "1")  settingsIni->WriteString("true", "Login", "DiscordRPC");
			if(RPCCheck == "0")  settingsIni->WriteString("false", "Login", "DiscordRPC");
		
			//if (Features::Token == "NULL") g_Logger->Info("[Error] Token is null/undefined");
			//Auth Here
			//token_validation(Features::Token);
			if (RPCCheck == "1")
			{
				Discord_Initialize("672175775524323338", nullptr, 1, NULL); //531076690206523412
				Features::UpdatePresence();
			}
			g_GameFunctions = std::make_unique<GameFunctions>();
			g_GameVariables = std::make_unique<GameVariables>();

			// Wait for the game to load
			/**
			while (*g_GameVariables->m_GameState != 0)
			{
				std::this_thread::sleep_for(3s);
				std::this_thread::yield();
			}
			*/
			g_GameVariables->PostInit();
			g_Invoker = std::make_unique<Invoker>();
			//g_CustomText = std::make_unique<CustomText>();
			//g_D3DRenderer = std::make_unique<D3DRenderer>();
			g_UiManager = std::make_unique<UserInterface::UIManager>();
			g_YtdLoader = std::make_unique<YTDLoader>();
			g_ScriptManager = std::make_unique<ScriptManager>();
			g_MainScript = std::make_shared<MainScript>();
			g_LogScript = std::make_shared<LogScript>();
			g_CallbackScript = std::make_shared<CallbackScript>();
			g_FiberScript = std::make_shared<FiberScript>();

			g_ScriptManager->AddScript(g_MainScript);

			g_ScriptManager->AddScript(g_LogScript);
			g_ScriptManager->AddScript(g_CallbackScript);
			g_ScriptManager->AddScript(g_FiberScript);

			g_Hooking = std::make_unique<Hooking>();
			g_Hooking->Hook();
			if(Elite) ScriptEngine::Initialize();
			g_Logger->Info("Moonlight VIP has been successfully loaded!");
			g_Logger->Info("================================================");
			g_Logger->Info("Press NUMPAD (*) to open the menu!");
			g_Logger->Info("Press DELETE to unload the menu.");
			while (g_Running)
			{
				if (IsKeyPressed(VK_DELETE))
				{
					g_Running = false;
				}
				std::this_thread::sleep_for(3ms);
				std::this_thread::yield();
			}
			g_Logger->Info("Moonlight VIP Unloaded.");
			std::this_thread::sleep_for(500ms);
			g_Hooking->Unhook();
			g_ScriptManager.reset();
			g_LogScript.reset();
			g_MainScript.reset();
			g_CallbackScript.reset();
			g_UiManager.reset();
			g_YtdLoader.reset();
			g_FiberScript.reset();
			//g_D3DRenderer.reset();
			//g_CustomText.reset();
			g_Invoker.reset();
			std::this_thread::sleep_for(500ms);
			g_Hooking.reset();//
			//g_TranslationManager.reset();
			g_GameVariables.reset();
			g_GameFunctions.reset();
			g_Logger.reset();

			CloseHandle(g_main_thread);
			FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
			FreeLibraryAndExitThread(g_Module, 0);
		}, nullptr, 0, nullptr);
	}

	return true;
}
