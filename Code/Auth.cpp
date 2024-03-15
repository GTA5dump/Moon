#include "Common.hpp"
#include "WinHttpClient.h"
#include "sha512.h"
#include <signal.h>

void FailClose()
{
	g_Running = false;
}
void FailHwid()
{
	g_Logger->Info("Your HWID is incorrect.");
	FailClose();
}
void FailLogin()
{
	g_Logger->Info("Failed login.  Check your username and password!"); //g_Logger->Info("Failed login.  Make sure you're using your forum credentials!");
	FailClose();
}
void TokenFail()
{
	g_Logger->Info("Authorization: FAILURE");
	FailClose();
}
void FailRole()
{
	g_Logger->Info("Role validation failed. You're not authorized for the menu.");
	FailClose();
}


void get_total_memory(std::string& buffer) {
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	buffer += std::to_string(statex.ullTotalPhys);
}

void get_computer_name(std::string& buffer) {
	char tmp[MAX_COMPUTERNAME_LENGTH + 1] = {};
	DWORD size = sizeof(tmp);
	GetComputerNameA(tmp, &size);
	buffer += tmp;
}

std::string get_hwid() {
	std::string buffer;

	get_total_memory(buffer);
	get_computer_name(buffer);
	return buffer;
}

int StringToWString(std::wstring& ws, const std::string& s)
{
	std::wstring wsTmp(s.begin(), s.end());

	ws = wsTmp;

	return 0;
}


Ini* settingsIni = new Ini(DocumentsPath());

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

int is_authed(std::string& username, std::string& password) {
	//CA2W ca2w(username.c_str());
	wstring newusername;
	StringToWString(newusername, username);

	//CA2W ca2w2(password.c_str());
	wstring newpassword;
	StringToWString(newpassword, password);

	//CA2W ca2w3(get_hwid().c_str());
	wstring newhwid;
	StringToWString(newhwid, get_hwid());
	// std::wstring finallink = L"https://moonlightgta.xyz/auth.php?u=" + newusername + L"&password=" + newpassword + L"&hwid=" + newhwid;
	std::wstring finallink = L"https://186.167.17.181/auth.php?username=" + newusername + L"&password=" + newpassword + L"&hwid=" + newhwid;
	WinHttpClient client(finallink);
	std::wstring response = client.GetResponseContent();


	if ((std::wstring)response == (std::wstring)L"Access granted") {
		notifyMap("~r~Check auth.ini file in Documents/Moonlight!", 1);
		ScriptEventLog("~r~[Login]: ~g~Successfully Logged In!");
		loggedin = true;
		return 1;
	}
	else if ((std::wstring)response == (std::wstring)L"Bad Access")
	{
		ScriptEventLog("~r~[Login]: ~r~User/Pass Invalid! ~y~Unloading...");
		FailLogin();
		return 0;
	}
	else if ((std::wstring)response == (std::wstring)L"Bad HWIDBad Access")
	{
		ScriptEventLog("~r~[Login]: ~r~Your HWID doesn't match with this account!");
		FailHwid();
		return 0;
	}
}