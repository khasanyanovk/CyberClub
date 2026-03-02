# CyberClub

An application for analyzing the effectiveness of a computer club.

### Requirements
- CMake 3.14+
- C++20 compiler (GCC, Clang, MSVC)

## Build and run

### Clone repository
```bash
git clone https://github.com/khasanyanovk/CyberClub.git
cd ./CyberClub
```

### Build project
```bash
cmake -B build
cmake --build build --config Release
```

### Run application
```bash
# Linux/macOS
./build/cyber-club examples/test_input.txt        
# Windows  
.\build\Release\cyber-club.exe examples\test_input.txt  
```

### Run tests
```bash
# Linux/macOS
./build/tests/cyber-club-tests  
# Windows
.\build\tests\Release\cyber-club-tests.exe  
```

## CI/CD

Project use GitHub Actions for automatic check on next platforms: Ubuntu, Windows, MacOs.  
Workflow structure: build -> test -> run

1. **Build** — compile project and upload artifacts for next usage
2. **Test** — download test artifacts and run tests
3. **Run Application** — download app artifact, run application and checking the correctness of the result for the test data.
