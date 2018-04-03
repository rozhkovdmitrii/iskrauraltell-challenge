#include <io.h>
#include <fcntl.h>

#include "Utilities.h"
#include "Constants.h"


namespace iskratelChallenge {

	void utilities::setCLI_EncodingUTF8Mode() {
		_setmode(_fileno(stdout), _O_U8TEXT);
		_setmode(_fileno(stdin), _O_U8TEXT);
		_setmode(_fileno(stderr), _O_U8TEXT);
	}


	namespace fs = experimental::filesystem;

	void argumentsProcessing::CLArguments::checkArgs(int argc, wchar_t ** argv) {
		if (argc <  4)
			throw constants::Usage;

		fs::path srcDirPath(argv[1]);
		if (!fs::exists(srcDirPath) || !fs::is_directory(srcDirPath))
			throw wstring(L"Error source dir wrong - ") + srcDirPath.wstring() + wstring(L"\n") + constants::Usage;

		fs::path dstDirPath(argv[2]);
		if (!fs::exists(dstDirPath) || !fs::is_directory(dstDirPath))
			throw  wstring(L"dest dir wrong - ") + dstDirPath.wstring() + wstring(L"\n") + constants::Usage;

		wregex queryRegex(constants::QueryPattern);
		for (int i = 3; i < argc; ++i)
			if (!regex_match(argv[i], queryRegex))
				throw wstring(L"wrong query: ") + wstring(argv[i]) + wstring(L"\n") + constants::Usage;
	}

	void argumentsProcessing::CLArguments::checkAndReadArgs(int argc, wchar_t ** argv) {
		checkArgs(argc, argv);
		srcDir = fs::path(argv[1]);
		dstDir = fs::path(argv[2]);

		wregex queryRegex(constants::QueryPattern);
		for (int i = 3; i < argc; ++i)
			queries.push_back(argv[i]);
	}


}

