#pragma once
#include <utility>
#include <string>
#include <sstream>
#include <regex>
#include <locale>
#include <iostream>
#include <experimental/filesystem>

using namespace std;

namespace iskratelChallenge {

	namespace utilities {

		template<typename CharType>
		struct DotSepNumpunctFacet : std::numpunct<CharType> {
			CharType do_decimal_point() const {
				return '.'; 
			}
		};

		template<typename wstream_type>
		wstream_type imbuedWitUTF8Stream(const wstring & fileName) {
			wstream_type ws(fileName);
			ws.imbue(locale(ws.getloc(), new std::codecvt_utf8_utf16<wchar_t, 0x10FFFF, consume_header>));
			return ws;
		}

		void setCLI_EncodingUTF8Mode();
	}

	namespace argumentsProcessing {

		class CLArguments {
			CLArguments() = default;
			static void checkArgs(int argc, wchar_t ** argv);
		public:
			CLArguments(const CLArguments &) = delete;
			CLArguments(const CLArguments &&) = delete;
			CLArguments & operator=(const CLArguments &) = delete;
			CLArguments & operator=(const CLArguments &&) = delete;

			static CLArguments & instance() { 
				static CLArguments _instance;
				return _instance;
			};
			experimental::filesystem::path srcDir;
			experimental::filesystem::path dstDir;
			vector<wstring> queries;
			void checkAndReadArgs(int argc, wchar_t ** argv);

		};


	}
}


