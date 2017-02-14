#include "BackupExecutor.h"
#include <memory>
#include <string>

BackupExecutor::BackupExecutor(long _backupKey, std::string _driveLetter,
	BackupStyle _backupStyle,BackupType _BackupType, bool _isCompressed, bool _isSplited)//example: C
{
	if (_BackupType == PlanB::BackupType::System)
	{ 
		pBackup = new SystemBackup(01, "O", "C:\\", TRUE, TRUE);
		pBackup->exec();
	}


}
//BackupExecutor::exec() 
//{
//	pBackup->exec(parameter);
//}

BackupExecutor::~BackupExecutor()
{
	delete pBackup;
}




