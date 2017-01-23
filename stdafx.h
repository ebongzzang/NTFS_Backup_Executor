#pragma once
#ifndef STDAFX

#define STDAFX

#include <iostream>
#include <Windows.h>
#include <virtdisk.h>
#include <initguid.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <intrin.h>
#include <time.h>

#pragma comment(lib,"uuid.lib")
#pragma comment(lib, "virtdisk.lib")
#pragma comment(lib, "rpcrt4.lib")

#define VHD_FEATURES_TEMPORARY 0x01
#define VHD_FEATURES_RESERVED 0x02 
#define VHD_VERSION 0x00010000
#define VHD_CREATOR_TOOL "plan"
#define VHD_CREATOR_VERSION 0x00010000 
#define VHD_CREATOR_OS 0x5769326B
#define VHD_FOOTER_COOKIE "conectix"
#define VHD_HEADER_COOKIE "cxsparse"


#endif