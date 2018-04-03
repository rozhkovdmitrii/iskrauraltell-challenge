#pragma once
#include <string>
#include <memory>
#include <map>
#include <utility>
#include <iterator>
#include <filesystem>

namespace iskratelChallenge {

	using namespace std;
	using ArticleQueryT = pair<wstring, wstring>;

	struct Article {
		using ArticleSPtrT = shared_ptr<Article>;
		using ArticleWPtrT = weak_ptr<Article>;

		Article(const wstring & name, const wstring & producer, const double & maxPrice, const double & weight);

		wstring name;
		wstring producer;
		double maxPrice;
		double weight;

		static ArticleSPtrT fromWString(const wstring & strPresense);
		wstring toString();
	};

	class ArticleStorage {
		ArticleStorage() = default;
	public:
		static ArticleStorage& instance();

		using ArticleSPtrCollectionT = multimap<wstring, Article::ArticleSPtrT>;
		using ArticleWPtrCollectionT = multimap<wstring, Article::ArticleWPtrT>;
		using ArticleWPtrRangeT = pair<ArticleWPtrCollectionT::iterator, ArticleWPtrCollectionT::iterator>;

		enum class RangeType {
			producer,
			article
		};

		void loadFromFile(const wstring & sourceFilePath);
		void saveToFile(const ArticleQueryT & query, const experimental::filesystem::path & destFilePath);

		ArticleWPtrRangeT articlesByProducerRange(const wstring & producer);
		ArticleWPtrRangeT articlesByNameRange(const wstring & articleName);
		ArticleWPtrRangeT articlesBy(RangeType rngType, const wstring & queryValue);

		void clearArticlesBy(const wstring & fileName);
		void clear();
	private:
		void pushArticle(const wstring & fileName, Article::ArticleSPtrT articlePresense);
		ArticleWPtrCollectionT _producerIndex;
		ArticleWPtrCollectionT _articleNameIndex;
		ArticleSPtrCollectionT _srcFileIndex;
	};


};