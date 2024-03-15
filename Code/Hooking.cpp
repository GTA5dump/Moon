#include "Hooking.hpp"
#include "Game.hpp"
#include "ScriptManager.hpp"
#include "CustomText.hpp"
#include "srcEngine.h"
#include "Util.hpp"
#include "D3DRenderer.hpp"
#include "UI/UIManager.hpp"
#include <MinHook/MinHook.h>
#include <discord_rpc.h>
#include "Hashes.h"
#include <discord_register.h>
#include "SHVScriptManager.h"
DWORD wakeAt;

#ifdef Moon_DEBUG
#  pragma comment(lib, "MinHook-Debug.lib")
#else
#  pragma comment(lib, "MinHook-Release.lib")
#  pragma comment(lib, "discord-rpc.lib")
#endif

namespace Big
{
	namespace
	{
		std::uint32_t g_HookFrameCount{};
	}
	scrThread* ActiveThread()
	{
		return *(scrThread**)(*(std::uintptr_t*)(__readgsqword(0x58)) + 0x830);
	}
	void notifyBlocked(char * fmt, ...)
	{
		char buf[2048] = { 0 };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(buf, fmt, va_alist);
		va_end(va_alist);

		char buff2[2048] = { 0 };
		sprintf_s(buff2, "%s", buf);

		UI::_SET_NOTIFICATION_BACKGROUND_COLOR(6);
		UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(buff2);
		UI::_DRAW_NOTIFICATION(1, 1);
	} void notifyBlocked(std::string str) { notifyBlocked(&str[0]); }

	bool Hooks::IsDlcPresent(std::uint32_t hash)
	{
		if (Features::DevMode)
		{
			if (hash == 2532323046UL)
			{
				return true;
			}
		}
		return static_cast<decltype(&IsDlcPresent)>(g_Hooking->m_OriginalIsDlcPresent)(hash);
	}
	bool Hooks::IsEntityUpsideDown(std::uint32_t entity)
	{
		if (g_Running && g_HookFrameCount != *g_GameVariables->m_FrameCount)
		{
			g_HookFrameCount = *g_GameVariables->m_FrameCount;
			g_ScriptManager->OnGameTick();
		}
		return static_cast<decltype(&IsEntityUpsideDown)>(g_Hooking->m_OriginalIsEntityUpsideDown)(entity);
	}
	int Hooks::GetEventData(int eventGroup, int eventIndex, int* argStruct, int argStructSize)
	{
		bool result = static_cast<decltype(&GetEventData)>(g_Hooking->m_OriginalGetEventData)(eventGroup, eventIndex, argStruct, argStructSize);
		if (result)
		{
			char* name = PLAYER::GET_PLAYER_NAME(argStruct[1]);
			if (Features::LogEvent)
			{
				g_Logger->Info("Script Event Group: %i", eventGroup);
				g_Logger->Info("Script Event Index: %i", eventIndex);
				g_Logger->Info("Script Event Argcount: %u", argStructSize);
				for (unsigned int i = 0; i < argStructSize; ++i)
				{
					g_Logger->Info("Script Event Args[%i]: %i", i, argStruct[i]);
				}
			}
		}

		return static_cast<decltype(&GetEventData)>(g_Hooking->m_OriginalGetEventData)(eventGroup, eventIndex, argStruct, argStructSize);
	}
	bool Hooks::TestHook(__int64 a, __int64 b, int c, char d)
	{
		string int1 = to_string(a);
		string int2 = to_string(b);
		string int3 = to_string(c);
		string int4 = to_string(d);
		g_Logger->Info(int1.c_str());
		g_Logger->Info(int2.c_str());
		g_Logger->Info(int3.c_str());
		g_Logger->Info(int4.c_str());
		return true;
	}
	struct RttiCompleteObjectLocator
	{
		std::uint32_t m_Signature;
		std::uint32_t m_OffsetFromBase;
		std::uint32_t m_ConstructorDisplacement;
		std::uint32_t m_TypeDescRva;
		std::uint32_t m_HierarchyDescRva;
		std::uint32_t m_BaseClassRva;
	};

	struct RttiTypeDesc
	{
		virtual ~RttiTypeDesc() = 0;
		char* m_UnmangledName;
		char m_MangledName[1];
	};

