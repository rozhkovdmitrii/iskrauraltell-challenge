#include <regex>
#include <sstream>
#include <fstream>
#include <algorithm>

#include <locale>
#include <codecvt>
#include <experimental/filesystem>


#include "include/ArticleStorage.h"
#include "include/Utilities.h"
#include "include/Constants.h"




namespace iskratelChallenge {

	using namespace std;
	namespace fs = experimental::filesystem;

	Article::Article(const wstring & _name, const wstring & _producer, const double & _maxPrice, const double & _weight)
		: name(_name), producer(_producer), maxPrice(_maxPrice), weight(_weight) {}

	Article::ArticleSPtrT Article::fromWString(const wstring & strPresense) {
		static const wregex articleRegex(constants::ArticlePattern);
		wsmatch articleMatch;
		if (!regex_match(strPresense, articleMatch, articleRegex))
			throw wstring() + L"Wrong article article presense in \"" + strPresense + L"\"";
		double maxPrice = stod(articleMatch[3].str());
		double weight = stod(articleMatch[5].str());
		return make_shared<Article>(articleMatch[1], articleMatch[2], maxPrice, weight);
	}
	 
	wstring Article::toString() {
		wstringstream ss;
		ss << name << ", " << producer << ", " << maxPrice << ", " << weight;
		return ss.str();
	}

	ArticleStorage & iskratelChallenge::ArticleStorage::instance() {
		static ArticleStorage instance;		
		return instance;
	}

	void ArticleStorage::loadFromFile(const wstring & sourceFilePath) {
		wifstream ifs = utilities::imbuedWitUTF8Stream<wifstream>(sourceFilePath);
		if (!ifs.is_open())
			throw L"Something wrong with source file";
		
		wstring fileName = fs::path(sourceFilePath).filename();
		clearArticlesBy(fileName);

		wstring presense;
		for (; ifs; getline(ifs, presense)) {
			try {
				pushArticle(fileName, Article::fromWString(presense));
			} catch (...) {
				continue;
			}
		}
		ifs.close();
	}

	void ArticleStorage::saveToFile(const ArticleQueryT & query, const experimental::filesystem::path & destFilePath) {
		static map<wstring, RangeType> queryTypes{ { L"producer", RangeType::producer }, { L"article", RangeType::article } };

		wofstream ofs = utilities::imbuedWitUTF8Stream<wofstream>(destFilePath);
		
		if (!ofs)
			throw wstring(L"Can't write results into file ") + destFilePath.wstring();

		const wstring & filterValue = query.second;
		
		ArticleWPtrRangeT articlesRange = articlesBy(queryTypes[query.first], filterValue);
		for (ArticleWPtrCollectionT::iterator articleItr = articlesRange.first; articleItr != articlesRange.second; ++articleItr) {
			Article::ArticleSPtrT sptr = articleItr->second.lock();
			ofs << sptr->toString() << endl;
		}
		ofs.close();
	}

	ArticleStorage::ArticleWPtrRangeT ArticleStorage::articlesByProducerRange(const wstring & producer) {
		return _producerIndex.equal_range(producer);
	}

	ArticleStorage::ArticleWPtrRangeT ArticleStorage::articlesByNameRange(const wstring & articleName) {
		return _articleNameIndex.equal_range(articleName);
	}

	ArticleStorage::ArticleWPtrRangeT ArticleStorage::articlesBy(RangeType rngType, const wstring & queryValue) {
		if (rngType == RangeType::producer)
			return articlesByProducerRange(queryValue);
		else //if (rngType == RangeType::article)
			return articlesByNameRange(queryValue);

	}

	void ArticleStorage::clearArticlesBy(const wstring & fileName) {
		_srcFileIndex.erase(fileName);

		auto clearWithPtrExpired = [](ArticleWPtrCollectionT & wptrcollection) {
			auto it = wptrcollection.begin();
			while (it !=  wptrcollection.end())
				it->second.expired() ? wptrcollection.erase(it++) : ++it;
		};
		clearWithPtrExpired(_producerIndex);
		clearWithPtrExpired(_articleNameIndex);

	}

	void ArticleStorage::clear() {
		_srcFileIndex.clear();
		_producerIndex.clear();
		_articleNameIndex.clear();
	}

	void ArticleStorage::pushArticle(const wstring & fileName, Article::ArticleSPtrT articleSPtr) {
		Article::ArticleWPtrT wptr = articleSPtr;
		_srcFileIndex.insert(pair<wstring, Article::ArticleSPtrT>(fileName, articleSPtr));
		_producerIndex.insert(pair<wstring, Article::ArticleWPtrT>(articleSPtr->producer, articleSPtr));
		_articleNameIndex.insert(pair<wstring, Article::ArticleWPtrT>(articleSPtr->name, articleSPtr));
	}

}