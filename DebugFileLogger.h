//---------------------------------------------------------------------------
#ifndef DebugFileLoggerH
#define DebugFileLoggerH

#include <stdio.h>

class TDebugFileLogger
{
private:
		FILE* FFile;
		char  FFileName[260];

		void BuildLogFileName();
		void DeleteOldLogFiles();

public:
		TDebugFileLogger();
		~TDebugFileLogger();

		bool Open();
		void Close();
		bool WriteLine(const char* text);

		const char* GetFileName() const;
		bool IsOpen() const;
};

//---------------------------------------------------------------------------

#endif
