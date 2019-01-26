# Project Digger

*Tested with **g++** as well as **VC++**, either with **-std=c++11** and **-std=c++14***

## Initial setup instructions
**[Linux]**
 - Install **SDL2**, as stated in [their site](https://wiki.libsdl.org/Installation)  
   *(**Debian**-based systems [including **Ubuntu**]) can simply run these commands:*
    ```
    sudo apt-get install libsdl2-2.0
    sudo apt-get install libsdl2-dev
    sudo apt-get install libsdl2-image-2.0
    sudo apt-get install libsdl2-image-dev
    sudo apt-get install libsdl2-ttf-2.0
    sudo apt-get install libsdl2-ttf-dev
    ```
 - Link **SDL2**, **SDL2_image** and **SDL2_ttf** for your preferred IDE
 
**[Windows] (Visual Studio)**
 - Right click on your project's **References** -> **Manage NuGet Packages**
 - Search for **sdl2** and install **sdl2 by Sam Lantinga, SDL Contributors (Version 2.0.5)**
 - Search for **sdl2 image** and install **sdl2_image.nuget by Sam Lantinga and SDL2 contributors (Version 2.0.4)**
 - Search for **sdl2 ttf** and install **sdl2_ttf.nuget by Sam Lantinga and SDL2 contributors (Version 2.0.14.2)**  
 *(Folders **"/Levels"**, **"/Sprites"**, **"/Fonts"** and **"/Sounds"** must be placed where your **project file** is located [.vcxproj])*

**[Windows] (other IDE)**
 - Download **SDL2**, **SDL2_image** and **SDL2_ttf** for windows
 - Install **SDL2** as stated in [their site](https://wiki.libsdl.org/Installation)
 - Link **SDL2**, **SDL2_image** and **SDL2_ttf** for your preferred IDE


## Features
 - Levels loading from files
 - Textures system
 - Dirt & tunnels with varying size
 - Digger movement
 - Emeralds & scoring
 - Bags & gold
 - Enemy tracking
 - Lives
 - Enemy spawner with fixed amount of enemies
 - Enemies eat gold
 - ***[TODO]** Enemies transform & dig*
 - Enemies push bags
 - Enemies kill Digger
 - ***[TODO]** Out of lives restarts game*
 - ***[TODO]** Highscore system*
 - Bags kill everything they fall on
 - Digger can fire
 - ***[TODO]** Main menu*
 - ***[TODO]** Pause menu* 
 - ***[TODO]** Sound system*
 - ***[TODO]** Bonus mode*
 - Pushing bags slows down Digger
