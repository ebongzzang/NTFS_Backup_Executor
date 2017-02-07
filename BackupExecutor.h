#pragma once
#ifndef Backup_Executor
#define Backup_Executor
#include "stdafx.h"
#include "PlanBCommon.h"
#define SECTOR_SIZE 512
#define CLUSTER_SIZE 4096

using PlanB::BackupStyle;
using PlanB::BackupType;
using PlanB::JobStatus;

class BackupExecutor 
{
	
public:
	BackupExecutor(long _backupKey,  std::string _driveLetter,  BackupStyle _backupStyle, 
		BackupType _BackupType); //style:full,incremental type - file,system, sourcepath
	//driveLetter
	HANDLE * getDiskHandle();

	char * readMBR();
	bool setDiskHandle();
	~BackupExecutor();

private:
	long backupKey;
	std::string driveLetter;
	BackupStyle backupStyle;
	BackupType backupType;

//	Backup * pBackup;


	std::wstring Physicaldisk; //EX. \\.\PHYSICALDRIVE0

	LARGE_INTEGER disk_offset;
	DWORD offsetCount;
	HANDLE diskHandle;
	std::string PartitionType;


	char readBuffer[SECTOR_SIZE] = { 0, };
	//void * buffer;

};








#endif