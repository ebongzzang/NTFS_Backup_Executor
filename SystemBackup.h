#ifndef SYSTEM_BACKUP_H
#define SYSTEM_BACKUP_H
#include "backup.h"
#include "PlanBCommon.h"
#define SECTOR_SIZE 512
#define CLUSTER_SIZE 4096
class SystemBackup : public Backup
{
public:
	SystemBackup(long _backupKey, std::wstring _sourcePath, std::wstring _destinationPath,
	bool _isCompressed, bool _isSplited);
	virtual PlanB::BackupResult*  exec();
	virtual PlanB::JobStatus cancel();
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
