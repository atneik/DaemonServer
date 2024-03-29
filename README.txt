/*
** Aniket Handa
** GNU GPL v3
** Ref: W. Richard Stevens' UNIX Network Programming 2nd edition.
*/

Read me for DaemonServer
=============================

Contents
=============================
	1.	About
	
	2.	Build & Run
		2.1	*nix
		2.2	Windows
		2.3	Xcode
	
	3.	Usage
		3.1 Client
		3.2 Server
		3.3 Server as daemon
		3.4 How to kill a daemon Server

1.	About
=============================
A basic Client-Server supporting multiple clients with an option of running Server in the background.

2.	Build & Run
=============================
	
2.1	*nix
-----------------------------

	What will be needed:

	1)	GCC
	3)	Cmake (optional)

Open the root dir of the code in the Terminal. Then,

	$ mkdir build
	$ cd build
	$ cmake ..
	$ make

If no errors you will see some executables in /build/src/
If you don't have Cmake, you can simply build the solution on *nix systems by:
	cd to the root of the src
	$make

2.2	Windows
-----------------------------

	What will be needed:

	1)	Visual Studio
	3)	Cmake

Open the root dir of the code in the Command prompt. Then,

	$ mkdir build
	$ cd build
	$ cmake -g "Visual Studio" ..

If no errors you will see Visual Studio's solution (.snl) in the default dir.

2.3	Mac OS Xcode
-----------------------------

	What will be needed:

	1)	Apple Developer Tools
	2)	Xcode
	3)	Cmake

Open the root dir of the code in the Terminal. Then,

	$ mkdir build
	$ cd build
	$ cmake -g "Xcode" ..

If no errors you will see Xcode's solution in the default dir.

3	Usage 
=============================

cd to the root of the package then:

3.1	Client
-----------------------------
	$ ./Client [hostname] [mnemonic name]

	Eg: $. /Client localhost Adam

3.2	Server
-----------------------------
	$. /Server

3.2	Server as daemon
-----------------------------
	$. /Server -d

3.4	How to Kill daemon Server
-----------------------------
Send the kill signal to the daemon process.

	$ sudo ps -ax | grep Server
	$ sudo kill -s kill [PID]

EOF