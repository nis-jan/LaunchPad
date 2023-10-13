# LaunchPad

Application to enable faster workflows when working on a windows machine.
It is developed in C++ with Qt for the graphics.

## Installation:
Open CMakeLists.txt using qtcreator and compile it. Use windeployqt to deploy the compiled code.<br>
Copy the "applocations.conf"-File to the Directory where the executable is located. They always have to be in the same folder. Then you can create a Link to the .exe and put the link into your windows autostart folder if you want it to be automatically enabled. It will be active after reboot.
If you dont want to reboot, you can also just start it by launching the .exe.