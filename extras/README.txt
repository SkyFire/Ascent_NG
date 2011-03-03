Ascent Win32/64 Libraries Distribution
======================================

Installation Instructions:
Navigate to the directory of your chosen compiler (adjust pathes if necessary):

  VS 2010: C:\Program Files\Microsoft Visual Studio 10.0\VC

Copy the files in VC\include to VC\include.
Copy the files in VC\lib to VC\lib.

Next, you need to compile the zlib and PCRE libraries.
Depending on your compiler, open the solution under Sources:
  
  VS 2010: VC100-solution.sln

Depending on the architecture you want to build for, build the configuration:

  Release/Win32 if you are under Win32,
  Release/x64 if you are under Win64.

This should make a folder called Output_Release, or Output_x64_Release if you are building for X64.
In this folder you should find pcre.lib and zlib.lib.

Copy these two .lib files into:

  (for Win32)
  VS 2010: C:\Program Files\Microsoft Visual Studio 10.0\VC\lib

  (for Win64)
  VS 2010: C:\Program Files\Microsoft Visual Studio 10.0\VC\lib\amd64

The two DLL files provided with this distribution should go in your binary folder, OR
under C:\Windows\System32.

This only has to be done once per compiler!
When you update Ascent, this doesn't need to be done each time.

Now you can build Ascent as normal.