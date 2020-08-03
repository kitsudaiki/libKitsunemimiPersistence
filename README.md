# libKitsunemimiPersistence

![Gitlab pipeline status](https://img.shields.io/gitlab/pipeline/tobiasanker/libKitsunemimiPersistence?label=build%20and%20test&style=flat-square)
![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/tobiasanker/libKitsunemimiPersistence?label=version&style=flat-square)
![GitHub](https://img.shields.io/github/license/tobiasanker/libKitsunemimiPersistence?style=flat-square)
![C++Version](https://img.shields.io/badge/c%2B%2B-14-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux--x64-lightgrey?style=flat-square)

## Description

This library contains all my functions for interactions with the storage. At the moment its the smalest of my projects and only contains functionality to read, modify and write binaray- and text- files, handle an sqlite3-database and write log-files.

### Content Overview

#### binary-files

These are some functions to map the data-buffer of libKitsunemimiCommon to the storage to persist the data of the buffer and restore them. The functions use direct read- and write-oberations to make things faster, but this requires more custom control.

#### text-files

Methods to read text files, write text files, append new text to an existing text-file, replace a line within an existing text-file identified by a line number and repace content within an existing text-file identified by matching the old content.

#### sqlite-database

Simple handling class to connect to a sqlite database and send sql-commands to the database. The results are converted into table-items of libKitsunemimiCommon for better handling of the results of the database and to easily print the results on commandline.

#### log-writer

Its a simple and really easy to use logger to wirte messages with timestamps to a log-file. 

## Build

### Requirements

name | repository | version | task
--- | --- | --- | ---
g++ | g++ | >= 6.0 | Compiler for the C++ code.
make | make | >= 4.0 | process the make-file, which is created by qmake to build the programm with g++
qmake | qt5-qmake | >= 5.0 | This package provides the tool qmake, which is similar to cmake and create the make-file for compilation.
sqlite3 library | libsqlite3-dev | >= 3.0 | handling of sqlite databases


Installation on Ubuntu/Debian:

```bash
sudo apt-get install g++ make qt5-qmake libboost-filesystem-dev libsqlite3-dev
```

IMPORTANT: All my projects are only tested on Linux. 

### Kitsunemimi-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsunemimiCommon | v0.14.0 |  https://github.com/tobiasanker/libKitsunemimiCommon.git

HINT: These Kitsunemimi-Libraries will be downloaded and build automatically with the build-script below.

### build library

In all of my repositories you will find a `build.sh`. You only have to run this script. It doesn't required sudo, because you have to install required tool via apt, for example, by yourself. But if other projects from me are required, it download them from github and build them in the correct version too. This script is also use by the ci-pipeline, so its tested with every commit.


Run the following commands:

```
git clone https://github.com/tobiasanker/libKitsunemimiPersistence.git
cd libKitsunemimiPersistence
./build.sh
cd ../result
```

It create automatic a `build` and `result` directory in the directory, where you have cloned the project. At first it build all into the `build`-directory and after all build-steps are finished, it copy the include directory from the cloned repository and the build library into the `result`-directory. So you have all in one single place.

Tested on Debian and Ubuntu. If you use Centos, Arch, etc and the build-script fails on your machine, then please write me a mail and I will try to fix the script.

## Usage

### binary-files

**Header-file:** `libKitsunemimiPersistence/files/binary_file.h`

This file contains the class for read and write of binary-files. It use the data-buffer of libKitsunemimiCommon as cache for all operations. The operations using posix-method with direct-flag to skip the page-chache of the linux-kernel. This makes operations with big block a bit faster because the data are less often copied. This has the result, that all read and write operations are synchronized.

This results in the requirement, that segments to read from storage or write to storage should be as big as possible or else the latency makes the whole thing very very slow. The class should be run in an extra thread, with handle all operations and makes the whole sync asynchon again.

There are only 4 operations at the moment:

- allocate more memory on the storage to make the file bigger
- read the current size of the file from the storage (for the case you open an existing file)
- wirte data from the buffer to the file
- read data from the file into the buffer

All operations return only a bool-value, which say if it was successful or not.

HINT: The data-buffer will be not be binded anymore in the next version.

Example:

```cpp
#include <libKitsunemimiPersistence/files/binary_file.h>

std::string filePath = "/tmp/testfile.bin";
Common::DataBuffer buffer(2);

// write somethin into the buffer
int testvalue = 42;
buffer.addData(&testvalue);

// create binary file and bind buffer
BinaryFile binaryFile(m_filePath);

// allocate 4 x 4 KiB (4 blocks)
binaryFile.allocateStorage(4,       // <-- number blocks 
                           4096);   // <-- size of a single block

// write data to the storage
binaryFile.writeSegment(&buffer,   // <-- source-buffer
                        0,         // <-- startblock of write-oberation within the file
                        1,         // <-- number of blocks (each 4 KiB) to write
                        0)         // <-- startblock of the data within the buffer

// read data to the storage
binaryFile.readSegment(&buffer,    // <-- target-buffer
                       0,          // <-- startblock of the data within the file
                       1,          // <-- number of blocks (each 4 KiB) to write
                       1)          // <-- startblock of write-oberation within the buffer

// close file
binaryFile.closeFile()
```

### text-files

**Header-file:** `libKitsunemimiPersistence/files/text_file.h`

Every action open and close the text-file. With this I don't need to handle an additional object-instance and operations on a text-file are really rare compared to working on a binary-file, so the addional time consumption for open and close the file has no special meaning for the performance.

All methods return a pair of bool-value as first element and a string-value as second element. The bool-value says, if the call was successful or not. When successful, the string-value contains the result, or if not successful, the string contains an error-message.

Little example:

```cpp
#include <libKitsunemimiPersistence/files/text_file.h>

std::string filePath = "/tmp/textfile.txt";

std::string content = "this is a test\n"
                      "and this is a second line";

std::pair<bool, std::string> ret;
std::string errorMessage = "";

// write text to file
bool writeResult = writeFile(filePath, 
                             content, 
                             errorMessage,
                             false);        // <-- force-flag, 
                                            //     with false it fails if file already existing
                                  
// add new text to the file
bool appendResult = appendText(filePath,
                               "\nand a third line",
                               errorMessage);

// read updated file
std::pair<bool, std::string> readResult = readFile(filePath, errorMessage);
// readResult.second would now contans:
//
// "this is a test\n"
// "and this is a second line\n"
// "and a third line";

```

### sqlite-database

**Header-file:** `libKitsunemimiPersistence/database/sqlite.h`

This is a simple sqlite database handler to run sql-queries against the database and get the result in form of a table-item for easier result-handling and printing. See the following example:


```cpp
#include <libKitsunemimiPersistence/database/sqlite.h>
#include <libKitsunemimiCommon/common_items/table_item.h>

std::pair<bool, std::string> result;

// create and init database
Sqlite testDB;
result = testDB.initDB("/tmp/testdb.db");
// first of the result is, if it was successful or not
// second of the result is the error-message, if failed

std::string sql = <Any SQL-query>

// run sql-qurey
Kitsunemimi::Common::TableItem resultItem;
result = testDB.execSqlCommand(&resultItem, sql);
// first of the result is, if it was successful or not
// second of the result is the error-message, if failed

// If successful, the resultItem contains the result of the sql-query. 
// It can easily converted into a string with `toString()`


testDB.close();

```

### log-writer

**Header-file:** `libKitsunemimiPersistence/logger/logger.h`

Its a simple class to write log-messages together with a timestamp one after another to a log-file. It only has to be initialized at the beginning of the program and can be used at every point in the same code. When want to add an entry to the log, you don't need to check, if the logger is initialized.

IMPORTANT: Adding entries to the log is thread-save, but initializing and closing the logger is NOT. This is normally no problem, but I only mention it, to be sure that you know this. It is not save to init or close the logger, while other threads with log-calls are running!


Initializing at the anytime somewhere in your code.

```cpp
#include <libKitsunemimiPersistence/logger/logger.h>

// initializing logger to write into a file
bool ret1 = Kitsunemimi::Persistence::initFileLogger("/tmp", "testlog", true);
// arguments:
//      first argument: directory-path
//      second argument: base file name
//      third argument: true to enable debug-output. if false only output of info, warning and error
//
// result:
//      true, if initializing was successfule, else false

// initializing logger to write log-messages on the console output
Kitsunemimi::Persistence::initConsoleLogger(true);
// argument: true to enable debug-output. if false only output of info, warning and error

```

Using the logger somewhere else in your code. You only need to import the header and then call the log-methods. Like already mentioned, there is no check necessary, if the logger is initialized or not. See following example: 

```cpp
#include <libKitsunemimiPersistence/logger/logger.h>

LOG_DEBUG("debug-message");
LOG_INFO("info-message");
LOG_WARNING("warning-message");
LOG_ERROR("error-message");

/**
The log-file would look like this:

2019-9-7 22:54:1 ERROR: error-message     
2019-9-7 22:54:1 WARNING: warning-message 
2019-9-7 22:54:1 DEBUG: debug-message     
2019-9-7 22:54:1 INFO: info-message  
*/

```

## Contributing

Please give me as many inputs as possible: Bugs, bad code style, bad documentation and so on.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
