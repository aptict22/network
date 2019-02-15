#pragma once
#include "utils.h"
#include <string>


class AVReport
{
protected:
	std::string Win_Class_Name;
	std::string Win_Capture;
public:
	AVReport(std::string class_name, std::string Capture) : Win_Class_Name(class_name), Win_Capture(Capture) {};
	~AVReport() {};
	virtual void GetScreenShot() = 0;
	virtual std::string GetReportString() = 0;
	virtual std::string GetWinClassName() = 0;
	virtual std::string GetCapture() = 0;
};

class ESET32 : public AVReport
{
public:
	ESET32() : AVReport("EsetBaseWindow", "ESET Desktop view") {};
	std::string GetWinClassName() override;
	std::string GetCapture() override;
	void GetScreenShot() override;
	std::string GetReportString() override;
};
/*
class Kaspersky : public AVReport
{
public:
	Kaspersky() : AVReport("Kaspersky", "AlertWindow") {};
	std::string GetWinClassName() override;
	std::string GetCapture() override;
	void GetScreenShot() override;
	std::string GetReportString(std::string path) override;
private:

};*/