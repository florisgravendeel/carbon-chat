# Carbon Chat
> A WebSockets Chat system implementation using the [Carbon Language](https://github.com/carbon-language/carbon-lang) and C++.

## Running CarbonChat
I highly recommend to download one of our [releases](https://github.com/florisgravendeel/carbon-chat/releases)(Windows/macOS). It's the quickest way run to the project.

The client and server are combined into one file C++ executable. So there's no need to download multiple files. 

## Prerequisite for development
- C++
- [CMake](https://cmake.org/download/)
- [Boost 1.81.0](https://www.boost.org/users/download/)
    - On Windows Boost should be installed in `C:/local/boost_1_81_0` (Default Installation Folder)
    - On Mac, run `brew install boost` (Note: [Homebrew](https://brew.sh) required)
-  <del>Carbon is required. See the install guide [here](https://github.com/carbon-language/carbon-lang#getting-started).
- The submodules are also required, don't forget to pull them as well.
```
git clone https://github.com/florisgravendeel/carbon-chat.git
git submodule update --init
```

