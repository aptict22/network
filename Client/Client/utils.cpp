#pragma once
#include "utils.h"
#include "Client.h"
#include "AV.h"
#include <ShellAPI.h>

extern Client myClient;

ESET32 *nod = new ESET32();

bool testFile(std::string path)
{
	//HANDLE hfile = NULL;
	std::cout << "start test" << std::endl;
	Sleep(2000);
	std::wstring wPath = s2ws(path);
	if (checkFileRead(path))
	{
		std::string str = nod->GetAVName() + ": ";
		std::cout << "GetScreen" << std::endl;
		nod->GetScreenShot();
		std::cout << "scrennOk" << std::endl;
		str += nod->GetReportString();
		std::cout << "Report Ok!" << std::endl;
		myClient.SendString(str);
		return true;
	}
	//Run file
/*	
	SHELLEXECUTEINFO ShExecInfo = { 0 };

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = wPath.c_str();
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOWNORMAL;
	ShExecInfo.hInstApp = NULL;

	BOOL result = ShellExecuteExW(&ShExecInfo);
	std::cout << "Second target run!" << std::endl;
	if (!result)
	{
		std::string str = "";
		nod->GetScreenShot();
		std::cout << "scrennOk" << std::endl;
		str = nod->GetReportString();
		std::cout << "Report Ok!" << std::endl;
		myClient.SendString(str);
		return true;
	}
	Sleep(2000);
	HANDLE hfile = ShExecInfo.hProcess;
	DWORD lpExitCode = 0;
	BOOL exitCode = GetExitCodeProcess(hfile, &lpExitCode);
	std::cout << "Second target OK!"<< std::endl << "Third target run!" << std::endl;
	if (lpExitCode != 0)
	{
		std::string str = "Clear";
		myClient.SendString(str);
		TerminateProcess(hfile, 0);
		CloseHandle(hfile);
		std::cout << "OK!" << std::endl;
		return true;
	}
	else
	{
		std::string str = "";
		nod->GetScreenShot();
		std::cout << "scrennOk" << std::endl;
		str = nod->GetReportString();
		std::cout << "Report Ok!" << std::endl;
		myClient.SendString(str);
		//getDetectMark();
		return true;
	}
	std::string str = "Clear";
	std::cout << "Clear OK!" << std::endl;
	myClient.SendString(str);
	TerminateProcess(hfile, 0);
	CloseHandle(hfile);*/
	return false;
}

BOOL checkFileRead(std::string path)
{
	DWORD FileSize = 0;
	std::cout << "CheckFileread" << std::endl;
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "detect" << std::endl;
		CloseHandle(hFile);
		return true;
	}
	FileSize = GetFileSize(hFile, NULL);
	if (FileSize == INVALID_FILE_SIZE || FileSize == 0)
	{
		std::cout << "detect" << std::endl;
		CloseHandle(hFile);
		return true;
	}
	CloseHandle(hFile);
	return false;
}

std::wstring s2ws(const std::string& str)
{
	if (str.empty())
		return std::wstring();

	int size_needed;

	size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);

	std::wstring wstrTo(size_needed, 0);

	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);

	return wstrTo;
}

HBITMAP screenshot(RECT rect)
{
	HDC hScreenDC = GetDC(NULL);
	// and a device context to put it in
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	int width = GetDeviceCaps(hScreenDC, HORZRES);
	int height = GetDeviceCaps(hScreenDC, VERTRES);

	// maybe worth checking these are positive values
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, abs(rect.right - rect.left), abs(rect.top - rect.bottom));

	// get a new bitmap
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

	BitBlt(hMemoryDC, 0, 0, abs(rect.right - rect.left), abs(rect.top - rect.bottom), hScreenDC, rect.left, rect.top, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

	// clean up
	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);
	return hBitmap;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	std::string Name = "Kaspersky";
	std::string caption = "AlertWindow";
	//char class_name[80];
	char win_caption[80];
	//GetClassNameA(hwnd, class_name, sizeof(class_name));
	GetWindowTextA(hwnd, win_caption, 80);
	if (win_caption == caption)
	{
		RECT rect;
		HBITMAP bmp;
		CImage image;

		Sleep(1500);
		SetForegroundWindow(hwnd);
		GetWindowRect(hwnd, &rect);
		bmp = screenshot(rect);
		image.Attach(bmp);
		Name += ".png";
		std::wstring saveName = s2ws(Name);
		image.Save(saveName.c_str());
		//find = true;
		std::cout << "find!" << std::endl;
		return true;

	}
	return true;
}


std::string getDetectMark(AntiVirusName Name)
{
	switch (Name)
	{
	case ESET:
		break;
	case Kaspersky:
		break;
	default:
		break;
	}
	return "";
}

std::string ws2s(const std::wstring& wstr)
{
	if (wstr.empty())
		return std::string();

	int size_needed;

	size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);

	std::string strTo(size_needed, 0);

	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);

	return strTo;

	return "";
}