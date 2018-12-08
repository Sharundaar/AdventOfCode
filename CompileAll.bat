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

echo Compiling Day 6...
g++ ./6-1/main.cpp -o ./6-1/app.exe -O2 -static-libgcc -static-libstdc++
g++ ./6-2/main.cpp -o ./6-2/app.exe -O2 -static-libgcc -static-libstdc++

echo Compiling Day 7...
g++ ./7-1/main.cpp -o ./7-1/app.exe -O2 -static-libgcc -static-libstdc++
REM g++ ./7-2/main.cpp -o ./7-2/app.exe -O2 -static-libgcc -static-libstdc++