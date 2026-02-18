#!/bin/bash
i686-w64-mingw32-g++ -shared -o ddraw.dll dllmain.cpp ddraw.def \
    -DBUILDING_DLL=1 \
    -Wl,--add-stdcall-alias \
    -lws2_32 -luser32 \
    -static-libgcc -static-libstdc++