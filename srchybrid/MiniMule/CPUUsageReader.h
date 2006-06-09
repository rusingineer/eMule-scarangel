#pragma once

class CPUUsageReader {
public:
	CPUUsageReader();
	~CPUUsageReader();
	int ReadCPUUsage();

private:
	bool fNTMethod;
	HKEY hkeyKernelCPU;

	unsigned __int64 kt_last;
	unsigned __int64 ut_last;
	unsigned __int64 st_last;
};
