@echo off

echo Compiling Day 1...
g++ ./1-1/main.cpp -o ./1-1/app.exe -O2 -static-libgcc -static-libstdc++
g++ ./1-2/main.cpp -o ./1-2/app.exe -O2 -static-libgcc -static-libstdc++

echo Compiling Day 2...
g++ ./2-1/main.cpp -o ./2-1/app.exe -O2 -static-libgcc -static-libstdc++
g++ ./2-2/main.cpp -o ./2-2/app.exe -O2 -static-libgcc -static-libstdc++

echo Compiling Day 3...
g++ ./3-1/main.cpp -o ./3-1/app.exe -O2 -static-libgcc -static-libstdc++
g++ ./3-2/main.cpp -o ./3-2/app.exe -O2 -static-libgcc -static-libstdc++

echo Compiling Day 4...
g++ ./4-1/main.cpp -o ./4-1/app.exe -O2 -static-libgcc -static-libstdc++
g++ ./4-2/main.cpp -o ./4-2/app.exe -O2 -static-libgcc -static-libstdc++

echo Compiling Day 5...
g++ ./5-1/main.cpp -o ./5-1/app.exe -O2 -static-libgcc -static-libstdc++
g++ ./5-2/main.cpp -o ./5-2/app.exe -O2 -static-libgcc -static-libstdc++