# cppbase
cppbase是一个跨平台C++17基础库，提供一些C++项目常用的功能，如日志、线程池、TCP/UDP客户端以及服务端等。采用CMake编译和集成一系列第三封库（详见thirdparty)。

Cross-platform C++17 base library providing common features for C++ projects, such as logging, thread pool, TCP/UDP clien and server, etc. It uses CMake [DownloadProject](https://github.com/Crascit/DownloadProject) to build and integrate a set of thirdparty libraries (see thirdparty folder for details).

## 在QtCreator里配置clang-format (Setup clang-format in QtCreator)
* 帮助->关于插件->勾选Beautifier 重启QtCreator (Help->Plugin->Beautifier Restart QtCreator)
* 工具->选项->Beautifier->Clang-Format 添加程序所在目录，并配置风格 (Tools->Options->Beautifier->Clang-Format Choose command and style)
* 工具->选项->环境->键盘->ClangFormat 配置快捷键 (Tools->Options->environment->keyboard->ClangFormat Set hotkeys)
