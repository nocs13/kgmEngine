# Microsoft Developer Studio Project File - Name="kgmMain" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=kgmMain - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "kgmMain.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "kgmMain.mak" CFG="kgmMain - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "kgmMain - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "kgmMain - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kgmMain - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "kgmMain - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\kgmMain.lib"

!ENDIF 

# Begin Target

# Name "kgmMain - Win32 Release"
# Name "kgmMain - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\kgmApp.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmFile.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmFileMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmMain.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmOAL.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmObject.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmOctTree.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmOGLExt.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmResource.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmSound.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmXml.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\kgmActions.h
# End Source File
# Begin Source File

SOURCE=.\kgmApp.h
# End Source File
# Begin Source File

SOURCE=.\kgmArchive.h
# End Source File
# Begin Source File

SOURCE=.\kgmBase.h
# End Source File
# Begin Source File

SOURCE=.\kgmDll.h
# End Source File
# Begin Source File

SOURCE=.\kgmEnvironment.h
# End Source File
# Begin Source File

SOURCE=.\kgmEvent.h
# End Source File
# Begin Source File

SOURCE=.\kgmFile.h
# End Source File
# Begin Source File

SOURCE=.\kgmFileMemory.h
# End Source File
# Begin Source File

SOURCE=.\kgmIGraphics.h
# End Source File
# Begin Source File

SOURCE=.\kgmList.h
# End Source File
# Begin Source File

SOURCE=.\kgmMain.h
# End Source File
# Begin Source File

SOURCE=.\kgmMath.h
# End Source File
# Begin Source File

SOURCE=.\kgmMemory.h
# End Source File
# Begin Source File

SOURCE=.\kgmOAL.h
# End Source File
# Begin Source File

SOURCE=.\kgmObject.h
# End Source File
# Begin Source File

SOURCE=.\kgmOctTree.h
# End Source File
# Begin Source File

SOURCE=.\kgmOGL.h
# End Source File
# Begin Source File

SOURCE=.\kgmQuadTree.h
# End Source File
# Begin Source File

SOURCE=.\kgmResource.h
# End Source File
# Begin Source File

SOURCE=.\kgmStack.h
# End Source File
# Begin Source File

SOURCE=.\kgmString.h
# End Source File
# Begin Source File

SOURCE=.\kgmSystem.h
# End Source File
# Begin Source File

SOURCE=.\kgmThread.h
# End Source File
# Begin Source File

SOURCE=.\kgmTime.h
# End Source File
# Begin Source File

SOURCE=.\kgmTypes.h
# End Source File
# Begin Source File

SOURCE=.\kgmWindow.h
# End Source File
# Begin Source File

SOURCE=.\kgmXml.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\kgmRender\kgmOctTree.h
# End Source File
# Begin Source File

SOURCE=..\kgmRender\kgmQuadTree.h
# End Source File
# End Target
# End Project
