rem tools\jcf.exe -obfuscate -F tests\JcfMiscFunctions.pas -out jcfTestO.pas
rem tools\jcf.exe -clarify   -F tests\JcfMiscFunctions.pas -out jcfTestC.pas
tools\jcf.exe -obfuscate -F tests\JcfMiscFunctions.pas
tools\jcf.exe -clarify   -F tests\JcfMiscFunctions.pas
