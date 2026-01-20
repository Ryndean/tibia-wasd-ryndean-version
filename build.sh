#!/bin/bash
i686-w64-mingw32-g++ -shared -o ddraw.dll dllmain.cpp \
    -DBUILDING_DLL=1 \
    -Wl,--add-stdcall-alias \
    -lws2_32 \
    -static-libgcc -static-libstdc++
