/**
 * @file        file_methods_test.cpp
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2019 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "file_methods_test.h"
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiCommon/process_execution.h>

namespace Kitsunemimi
{
namespace Persistence
{

/**
 * @brief constructor
 */
FileMethods_Test::FileMethods_Test()
    : Kitsunemimi::Common::Test("FileMethods_Test")
{
    doesPathExist_test();
    doesFileExist_test();
    doesDirExist_test();
    renameFileOrDir_test();
    copyPath_test();
    deleteFileOrDis_test();
}

/**
 * @brief doesPathExist_test
 */
void
FileMethods_Test::doesPathExist_test()
{
    const std::string testPath = "/tmp/doesPathExist_test_testfile";
    Kitsunemimi::Common::runSyncProcess(std::string("rm " + testPath).c_str());

    TEST_EQUAL(doesPathExist(testPath), false);

    Kitsunemimi::Common::runSyncProcess(std::string("touch " + testPath).c_str());

    TEST_EQUAL(doesPathExist(testPath), true);

    Kitsunemimi::Common::runSyncProcess(std::string("rm " + testPath).c_str());
}

/**
 * @brief doesFileExist_test
 */
void
FileMethods_Test::doesFileExist_test()
{
    const std::string testFilePath = "/tmp/doesFileExist_test_testfile";
    const std::string testDirPath = "/tmp/doesFileExist_test_testdir";

    Kitsunemimi::Common::runSyncProcess(std::string("rm " + testFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("rm -r " + testDirPath).c_str());

    TEST_EQUAL(doesFileExist(testFilePath), false);
    TEST_EQUAL(doesFileExist(testDirPath), false);

    Kitsunemimi::Common::runSyncProcess(std::string("touch " + testFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("mkdir " + testDirPath).c_str());

    TEST_EQUAL(doesFileExist(testFilePath), true);
    TEST_EQUAL(doesFileExist(testDirPath), false);

    Kitsunemimi::Common::runSyncProcess(std::string("rm " + testFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("rm -r " + testDirPath).c_str());
}

/**
 * @brief doesDirExist_test
 */
void
FileMethods_Test::doesDirExist_test()
{
    const std::string testFilePath = "/tmp/doesDirExist_test_testfile";
    const std::string testDirPath = "/tmp/doesDirExist_test_testdir";

    Kitsunemimi::Common::runSyncProcess(std::string("rm " + testFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("rm -r " + testDirPath).c_str());

    TEST_EQUAL(doesDirExist(testFilePath), false);
    TEST_EQUAL(doesDirExist(testDirPath), false);

    Kitsunemimi::Common::runSyncProcess(std::string("touch " + testFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("mkdir " + testDirPath).c_str());

    TEST_EQUAL(doesDirExist(testFilePath), false);
    TEST_EQUAL(doesDirExist(testDirPath), true);

    Kitsunemimi::Common::runSyncProcess(std::string("rm " + testFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("rm -r " + testDirPath).c_str());
}

/**
 * @brief renameFileOrDir_test
 */
void
FileMethods_Test::renameFileOrDir_test()
{
    std::pair<bool, std::string> result;

    const std::string oldFilePath = "/tmp/renameFileOrDir_test_testfile_ALT";
    const std::string newFileName = "/tmp/renameFileOrDir_test_testfile_NEU";

    Kitsunemimi::Common::runSyncProcess(std::string("touch " + oldFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("rm " + newFileName).c_str());

    result = renameFileOrDir(oldFilePath, newFileName);
    TEST_EQUAL(result.first, true);

    result = renameFileOrDir(oldFilePath, newFileName);
    TEST_EQUAL(result.first, false);

    TEST_EQUAL(doesPathExist(oldFilePath), false);
    TEST_EQUAL(doesPathExist(newFileName), true);

    Kitsunemimi::Common::runSyncProcess(std::string("rm " + oldFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("rm " + newFileName).c_str());
}

/**
 * @brief copyPath_test
 */
void
FileMethods_Test::copyPath_test()
{
    std::pair<bool, std::string> result;

    const std::string oldFilePath = "/tmp/copyFile_test_testfile_ALT";
    const std::string newFileName = "/tmp/copyFile_test_testfile_NEU";

    Kitsunemimi::Common::runSyncProcess(std::string("touch " + oldFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("rm " + newFileName).c_str());

    result = copyPath(oldFilePath, newFileName);
    TEST_EQUAL(result.first, true);

    result = copyPath(oldFilePath, newFileName, false);
    TEST_EQUAL(result.first, false);

    TEST_EQUAL(doesPathExist(oldFilePath), true);
    TEST_EQUAL(doesPathExist(newFileName), true);

    Kitsunemimi::Common::runSyncProcess(std::string("rm " + oldFilePath).c_str());
    Kitsunemimi::Common::runSyncProcess(std::string("rm " + newFileName).c_str());
}

/**
 * @brief deleteFileOrDis_test
 */
void
FileMethods_Test::deleteFileOrDis_test()
{
    const std::string testPath = "/tmp/deleteFileOrDis_test_testfile";

    TEST_EQUAL(deleteFileOrDir(testPath), false);

    Kitsunemimi::Common::runSyncProcess(std::string("touch " + testPath).c_str());

    TEST_EQUAL(deleteFileOrDir(testPath), true);
    TEST_EQUAL(deleteFileOrDir(testPath), false);

    TEST_EQUAL(doesPathExist(testPath), false);
}

}
}
