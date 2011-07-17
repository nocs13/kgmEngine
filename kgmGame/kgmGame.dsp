# Microsoft Developer Studio Project File - Name="kgmGame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=kgmGame - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "kgmGame.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "kgmGame.mak" CFG="kgmGame - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "kgmGame - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "kgmGame - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kgmGame - Win32 Release"

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

!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\kgmGame.lib"

!ENDIF 

# Begin Target

# Name "kgmGame - Win32 Release"
# Name "kgmGame - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\kgmActor.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmBarrier.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmBomb.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmCase.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmController.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmDeformer.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmDestruction.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmDoor.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGameAI.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGameBase.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGameLogic.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGamePhysics.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGameResources.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGameScene.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGameSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGameTools.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmItem.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmKey.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmLandscape.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmLock.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmParticles.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmPawn.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmSwitch.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmTransporter.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmTrigger.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmWeapon.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\kgmActor.h
# End Source File
# Begin Source File

SOURCE=.\kgmBarrier.h
# End Source File
# Begin Source File

SOURCE=.\kgmBomb.h
# End Source File
# Begin Source File

SOURCE=.\kgmBullet.h
# End Source File
# Begin Source File

SOURCE=.\kgmCase.h
# End Source File
# Begin Source File

SOURCE=.\kgmController.h
# End Source File
# Begin Source File

SOURCE=.\kgmDeformer.h
# End Source File
# Begin Source File

SOURCE=.\kgmDestruction.h
# End Source File
# Begin Source File

SOURCE=.\kgmDoor.h
# End Source File
# Begin Source File

SOURCE=.\kgmDress.h
# End Source File
# Begin Source File

SOURCE=.\kgmEffect.h
# End Source File
# Begin Source File

SOURCE=.\kgmElevator.h
# End Source File
# Begin Source File

SOURCE=.\kgmFerry.h
# End Source File
# Begin Source File

SOURCE=.\kgmFlame.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameAI.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameApp.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameBase.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameCollision.inl
# End Source File
# Begin Source File

SOURCE=.\kgmGameGraphics.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameGui.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameLogic.h
# End Source File
# Begin Source File

SOURCE=.\kgmGamePhysics.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameResources.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameScene.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameSettings.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameTools.h
# End Source File
# Begin Source File

SOURCE=.\kgmGameWindow.h
# End Source File
# Begin Source File

SOURCE=.\kgmIGame.h
# End Source File
# Begin Source File

SOURCE=.\kgmInventory.h
# End Source File
# Begin Source File

SOURCE=.\kgmItem.h
# End Source File
# Begin Source File

SOURCE=.\kgmKgfLoader.h
# End Source File
# Begin Source File

SOURCE=.\kgmLandscape.h
# End Source File
# Begin Source File

SOURCE=.\kgmLock.h
# End Source File
# Begin Source File

SOURCE=.\kgmModifier.h
# End Source File
# Begin Source File

SOURCE=.\kgmParticles.h
# End Source File
# Begin Source File

SOURCE=.\kgmPawn.h
# End Source File
# Begin Source File

SOURCE=.\kgmSmoke.h
# End Source File
# Begin Source File

SOURCE=.\kgmStatic.h
# End Source File
# Begin Source File

SOURCE=.\kgmSurface.h
# End Source File
# Begin Source File

SOURCE=.\kgmSwitch.h
# End Source File
# Begin Source File

SOURCE=.\kgmTransporter.h
# End Source File
# Begin Source File

SOURCE=.\kgmTrigger.h
# End Source File
# Begin Source File

SOURCE=.\kgmTriggers.h
# End Source File
# Begin Source File

SOURCE=.\kgmWater.h
# End Source File
# Begin Source File

SOURCE=.\kgmWeapon.h
# End Source File
# End Group
# End Target
# End Project
