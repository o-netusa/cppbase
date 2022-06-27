## Thirdparty libraries
* asio (https://github.com/chriskohlhoff/asio) provides cross-platform network socket and serial port APIs.
* cereal (https://github.com/USCiLab/cereal) provides serialization and deserialization functionality.
* cxxopts (https://github.com/jarro2783/cxxopts) provides cross-platform command line options support.
* eigen (https://gitlab.com/libeigen/eigen) C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.
* fmt (https://github.com/fmtlib/fmt) provides cross-platform C++ string formatting.
* gettext (https://www.gnu.org/software/gettext/) provides internationalization and localization (i18n and l10n).
* Google Test (https://github.com/google/googletest) C++ unit test framework.
* rapidjson (https://github.com/Tencent/rapidjson) provides JSON encoding/decoding functionalities.
* rapidxml (http://rapidxml.sourceforge.net/) provides XML encoding/decoding functionalities.
* rttr (https://github.com/rttrorg/rttr) provides C++ runtime type reflection.
* spdlog (https://github.com/gabime/spdlog) provides cross-platform logging functionality.
* stduuid (https://github.com/mariusbancila/stduuid) provides cross-platform uuid functionality.
* taskflow (https://github.com/taskflow/taskflow) helps to quickly write parallel and heterogeneous task programs in modern C++.
* yaml-cpp (https://github.com/jbeder/yaml-cpp)  is a YAML parser and emitter in C++ matching the YAML 1.2 spec.

## How to add new 3rd party library
* Add the 3rd party library package (.zip, tar.gz and etc.) to the subdirectory with its name (e.g. asio).
* Add the "DownloadProject" section into the CMakeLists.txt (see CMakeLists.txt for details).
