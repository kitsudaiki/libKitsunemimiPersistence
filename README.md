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

paket | version
--- | ---
g++ | 6.3.0
qt5-qmake | 5.7.1
libboost-filesystem-dev | 1.62

This are the version I have installed under Debian Stable via apt. Some older or newer version should work as well. I write my projects with the Qt-creator, but without Qt itself. Thats why qmake is required to build my projects.

IMPORTANT: All my projects are only tested on Linux. 

#### Kitsune-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsuneCommon | 0.5.x |  https://github.com/tobiasanker/libKitsuneCommon.git


### build library

In all of my repositories you will find a `build.sh`. You only have to run this script. It doesn't required sudo, because you have to install required tool via apt, for example, by yourself. But if other projects from me are required, it download them from github and build them in the correct version too. This script is also use by the ci-pipeline, so its tested with every commit.

Before running the build-script:

```bash
.
└── libKitsunePersistence
    ├── build.sh
    └── ...
```

After running the build-script:

```bash
.
├── build
│   ├── libKitsuneCommon
│   │   └── ...
│   └── libKitsunePersistence
│       └── ...
│
├── libKitsuneCommon
│   └── ...
├── libKitsunePersistence
│   ├── build.sh
│   └── ...
│
└── result
    ├── include
    │   ├── libKitsuneCommon
    │   │   └── ...
    │   └── libKitsunePersistence
    │       └── ...
    │
    ├── libKitsuneCommon.so -> libKitsuneCommon.so.0.4.0
    ├── libKitsuneCommon.so.0 -> libKitsuneCommon.so.0.4.0
    ├── libKitsuneCommon.so.0.4 -> libKitsuneCommon.so.0.4.0
    ├── libKitsuneCommon.so.0.4.0
    │
    ├── libKitsunePersistence.so -> libKitsunePersistence.so.0.2.0
    ├── libKitsunePersistence.so.0 -> libKitsunePersistence.so.0.2.0
    ├── libKitsunePersistence.so.0.2 -> libKitsunePersistence.so.0.2.0
    └── libKitsunePersistence.so.0.2.0
```

It create automatic a `build` and `result` directory in the directory, where you have cloned the project. At first it build all into the `build`-directory and after all build-steps are finished, it copy the include directory from the cloned repository and the build library into the `result`-directory. So you have all in one single place.

Tested on Debian and Ubuntu. If you use Centos, Arch, etc and the build-script fails on your machine, then please write me a mail and I will try to fix the script.


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
