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

## How to add new 3rd party library
* Add the 3rd party library package (.zip, tar.gz and etc.) to the subdirectory with its name (e.g. asio).
* Add the "DownloadProject" section into the CMakeLists.txt (see CMakeLists.txt for details).