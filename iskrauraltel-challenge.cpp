#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "include/QueryProcessor.h"
#include "include/Utilities.h"
#include "include/DirChangesNotifier.hpp"
#include <filesystem>

using namespace std;
using namespace iskratelChallenge;


void doArticleQueryProcessing(int argc, wchar_t ** argv) {
	argumentsProcessing::CLArguments & ags = argumentsProcessing::CLArguments::instance();
	ags.checkAndReadArgs(argc, argv);

	QueryProcessor qp(ags.srcDir, ags.dstDir, ags.queries);
	qp.processQueries();

	DirChangesObserver sourceDirObserver(ags.srcDir);

	static auto handleDirectoryEvent = [&qp](const experimental::filesystem::path & srcDirPath, const PFILE_NOTIFY_INFORMATION pInfo) {
		ArticleStorage & as = ArticleStorage::instance();
		wstring fileName = wstring(pInfo->FileName, pInfo->FileNameLength / sizeof(WCHAR));
		experimental::filesystem::path filePath = srcDirPath / fileName;
		if (!regex_match(fileName, wregex(constants::SourceFilePattern)))
			return;
		switch (pInfo->Action) {
		case FILE_ACTION_ADDED:
		case FILE_ACTION_MODIFIED:
		case FILE_ACTION_RENAMED_NEW_NAME:
			as.loadFromFile(filePath);
			break;
		case FILE_ACTION_REMOVED:
		case FILE_ACTION_RENAMED_OLD_NAME:
			as.clearArticlesBy(filePath);
		}
		qp.SaveToDestDir();
	};

	sourceDirObserver.observe(handleDirectoryEvent);
}


int wmain(int argc, wchar_t ** argv) {
	utilities::setCLI_EncodingUTF8Mode();
	try {
		doArticleQueryProcessing(argc, argv);
	} catch (const wchar_t * e) {
		wcerr << e << endl;
		ExitProcess(GetLastError());
	}
	return 0;
}

