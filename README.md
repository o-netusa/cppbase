![workflow](https://github.com/o-netusa/cppbase/actions/workflows/cmake.yml/badge.svg)

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
└── common/Timer.h
config
└── config/log.conf
logging
├── logging/Logging.h
└── logging/spdlog_setup
network
├── network/Common.h
├── network/TcpClient.h
├── network/TcpServer.h
├── network/UdpClient.h
└── network/UdpServer.h
```

## Thirdparty libraries
* asio (https://github.com/chriskohlhoff/asio) provides cross-platform network socket and serial port APIs
* cxxopts (https://github.com/jarro2783/cxxopts) provides cross-platform command line options support
* eigen (https://gitlab.com/libeigen/eigen) C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.
* fmt (https://github.com/fmtlib/fmt) provides cross-platform C++ string formatting
* Google Test (https://github.com/google/googletest) C++ unit test framework
* rapidjson (https://github.com/Tencent/rapidjson) provides JSON encoding/decoding functionalities
* rapidxml (http://rapidxml.sourceforge.net/) provides XML encoding/decoding functionalities
* sigslot (https://github.com/palacaze/sigslot) provides cross-platform event and handler functionalities similar to Qt signal/slot
* spdlog (https://github.com/gabime/spdlog) provides cross-platform logging functionality
* stduuid (https://github.com/mariusbancila/stduuid) provides cross-platform uuid functionality

## Setup clang-format in QtCreator
* Enable "Help->Plugin->Beautifier" and restart QtCreator
* Add the source code directory in "Tools->Options->Beautifier->Clang-Format" and choose command and style
* Setup hotkeys from "Tools->Options->environment->keyboard->ClangFormat"
