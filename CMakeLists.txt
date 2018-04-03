cmake_minimum_required(VERSION 3.9)

project(iskrauraltel-challenge)

include_directories(include)
link_directories(lib)

add_executable(article-storage iskrauraltel-challenge.cpp ArticleStorage.cpp QueryProcessor.cpp Utilities.cpp)


add_executable(article-storage-tests iskrauraltel-test.cpp ArticleStorage.cpp QueryProcessor.cpp Utilities.cpp)

TARGET_LINK_LIBRARIES(article-storage-tests google-test.lib)
