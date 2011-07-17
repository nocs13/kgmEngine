# Microsoft Developer Studio Generated NMAKE File, Based on kgmMath.dsp
!IF "$(CFG)" == ""
CFG=kgmMath - Win32 Debug
!MESSAGE No configuration specified. Defaulting to kgmMath - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "kgmMath - Win32 Release" && "$(CFG)" != "kgmMath - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kgmMath - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\kgmMath.lib"


CLEAN :
	-@erase "$(INTDIR)\kgmMath.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\kgmMath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\kgmMath.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmMath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\kgmMath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmMath.obj"

"$(OUTDIR)\kgmMath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "kgmMath - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\kgmMath.lib"


CLEAN :
	-@erase "$(INTDIR)\kgmMath.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\kgmMath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\kgmMath.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmMath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:".\kgmMath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmMath.obj"

".\kgmMath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("kgmMath.dep")
!INCLUDE "kgmMath.dep"
!ELSE 
!MESSAGE Warning: cannot find "kgmMath.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "kgmMath - Win32 Release" || "$(CFG)" == "kgmMath - Win32 Debug"
SOURCE=.\kgmMath.cpp

"$(INTDIR)\kgmMath.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

