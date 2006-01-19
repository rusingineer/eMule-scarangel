#include "stdafx.h"

#include "SystemInfo.h"

CSystemInfo::CSystemInfo(){
	CSysInfo::Init();
}

CSystemInfo::~CSystemInfo(){

}

int CSystemInfo::GetCpuUsage(){
	return CPUUsageReader::ReadCPUUsage();
}

DWORD CSystemInfo::GetMemoryUsage(){
	return CSysInfo::GetProcessMemoryUsage();
}