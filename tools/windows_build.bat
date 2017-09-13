call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\VsDevCmd.bat"
set third_party_path=%1
set qmake_path=%2
set platform=%3
set build_type=%4
if %platform%==amd64 (
	call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64
)
mkdir ..\third_party
xcopy %third_party_path% ..\third_party /e
cd ..\sdk
call %qmake_path%\qmake.exe sdk.pro -spec win32-msvc2013 "CONFIG+="%build_type% 
call C:\Qt\Tools\QtCreator\bin\jom.exe clean
call C:\Qt\Tools\QtCreator\bin\jom.exe
cd ..\samples
call %qmake_path%\qmake.exe samples.pro -spec win32-msvc2013 "CONFIG+="%build_type% 
call C:\Qt\Tools\QtCreator\bin\jom.exe clean
call C:\Qt\Tools\QtCreator\bin\jom.exe
cd ..\bin
copy tests\*UnitTest* .
set PATH=%qmake_path%;%PATH%
set PATH=..\third_party\lib\eay;%PATH%

IF /I "%build_type"=="debug" (
  rmscryptoUnitTestsd.exe -xunitxml > tests\rmsUnitTestResults.txt
  ::rmsauthUnitTestsd.exe -xunitxml >> tests\rmsUnitTestResults.txt
  rmsplatformUnitTestsd.exe -xunitxml >> tests\rmsUnitTestResults.txt
  RestClientsUnitTestsd.exe -xunitxml >> tests\rmsUnitTestResults.txt
)
IF /I "%build_type"=="release" (
  rmscryptoUnitTests.exe -xunitxml > tests\rmsUnitTestResults.txt
  ::rmsauthUnitTests.exe -xunitxml >> tests\rmsUnitTestResults.txt
  rmsplatformUnitTests.exe -xunitxml >> tests\rmsUnitTestResults.txt
  RestClientsUnitTests.exe -xunitxml >> tests\rmsUnitTestResults.txt
)

cd ..