A ddl  foi compilada com os seguintes comandos:
g++ -DBUILDING_INDT_DLL -O3 -Wall -c -fmessage-length=0 -osrc\indt_dll.o ..\src\indt_dll.cpp
g++ -shared -Wl,--out-implib=libindt_dll.a -olibindt_dll.dll src\indt_dll.o
