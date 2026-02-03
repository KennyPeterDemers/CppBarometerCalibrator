@echo on
setlocal

REM ---- EDIT THIS to your actual output folder ----
set "RELEASE_DIR=C:\Repos\CppBarometerCalibrator\Win32\Release"

REM ---- Output MSI name ----
set "OUT_MSI=BarometerCalibrator.msi"

REM ---- WXS file ----
set "WXS=BarometerCalibrator.wxs"

REM ---- Log file ----
set "LOG=build_wix.log"

REM ---- Build (capture all output to log) ----
wix build "%WXS%" ^
  -o "%OUT_MSI%" ^
  -bindpath ReleaseDir="%RELEASE_DIR%" ^
  -bindvariable WixUILicenseRtf="C:\Repos\CppBarometerCalibrator\InstallerWix\license.rtf" ^
  -ext WixToolset.UI.wixext ^
  -culture en-US ^
  -v > "%LOG%" 2>&1

REM ---- Decide success based on whether MSI exists ----
if exist "%OUT_MSI%" (
  echo.
  echo BUILD SUCCEEDED: %OUT_MSI%
  echo Log saved to: %CD%\%LOG%
) else (
  echo.
  echo BUILD FAILED (no MSI produced).
  echo See log: %CD%\%LOG%
)

pause
