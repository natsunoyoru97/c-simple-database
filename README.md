# cpp-simple-database

## Toolchain
- Language: C++17
- Compiler: clang++
- Build System: xmake
- Unit Test: gtest

## Get Started(Default)
- Build: 
```
xmake
```
- Run target: 
```
xmake run target
```
- Run target (debug mode): 
```
xmake run -d target
```
- Run test: 
```
xmake run test
```
- Run test (debug mode): 
```
xmake run -d test
```

## Enable/Disable asan
You can also enable address sanitizer(asan) for this project.
The option is DISABLED on default.

- Enable asan: 
```shell
xmake f --asan=y
```
- Disable asan:
```shell
xmake f --asan=n
```

Note it is a CONFIGURATION option and you don't need to pass the flag every time you build the project. 
After reconfiguration, you should rebuild the project.

### Setup pre-push hook
- Install a few binary for C++ language: `clang-format`, `cppcheck`, `cpplint`
- Install pre-push hook:
```
pre-commit install -t pre-push
```
