![workflow](https://github.com/o-netusa/cppbase/actions/workflows/build.yml/badge.svg)

# cppbase
Cross-platform C++17 base library providing common features for C++ projects, such as logging, thread pool, TCP/UDP clien and server, etc. It uses CMake [DownloadProject](https://github.com/Crascit/DownloadProject) to build and integrate a set of thirdparty libraries (see thirdparty folder for details).

## Directory tree
```bash
common
├── common/BlockingQueue.h
├── common/Encoding.h
├── common/FileSystem.h
├── common/Global.h
├── common/MemoryLeaks.h
├── common/Semaphore.h
├── common/ThreadPool.h
├── common/Timer.h
└── common/Variant.h
config
└── config/log.conf
locale
logging
├── logging/Logging.h
└── logging/spdlog_setup
network
├── network/Common.h
├── network/TcpClient.h
├── network/TcpServer.h
├── network/UdpClient.h
└── network/UdpServer.h
tests
```

## Thirdparty libraries
[thirdparty/README.md](https://github.com/o-netusa/cppbase/tree/master/thirdparty/README.md)

## Setup clang-format in QtCreator
* Enable "Help->Plugin->Beautifier" and restart QtCreator
* Add the source code directory in "Tools->Options->Beautifier->Clang-Format" and choose command and style
* Setup hotkeys from "Tools->Options->environment->keyboard->ClangFormat"
