#ifndef LOCAL_MANAGER
#define LOCAL_MANAGER
#include "stdafx.h"

#define SECTOR_SIZE 512
#define CLUSTER_SIZE 4096

class LocalManager
{
public:
	LocalManager(std::string _driveLetter);
	HANDLE * getDiskHandle();
	char * readMBR();
	bool setDiskHandle();
	~LocalManager();

private:
	std::wstring drive; //EX. \\.\PHYSICALDRIVE0
	LARGE_INTEGER disk_offset;
	DWORD offsetCount;
	HANDLE diskHandle;
	std::string PartitionType;
	std::wstring s2ws(const std::string& s); // seperate
	char readBuffer[512] = { 0, };
	//void * buffer;

};








#endif