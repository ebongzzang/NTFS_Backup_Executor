#include "BackupExecutor.h"
#include <memory>
#include <string>
#include "stringToolset.h"

BackupExecutor::BackupExecutor(long _backupKey, std::string _driveLetter,
	BackupStyle _backupStyle,BackupType _BackupType)
	: backupKey(_backupKey), backupStyle(_backupStyle), backupType(_BackupType)//example: C
{
	std::wstring uncPath = convertDriveLetter(_driveLetter);

	HANDLE handle = CreateFile(uncPath.c_str(), GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	//unique_ptr?

	if (handle == INVALID_HANDLE_VALUE)
	{
		std::cout << "Convert DriveLetter to UNC path failed.  " << GetLastError() << std::endl;
	}

	Physicaldisk = determinePhysicalDisk(handle);
	CloseHandle(handle);
}

BackupExecutor::~BackupExecutor()
{
	
}

HANDLE * BackupExecutor::getDiskHandle()
{
	if (diskHandle == INVALID_HANDLE_VALUE)
	{
		return nullptr;
	}

	return &diskHandle;
}
bool BackupExecutor::setDiskHandle()
{
	diskHandle = CreateFile(Physicaldisk.c_str(), GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (diskHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "set DiskHandle Error!" << GetLastError() << std::endl;
		return false;
	}
	return true;
}
char * BackupExecutor::readMBR() //double pointer
{
	disk_offset.QuadPart = 0;
	if (!SetFilePointerEx(diskHandle, disk_offset, NULL, FILE_BEGIN))
	{
		std::cout << "filePointer error" << GetLastError() << std::endl;
		return nullptr;
	}
		

	if (!ReadFile(diskHandle, readBuffer, SECTOR_SIZE, &offsetCount, 0)) // These are not multiples of 512 or 4096, hence the "Invalid argument" error.
	{
		std::cout << "readfile error" << GetLastError() << std::endl;
		return nullptr;
	}

	/*GET_LENGTH_INFORMATION length;
	DWORD count;
	DeviceIoControl(diskHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &length, sizeof(GET_LENGTH_INFORMATION),&count,NULL);
	std::cout << "gem error" << GetLastError() <<std::endl;
	std::cout << length.Length.QuadPart << std::endl;
	*/
 	return readBuffer;

	//ofstream mbrtest;
	//mbrtest.open("C:\\hi.mbr", std::ios::binary);
	//mbrtest.write(readBuffer, 512);

}

