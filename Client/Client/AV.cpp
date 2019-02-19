#include "AV.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <ShlObj.h>
#include <ShellAPI.h>


bool find = false;

std::string ESET32::GetWinClassName()
{
	return Win_Class_Name;
}

std::string ESET32::GetCapture()
{
	return Win_Capture;
}

BOOL CALLBACK EnumWindowsProcNOD(HWND hwnd, LPARAM lParam)
{
	ESET32 *nod = new ESET32();
	std::string Name = nod->GetWinClassName();
	char win_caption[80];
	GetWindowTextA(hwnd, win_caption, 80);
	if (win_caption == nod->GetCapture())
	{
		DWORD dwStyle = (DWORD)GetWindowLong(hwnd, GWL_STYLE);
		if (dwStyle & WS_VISIBLE)
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
			find = true;
			std::cout << "find!" << std::endl;
			delete nod;
			return false;
		}
	}
	return true;
}

void ESET32::GetScreenShot()
{
	while (!find)
	{
		Sleep(500);
		EnumWindows(EnumWindowsProcNOD, NULL);
		Sleep(100);
	}
	find = false;
}

std::string ESET32::GetReportString()
{
	wchar_t DesktopPath[MAX_PATH];
	std::wstring ImagePath = L"";

	SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, 0, DesktopPath);

	ImagePath.assign(DesktopPath);
	ImagePath += L"\\";
	ImagePath += s2ws(this->GetWinClassName()) + L".png";

	std::wstring tesseractPath = L"";
	tesseractPath.assign(DesktopPath);
	tesseractPath += L"\\Tesseract\\tesseract.exe";

	std::wstring OutPath = L"";
	OutPath.assign(DesktopPath);
	OutPath += L"\\out.txt";

	std::wstring param = ImagePath + L" " + OutPath;

	SHELLEXECUTEINFO ShExecInfo = { 0 };

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = tesseractPath.c_str();
	ShExecInfo.lpParameters = param.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOWNORMAL;
	ShExecInfo.hInstApp = NULL;

	BOOL result = ShellExecuteExW(&ShExecInfo);

	HANDLE hFile = ShExecInfo.hProcess;
	CloseHandle(hFile);
	Sleep(7000);
	std::string outstring = "";
	std::ifstream file(ws2s(OutPath));
	file >> outstring;
	file.close();
	std::cout << outstring << std::endl;
	return outstring;
}


