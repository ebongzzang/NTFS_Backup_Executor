#ifndef Backup_Executor
#define Backup_Executor
#include "stdafx.h"

#define SECTOR_SIZE 512
#define CLUSTER_SIZE 4096

class BackupExecutor
{
public:
	BackupExecutor(std::string _driveLetter);
	HANDLE * getDiskHandle();

	char * readMBR();
	bool setDiskHandle();
	~BackupExecutor();

private:
	std::wstring disk; //EX. \\.\PHYSICALDRIVE0
	LARGE_INTEGER disk_offset;
	DWORD offsetCount;
	HANDLE diskHandle;
	std::string PartitionType;
	std::wstring s2ws(const std::string& s); // need independent class
	std::wstring convertDriveLetter(std::string dLetter);
	void determinePhysicalDisk(HANDLE drive);
	char readBuffer[SECTOR_SIZE] = { 0, };
	//void * buffer;

};








#endif