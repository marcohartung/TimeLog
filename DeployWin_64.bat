rem Create zip file with .exe and all dlls for deployment
rem Marco Hartung 06.09.2018

cd..
if not exist "build" mkdir build
if exist "build\TimeLog_Winx64" del /Q /F /S "build\TimeLog_Winx64"
mkdir build\TimeLog_Winx64
mkdir build\TimeLog_Winx64\tmp

copy /Y "build-TimeLog-Desktop_Qt_5_11_2_MSVC2017_64bit2-Release\release\TimeLog.exe" "build\TimeLog_Winx64\tmp"

cd build\TimeLog_Winx64\tmp
"c:\Qt\5.11.2\msvc2017_64\bin\windeployqt.exe" "TimeLog.exe"
cd..
"c:\Program Files\7-Zip\7z.exe" a TimeLog.zip .\tmp\*
del /F /Q /S "tmp"
RD /s /q "tmp"

pause
