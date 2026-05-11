@echo off
setlocal
echo ======================================
echo Unreal Project Full Rebuild Script
echo ======================================

echo Waiting for Unreal Editor to close...
:wait_for_editor
tasklist /FI "IMAGENAME eq UnrealEditor.exe" | find /I "UnrealEditor.exe" >nul
if %errorlevel%==0 (
 timeout /t 2 >nul
 goto wait_for_editor
)
echo Unreal Editor closed.

echo.
echo Deleting old build folders...
rmdir /s /q "C:/Users/loicg/Github/PumpkingProject/Pumpking\Binaries"
rmdir /s /q "C:/Users/loicg/Github/PumpkingProject/Pumpking\Intermediate"
del /q "C:/Users/loicg/Github/PumpkingProject/Pumpking\*.sln" >nul 2>&1

echo.
echo Regenerating Visual Studio project files...
set UBT_PATH="C:/Program Files/Epic Games/UE_5.6/\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
if not exist %UBT_PATH% set UBT_PATH="C:/Program Files/Epic Games/UE_5.6/\Engine\Binaries\DotNET\UnrealBuildTool.exe"
if not exist %UBT_PATH% (
 echo [ERROR] UnrealBuildTool.exe not found!
 pause
 exit /b 1
)
%UBT_PATH% -projectfiles -project="C:/Users/loicg/Github/PumpkingProject/Pumpking/Pumpking.uproject" -game -engine -progress
if %errorlevel% neq 0 (
 echo [ERROR] Failed to generate project files.
 pause
 exit /b %errorlevel%
)

echo.
echo Compiling project...
%UBT_PATH% PumpkingEditor Win64 Development -project="C:/Users/loicg/Github/PumpkingProject/Pumpking/Pumpking.uproject" -progress
if %errorlevel% neq 0 (
 echo [ERROR] Build failed.
 pause
 exit /b %errorlevel%
)

echo.
echo Launching Unreal Editor...
start "" "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe" "C:/Users/loicg/Github/PumpkingProject/Pumpking/Pumpking.uproject" -Build

echo.
echo All tasks completed successfully!
pause
endlocal
