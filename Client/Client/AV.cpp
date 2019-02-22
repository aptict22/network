#include "AV.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <ShlObj.h>
#include <ShellAPI.h>


bool find = false;

std::string ESET32::GetWinClassName() const
{
	return Win_Class_Name;
}

std::string ESET32::GetCapture() const
{
	return Win_Capture;
}

std::string ESET32::GetAVName() const
{
	return AVName;
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
			Sleep(1000);
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
	OutPath += L"\\out";

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
	Sleep(3000);
	char buff[512];
	std::string str = "";
	std::string outstring = "";
	DWORD BytesRead = 0;
	OutPath += L".txt";
	HANDLE file = CreateFile(OutPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	ReadFile(file, &buff, 512, &BytesRead, NULL);
	CloseHandle(file);
	str.assign(buff);
//	std::cout << buff << std::endl;
	std::size_t pos = str.find('(', 2);
	pos++;
	for (; str[pos] != ')'; pos++)
		outstring += str[pos];
	std::cout << outstring << std::endl;
	return outstring;
}


