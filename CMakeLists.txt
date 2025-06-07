cmake_minimum_required(VERSION 3.10)
project(DES)

# C++11を有効化
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTestを見つける
find_package(GTest REQUIRED)

# インクルードディレクトリを追加
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS})

# メインのDESライブラリをビルド
add_library(des_lib
    src/des.cpp
    src/gensubkey.cpp
)

# メイン実行ファイルをビルド
add_executable(des main.cpp)
target_link_libraries(des des_lib)

# テスト実行ファイルをビルド
add_executable(des_test tests/des_test.cpp)
target_link_libraries(des_test des_lib ${GTEST_BOTH_LIBRARIES} pthread)

# テストを有効化
enable_testing()
add_test(NAME des_test COMMAND des_test)