#include <fstream>
#include <locale>
#include <iterator>
#include <Windows.h>

#include <io.h>
#include <fcntl.h>

#include "include/ArticleStorage.h"
#include "include/gtest/gtest.h"
#include "include/Utilities.h"
#include "include/Constants.h"

#include <experimental/filesystem>
namespace fs = experimental::filesystem;

namespace iskratelChallengeTests {
	using namespace iskratelChallenge;

	TEST(ArticleFromWrongStringShouldThrowException, ArticleClassChecks) {
		ASSERT_THROW(
			Article::fromWString(L",,,,,,"),
			wstring);

		ASSERT_NO_THROW(
			Article::fromWString(L"������ ������, ��� �������, 55, 600"));

		ASSERT_THROW(
			Article::fromWString(L"������ ������, ��� �������, 5 5, 6.00"),
			wstring);
	};

	TEST(CheckArticleInitialization, ArticleClassChecks) {
		Article::ArticleSPtrT ap = Article::fromWString(L"������ ������,		 ��� �������, 0.55,			600.77");
		ASSERT_EQ(ap->name, L"������ ������");
		ASSERT_EQ(ap->producer, L"��� �������");
		ASSERT_NEAR(ap->maxPrice, .55, 0.001);
		ASSERT_NEAR(ap->weight, 600.77, 0.001);
	};

	TEST(CheckToString, ArticleClassChecks) {
		Article::ArticleSPtrT ap = Article::fromWString(L"������ ������,		 ��� �������, .55,			600.77");
		wstring presense = ap->toString();
		ASSERT_EQ(L"������ ������, ��� �������, 0.55, 600.77", presense);
	}

	void writeArticleDataIntoFile(const wstring & fileName, const vector<wstring> & articleData) {
		wofstream output = utilities::imbuedWitUTF8Stream<wofstream>(fileName);
	
		for (const wstring & articlePresense : articleData) {
			output << articlePresense << endl;
		}
		output.flush();
		output.close();
	}

	static const wstring firstDataFilePath = L"./google-test-articles-temporary.data";
	static const wstring otherDataFilePath = L"./google-test-articles-temporary-1.data";

	TEST(CheckArticleStorageLoadFromFile, ArticleStorageClassChecks) {
		const vector<wstring> sourceData = {
			L"������ ������,                �������		, 100, 788.8",
			L"������ �������, �������, 110.5, 1000",
			L"������ �������, �������,			150, 500.4",
			L"�������, �������, 200,      250",
			L"��������, ��� �������, 450.99, 788.8",
			L"�������� ������, �������, 150, 35",
			L"��������, ������, 40, 400.3"
		};

		const vector<wstring> otherSourceData = {
			L"��������, ���� !!!ERROR,, ���, 50, 5500",
			L"��������, �������, 50.5, 5500.77",
			L"��������� � ��������� (500 �.), ��� �������, 200, 50",
			L"��������� � ������� (500 �.), ��� �������, 205, 50"
		};



		writeArticleDataIntoFile(firstDataFilePath, sourceData);
		writeArticleDataIntoFile(otherDataFilePath, otherSourceData);
		
		
		
		ArticleStorage & as = ArticleStorage::instance();
		as.loadFromFile(firstDataFilePath);
		as.loadFromFile(otherDataFilePath);

		fs::remove(experimental::filesystem::path(firstDataFilePath));
		fs::remove(experimental::filesystem::path(otherDataFilePath));

		ArticleStorage::ArticleWPtrRangeT prodRng = as.articlesByProducerRange(L"�������");
		ASSERT_EQ(6, distance(prodRng.first, prodRng.second));

		prodRng = as.articlesByProducerRange(L"��� �������");
		ASSERT_EQ(3, distance(prodRng.first, prodRng.second));

		prodRng = as.articlesByProducerRange(L"������");
		ASSERT_EQ(1, distance(prodRng.first, prodRng.second));

		prodRng = as.articlesByNameRange(L"��������");
		ASSERT_EQ(2, distance(prodRng.first, prodRng.second));

	}

	TEST(CheckArticleStorageDeleteByFilename, ArticleStorageClassChecks) {
		ArticleStorage & as = ArticleStorage::instance();
		auto pathDataPair = fs::path(firstDataFilePath);
		wstring fileName = pathDataPair.filename();
		as.clearArticlesBy(fileName);

		ArticleStorage::ArticleWPtrRangeT prodRng = as.articlesByProducerRange(L"�������");
		ASSERT_EQ(1, distance(prodRng.first, prodRng.second));

		prodRng = as.articlesByProducerRange(L"��� �������");
		ASSERT_EQ(2, distance(prodRng.first, prodRng.second));
		as.clear();
	}
}

int main(int argc, char *argv[]) {

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

