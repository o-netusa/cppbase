## 第三方软件
### Google Test (https://github.com/google/googletest)

### asio (https://github.com/chriskohlhoff/asio)

### curl (https://github.com/curl/curl)

### fmt (https://github.com/fmtlib/fmt)

### sigslot (https://github.com/palacaze/sigslot)

### spdlog (https://github.com/gabime/spdlog)

### stduuid (https://github.com/mariusbancila/stduuid)

### vld (https://github.com/KindDragon/vld) [MSVC Only]
* Using fork at https://github.com/razor950/vld for VS 2017/2019

## 如何添加新的第三方软件
* 在代码仓库根目录下生成第三方库的文件夹并加入第三方库的压缩包（可以是.zip, .tar.gz等格式）
* 在CMakeLists.txt中加入相应的配置（详见CMakeLists.txt）
