#ifndef FILEBACKUP_H
#define FILEBACKUP_H
#include "backup.h"
#include "PlanBCommon.h"
class FileBackup : public Backup
{
	/* Not Implemented */
public:
	~FileBackup();
	virtual PlanB::BackupResult * exec();
	virtual PlanB::JobStatus cancel();
	FileBackup(long _backupKey, std::wstring _sourcePath, std::wstring _destinationPath, bool _isCompressed, bool _isSplited);
private:

};


#endif