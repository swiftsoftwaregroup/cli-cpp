# cli-cpp

Template for Command Line Interface (CLI) tool in C++ using [argparse](). 

# Development

## Setup for macOS

### Compilers

```bash
# install Command Line Tools (CLT) for Xcode:
xcode-select --install
```

### Homebrew

```bash
# install
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### cmake

```bash
# install
brew install cmake 

# verify
cmake --version
```

### vcpkg

```bash
# install
git clone https://github.com/microsoft/vcpkg "$HOME/vcpkg"
cd $HOME/vcpkg && ./bootstrap-vcpkg.sh
```

### Visual Studio Code

Download and install from [Visual Studio Code](https://code.visualstudio.com/download) site.

Open Visual Studio Code and press `Cmd + Shift + p`. Select `Shell Command: Install 'code' command in PATH`. 

## Work on macOS

Configure project:

```bash
source configure.sh
```

Open the project in Visual Studio Code:

```bash
code .
```

### Build

```bash
# list presets
cmake --list-presets

# configure and build Debug
cmake --preset Debug
cmake --build --preset Debug

# or to build Release
cmake --preset Release
cmake --build --preset Release
```

###  Run

```bash
echo "John" > name.txt

./build/Debug/cli-cpp greet name.txt
./build/Debug/cli-cpp greet --language es name.txt
./build/Debug/cli-cpp greet -l bg name.txt
```

### Test

```bash
# run via ctest
ctest --preset Debug
ctest --preset Release

# run via GoogleTest
pushd ./build/Debug && ./cli-cpp-test && popd
pushd ./build/Release && ./cli-cpp-test && popd
```

### Docs

#### How to start? 

```bash
mkdir -p docs
cd docs
sphinx-quickstart
```

