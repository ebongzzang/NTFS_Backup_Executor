#pragma once

#include <string>
using std::string;

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

	static const string LOCAL_DB_PATH= "";
	static const string TASK_TABLE_FILE = "tasks.sqlite3";
	static const string JOB_TABLE_FILE = "jobs.sqlite3";
}