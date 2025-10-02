@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM ===== Config =====
if "%APP_NAME%"=="" set "APP_NAME=app"
set "VENDOR_DIR=vendors"
set "VENDOR_DIR_SOKOL=%VENDOR_DIR%\sokol"
set "VENDOR_DIR_WEBGPU=%VENDOR_DIR%\webgpu"
set "SOKOL_GFX=%VENDOR_DIR_SOKOL%\sokol_gfx.h"
set "SOKOL_APP=%VENDOR_DIR_SOKOL%\sokol_app.h"
set "SOKOL_GLUE=%VENDOR_DIR_SOKOL%\sokol_glue.h"
set "SOKOL_LOG=%VENDOR_DIR_SOKOL%\sokol_log.h"
set "BUILD_DEBUG=build\Debug"
set "BUILD_RELEASE=build\Release"
set "BUILD_WEB=build\web"

set "URL_GFX=https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_gfx.h"
set "URL_APP=https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_app.h"
set "URL_LOG=https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_log.h"
set "URL_GLUE=https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_glue.h"

REM ===== Ensure dirs =====
if not exist "%VENDOR_DIR%"         mkdir "%VENDOR_DIR%"
if not exist "%VENDOR_DIR_SOKOL%"   mkdir "%VENDOR_DIR_SOKOL%"
if not exist "%VENDOR_DIR_WEBGPU%"  mkdir "%VENDOR_DIR_WEBGPU%"
if not exist "%BUILD_DEBUG%"        mkdir "%BUILD_DEBUG%"
if not exist "%BUILD_RELEASE%"      mkdir "%BUILD_RELEASE%"
if not exist "%BUILD_WEB%"          mkdir "%BUILD_WEB%"

REM ===== Tools =====
where curl >nul 2>&1 && set "HAVE_CURL=1"
where clang-cl >nul 2>&1 && set "HAVE_CLANGCL=1"
where clang >nul 2>&1 && set "HAVE_CLANG=1"
where emcc >nul 2>&1 && set "HAVE_EMCC=1"

if "%~1"=="" (
  echo Usage: build.bat [native^|web] [sources...]
  exit /b 1
)

set "TARGET=%~1"
shift

REM ===== Download headers if missing =====
call :download_if_missing "%SOKOL_GFX%" "%URL_GFX%" || exit /b 1
call :download_if_missing "%SOKOL_APP%" "%URL_APP%" || exit /b 1
call :download_if_missing "%SOKOL_GLUE%" "%URL_GLUE%" || exit /b 1
call :download_if_missing "%SOKOL_LOG%" "%URL_LOG%" || exit /b 1

REM ===== Collect sources (args -> explicit; else src\*.c) =====
set "SRC_LIST="
set "HAVE_SRC="

if "%~1"=="" goto NoArgs

:ArgsLoop
if "%~1"=="" goto AfterCollect
set "HAVE_SRC=1"
set "SRC_LIST=!SRC_LIST! "%~1""
shift
goto ArgsLoop

:NoArgs
for %%f in (src\*.c) do (
  set "HAVE_SRC=1"
  set "SRC_LIST=!SRC_LIST! "%%f""
)

:AfterCollect
if not defined HAVE_SRC (
  echo [!] No C sources. Put .c files in src\ or pass files to build.bat
  exit /b 1
)

REM REM ===== Native Windows build =====

if defined HAVE_CLANGCL (
echo [i] Using clang-cl (MSVC-style)
set INCLUDES=/I%VENDOR_DIR_SOKOL% /I%VENDOR_DIR% /I%VENDOR_DIR_WEBGPU%
set LIBS=user32.lib gdi32.lib shell32.lib kernel32.lib winmm.lib opengl32.lib

echo [*] Building Debug...
clang-cl /nologo %INCLUDES% /Zi /Od /DDEBUG /DSOKOL_GLCORE33 %SRC_LIST% %LIBS% ^
/link /out:%BUILD_DEBUG%\%APP_NAME%.exe
if errorlevel 1 ( echo [x] Debug build failed. & exit /b 1 )

echo [✓] Done.
echo     Debug : "%BUILD_DEBUG%\%APP_NAME%.exe"
exit /b 0
 )

REM ===== Web (Emscripten) build =====
if /I "%TARGET%"=="web" (
  if not defined HAVE_EMCC (
    echo [!] emcc not found in PATH.
    exit /b 1
  )
  echo [i] Using emcc (Emscripten)
  set "INCLUDES=-I%VENDOR_DIR_SOKOL% -I%VENDOR_DIR% -std=c11"

  echo [*] Building Web (WASM)...
  emcc %SRC_LIST% %INCLUDES% -sEXPORTED_FUNCTIONS="['_setResolution', '_main']" -o index.js^
    --use-port=emdawnwebgpu -sALLOW_MEMORY_GROWTH=1 -sASSERTIONS=1 -sWASM=1
  if errorlevel 1 ( echo [x] Web build failed. & exit /b 1 )

  echo [✓] Done.
  echo     Web : "%BUILD_WEB%\index.html"
  exit /b 0
)

echo [!] Unknown target: %TARGET%
exit /b 1


REM ===== Helpers =====
:download_if_missing
REM %1 = target file, %2 = url
if exist %~1 (
  echo [=] %~nx1 exists, skipping download.
  exit /b 0
)
echo [>] Downloading %~nx1 ...
if defined HAVE_CURL (
  curl -fsSL -o %~1 %~2
) else (
  powershell -NoProfile -Command "Invoke-WebRequest -UseBasicParsing -Uri '%~2' -OutFile '%~1'"
)
if errorlevel 1 (
  echo [!] Failed to download %~nx1
  exit /b 1
)
exit /b 0

