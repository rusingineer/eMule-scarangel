#pragma once

#include "SysInfo.h"
#include "CPUUsageReader.h"

class CSystemInfo : public CSysInfo, public CPUUsageReader
{
public:
	CSystemInfo();
	~CSystemInfo();

	int		GetCpuUsage();
	uint32	GetMemoryUsage();
	int		GetGlobalCpuUsage(); // added - Stulle
	uint32	GetGlobalMemoryUsage();
};
