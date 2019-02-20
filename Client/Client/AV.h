#pragma once
#include "utils.h"
#include <string>


class AVReport
{
protected:
	std::string Win_Class_Name;
	std::string Win_Capture;
	std::string AVName;
public:
	AVReport(std::string class_name, std::string Capture, std::string Name)
		: Win_Class_Name(class_name), Win_Capture(Capture), AVName(Name) {};
	~AVReport() {};
	virtual void GetScreenShot() = 0;
	virtual std::string GetReportString() = 0;
	virtual std::string GetWinClassName() const = 0;
	virtual std::string GetCapture() const = 0;
	virtual std::string GetAVName() const = 0;
};

class ESET32 : public AVReport
{
public:
	ESET32() : AVReport("EsetBaseWindow", "ESET Desktop view", "NOD32") {};
	std::string GetWinClassName() const override;
	std::string GetCapture() const override;
	void GetScreenShot() override;
	std::string GetReportString() override;
	std::string GetAVName() const override;
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