#pragma once
#include <atlimage.h>
#include <winuser.h>
#include <string>

enum AntiVirusName
{
	ESET,
	Kaspersky
};

HBITMAP screenshot(RECT rect);
bool testFile(std::string fullpath);
BOOL checkFileRead(std::string path);
std::wstring s2ws(const std::string& str);
std::string ws2s(const std::wstring& wstr);