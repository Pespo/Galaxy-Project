Galaxy Project
==============
December 2012 / January 2013
----------------------------
Guillaume ALBESPY
Guillaume BREDA
----------------
IMAC Engineering School, prom 2013
----------------------------------

Build on Linux
--------------
<pre>
	premake4 gmake
	make config=debug
	make config=release
</pre>

Tested on Archlinux64 with NVidia drivers 304.60, Ubuntu 12.04 64 with NVidia drivers 310.14.


Build on Mac OSX
----------------
<pre>
	premake4 gmake
	make config=debug
	make config=release
</pre>

Tested on Mac OSX 10.7 with geForce GPU
Note: Using core profile on macosx generates a GL error at context creation.


Build on Windows
----------------
<pre>
	premake4 vs2008
	Use the .sln file as usual in Visual Studio
</pre>

Tested on Windows 7 64 with geForce GPU


Running
--------
Use the command line or in Visual Studio set launch parameters to the following :
<pre>
	bin/debug/galaxy_project/galaxy_project
</pre>


How to use the program
----------------------
Use the keys Z,Q,S,D on an AZERTY keyboard and your mouse to move the free camera.
Press H to use your mouse on the Setting menu, press it again to take back the control of the camera.
Press G to make the Settings panel disapear, press it again to make it appear.