#!/bin/bash

set -e
export GTEST_COLOR=1
export CTEST_OUTPUT_ON_FAILURE=true

CMAKE_LINKER_OPTS="-DCMAKE_EXE_LINKER='-fuse-ld=gold'"
CMAKE_CONFIG_OPTS="-DHUNTER_CONFIGURATION_TYPES=Debug -DCMAKE_BUILD_TYPE=Debug"
CMAKE_TOOLCHAIN_OPTS="-DCMAKE_TOOLCHAIN_FILE='`pwd`/tools/polly/gcc-7-cxx17-pic.cmake'"
CMAKE_OPTS="$CMAKE_LINKER_OPTS $CMAKE_CONFIG_OPTS $CMAKE_TOOLCHAIN_OPTS"

cmake -H. -B_builds $CMAKE_OPTS -DBUILD_COVERAGE=ON 
cmake --build _builds
cmake --build _builds --target test
cmake --build _builds --target gcov
cmake --build _builds --target lcov
gcovr -r  .

REPORT_DATA=$(gcovr -r  . | base64 | tr -d '\n')
POST_DATA="{\"report\": \"$REPORT_DATA\", \"slug\": \"$TRAVIS_REPO_SLUG\", \"head_branch\": \"$TRAVIS_BRANCH\", \"head_sha\": \"$TRAVIS_COMMIT\"}"

if [[ $TRAVIS_PULL_REQUEST == 'false' ]]; then
  curl -H "Content-Type: application/json" -d "$POST_DATA" -X POST http://borodin.dev.bmstu.cloud/api/coverage/
fi
