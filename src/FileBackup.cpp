#include "FileBackup.h"

FileBackup::FileBackup(long _backupKey, std::wstring _sourcePath, std::wstring _destinationPath,
	bool _isCompressed, bool _isSplited) : Backup(_backupKey, _sourcePath, _destinationPath, _isCompressed, _isSplited)
{

}
PlanB::BackupResult * FileBackup::exec()
{
	/* not implemented */

	result->status = PlanB::JobStatus::SUCCESS;
	result->backupKey = backupKey;
	result->resultFilePath = destinationPath;
	result->backupTargetPath = sourcePath;
	result->style = PlanB::BackupStyle::FULL;
	result->type = PlanB::BackupType::File;

	return result;
}
PlanB::JobStatus FileBackup::cancel()
{
	return PlanB::JobStatus::CANCLED;
	/* Not Implemented*/
}
FileBackup::~FileBackup()
{

}

