# LaunchPad

Application to enable faster workflows when working on a windows machine.
It is developed in C++ with Qt for OpenSource for the GUI.

## Installation
Open CMakeLists.txt using qtcreator and compile it. Use windeployqt to deploy the compiled code.<br>
Copy the "applocations.conf"-File to the Directory where the executable is located. They always have to be in the same folder. Then you can create a Link to the .exe and put the link into your windows autostart folder if you want it to be automatically enabled. It will be active after reboot.
If you dont want to reboot, you can also just start it by launching the .exe.


## Comments
I'm aware that this is far from cleancode, but this was a little tinkering project and i didn't yet have the time to clean up the code.
But I learned a lot with this project and I thought this might help others with their projects, so here you go :)