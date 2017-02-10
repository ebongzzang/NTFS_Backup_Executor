#pragma once
#ifndef Backup_Executor
#define Backup_Executor
#include "stdafx.h"
#include "PlanBCommon.h"
#include "backup.h"
#include "SystemBackup.h"

using PlanB::BackupStyle;
using PlanB::BackupType;
using PlanB::JobStatus;

class BackupExecutor 
{
	
public:
	BackupExecutor(long _backupKey,  std::string _sourcePath,  BackupStyle _backupStyle, 
		BackupType _BackupType, bool _isCompressed, bool _isSplited); 
	//style:full,incremental type - file,system, sourcepath

	~BackupExecutor();

private:
	Backup * pBackup;


};








#endif