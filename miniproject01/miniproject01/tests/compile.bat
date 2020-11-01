nasm -f win32 sources.asm -o solution.o
ld -o application.exe solution.o C:/windows/system32/msvcrt.dll C:/windows/system32/user32.dll C:/windows/system32/kernel32.dll -e main
pause
