# Microsoft Developer Studio Generated NMAKE File, Based on kgmMain.dsp
!IF "$(CFG)" == ""
CFG=kgmMain - Win32 Debug
!MESSAGE No configuration specified. Defaulting to kgmMain - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "kgmMain - Win32 Release" && "$(CFG)" != "kgmMain - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kgmMain - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\kgmMain.lib"


CLEAN :
	-@erase "$(INTDIR)\kgmApp.obj"
	-@erase "$(INTDIR)\kgmArchive.obj"
	-@erase "$(INTDIR)\kgmFile.obj"
	-@erase "$(INTDIR)\kgmFileMemory.obj"
	-@erase "$(INTDIR)\kgmMain.obj"
	-@erase "$(INTDIR)\kgmMemory.obj"
	-@erase "$(INTDIR)\kgmObject.obj"
	-@erase "$(INTDIR)\kgmOctTree.obj"
	-@erase "$(INTDIR)\kgmResource.obj"
	-@erase "$(INTDIR)\kgmSystem.obj"
	-@erase "$(INTDIR)\kgmWindow.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\kgmMain.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\kgmMain.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmMain.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\kgmMain.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmArchive.obj" \
	"$(INTDIR)\kgmFile.obj" \
	"$(INTDIR)\kgmFileMemory.obj" \
	"$(INTDIR)\kgmMain.obj" \
	"$(INTDIR)\kgmMemory.obj" \
	"$(INTDIR)\kgmObject.obj" \
	"$(INTDIR)\kgmOctTree.obj" \
	"$(INTDIR)\kgmResource.obj" \
	"$(INTDIR)\kgmWindow.obj" \
	"$(INTDIR)\kgmApp.obj" \
	"$(INTDIR)\kgmSystem.obj"

"$(OUTDIR)\kgmMain.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "kgmMain - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\kgmMain.lib"


CLEAN :
	-@erase "$(INTDIR)\kgmApp.obj"
	-@erase "$(INTDIR)\kgmArchive.obj"
	-@erase "$(INTDIR)\kgmFile.obj"
	-@erase "$(INTDIR)\kgmFileMemory.obj"
	-@erase "$(INTDIR)\kgmMain.obj"
	-@erase "$(INTDIR)\kgmMemory.obj"
	-@erase "$(INTDIR)\kgmObject.obj"
	-@erase "$(INTDIR)\kgmOctTree.obj"
	-@erase "$(INTDIR)\kgmResource.obj"
	-@erase "$(INTDIR)\kgmSystem.obj"
	-@erase "$(INTDIR)\kgmWindow.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\kgmMain.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\kgmMain.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmMain.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:".\kgmMain.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmArchive.obj" \
	"$(INTDIR)\kgmFile.obj" \
	"$(INTDIR)\kgmFileMemory.obj" \
	"$(INTDIR)\kgmMain.obj" \
	"$(INTDIR)\kgmMemory.obj" \
	"$(INTDIR)\kgmObject.obj" \
	"$(INTDIR)\kgmOctTree.obj" \
	"$(INTDIR)\kgmResource.obj" \
	"$(INTDIR)\kgmWindow.obj" \
	"$(INTDIR)\kgmApp.obj" \
	"$(INTDIR)\kgmSystem.obj"

".\kgmMain.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("kgmMain.dep")
!INCLUDE "kgmMain.dep"
!ELSE 
!MESSAGE Warning: cannot find "kgmMain.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "kgmMain - Win32 Release" || "$(CFG)" == "kgmMain - Win32 Debug"
SOURCE=.\kgmApp.cpp

"$(INTDIR)\kgmApp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmArchive.cpp

"$(INTDIR)\kgmArchive.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmFile.cpp

"$(INTDIR)\kgmFile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmFileMemory.cpp

"$(INTDIR)\kgmFileMemory.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmMain.cpp

"$(INTDIR)\kgmMain.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmMemory.cpp

"$(INTDIR)\kgmMemory.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmObject.cpp

"$(INTDIR)\kgmObject.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmOctTree.cpp

"$(INTDIR)\kgmOctTree.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmResource.cpp

"$(INTDIR)\kgmResource.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmSystem.cpp

"$(INTDIR)\kgmSystem.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmWindow.cpp

"$(INTDIR)\kgmWindow.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