	inline const char* GetDynamicTypeName(void* obj)
	{
		__try
		{
			auto vtable = *(RttiCompleteObjectLocator***)obj;
			RttiCompleteObjectLocator* locator = *(vtable - 1);

			HMODULE base;
			if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)locator, &base))
			{
				RttiTypeDesc* desc = (RttiTypeDesc*)((std::uintptr_t)(base) + locator->m_TypeDescRva);
				return desc->m_MangledName;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

		return nullptr;
	}
	/*Metric AAC Detours*/















	static bool is_script_thread_good(scrThread *thread)
	{
		if (!thread || !thread->m_handler)
		{
			return false;
		}
		return !*g_GameVariables->is_session_started || thread->m_handler->is_networked();
	}
#include "SHVScriptEngine.h"
	int Hooks::GetNumOfEvents(int eventgroup)
	{
		if (g_Running && g_HookFrameCount != *g_GameVariables->m_FrameCount && is_script_thread_good(ActiveThread()))//
		{
			g_HookFrameCount = *g_GameVariables->m_FrameCount;
			g_ScriptManager->OnGameTick();
			switch (g_HookState)
			{
			case HookStateRunning:
			{
				SHVScriptManager::MainFiber();
			} break;

			case HookStateExiting:
			{
			} break;

			default:
				break;
			}
		}//
		return static_cast<decltype(&GetNumOfEvents)>(g_Hooking->m_OriginalGetNumOfEvents)(eventgroup);
	}

	int Hooks::ScriptVM(void* stack, signed __int64** globals, void* program, scrThreadContext* threadContext)
	{
		for (auto thread : *g_GameVariables->script_threads)
		{
			if (!thread)
			{
				continue;
			}
			if (thread->m_context.m_state != eThreadState::running)
			{
				continue;
			}
			if (thread->m_context.m_script_hash == CONSTEXPR_JOAAT("freemode") || CONSTEXPR_JOAAT("main_persistent"))
			{
				if (g_Running && g_HookFrameCount != *g_GameVariables->m_FrameCount && is_script_thread_good(ActiveThread()) && !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()))//
				{
					g_HookFrameCount = *g_GameVariables->m_FrameCount;
					g_ScriptManager->OnGameTick();
					if (true)
					{
						switch (g_HookState)
						{
						case HookStateRunning:
						{
							SHVScriptManager::MainFiber();
						} break;

						case HookStateExiting:
						{
						} break;

						default:
							break;
						}
					}
				}//
			}
		}
		return static_cast<decltype(&ScriptVM)>(g_Hooking->m_OriginalScriptVM)(stack, globals, program, threadContext);

	}
















	bool Hooks::NetworkCanBail()
	{
		return false;
	}
	int Hooks::TriggerScriptEvent(bool unk0, uint64_t* args, int argCount, int bitFlags)
	{
		if (Features::LogEvent)
		{
			g_Logger->Info("-------------------------");
			g_Logger->Info("TSE");
			for (int i = 0; i < argCount; i++)
				g_Logger->Info("args[%i]: 0x%016llx => %d", i, args[i], args[i]);
			g_Logger->Info("-------------------------");
		}
		return static_cast<decltype(&TriggerScriptEvent)>(g_Hooking->m_OriginalTriggerScriptEvent)(unk0, args, argCount, bitFlags);
	}
	void Hooks::DisableErrorScreen(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL background)
	{
			if (SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME() == GAMEPLAY::GET_HASH_KEY("shop_controller"))
			{
				return;
			}
		return static_cast<decltype(&DisableErrorScreen)>(g_Hooking->m_OriginalDisableErrorScreen)(entryHeader, entryLine1, instructionalKey, entryLine2, p4, p5, p6, p7, background);
	}
	bool command_check(std::string& str, std::string check)
	{
		for (int i = 0; i < str.size(); i++)
			str[i] = tolower(str[i]);

		std::size_t found = str.find_first_of(' ');
		if (found != std::string::npos)
		{
			if (str.substr(0, found) == check)
			{
				str = str.substr(found + 1, str.size());
				return true;
			}
		}
		else
		{
			if (str == check)
				return true;
		}
		return false;
	}

	
	const char* Hooks::GetLabelText(void* unk, const char* label)
	{
		if (g_Running)
			if (auto text = g_CustomText->GetText(Joaat(label)))
				return text;

		return static_cast<decltype(&GetLabelText)>(g_Hooking->m_OriginalGetLabelText)(unk, label);
	}
	bool Hooks::DenyDrops(void *classidonthave, void *unk, CPed* ped)
	{
		if (Features::AntiMoneyDrop)
		{
			return false;
		}
		return static_cast<decltype(&DenyDrops)>(g_Hooking->m_OriginalDenyDrops)(classidonthave, unk, ped);
	}
	LRESULT Hooks::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		SHVScriptManager::WndProc(hWnd, msg, wParam, lParam);
		//g_D3DRenderer->WndProc(hWnd, msg, wParam, lParam);
		return static_cast<decltype(&WndProc)>(g_Hooking->m_OriginalWndProc)(hWnd, msg, wParam, lParam);
	}

	HRESULT Hooks::Present(IDXGISwapChain* dis, UINT syncInterval, UINT flags)
	{
		if (g_Running)
		{
			g_D3DRenderer->BeginFrame();
			g_ScriptManager->OnD3DTick();
			g_D3DRenderer->EndFrame();
		}

		return g_Hooking->m_D3DHook.GetOriginal<decltype(&Present)>(PresentIndex)(dis, syncInterval, flags);
	}
	HRESULT Hooks::ResizeBuffers(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
	{
		if (g_Running)
		{
			g_D3DRenderer->PreResize();
			auto hr = g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
			if (SUCCEEDED(hr))
			{
				g_D3DRenderer->PostResize();
			}

			return hr;
		}
		return g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
	}
#include "Functions.h"
	void Hooks::NetworkManager(__int64 a1, CNetGamePlayers* a2, CNetGamePlayers* a3, unsigned __int16 a4, int a5, int a6, __int64 a7, __int64 a8)
	{
		auto ptr = Memory::pattern("66 83 FA ? 73 13 0F B7 C2 4C 89 84 C1 ? ? ? ? 4C 89 8C C1").count(1).get(0).get<char>(0);
		auto num_events = *(std::uint8_t*)(ptr + 3);
		auto name_offset = *(std::int32_t*)(ptr + 21);
		auto eventName = reinterpret_cast<char*>(*reinterpret_cast<__int64*>(a1 + 8 * a4 + name_offset));

		if (Features::LogEvent)
		{
			g_Logger->Info("------------------");
			g_Logger->Info(eventName);
			g_Logger->Info(a2->PlayerInfo->sName);
			g_Logger->Info(a3->PlayerInfo->sName);
			g_Logger->Info(to_string(a4).c_str());
			g_Logger->Info(to_string(a5).c_str());
			g_Logger->Info(to_string(a6).c_str());
			g_Logger->Info(to_string(a7).c_str());
			g_Logger->Info(to_string(a8).c_str());
			g_Logger->Info("------------------");
		}
		if (Features::REMOVE_WEAPON_EVENT)
		{
			if (a3 == g_GameFunctions->m_GetPlayerNetworkId(PLAYER::PLAYER_ID()))
			{
				if (a4 == 14)
				{
					if (Features::EventNotif)
					notifyBlocked("Event: REMOVE_ALL_WEAPONS_EVENT\nFrom: " + (std::string)a2->PlayerInfo->sName + "\nStatus: Blocked");
						return;
				}
			}
		}
		if (Features::EXPLOSION_EVENT)
		{
			if (a3 == g_GameFunctions->m_GetPlayerNetworkId(PLAYER::PLAYER_ID()))
			{
				if (a4 == 11)
				{
					if (Features::EventNotif)
					notifyBlocked("Event: EXPLOSION_EVENT\nFrom: " + (std::string)a2->PlayerInfo->sName + "\nStatus: Blocked");
					return;
				}
			}
		}
		if (Features::GAME_WEATHER_EVENT)
		{
			if (a3 == g_GameFunctions->m_GetPlayerNetworkId(PLAYER::PLAYER_ID()))
			{
				if (a4 == 10)
				{
					if (Features::EventNotif)
					notifyBlocked("Event: GAME_WEATHER_EVENT\nFrom: " + (std::string)a2->PlayerInfo->sName + "\nStatus: Blocked");
					return;
				}
			}
		}
		if (Features::GAME_CLOCK_EVENT)
		{
			if (a3 == g_GameFunctions->m_GetPlayerNetworkId(PLAYER::PLAYER_ID()))
			{
				if (a4 == 9)
				{
					if (Features::EventNotif)
					notifyBlocked("Event: GAME_CLOCK_EVENT\nFrom: " + (std::string)a2->PlayerInfo->sName + "\nStatus: Blocked");
					return;
				}
			}
		}
		if (Features::FIRE_EVENT)
		{
			if (a3 == g_GameFunctions->m_GetPlayerNetworkId(PLAYER::PLAYER_ID()))
			{
				if (a4 == 16)
				{
					if (Features::EventNotif)
					notifyBlocked("Event: FIRE_EVENT\nFrom: " + (std::string)a2->PlayerInfo->sName + "\nStatus: Blocked");
					return;
				}
			}
		}
		if (Features::NETWORK_CLEAR_PED_TASKS_EVENT)
		{
			if (a3 == g_GameFunctions->m_GetPlayerNetworkId(PLAYER::PLAYER_ID()))
			{
				if (a4 == 43)
				{
					if (Features::EventNotif)
					notifyBlocked("Event: NETWORK_CLEAR_PED_TASKS_EVENT\nFrom: " + (std::string)a2->PlayerInfo->sName + "\nStatus: Blocked");
					return;
				}
			}
		}
		if (a4 == 77)
		{
			Modders[a2->m_player_id] = 1;
		}
		return static_cast<decltype(&NetworkManager)>(g_Hooking->m_OriginalNetworkManager)(a1, a2, a3, a4, a5, a6, a7, a8);
	}
	bool Hooks::EventManager(void* events, CNetGamePlayers *sourcePlayer, CNetGamePlayers *targetPlayer)
	{
		if (targetPlayer == g_GameFunctions->m_GetPlayerNetworkId(PLAYER::PLAYER_ID()))
		{
			if (Features::BlockAllProtex)
			{
				return false;
			}
			auto args = (std::int64_t*)(((std::uintptr_t)events) + 0x70);
			auto argCount = *(std::uint32_t*)(((std::uintptr_t)events) + 0x224) / sizeof(std::int64_t);
			switch (args[0])
			{
			case 713068249:
				if (protex9)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (args[2] == -1695772783)//
						{
							if (Features::EventNotif)
								notifyBlocked("Event: Transaction Error Event\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");
							return false;
						}
					}
				}
				break;
			case 552065831:
				if (protex8)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Sound Spam Event\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");
						return false;
					}
				}
				break;
			case -125347541:
				if (protex7)
				{
					if (Features::EventNotif)
						notifyBlocked("Event: Teleport Event\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

					return false;
				}
				break;
			case -615431083:
				if (protex4)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Remote OTR Event\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
					}
				break;
			case -1049031463:
				if (protex9)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Transaction/Banner Spam\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
			case 1385453222:
				if (protex11)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Bounty\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
			case 693546501:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [1] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case 149365611:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [2] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -941739545:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [3] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -1941292498:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [4] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case 1953937033:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [5] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case 297770348:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [6] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -815817885:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [7] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -498955166:
				if (protex17)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (args[2] == -1432407380)
						{
							if (Features::EventNotif)
								notifyBlocked("Event: Camera Spam Event\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");
							return false;
						}
					}
				}
				
			case 20218675039:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [8] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
				
			case -1424895288:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [9] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -1264063129:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [10] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case 423635655:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [11] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case 400031869:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [12] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;

			case -531496987:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [13] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case 1062544784:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [14] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -803645682:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [15] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -771910813:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [16] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;	
			case 1476326089:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [17] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case 1800294560:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [18] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case 1209128713:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [19] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -123645928:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [20] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -332594529:
				if (protex1)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Kick Event [21] \nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -163616977:
				if (protex12)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Remove Wanted Event\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -348418057:
				if (protex6)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: Send To Job Event\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				
				break;
			case -701823896:
				if (protex2)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: CEO Kick Event\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			case -327286343:
				if (protex3)
				{
					if (args[1] == PLAYER::PLAYER_ID())
					{
						if (Features::EventNotif)
							notifyBlocked("Event: CEO Ban Event\nFrom: " + (std::string)sourcePlayer->PlayerInfo->sName + "\nStatus: Blocked");

						return false;
					}
				}
				break;
			}
		}
		return static_cast<decltype(&EventManager)>(g_Hooking->m_OriginalEventManager)(events, sourcePlayer, targetPlayer);
	}
	CNetGamePlayers* Hooks::GetPlayerByIndex(int index)
	{
		if (index < 0 || index >= 32)
		{
			return nullptr;
		}

		return g_GameVariables->g_players[index];
	}
	bool Hooks::InfoEvent(void* thisptr, CNetGamePlayer* source, CNetGamePlayer* target)
	{
		g_Logger->Info(source->GetName());
		Modders[source->m_player_id] = true;
		return static_cast<decltype(&InfoEvent)>(g_Hooking->m_OriginalInfoEvent)(thisptr, source, target);
	}
	bool Hooks::ReportMyselfEvent(void* thisptr, CNetGamePlayer* source, CNetGamePlayer* target)
	{
		if (source = nullptr) 		return static_cast<decltype(&ReportMyselfEvent)>(g_Hooking->m_OriginalReportMyselfEvent)(thisptr, source, target);
		if (target = nullptr) 		return static_cast<decltype(&ReportMyselfEvent)>(g_Hooking->m_OriginalReportMyselfEvent)(thisptr, source, target);
		if (thisptr = nullptr)		return static_cast<decltype(&ReportMyselfEvent)>(g_Hooking->m_OriginalReportMyselfEvent)(thisptr, source, target);
		Modders[source->m_player_id] = true;
		return static_cast<decltype(&ReportMyselfEvent)>(g_Hooking->m_OriginalReportMyselfEvent)(thisptr, source, target);
	}
	void Hooks::ResourceError(std::uint32_t errorHash, bool terminateProcess)
	{
		if (errorHash == 2207393597) {
			g_Logger->Info("Same Hash, passed to Resource Error");
			terminateProcess = 0;
			return;
		}
			terminateProcess = 0;
		g_Logger->Info(to_string(errorHash).c_str());
		g_Logger->Info(to_string(terminateProcess).c_str());
		return;
	}
	bool Hooks::ResourceCall(std::uint32_t errorHash)
	{
		if (errorHash == 2207393597)
		{
			g_Logger->Info("Same hash...");
			return 1;
		}
		errorHash = 0;
		g_Logger->Info(to_string(errorHash).c_str());
		return 1;
	}
