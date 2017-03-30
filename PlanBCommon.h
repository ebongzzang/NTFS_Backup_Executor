#pragma once

#include <string>

using std::string;
using std::wstring;
namespace PlanB{

	enum BackupType {
		File,
		Directory,
		Volume,
		System
	};

	enum JobStatus {
		QUEUED,
		RUNNING,
		FAILED,
		SUCCESS,
		CANCLED,
		DELETED
	};

	enum BackupStyle {
		FULL,
		INCREMENTAL
	};
	enum BackupAdapter {
		UNDEFINED,
		PLANNER,
		S3
	};
	
	struct BackupResult
	{
		BackupType type;
		BackupStyle style;
		JobStatus status;

		long backupKey;
		wstring backupTargetPath;
		wstring resultFilePath;
		string time;
	};

	static const string LOCAL_DB_PATH= "";
	static const string TASK_TABLE_FILE = "tasks.sqlite3";
	static const string JOB_TABLE_FILE = "jobs.sqlite3";
}