#include "BackupExecutor.h"
#include <memory>
#include <string>

BackupExecutor::BackupExecutor(long _backupKey, std::string _sourcePath,
	BackupStyle _backupStyle, BackupType _backupType, bool _isCompressed, bool _isSplited) 
	: backupKey(_backupKey),sourcePath(_sourcePath),backupStyle(_backupStyle)
	,backupType(_backupType),isCompressed(_isCompressed),isSplited(_isSplited)
{

}
PlanB::BackupResult * BackupExecutor::exec() 
{
	if (backupType == PlanB::BackupType::System)
	{
		pBackup = new SystemBackup(01, L"F", L"result.raw", TRUE, TRUE);
		return pBackup->exec();
	}
	else if (backupType == PlanB::BackupType::File)
	{
		pBackup = new FileBackup(01,L"F", L"result.raw", TRUE, TRUE);
		return pBackup->exec();
	}
	else
	{
		PlanB::JobStatus::FAILED;
	}
}

BackupExecutor::~BackupExecutor()
{
	delete pBackup;
}




