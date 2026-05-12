//---------------------------------------------------------------------------

#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)

#include <vcl.h>
#pragma hdrstop

#include "DebugFileLogger.h"

#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#pragma package(smart_init)

TDebugFileLogger::TDebugFileLogger() : FFile(NULL)
{
    FFileName[0] = '\0';
}

TDebugFileLogger::~TDebugFileLogger()
{
    Close();
}

const char* TDebugFileLogger::GetFileName() const
{
    return FFileName;
}

bool TDebugFileLogger::IsOpen() const
{
    return (FFile != NULL);
}

void TDebugFileLogger::BuildLogFileName()
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    // Example: DebugLog_20260330_154512.log
    sprintf(
        FFileName,
        "DebugLog_%04d%02d%02d_%02d%02d%02d.log",
        st.wYear,
        st.wMonth,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond
    );
}

void TDebugFileLogger::DeleteOldLogFiles()
{
    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    SYSTEMTIME st;
    GetLocalTime(&st);

    char todayPrefix[32];
    sprintf(todayPrefix, "DebugLog_%04d%02d%02d_", st.wYear, st.wMonth, st.wDay);

    hFind = FindFirstFileA("DebugLog_*.log", &findData);
    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do
    {
        // Keep today's files, delete older ones
        if (strncmp(findData.cFileName, todayPrefix, strlen(todayPrefix)) != 0)
        {
            DeleteFileA(findData.cFileName);
        }
    }
    while (FindNextFileA(hFind, &findData));

    FindClose(hFind);
}

bool TDebugFileLogger::Open()
{
    Close();

    DeleteOldLogFiles();
    BuildLogFileName();

    FFile = fopen(FFileName, "w");
    if (FFile == NULL)
        return false;

    return true;
}

void TDebugFileLogger::Close()
{
    if (FFile != NULL)
    {
        fclose(FFile);
        FFile = NULL;
    }
}

bool TDebugFileLogger::WriteLine(const char* text)
{
    if (FFile == NULL)
        return false;

    if (text == NULL)
        return false;

    // Write text + CRLF
    if (fprintf(FFile, "%s\r", text) < 0)
        return false;

    // Flush immediately so nothing is lost if app crashes
    fflush(FFile);

    return true;
}