//2207393597
	void Hooks::EventQue(void* thisptr, netGameEvent* event)
	{
		if (NETWORK::NETWORK_IS_SESSION_STARTED())
		{
			if (event != nullptr)
			{
				if (event->eventId == 0x4Au)
				{
					delete event;
					return;
				}
				if (event->eventId == 0x52u)
				{
					delete event;
					return;
				}
				if (event->eventId == 0x53u)
				{
					delete event;
					return;
				}
				if (event->eventId == 0x49u)
				{
					delete event;
					return;
				}
				if (event->eventId == 0x4Du)
				{
					delete event;
					return;
				}
			}
		}
		return static_cast<decltype(&EventQue)>(g_Hooking->m_OriginalEventQue)(thisptr, event);
	}
	int Hooks::JoinPlayer(__int64 a1, int a2, __int64 *a3, int a4) {
		if (Joiner)
		{
			if (a3 == 0)
			{
				return static_cast<decltype(&JoinPlayer)>(g_Hooking->m_OriginalJoinPlayer)(a1, a2, a3, a4);
			}
			*a3 = Join;

		}
		return static_cast<decltype(&JoinPlayer)>(g_Hooking->m_OriginalJoinPlayer)(a1, a2, a3, a4);
	}
	int Hooks::Pattern(int a1, __int64 a2, unsigned int a3, __int64 a4, __int64 a5, __int64 a6, __int64 a7)
	{
		if (Joiner) {
			a2 = Join;
			Joiner = false;
		}
		g_Logger->Info("----------------");
		g_Logger->Info(to_string(a1).c_str());
		g_Logger->Info(to_string(a2).c_str());
		g_Logger->Info(to_string(a3).c_str());
		g_Logger->Info(to_string(a4).c_str());
		g_Logger->Info(to_string(a5).c_str());
		g_Logger->Info(to_string(a6).c_str());
		g_Logger->Info(to_string(a7).c_str());
		g_Logger->Info("----------------");
		return static_cast<decltype(&Pattern)>(g_Hooking->m_OriginalPattern)(a1, a2, a3, a4, a5, a6, a7);
	}
	int Hooks::SensorChat(__int64 chat_menu, const char* user_text, const char** output_text)
	{
		if (Features::ChatSensor)
		{
			return -1;
		}
		return static_cast<decltype(&SensorChat)>(g_Hooking->m_OriginalSensorChat)(chat_menu, user_text, output_text);
	}
	void Hooks::EventPat(__int64 a1, unsigned int a2, unsigned int a3, unsigned int a4)
	{
		g_Logger->Info("-----Event Pat-------");
		g_Logger->Info(to_string(a1).c_str());
		g_Logger->Info(to_string(a2).c_str());
		g_Logger->Info(to_string(a3).c_str());
		g_Logger->Info(to_string(a4).c_str());
		g_Logger->Info("---------------------");
	}
	Hooking::Hooking():
		m_D3DHook(g_GameVariables->m_Swapchain, 18)
	{
		MH_Initialize();

		//MH_CreateHook(g_GameFunctions->m_IsEntityUpsideDown, &Hooks::IsEntityUpsideDown, &m_OriginalIsEntityUpsideDown); Old Hook
		MH_CreateHook(g_GameFunctions->m_ScriptVM, &Hooks::ScriptVM, &m_OriginalScriptVM);
//		MH_CreateHook(g_GameFunctions->m_GetNumOfEvents, &Hooks::GetNumOfEvents, &m_OriginalGetNumOfEvents);
	//	MH_CreateHook(g_GameFunctions->m_IsDlcPresent, &Hooks::IsDlcPresent, &m_OriginalIsDlcPresent);
		MH_CreateHook(g_GameFunctions->m_TriggerScriptEvent, &Hooks::TriggerScriptEvent, &m_OriginalTriggerScriptEvent);
		MH_CreateHook(g_GameFunctions->m_EventManager, &Hooks::EventManager, &m_OriginalEventManager);
		MH_CreateHook(g_GameFunctions->m_NetworkEvents, &Hooks::NetworkManager, &m_OriginalNetworkManager);
		MH_CreateHook(g_GameFunctions->m_ChatSensor, &Hooks::SensorChat, &m_OriginalSensorChat);
		MH_CreateHook(g_GameFunctions->m_EventPat, &Hooks::EventPat, &m_OriginalEventPat);
		MH_CreateHook(g_GameFunctions->m_DenyDrops, &Hooks::DenyDrops, &m_OriginalDenyDrops);
		MH_CreateHook(g_GameFunctions->m_DisableErrorScreen, &Hooks::DisableErrorScreen, &m_OriginalDisableErrorScreen);
	//	MH_CreateHook(g_GameFunctions->m_GetEventData, &Hooks::GetEventData, &m_OriginalGetEventData);
		MH_CreateHook(g_GameFunctions->m_JoinPattern, &Hooks::JoinPlayer, &m_OriginalJoinPlayer);
		MH_CreateHook(g_GameFunctions->m_WndProc, &Hooks::WndProc, &m_OriginalWndProc);
		//m_D3DHook.Hook(&Hooks::Present, Hooks::PresentIndex);
		//m_D3DHook.Hook(&Hooks::ResizeBuffers, Hooks::ResizeBuffersIndex);
	}

	Hooking::~Hooking() noexcept
	{
		MH_RemoveHook(g_GameFunctions->m_WndProc);
		MH_RemoveHook(g_GameFunctions->m_ScriptVM);
		MH_RemoveHook(g_GameFunctions->m_GetNumOfEvents);
		MH_RemoveHook(g_GameFunctions->m_DenyDrops);
		//MH_RemoveHook(g_GameFunctions->m_IsDlcPresent);
		MH_RemoveHook(g_GameFunctions->m_DisableErrorScreen);//
		MH_RemoveHook(g_GameFunctions->m_EventPat);//
		MH_RemoveHook(g_GameFunctions->m_ChatSensor);//
		MH_RemoveHook(g_GameFunctions->m_NetworkEvents);
		MH_RemoveHook(g_GameFunctions->m_ChatMessage);
		MH_RemoveHook(g_GameFunctions->m_EventManager);
		MH_RemoveHook(g_GameFunctions->m_TriggerScriptEvent);
		//MH_RemoveHook(g_GameFunctions->m_GetEventData);
		MH_Uninitialize();
	}

	void Hooking::Hook()
	{
		//m_D3DHook.Enable();
		MH_EnableHook(MH_ALL_HOOKS);
	}

	void Hooking::Unhook()
	{
		//m_D3DHook.Disable();
		MH_DisableHook(MH_ALL_HOOKS);
		Discord_Shutdown();//
	}
	void set_active_thread(scrThread* thread)
	{
		*(scrThread**)(*(std::uintptr_t*)(__readgsqword(0x58)) + 0x830) = thread;
	}
	struct objectHash
	{
		DWORD* ptr;
		DWORD	hash;
	};

