# Microsoft Developer Studio Project File - Name="kgmMath" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=kgmMath - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "kgmMath.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "kgmMath.mak" CFG="kgmMath - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "kgmMath - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "kgmMath - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kgmMath - Win32 Release"

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

!ELSEIF  "$(CFG)" == "kgmMath - Win32 Debug"

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
# ADD LIB32 /nologo /out:".\kgmMath.lib"

!ENDIF 

# Begin Target

# Name "kgmMath - Win32 Release"
# Name "kgmMath - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\kgmMath.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\kgmBase.h
# End Source File
# Begin Source File

SOURCE=.\kgmBox2d.h
# End Source File
# Begin Source File

SOURCE=.\kgmBox3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmCylinder3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmFrustum3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmLine3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmMath.h
# End Source File
# Begin Source File

SOURCE=.\kgmMatrix3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmPlane3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmPolygon3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmQuaternion.h
# End Source File
# Begin Source File

SOURCE=.\kgmRay3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmRect.h
# End Source File
# Begin Source File

SOURCE=.\kgmSphere3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmTrajectory3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmTriangle3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmVector2d.h
# End Source File
# Begin Source File

SOURCE=.\kgmVector3d.h
# End Source File
# Begin Source File

SOURCE=.\kgmVector4d.h
# End Source File
# End Group
# End Target
# End Project
