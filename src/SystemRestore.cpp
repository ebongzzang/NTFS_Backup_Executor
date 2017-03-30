#include "SystemRestore.h"
#include <Windows.h>
#include <iostream>
#define CLUSTER_SIZE 4096
SystemRestore::SystemRestore(BackupResult * _Source)
	: Restore(_Source)
{

}
SystemRestore::~SystemRestore()
{

}

PlanB::JobStatus  SystemRestore::exec()
{
	HANDLE targetDisk = CreateFile(backupTargetPath.c_str(), GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
	HANDLE sourceFile = CreateFile(resultFilePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
	//TODO: \\Device\HardDiskN
	LARGE_INTEGER sourceFileSize;
	sourceFileSize.QuadPart = 0;
	GetFileSizeEx(sourceFile, &sourceFileSize);
	DWORD cnt;
	char readBuffer2[65536];

	DeviceIoControl(targetDisk, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &cnt, NULL);
	DeviceIoControl(targetDisk, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &cnt, NULL);

	for (int j = 0; j < (sourceFileSize.QuadPart / CLUSTER_SIZE) / 16; j++)
	{
		if (!ReadFile(sourceFile, readBuffer2, CLUSTER_SIZE * 16, &cnt, NULL))
			std::cout << "failed" << std::endl;
		if (!WriteFile(targetDisk, readBuffer2, CLUSTER_SIZE * 16, &cnt, NULL))
			std::cout << "write file error" << GetLastError() << std::endl;
	}

	DeviceIoControl(targetDisk, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &cnt, NULL);

	CloseHandle(targetDisk);
	CloseHandle(sourceFile);
	
	return JobStatus::SUCCESS;
}

PlanB::JobStatus SystemRestore::cancel()  /*Not Implemented */
{
	return JobStatus::CANCLED;

}