echo off
del TextTP.rar
rem cd out\release
copy /Y out\debug\*.xml doc\*.xml
copy /Y out\debug\*.lng doc\*.lng
copy /Y out\debug\*.hlf doc\*.hlf
copy /Y out\debug\templates\*.* doc\templates\*.*
rar32 a -y -ep -apTextTP  texttp.rar out\release\TextTP.dll out\release\pluginfo.exe 
rar32 a -y -ep -apTextTP\fmt TextTP.rar out\release\ext-fmt.dll out\release\text-fmt.dll

rar32 a -y -ep -apTextTP\tools TextTP.rar tools\AStyle.exe
rar32 a -y -ep -apTextTP\tools\AstDoc TextTP.rar tools\AstDoc\*.*

rar32 a -y -ep -apTextTP  texttp.rar doc\*.hlf doc\*.lng doc\*.xml
rar32 a -y -ep -apTextTP\templates texttp.rar doc\Templates\*.cpp doc\Templates\*.h doc\Templates\*.txt
rar32 a -y -ep -apTextTP\templates texttp.rar doc\Templates\*.xml doc\Templates\*.htm
rar32 a -y -ep -apTextTP TextTP.rar doc\*.reg
rar32 a -y -ep -apTextTP TextTP.rar doc\relnotes.txt doc\readme.txt doc\Announce.txt
rar32 a -y -ep -apTextTP TextTP.rar doc\hotkeys-ru.rtf doc\old-hotkeys-ru.rtf

rar32 a -y -ep TextTP.rar File_id.diz Descript.ion

rem arc plugins sources
rar32 a -y -ep -apTextTP TextTP.rar ext-fmt.vcproj pluginfo.vcproj text-fmt.vcproj
rar32 a -y -ep -apTextTP\src TextTP.rar src\fmtUtils.cpp src\fmtUtils.h src\itextfmt.h src\ttpapi.h
rar32 a -y -ep -apTextTP\src\plugins\ext-fmt TextTP.rar src\plugins\ext-fmt\ext-fmt.cpp src\plugins\ext-fmt\ext-fmt.def
rar32 a -y -ep -apTextTP\src\plugins\text-fmt TextTP.rar src\plugins\plain-text\text-fmt.cpp 
rar32 a -y -ep -apTextTP\src\plugins\text-fmt TextTP.rar src\plugins\plain-text\text-fmt.def 
rar32 a -y -ep -apTextTP\src\plugins\text-fmt TextTP.rar src\plugins\plain-text\pluginfo.cpp

rem rar32 a -y -apTextTP  texttp.rar 
rem rar32 a -y -apTextTP  texttp.rar 
rem cd ..\..

rar32 a -y -ep -apTextTP TextTP.rar doc\ttp.cnt doc\TTP.GID doc\TTP.HLP doc\ttp.toc


rar32 a -y -ep ttprtf.rar doc\ttpug.rtf
