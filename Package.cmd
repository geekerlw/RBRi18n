@echo off
setlocal

set PLUGIN_NAME=RBRi18n
set VERSION=0.1.0
set BUILD_DIR=Release
set OUTPUT_DIR=Release\Package

echo === %PLUGIN_NAME% v%VERSION% Packager ===

if not exist "%BUILD_DIR%\%PLUGIN_NAME%.dll" (
    echo ERROR: %BUILD_DIR%\%PLUGIN_NAME%.dll not found. Build the project first.
    exit /b 1
)

if exist "%OUTPUT_DIR%" rd /s /q "%OUTPUT_DIR%"
mkdir "%OUTPUT_DIR%\Plugins"
mkdir "%OUTPUT_DIR%\%PLUGIN_NAME%"

copy /y "%BUILD_DIR%\%PLUGIN_NAME%.dll" "%OUTPUT_DIR%\Plugins\"
copy /y "%PLUGIN_NAME%\*.zh" "%OUTPUT_DIR%\%PLUGIN_NAME%\"
copy /y "%PLUGIN_NAME%\*.en" "%OUTPUT_DIR%\%PLUGIN_NAME%\" 2>nul

pushd "%OUTPUT_DIR%"
powershell -Command "Compress-Archive -Path 'Plugins','%PLUGIN_NAME%' -DestinationPath '..\%PLUGIN_NAME%-v%VERSION%.zip' -Force"
popd

echo.
echo Done: %BUILD_DIR%\%PLUGIN_NAME%-v%VERSION%.zip
