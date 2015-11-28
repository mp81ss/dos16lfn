@echo off
if "%1" == "clean" goto clean
jwasm -W4 *.asm
echo.
jwlib -b -c -n -s -t ..\lib\dos16lfn.lib +main.obj +getlong.obj +getshort.obj +attribs.obj +open.obj +rename.obj +unlink.obj +mkdir.obj +chdir.obj +rmdir.obj +getcwd.obj
goto end

:clean
del *.obj
del ..\lib\dos16lfn.lib

:end
