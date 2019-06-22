Start-BitsTransfer -Source https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip -Destination SDL2-devel-2.0.9-VC.zip
Expand-Archive SDL2-devel-2.0.9-VC.zip
Move-Item .\SDL2-devel-2.0.9-VC\SDL2-2.0.9 .\
Remove-Item .\SDL2-devel-2.0.9-VC
Copy-Item .\SDL2-2.0.9\lib\x86\SDL2.dll ..\res\

Start-BitsTransfer -Source https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.15-VC.zip -Destination SDL2_ttf-devel-2.0.15-VC.zip
Expand-Archive SDL2_ttf-devel-2.0.15-VC.zip
Move-Item .\SDL2_ttf-devel-2.0.15-VC\SDL2_ttf-2.0.15 .\
Remove-Item .\SDL2_ttf-devel-2.0.15-VC
Copy-Item .\SDL2_ttf-2.0.15\lib\x86\libfreetype-6.dll ..\res\
Copy-Item .\SDL2_ttf-2.0.15\lib\x86\SDL2_ttf.dll ..\res\
Copy-Item .\SDL2_ttf-2.0.15\lib\x86\zlib1.dll ..\res\

Pause
