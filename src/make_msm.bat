@echo off
set OUTPUT_LIB=..\lib\dos16lfn.lib
if "%1" == "clean" goto clean
ml /W3 /Cp /nologo /c *.asm
if exist %OUTPUT_LIB% del %OUTPUT_LIB%
lib %OUTPUT_LIB% /noignorecase /noextdictionary +main.obj +getlong.obj +getshort.obj +attribs.obj +open.obj +rename.obj +unlink.obj +mkdir.obj +chdir.obj +rmdir.obj +getcwd.obj ,
goto end

:clean
del *.obj
del

:end
set OUTPUT_LIB=
