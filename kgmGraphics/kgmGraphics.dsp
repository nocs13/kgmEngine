# Microsoft Developer Studio Project File - Name="kgmGraphics" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=kgmGraphics - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "kgmGraphics.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "kgmGraphics.mak" CFG="kgmGraphics - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "kgmGraphics - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "kgmGraphics - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kgmGraphics - Win32 Release"

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

!ELSEIF  "$(CFG)" == "kgmGraphics - Win32 Debug"

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
# ADD LIB32 /nologo /out:".\kgmGraphics.lib"

!ENDIF 

# Begin Target

# Name "kgmGraphics - Win32 Release"
# Name "kgmGraphics - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\kgmGraphics.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGui.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGuiButton.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGuiCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGuiList.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGuiScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmGuiSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmShader.cpp
# End Source File
# Begin Source File

SOURCE=.\kgmTexture.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\kgm4Color.h
# End Source File
# Begin Source File

SOURCE=.\kgmAnimation.h
# End Source File
# Begin Source File

SOURCE=.\kgmAnimator.h
# End Source File
# Begin Source File

SOURCE=.\kgmCamera.h
# End Source File
# Begin Source File

SOURCE=..\kgmGame\kgmDummy.h
# End Source File
# Begin Source File

SOURCE=.\kgmEffect.h
# End Source File
# Begin Source File

SOURCE=.\kgmFog.h
# End Source File
# Begin Source File

SOURCE=.\kgmFont.h
# End Source File
# Begin Source File

SOURCE=.\kgmFramed.h
# End Source File
# Begin Source File

SOURCE=.\kgmFrustum.h
# End Source File
# Begin Source File

SOURCE=.\kgmGraphical.h
# End Source File
# Begin Source File

SOURCE=.\kgmGui.h
# End Source File
# Begin Source File

SOURCE=.\kgmGuiButton.h
# End Source File
# Begin Source File

SOURCE=.\kgmGuiCombo.h
# End Source File
# Begin Source File

SOURCE=.\kgmGuiList.h
# End Source File
# Begin Source File

SOURCE=.\kgmGUIMenu.h
# End Source File
# Begin Source File

SOURCE=.\kgmGuiScroll.h
# End Source File
# Begin Source File

SOURCE=.\kgmLandscape.h
# End Source File
# Begin Source File

SOURCE=.\kgmLight.h
# End Source File
# Begin Source File

SOURCE=.\kgmMaterial.h
# End Source File
# Begin Source File

SOURCE=.\kgmMesh.h
# End Source File
# Begin Source File

SOURCE=.\kgmNode.h
# End Source File
# Begin Source File

SOURCE=.\kgmPicture.h
# End Source File
# Begin Source File

SOURCE=.\kgmPolygon.h
# End Source File
# Begin Source File

SOURCE=.\kgmShader.h
# End Source File
# Begin Source File

SOURCE=.\kgmSkeleton.h
# End Source File
# Begin Source File

SOURCE=.\kgmSkin.h
# End Source File
# Begin Source File

SOURCE=.\kgmSprite.h
# End Source File
# Begin Source File

SOURCE=.\kgmTexture.h
# End Source File
# End Group
# End Target
# End Project
