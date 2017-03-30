#include "RestoreExecutor.h"

RestoreExecutor::RestoreExecutor(BackupResult* _result)
{
	result = new PlanB::BackupResult;
	result = _result;
}

RestoreExecutor::~RestoreExecutor()
{
	delete result;
}

JobStatus RestoreExecutor::exec()
{
	pRestore = new SystemRestore(result);
	pRestore->exec();
	return PlanB::JobStatus::SUCCESS;
}
