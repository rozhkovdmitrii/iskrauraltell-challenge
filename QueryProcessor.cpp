#include <regex>
#include <fstream>
#include <string>

#include "include/QueryProcessor.h"
#include "include/Constants.h"

namespace iskratelChallenge {

	iskratelChallenge::QueryProcessor::QueryProcessor(const wstring & srcDirPath, const wstring & dstDirPath, const vector<wstring> queries)
		:_srcDirPath(srcDirPath), _dstDirPath(dstDirPath) {
		intializeQueries(queries);
	}

	void QueryProcessor::readSourceDir() {
		wregex dataRegex(constants::SourceFilePattern);
		ArticleStorage & as = ArticleStorage::instance();
		as.clear();
		for (auto & entry : fs::directory_iterator(_srcDirPath)) {
			const fs::path & path = entry.path();
			if (!regex_match(path.wstring(), dataRegex))
				continue;
			as.loadFromFile(path);
		}
	}

	void QueryProcessor::SaveToDestDir() {
		unsigned num = 0;
		ArticleStorage & as = ArticleStorage::instance();
		for (const ArticleQueryT & query : _queries) {
			wstring filename(to_wstring(num++) + L".result");
			fs::path dstFilePath = _dstDirPath / filename;
			as.saveToFile(query, dstFilePath);
		}
	}

	void QueryProcessor::deleteDestFiles() {
		wregex resultRegex(constants::DestFilePattern);
		for (auto & entry : fs::directory_iterator(_dstDirPath)) {
			const fs::path & path = entry.path();
			if (!regex_match(path.wstring(), resultRegex))
				continue;
			fs::remove(path);
		}
	}

	void QueryProcessor::processQueries() {
		readSourceDir();
		deleteDestFiles();
		SaveToDestDir();
	}

	void QueryProcessor::intializeQueries(const vector<wstring> & queries) {
		wregex queryRegex(constants::QueryPattern);
		wsmatch queryMatch;
		for (const wstring & query : queries) {
			if (!regex_match(query, queryMatch, queryRegex))
				continue;
			_queries.push_back(ArticleQueryT(queryMatch[1].str(), queryMatch[2].str()));
		}
	}



	

}

