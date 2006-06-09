#include "stdafx.h"

#include "SystemInfo.h"
#include "GetCpuUsage.h" // added - Stulle

CSystemInfo::CSystemInfo(){
	CSysInfo::Init();
}

CSystemInfo::~CSystemInfo(){

}

int CSystemInfo::GetCpuUsage(){
	return CPUUsageReader::ReadCPUUsage();
}

uint32 CSystemInfo::GetMemoryUsage(){
	return CSysInfo::GetProcessMemoryUsageInt();
}

// added - Stulle
int CSystemInfo::GetGlobalCpuUsage(){
	return ReadGlobalCpuUsage();
}

uint32 CSystemInfo::GetGlobalMemoryUsage(){
	return (uint32)(CSysInfo::GetTotalPhys()-CSysInfo::GetAvailPhys());
}