#pragma once
#include <string>
#include <vector>
#include <utility>
#include <experimental/filesystem>

#include "include/ArticleStorage.h"

using namespace std;

namespace iskratelChallenge {


	namespace fs = experimental::filesystem;
	
	class QueryProcessor {
	public:

		QueryProcessor(const wstring & srcDirPath, const wstring & dstDirPath, const vector<wstring> queries);

		void processQueries();
		void SaveToDestDir();
	private:
		void intializeQueries(const vector<wstring> & queries);
		void readSourceDir();
		void deleteDestFiles();

		fs::path _srcDirPath;
		fs::path _dstDirPath;
		vector<ArticleQueryT> _queries;
	};

}