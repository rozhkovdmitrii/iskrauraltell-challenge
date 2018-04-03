#pragma once

#include <string>
#include <filesystem>
#include <regex>

#include <Windows.h>

#include "include/Constants.h"
#include "ArticleStorage.h"


namespace iskratelChallenge {
	using namespace std;



	class DirChangesObserver {
		
	public:
		DirChangesObserver(const experimental::filesystem::path & path): _srcPath(path) {}
		
		template<typename T>
		void observe(const T & handleEvent);

	private:
		experimental::filesystem::path _srcPath;
	};



	template<typename T>
	void DirChangesObserver::observe(const T & handleEvent) {
		HANDLE hDirectory = CreateFileW(
			_srcPath.wstring().c_str(),
			FILE_LIST_DIRECTORY,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS,
			NULL
		);

		if (hDirectory == INVALID_HANDLE_VALUE) {
			int lastError = GetLastError();
			wchar_t error[1024];
			FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, lastError, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), error, sizeof(error), 0);
			wostringstream oss;
			oss << L"ERROR: Cannot open the directory " << _srcPath << " - " << error;
			throw oss.str();
		}

		static const DWORD LPBUFFER_SIZE_RESTRICTION = 64 * 1024;
		CHAR lpBuffer[LPBUFFER_SIZE_RESTRICTION];
		DWORD BytesReturned;


		
		static const int readChangesFlags = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE;
		while (ReadDirectoryChangesW(hDirectory, &lpBuffer, sizeof(lpBuffer), FALSE, readChangesFlags, &BytesReturned, NULL, NULL)) {
			DWORD lpBufferOffset = 0;
			DWORD pInfoOffset = 0;
			PFILE_NOTIFY_INFORMATION pInfo;
			do {
				pInfo = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(&lpBuffer[pInfoOffset]);
				pInfo->NextEntryOffset;
				handleEvent(_srcPath, pInfo);
				pInfoOffset += pInfo->NextEntryOffset;

			} while (pInfoOffset);

		}

	}


}