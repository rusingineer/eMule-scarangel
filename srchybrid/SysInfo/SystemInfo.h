#pragma once

#include "SysInfo.h"
#include "CPUUsageReader.h"

class CSystemInfo : public CSysInfo, public CPUUsageReader
{
public:
	CSystemInfo();
	~CSystemInfo();

	int		GetCpuUsage();
	DWORD	GetMemoryUsage();
};
