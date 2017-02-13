#ifndef SYSTEM_BACKUP_H
#define SYSTEM_BACKUP_H
#include "backup.h"
#include "PlanBCommon.h"
#define SECTOR_SIZE 512
#define CLUSTER_SIZE 4096
class SystemBackup : public Backup
{
public:
	SystemBackup(long _backupKey, std::string _sourcePath, std::string _destinationPath,
		bool _isCompressed, bool _isSplited) : Backup(_backupKey, _sourcePath, _destinationPath, _isCompressed, _isSplited)
	{
		std::wstring uncPath = convertDriveLettertoUNC(sourcePath);
		//unc -> physicaldisk 

		HANDLE handle = CreateFile(uncPath.c_str(), GENERIC_READ | GENERIC_WRITE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		//unique_ptr?

		if (handle == INVALID_HANDLE_VALUE)
		{
			std::cout << "Convert DriveLetter to UNC path failed." << GetLastError() << std::endl;
		}

		physicalDisk = determinePhysicalDisk(handle);

		CloseHandle(handle);
	}
	virtual PlanB::JobStatus exec();
//	virtual PlanB::JobStatus cancel();
	virtual ~SystemBackup();

private:
	std::wstring physicalDisk;
	HANDLE physicalDiskHandle;
	std::wstring determinePhysicalDisk(HANDLE drive);
	std::wstring SystemBackup::writeBitmap();
	void SystemBackup::exportResultFile(std::wstring filename);
	bool readMBR();
	char * mbrBuffer;
	DWORD count;
	HANDLE vssHandle;


};



#endif // !SYSTEM_BACKUP_H
