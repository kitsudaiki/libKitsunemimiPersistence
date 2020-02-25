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

using Kitsunemimi::runSyncProcess;

namespace Kitsunemimi
{
namespace Persistence
{

/**
 * @brief constructor
 */
FileMethods_Test::FileMethods_Test()
    : Kitsunemimi::CompareTestHelper("FileMethods_Test")
{
    doesPathExist_test();
    isFile_test();
    isDir_test();
    getParent_test();
    listFiles_test();
    renameFileOrDir_test();
    copyPath_test();
    createDirectory_test();
    deleteFileOrDir_test();
}

/**
 * @brief doesPathExist_test
 */
void
FileMethods_Test::doesPathExist_test()
{
    const std::string testPath = "/tmp/doesPathExist_test_testfile";
    runSyncProcess(std::string("rm " + testPath).c_str());

    TEST_EQUAL(doesPathExist(testPath), false);

    runSyncProcess(std::string("touch " + testPath).c_str());

    TEST_EQUAL(doesPathExist(testPath), true);

    runSyncProcess(std::string("rm " + testPath).c_str());
}

/**
 * @brief isFile_test
 */
void
FileMethods_Test::isFile_test()
{
    const std::string testFilePath = "/tmp/isFile_test_testfile";
    const std::string testDirPath = "/tmp/isFile_test_testdir";

    runSyncProcess(std::string("rm " + testFilePath).c_str());
    runSyncProcess(std::string("rm -r " + testDirPath).c_str());

    TEST_EQUAL(isFile(testFilePath), false);
    TEST_EQUAL(isFile(testDirPath), false);

    runSyncProcess(std::string("touch " + testFilePath).c_str());
    runSyncProcess(std::string("mkdir " + testDirPath).c_str());

    TEST_EQUAL(isFile(testFilePath), true);
    TEST_EQUAL(isFile(testDirPath), false);

    runSyncProcess(std::string("rm " + testFilePath).c_str());
    runSyncProcess(std::string("rm -r " + testDirPath).c_str());
}

/**
 * @brief isDir_test
 */
void
FileMethods_Test::isDir_test()
{
    const std::string testFilePath = "/tmp/isDir_test_testfile";
    const std::string testDirPath = "/tmp/isDir_test_testdir";

    runSyncProcess(std::string("rm " + testFilePath).c_str());
    runSyncProcess(std::string("rm -r " + testDirPath).c_str());

    TEST_EQUAL(isDir(testFilePath), false);
    TEST_EQUAL(isDir(testDirPath), false);

    runSyncProcess(std::string("touch " + testFilePath).c_str());
    runSyncProcess(std::string("mkdir " + testDirPath).c_str());

    TEST_EQUAL(isDir(testFilePath), false);
    TEST_EQUAL(isDir(testDirPath), true);

    runSyncProcess(std::string("rm " + testFilePath).c_str());
    runSyncProcess(std::string("rm -r " + testDirPath).c_str());
}

/**
 * @brief getParent_test
 */
void
FileMethods_Test::getParent_test()
{
    const std::string testFilePath = "/tmp/isFile_test_testdir/isFile_test_testfile";
    const std::string testDirPath = "/tmp/isFile_test_testdir";

    runSyncProcess(std::string("mkdir " + testDirPath).c_str());
    runSyncProcess(std::string("touch " + testFilePath).c_str());

    TEST_EQUAL(getParent(testFilePath), testDirPath);
    TEST_EQUAL(getParent(testDirPath), std::string("/tmp"));

    runSyncProcess(std::string("rm " + testFilePath).c_str());
    runSyncProcess(std::string("rm -r " + testDirPath).c_str());
}

/**
 * @brief listFiles_test
 */
void
FileMethods_Test::listFiles_test()
{
    runSyncProcess(std::string("rm -r /tmp/listFiles_test/").c_str());
    runSyncProcess(std::string("mkdir /tmp/listFiles_test/").c_str());
    runSyncProcess(std::string("touch /tmp/listFiles_test/poi1").c_str());
    runSyncProcess(std::string("mkdir /tmp/listFiles_test/test1").c_str());
    runSyncProcess(std::string("touch /tmp/listFiles_test/test1/poi1").c_str());
    runSyncProcess(std::string("touch /tmp/listFiles_test/test1/poi2").c_str());
    runSyncProcess(std::string("mkdir /tmp/listFiles_test/test2").c_str());
    runSyncProcess(std::string("touch /tmp/listFiles_test/test2/poi1").c_str());
    runSyncProcess(std::string("touch /tmp/listFiles_test/test2/poi2").c_str());
    runSyncProcess(std::string("touch /tmp/listFiles_test/test2/poi3").c_str());

    std::vector<std::string> fileList;

    listFiles(fileList, "/tmp/listFiles_test/");
    TEST_EQUAL(fileList.size(), 6);
    fileList.clear();

    listFiles(fileList, "/tmp/listFiles_test/poi1");
    TEST_EQUAL(fileList.size(), 1);
    fileList.clear();

    listFiles(fileList, "/tmp/listFiles_test/test1");
    TEST_EQUAL(fileList.size(), 2);
    fileList.clear();

    listFiles(fileList, "/tmp/listFiles_test/", false);
    TEST_EQUAL(fileList.size(), 1);
    fileList.clear();

    listFiles(fileList, "/tmp/listFiles_test/", true, std::vector<std::string>() = {"test1"});
    TEST_EQUAL(fileList.size(), 4);
    fileList.clear();


    runSyncProcess(std::string("rm -r /tmp/listFiles_test/").c_str());
}

/**
 * @brief renameFileOrDir_test
 */
void
FileMethods_Test::renameFileOrDir_test()
{
    bool result = false;

    const std::string oldFilePath = "/tmp/renameFileOrDir_test_testfile_ALT";
    const std::string newFileName = "/tmp/renameFileOrDir_test_testfile_NEU";

    runSyncProcess(std::string("touch " + oldFilePath).c_str());
    runSyncProcess(std::string("rm " + newFileName).c_str());

    std::string errorMessage = "";

    result = renameFileOrDir(oldFilePath, newFileName, errorMessage);
    TEST_EQUAL(result, true);

    result = renameFileOrDir(oldFilePath, newFileName, errorMessage);
    TEST_EQUAL(result, false);

    TEST_EQUAL(doesPathExist(oldFilePath), false);
    TEST_EQUAL(doesPathExist(newFileName), true);

    runSyncProcess(std::string("rm " + oldFilePath).c_str());
    runSyncProcess(std::string("rm " + newFileName).c_str());
}

/**
 * @brief copyPath_test
 */
void
FileMethods_Test::copyPath_test()
{
    bool result = false;

    const std::string oldFilePath = "/tmp/copyFile_test_testfile_ALT";
    const std::string newFileName = "/tmp/copyFile_test_testfile_NEU";

    runSyncProcess(std::string("touch " + oldFilePath).c_str());
    runSyncProcess(std::string("rm " + newFileName).c_str());

    std::string errorMessage = "";

    result = copyPath(oldFilePath, newFileName, errorMessage);
    TEST_EQUAL(result, true);

    result = copyPath(oldFilePath, newFileName, errorMessage, false);
    TEST_EQUAL(result, false);

    TEST_EQUAL(doesPathExist(oldFilePath), true);
    TEST_EQUAL(doesPathExist(newFileName), true);

    runSyncProcess(std::string("rm " + oldFilePath).c_str());
    runSyncProcess(std::string("rm " + newFileName).c_str());
}

/**
 * @brief createDirectory_test
 */
void
FileMethods_Test::createDirectory_test()
{
    bool result = false;

    const std::string testDirPath = "/tmp/deleteFileOrDir_test_testdir/test1";

    runSyncProcess(std::string("rm -r " + testDirPath).c_str());
    TEST_EQUAL(doesPathExist(testDirPath), false);

    std::string errorMessage = "";

    result = createDirectory(testDirPath, errorMessage);
    TEST_EQUAL(result, true);

    TEST_EQUAL(doesPathExist(testDirPath), true);

    result = createDirectory(testDirPath, errorMessage);
    TEST_EQUAL(result, false);

    runSyncProcess(std::string("rm -r " + testDirPath).c_str());
}

/**
 * @brief deleteFileOrDir_test
 */
void
FileMethods_Test::deleteFileOrDir_test()
{
    bool result = false;

    const std::string testFilePath = "/tmp/deleteFileOrDir_test_testfile";
    const std::string testDirPath = "/tmp/deleteFileOrDir_test_testdir";

    runSyncProcess(std::string("touch " + testFilePath).c_str());
    runSyncProcess(std::string("mkdir " + testDirPath).c_str());

    std::string errorMessage = "";

    TEST_EQUAL(doesPathExist(testFilePath), true);
    TEST_EQUAL(doesPathExist(testDirPath), true);

    result = deleteFileOrDir(testFilePath, errorMessage);
    TEST_EQUAL(result, true);

    result = deleteFileOrDir(testDirPath, errorMessage);
    TEST_EQUAL(result, true);

    result = deleteFileOrDir(testDirPath, errorMessage);
    TEST_EQUAL(result, true);

    TEST_EQUAL(doesPathExist(testFilePath), false);
    TEST_EQUAL(doesPathExist(testDirPath), false);

    runSyncProcess(std::string("rm " + testFilePath).c_str());
    runSyncProcess(std::string("rm -r " + testDirPath).c_str());
}

}
}
