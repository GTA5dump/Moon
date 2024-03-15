#pragma once
#include "Common.hpp"
#define _HAS_STD_BYTE 0

std::wstring s2ws(const std::string& str);

int is_authed(std::string& username, std::string& password);


