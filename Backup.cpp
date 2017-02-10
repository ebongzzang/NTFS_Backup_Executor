#include "backup.h"
#include <string>
#include <iostream>

Backup::Backup(long _backupKey, std::string _sourcePath, std::string _destinationPath,
	bool _isCompressed, bool _isSplited) : backupKey(_backupKey), sourcePath(_sourcePath),
	destinationPath(_destinationPath), isCompressed(_isCompressed), isSplited(_isSplited)
{
	
	

}

Backup::~Backup()
{

}


VSS_SNAPSHOT_PROP Backup::takeSnapshot()
{
	std::wstring driveLetter;
	IVssBackupComponents *backupComponents;

	VSS_ID snapshotId; //snapshot pid
	VSS_ID snapshotSetId; //snapshot set id
	IVssAsync *async;
	VSS_SNAPSHOT_PROP snapshotProp;
	//VSS_SNAPSHOT_PROP snapshotProp = new VSS_SNAPSHOT_PROP();

	typedef HRESULT(STDAPICALLTYPE *_CreateVssBackupComponentsInternal)(
		__out IVssBackupComponents **ppBackup
		);

	typedef void (APIENTRY *_VssFreeSnapshotPropertiesInternal)(
		__in VSS_SNAPSHOT_PROP *pProp
		);


	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	_CreateVssBackupComponentsInternal CreateVssBackupComponentsInternal_I;
	_VssFreeSnapshotPropertiesInternal VssFreeSnapshotPropertiesInternal_I;
	
	driveLetter = strTowstr(sourcePath + ":\\");

	HMODULE vssapiBase = LoadLibrary(L"vssapi.dll");
	CreateVssBackupComponentsInternal_I = (_CreateVssBackupComponentsInternal)GetProcAddress(vssapiBase, "CreateVssBackupComponentsInternal"); //linking function
	VssFreeSnapshotPropertiesInternal_I = (_VssFreeSnapshotPropertiesInternal)GetProcAddress(vssapiBase, "VssFreeSnapshotPropertiesInternal");

	if (!CreateVssBackupComponentsInternal_I || !VssFreeSnapshotPropertiesInternal_I)
		abort(); // Handle error

	if (!SUCCEEDED(CreateVssBackupComponentsInternal_I(&backupComponents)))
		abort();		//intialize backup components.

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

	if (!SUCCEEDED(backupComponents->PrepareForBackup(&async)))
		abort();

	if (!SUCCEEDED(backupComponents->DoSnapshotSet(&async)))
		abort();

	async->Wait();

	async->Release();

	if (!SUCCEEDED(backupComponents->GetSnapshotProperties(snapshotId, &snapshotProp)))
		abort();

	std::wcout << "snapshot name: " << snapshotProp.m_pwszSnapshotDeviceObject << std::endl;
	// Use prop.m_pwszSnapshotDeviceObject to access your files.

	return snapshotProp;

	backupComponents->Release();
	CoUninitialize();




}

std::wstring Backup::strTowstr(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
std::wstring Backup::convertDriveLettertoUNC(std::string dLetter)
//Convert driveLetter to UNC path
{
	std::string tmpString;
	std::wstring tmpWstring;

	tmpString = +"\\\\.\\" + dLetter + ":";
	tmpWstring = strTowstr(tmpString);
	return tmpWstring;

}



