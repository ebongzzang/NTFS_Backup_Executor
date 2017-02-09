#include "backup.h"
#include <string>
#include "stringToolset.h"
Backup::Backup()
{

}

Backup::~Backup()
{

}


void Backup::takeSnapshot(std::string _driveLetter)
{
	std::wstring driveLetter;
	IVssBackupComponents *backupComponents;


	VSS_ID snapshotId; //snapshot pid
	VSS_ID snapshotSetId; //snapshot set id
	IVssAsync *async;
	VSS_SNAPSHOT_PROP snapshotProp;

	using _CreateVssBackupComponentsInternal = HRESULT(*STDAPICALLTYPE)
		(__out IVssBackupComponents **ppBackup);


	using _VssFreeSnapshotPropertiesInternal = void (APIENTRY *)
		(__in VSS_SNAPSHOT_PROP *pProp);

	if (!CoInitializeEx(NULL, COINIT_MULTITHREADED)) //initalize COM library
	{
		exit(1);
	}

	_CreateVssBackupComponentsInternal CreateVssBackupComponentsInternal_I;
	_VssFreeSnapshotPropertiesInternal VssFreeSnapshotPropertiesInternal_I;
	driveLetter = strTowstr(_driveLetter + ":\\");

	HMODULE vssapiBase = LoadLibrary(L"vssapi.dll");
	CreateVssBackupComponentsInternal_I = (_CreateVssBackupComponentsInternal)GetProcAddress(vssapiBase, "CreateVssBackupComponentsInternal"); //linking function
	VssFreeSnapshotPropertiesInternal_I = (_VssFreeSnapshotPropertiesInternal)GetProcAddress(vssapiBase, "VssFreeSnapshotPropertiesInternal");

	if (!CreateVssBackupComponentsInternal_I || !VssFreeSnapshotPropertiesInternal_I)
		abort(); // Handle error

	if (!SUCCEEDED(CreateVssBackupComponentsInternal_I(&backupComponents)))
		//intialize backup components.
		abort();

	if (!SUCCEEDED(backupComponents->InitializeForBackup()))
		abort();
	if (!SUCCEEDED(backupComponents->SetBackupState(FALSE, TRUE, VSS_BT_INCREMENTAL, TRUE)))
		abort();

	if (!SUCCEEDED(backupComponents->SetContext(VSS_CTX_CLIENT_ACCESSIBLE_WRITERS)))
		abort(); // 이 CONTEXT 지정 시 SYSTEM RESTORE APPLICATION와 같은 형식으로 SHADOW 생성
	if (!SUCCEEDED(backupComponents->StartSnapshotSet(&snapshotSetId)))
		abort();

	if (!SUCCEEDED(backupComponents->AddToSnapshotSet(const_cast<wchar_t *>(driveLetter.c_str()), GUID_NULL, &snapshotId)))
		abort(); //delete const


	if (!SUCCEEDED(backupComponents->DoSnapshotSet(&async)))
		abort();

	async->Wait();

	async->Release();

	if (!SUCCEEDED(backupComponents->GetSnapshotProperties(snapshotId, &snapshotProp)))
		abort();

	std::cout << "snapshot name" << snapshotProp.m_pwszSnapshotDeviceObject << std::endl;
	// Use prop.m_pwszSnapshotDeviceObject to access your files.

	backupComponents->Release();
	CoUninitialize();


}