# libKitsunePersistence

## Description

This library contains all my functions for interactions with the storage. At the moment its the smalest of my projects and only contains functionality to read, modify and write binaray- and text- files.

### About my kitsune libraries

My first libraries hab really common names like libCommon and so on, but I thought this could be hard to find my code between other libraries, when my projects will become bigger. So I decided to make a common name over all my libraries, similar to the boost-libraries, for a better handling. I decided to use `Kitsune` as name for my projects, because Kitsunemimi are moe. ;)

### About my repositories in general

At the moment there are over 10 Kitsune-libraries an multiple tools in my private repos. I plan to make all open-source, but I have much to learn about open-source-projects. Most of all I have to improve my ticket-handling. So please help me to improve my workflow.

All my projects are mirrored to gitlab, where I have still my gitlab-ci runner. 

I don't really like it to write much comments into header-files. More exactly I absolutly hate it, when I have to search the provided functions of a header between a massive amount of comments. Header-files are for me primary to have an overview, of all provided functions, which I have see as fast as possible. Comments of functions, parameters and return-values in my code are only written into the source-file. So when something is unclear for a specific method, than look into the source-file. If the comment there are not helpful for you, then please write me a mail or an issue to fix this. 

## Content Overview

At the moment there are only binaray- and text-files implemented. 

### binary-files

These are some functions to map the data-buffer of libKitsuneCommon to the storage to persist the data of the buffer and restore them. The functions use direct read- and write-oberations to make things faster, but this requires more custom control.

### text-files

Following actions on test-files are supported now:

- read text files
- write text files
- append new text to an existing text-file
- replace a line within an existing text-file identified by a line number
- repace content within an existing text-file identified by matching the old content


## Build

I write my projects with the Qt-creator, but without Qt itself. 

### Requirements

#### Official repositories

- g++
- qt5-qmake
- libboost-filesystem-dev

#### Kitsune-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsuneCommon | 0.4.x |  https://github.com/tobiasanker/libKitsuneCommon.git

Required directory hirarchie:

```
├── (build-directory)
│   ├── libKitsuneCommon
│   └── libKitsunePersistence
│
└── (source-file-directory)
    ├── libKitsuneCommon
    └── libKitsunePersistence
```

Directories with the source file have to be in the same directory and the same goes for the build directories.

### build library

IMPORTANT: qmake normally requires the Qt-framework, but it can also work alone when using `/usr/lib/x86_64-linux-gnu/qt5/bin/qmake` instead of `qmake` (tested on ubuntu).

#### build all

HINT: While writing the build-steps below, I figured out, that it would be more helpful, to add a short bash-script. This will be done with the next update of the readme.

1. create a source- and build-directory

`mkdir <PATH_TO_BUILD_DIR>`

`mkdir <PATH_TO_SOURCE_DIR>`

2. download libKitsuneCommon into 

`git clone --branch <TAG_OF_REQUIREMENTS> https://github.com/tobiasanker/libKitsuneCommon.git <PATH_TO_SOURCE_DIR>/libKitsuneCommon`

3. create build-directory for libKitsuneCommon

`mkdir <PATH_TO_BUILD_DIR>/libKitsuneCommon`

`cd <PATH_TO_BUILD_DIR>/libKitsuneCommon`

4. build libKitsuneCommon-lib

`/usr/lib/x86_64-linux-gnu/qt5/bin/qmake <PATH_TO_SOURCE_DIR>/libKitsuneCommon/libKitsuneCommon.pro`

`/usr/bin/make`

5. redo step 2 - 4 while replacing all `libKitsuneCommon` by `libKitsunePersistence` and replacing `<TAG_OF_REQUIREMENTS>` with the current version of this library.


## Usage

### binary-files

**Header-file:** `files/binary_file.h`

This file contains the class for read and write of binary-files. It use the data-buffer of libKitsuneCommon as cache for all operations. The operations using posix-method with direct-flag to skip the page-chache of the linux-kernel. This makes operations with big block a bit faster because the data are less often copied. This has the result, that all read and write operations are synchronized.

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
#include <files/binary_file.h>

std::string filePath = "/tmp/testfile.bin";
Common::DataBuffer buffer(2);

// write somethin into the buffer
int testvalue = 42;
buffer.addData(&testvalue);

// create binary file and bind buffer
BinaryFile binaryFile(m_filePath, &buffer);

// allocate 4 x 4 KiB (4 blocks)
binaryFile.allocateStorage(4);

// write data to the storage
binaryFile.writeSegment(0, 		// <-- startblock of write-oberation within the file
	                    1,      // <-- number of blocks (each 4 KiB) to write
	                    0)      // <-- startblock of the data within the buffer

// read data to the storage
binaryFile.readSegment(0, 		// <-- startblock of the data within the file
	                   1,       // <-- number of blocks (each 4 KiB) to write
	                   1)       // <-- startblock of write-oberation within the buffer

// close file
binaryFile.closeFile()
```

### text-files

**Header-file:** `files/text_file.h`

Every action open and close the text-file. With this I don't need to handle an additional object-instance and operations on a text-file are really rare compared to working on a binary-file, so the addional time consumption for open and close the file has no special meaning for the performance.

All methods return a pair of bool-value as first element and a string-value as second element. The bool-value says, if the call was successful or not. When successful, the string-value contains the result, or if not successful, the string contains an error-message.

Little example:

```cpp
#include <files/text_file.h>

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

## Contributing

Please give me as many inputs as possible: Bugs, bad code style, bad documentation and so on.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details