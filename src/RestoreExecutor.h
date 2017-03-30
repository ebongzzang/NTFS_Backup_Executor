#ifndef RESTORE_EXECUTOR_H
#define RESTORE_EXECUTOR_H
#include <string>
#include "PlanBCommon.h"
#include "Restore.h"
#include "SystemRestore.h"
using PlanB::BackupStyle;
using PlanB::BackupType;
using PlanB::JobStatus;
using PlanB::BackupResult;

class RestoreExecutor
{
public:
	RestoreExecutor(BackupResult* result);
	 JobStatus exec();
	~RestoreExecutor();

private:
	Restore * pRestore;
	BackupResult* result;
};


#endif // !RESTORE_H