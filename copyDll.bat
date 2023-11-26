copy /Y out\release\texttp.dll C:\Far\Plugins\Editor\TextTP\texttp.dll
if not exist C:\Far\Plugins\Editor\TextTP\fmt mkdir C:\Far\Plugins\Editor\TextTP\fmt
if not exist C:\Far\Plugins\Editor\TextTP\tools mkdir C:\Far\Plugins\Editor\TextTP\tools
if not exist C:\Far\Plugins\Editor\TextTP\tools\AstDoc mkdir C:\Far\Plugins\Editor\TextTP\tools\AstDoc

copy /Y out\release\text-fmt.dll C:\Far\Plugins\Editor\TextTP\fmt\text-fmt.dll
copy /Y out\release\ext-fmt.dll  C:\Far\Plugins\Editor\TextTP\fmt\ext-fmt.dll
copy /Y out\release\pluginfo.exe C:\Far\Plugins\Editor\TextTP\pluginfo.exe

copy /Y tools\AstDoc\*.html C:\Far\Plugins\Editor\TextTP\tools\AstDoc\
copy /Y tools\AStyle.exe    C:\Far\Plugins\Editor\TextTP\tools\AStyle.exe
copy /Y tools\jcf.exe       C:\Far\Plugins\Editor\TextTP\tools\jcf.exe
