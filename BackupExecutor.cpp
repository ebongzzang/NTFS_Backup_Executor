#include "BackupExecutor.h"
#include <memory>
#include <string>
#include "stringToolset.h"

BackupExecutor::BackupExecutor(long _backupKey, std::string _driveLetter,
	BackupStyle _backupStyle,BackupType _BackupType, bool _isCompressed, bool _isSplited)//example: C
{
	if (_BackupType == PlanB::BackupType::System)
	{ 
		pBackup = new SystemBackup(01, "G", "C:\\", TRUE, TRUE);
		pBackup->exec();
	}


}

BackupExecutor::~BackupExecutor()
{
	delete pBackup;
}




