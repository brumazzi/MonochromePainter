# x86_64-w64-mingw32-windres -o icon.o icon.rc

# x86_64-w64-mingw32-c++ src/* icon.o ./raylib.dll -I ./jsoncpp-1.9.5/include/ -I./include ./jsoncpp-1.9.5/src/lib_json/*.cpp -I./include -I./jsoncpp-1.9.5/include/jsoncpp/ -o MonocromePainter.exe -I/usr/local/include/ -I/home/brumazzi/Downloads/jsoncpp-1.9.5/include/ -Wl,--dynamicbase -Wl,--nxcompat -static -mwindows
# x86_64-w64-mingw32-c++ mapGenerator.cpp -o gmap.exe -Wl,--dynamicbase -Wl,--nxcompat -static -I ./jsoncpp-1.9.5/include/ -I./include ./jsoncpp-1.9.5/src/lib_json/*.cpp -I./include -I./jsoncpp-1.9.5/include/jsoncpp/

# rm icon.o