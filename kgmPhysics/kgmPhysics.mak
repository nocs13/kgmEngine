# Microsoft Developer Studio Generated NMAKE File, Based on kgmPhysics.dsp
!IF "$(CFG)" == ""
CFG=kgmPhysics - Win32 Debug
!MESSAGE No configuration specified. Defaulting to kgmPhysics - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "kgmPhysics - Win32 Release" && "$(CFG)" != "kgmPhysics - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "kgmPhysics.mak" CFG="kgmPhysics - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "kgmPhysics - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "kgmPhysics - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "kgmPhysics - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\kgmPhysics.lib"


CLEAN :
	-@erase "$(INTDIR)\kgmCollision.obj"
	-@erase "$(INTDIR)\kgmIntersection.obj"
	-@erase "$(INTDIR)\kgmPhysBody.obj"
	-@erase "$(INTDIR)\kgmPhysics.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\kgmPhysics.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\kgmPhysics.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmPhysics.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\kgmPhysics.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmCollision.obj" \
	"$(INTDIR)\kgmIntersection.obj" \
	"$(INTDIR)\kgmPhysBody.obj" \
	"$(INTDIR)\kgmPhysics.obj"

"$(OUTDIR)\kgmPhysics.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "kgmPhysics - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\kgmPhysics.lib"


CLEAN :
	-@erase "$(INTDIR)\kgmCollision.obj"
	-@erase "$(INTDIR)\kgmIntersection.obj"
	-@erase "$(INTDIR)\kgmPhysBody.obj"
	-@erase "$(INTDIR)\kgmPhysics.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\kgmPhysics.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\kgmPhysics.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmPhysics.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:".\kgmPhysics.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmCollision.obj" \
	"$(INTDIR)\kgmIntersection.obj" \
	"$(INTDIR)\kgmPhysBody.obj" \
	"$(INTDIR)\kgmPhysics.obj"

".\kgmPhysics.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("kgmPhysics.dep")
!INCLUDE "kgmPhysics.dep"
!ELSE 
!MESSAGE Warning: cannot find "kgmPhysics.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "kgmPhysics - Win32 Release" || "$(CFG)" == "kgmPhysics - Win32 Debug"
SOURCE=.\kgmCollision.cpp

"$(INTDIR)\kgmCollision.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmIntersection.cpp

"$(INTDIR)\kgmIntersection.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmPhysBody.cpp

"$(INTDIR)\kgmPhysBody.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmPhysics.cpp

"$(INTDIR)\kgmPhysics.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