#define get_array_size(array)	(sizeof(array) / sizeof(array[0]))
	static std::vector<objectHash>	g_antiCrash;
	DWORD __stdcall antiCrashSearch(LPVOID lpParam)
	{
		 static void* objectHashTable;
		 static void* objectHashTableSectionEnd;

		auto&& hashTableRegion = Signature("01 ? ? 08 43 7f 2e 27 ? ? FF 0F").VirtualScan();
		objectHashTable = hashTableRegion.Pointer();
		objectHashTableSectionEnd = hashTableRegion.End();
		Hash* hashptr = (Hash*)objectHashTable;
		char* end = (char*)objectHashTableSectionEnd - 0x20;

		for (; (char*)hashptr < end; ++hashptr)
		{
			if (*hashptr == 0)
				continue;
			for (int i = 0; i < get_array_size(forbidden_object); ++i)
			{
				if (*hashptr != forbidden_object[i])
					continue;
				g_antiCrash.push_back({ hashptr, *hashptr });
				g_Logger->Info("working");
			}
		}
		return S_OK;
	}
	bool Hooking::Swapping(bool toggle)
	{
		static HANDLE	hAntiCrash = 0;
		constexpr Hash	dildo = 0xafc55086;
		Hash* hashptr = nullptr;
		char* end = nullptr;
		g_Logger->Info("1");
		if (!g_antiCrash.empty() || hAntiCrash)
			goto LABEL_SET_VALUE;
		g_Logger->Info("2");
		if (!toggle)
			return true;
		g_Logger->Info("3");
		hAntiCrash = CreateThread(NULL,
			0,
			antiCrashSearch,
			NULL,
			0,
			nullptr);
		g_Logger->Info("4");
	LABEL_SET_VALUE:
		g_Logger->Info("5");
		if (WaitForSingleObject(hAntiCrash, 0) != WAIT_OBJECT_0)	//if the thread is still alive, return; it's still searching
			return false;
		g_Logger->Info("clause");
		if (hAntiCrash)
			CloseHandle(hAntiCrash);
		g_Logger->Info("6");
		if (!toggle)
		{
			g_Logger->Info("7");
			for (std::vector<objectHash>::iterator it = g_antiCrash.begin(); it != g_antiCrash.end(); ++it)
			{
				if (*it->ptr == 0xafc55086)
				{
					g_Logger->Info("Yes1");
				}
				if (it->hash == 0xafc55086)
				{
					g_Logger->Info("Yes2");
				}
				if (*it->ptr == 0xdf9841d7)
				{
					g_Logger->Info("Yes3");
				}
				if (it->hash == 0xdf9841d7)
				{
					g_Logger->Info("Yes4");
				}

				if (*it->ptr != 0xafc55086 && *it->ptr != it->hash)
				{
					g_Logger->Info("found");
					continue;
				}
				g_Logger->Info("8");
				*it->ptr = 0xafc55086;
				it->hash = 0xafc55086;
				*it->ptr = it->hash;
			}
			return true;
		}
		g_Logger->Info("9");
		for (std::vector<objectHash>::iterator it = g_antiCrash.begin(); it != g_antiCrash.end(); ++it)
			*it->ptr = 0xafc55086;
		return true;
	}
	scrThread* find_networked(std::uint32_t hash)
	{
		for (auto thread : *g_GameVariables->script_threads)
		{
			if (!thread)
			{
				continue;
			}
			if (thread->m_context.m_state != eThreadState::running)
			{
				continue;
			}
			if (thread->m_context.m_script_hash != hash)
			{
				continue;
			}
			return thread;
		}
		return nullptr;
	}
	void Hooks::tick()
	{
		scrThread* networked_thread{};
		if (*g_GameVariables->is_session_started && (networked_thread = find_networked(CONSTEXPR_JOAAT("freemode"))) != nullptr)
		{
			if (g_GameVariables->is_session_started)
			{
				auto og_thread = ActiveThread();

			}
		}
		else
		{
			g_ScriptManager->OnGameTick();
		}
	}
}
