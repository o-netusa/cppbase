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

## 如何添加新的第三方软件
* 在代码仓库根目录下生成第三方库的文件夹并加入第三方库的压缩包（可以是.zip, .tar.gz等格式）
* 在CMakeLists.txt中加入相应的配置（详见CMakeLists.txt）
