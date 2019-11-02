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

## Common Information aboud my projects

Here some common information about my projects and my code-styling. It's not complete and a bit short. I will write a styling-guide for my projects, where I will write this a bit longer with more explanation, why I make it like this.

### About my kitsune libraries

1. All my libraries beginning with `libKitsune`, because I needed a naming to identify my own libraries and I decided to use `Kitsune` as name, because Kitsunemimi are moe. ;)

2. The namespace of the code within the libraries is identically to the name of the library. So for example all content of `libKitsunemimiCommon` has the namespace `Kitsune::Common`.

3. If you only want to use the library, beside the binary you only ne the public methods and variables in the header-files, which are located in the `include`-directory of each `libKitsune`-repo. I try my best to make these as small and self-explaining, as possible. 

### About my repositories in general

1. I programming only on Linux. Sorry, but I have no time for supporting Windows and Mac.

2. All my projects are mirrored to gitlab, where I have still my gitlab-ci runner.

3. In each project-repository is an `build.sh`-script to make it easier for you to build the project by yourself. It download and link all required git-repositories in the correct version and link it all. This script is also used by the gitlab-ci-runner and so it is everytime up-to-date.

4. All my projects have unit-tests, which are executed by the ci-runner in each commit. Most of the functions are tested. Primary some negative-tests are missing. I try to avoid mocks in the tests as much as possible. Even tcp-socket-tests using the localhost for real data-transfer-tests.

5. For the naming of variables, methods and classes, I use camel case and for the names of files and directories, I use snake case.

6. About my comments:

    - Many comments in my code seems to be a bit useless, but I like to use them for optical separation. I also write them, when I think I finished the programming task and while writing the comments I recheck the code under the comment again and often I find improvements or bugs while this.

    - At first I planned to use doxygen comment-style for methods and files, but I think I will change this, because while writing the current state of the code, I don't like it to write big comments for each simple self-explaining method.

    - I don't like it to write much comments into header-files. More exactly I absolutly hate it, when I have to search the provided functions of a header between a massive amount of comments. Header-files are for me primary to have an overview of all provided functions, which I want to see as fast as possible. Comments of functions, parameters and return-values in my code are only written into the source-files. So when something is unclear for a specific method, then look into the source-file. If the comment there are not helpful for you, then please write me a mail or an issue to fix this. 

7. I try to avoid templates and macros as much as possible. I don't dislike them, but I only use them, when I don't have another solution. 

## Build

I write my projects with the Qt-creator, but without Qt itself. 

### Requirements

#### Official repositories

paket | version
--- | ---
g++ | 6.3.0
qt5-qmake | 5.7.1
libboost-filesystem-dev | 1.62
libsqlite3-dev | 3.16.2

This are the version I have installed under Debian Stable via apt. Some older or newer version should work as well. I write my projects with the Qt-creator, but without Qt itself. Thats why qmake is required to build my projects.

IMPORTANT: All my projects are only tested on Linux. 

#### Kitsune-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsunemimiCommon | v0.7.0 |  https://github.com/tobiasanker/libKitsunemimiCommon.git


### build library

In all of my repositories you will find a `build.sh`. You only have to run this script. It doesn't required sudo, because you have to install required tool via apt, for example, by yourself. But if other projects from me are required, it download them from github and build them in the correct version too. This script is also use by the ci-pipeline, so its tested with every commit.

Before running the build-script:

```bash
.
└── libKitsunemimiPersistence
    ├── build.sh
    └── ...
```

After running the build-script:

```bash
.
├── build
│   ├── libKitsunemimiCommon
│   │   └── ...
│   └── libKitsunemimiPersistence
│       └── ...
│
├── libKitsunemimiCommon
│   └── ...
├── libKitsunemimiPersistence
│   ├── build.sh
│   └── ...
│
└── result
    ├── include
    │   ├── libKitsunemimiCommon
    │   │   └── ...
    │   └── libKitsunemimiPersistence
    │       └── ...
    │
    ├── libKitsunemimiCommon.so.0 -> libKitsunemimiCommon.so.0.7.0
    ├── libKitsunemimiCommon.so.0.7 -> libKitsunemimiCommon.so.0.7.0
    ├── libKitsunemimiCommon.so.0.7.0
    │
    ├── libKitsunemimiPersistence.so.0 -> libKitsunemimiPersistence.so.0.5.0
    ├── libKitsunemimiPersistence.so.0.5 -> libKitsunemimiPersistence.so.0.5.0
    └── libKitsunemimiPersistence.so.0.5.0
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
                        0, 		   // <-- startblock of write-oberation within the file
	                    1,         // <-- number of blocks (each 4 KiB) to write
	                    0)         // <-- startblock of the data within the buffer

// read data to the storage
binaryFile.readSegment(&buffer,    // <-- target-buffer
                       0, 		   // <-- startblock of the data within the file
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

// write text to file
ret = writeFile(filePath, 
	            content, 
	            false);		// <-- force-flag, 
                            //     with false it fails if file already existing
                                  
// add new text to the file
ret = appendText(filePath,
                 "\nand a third line");

// read updated file
ret = readFile(filePath);
// ret.second would now contans:
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
Kitsune::Common::TableItem resultItem;
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

std::pair<bool, std::string> ret = Kitsune::Persistence::initLogger("/tmp", "testlog", true);
// arguments:
//      first argument: directory-path
//      second argument: base file name
//      third argument: true to enable debug-output. if false only output of info, warning and error
//      fourth argument: true to write log-output without timestamp additional to commandline
//
// result:
//      first of the result is, if it was successful or not
//      second of the result is the error-message, if failed

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
