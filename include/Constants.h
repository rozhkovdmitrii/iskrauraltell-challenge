#pragma once

namespace iskratelChallenge {

	namespace constants {

		static const wchar_t * Usage = L""
			"This program processes all .data files in sourceDir and put .result files according all queries passed in doublequotes.\n"
			"version 1.00, 2018-04-01\n"
			"(C) Dmitrii Rozhkov (rozhkovdmitrii@yandex.ru)\n"
			"Usage:\n"
			"\tarticle-storage.exe sourceDir destDir \"(producer|article) = concrete_value\" [\"(producer|article) = concrete_value\" ...]\n"
			"\n!!!BE AWARE!!! Input *.article files should be encoded as UTF8 for correct processing"
			"\n\nExamples:\n"
			"\tiskrauraltel-challenge.exe g:\\temp g:\\results \"producer = Дары кубани\" \"article = Картошка\" \"producer = Апшерон\"";

		static const wchar_t * ArticlePattern = L"^\\s*([^,]*[^\\s\\t,])\\s*,\\s*([^,]*[^\\s\\t,])\\s*,\\s*((\\d*[.])?\\d+)\\s*,\\s*((\\d*[.])?\\d+)\\s*$";
		static const wchar_t * QueryPattern = L"^\\s*(producer|article)\\s*=\\s*(.+)\\s*$";
		static const wchar_t * DestFilePattern = L"^.*[.]result$";
		static const wchar_t * SourceFilePattern = L"^.*[.]article$";
	}

}