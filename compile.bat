@echo off

if not exist build\ (
    mkdir build
)
cd build 
cmake .. --fresh
cmake --build .
cd .. 
cd bin
test.exe