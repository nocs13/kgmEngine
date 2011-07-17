# Microsoft Developer Studio Generated NMAKE File, Based on kgmRender.dsp
!IF "$(CFG)" == ""
CFG=kgmRender - Win32 Debug
!MESSAGE No configuration specified. Defaulting to kgmRender - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "kgmRender - Win32 Release" && "$(CFG)" != "kgmRender - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "kgmRender.mak" CFG="kgmRender - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "kgmRender - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "kgmRender - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "kgmRender - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\kgmRender.lib"


CLEAN :
	-@erase "$(INTDIR)\kgmMaterial.obj"
	-@erase "$(INTDIR)\kgmMesh.obj"
	-@erase "$(INTDIR)\kgmRender.obj"
	-@erase "$(INTDIR)\kgmShader.obj"
	-@erase "$(INTDIR)\kgmTerrain.obj"
	-@erase "$(INTDIR)\kgmTexture.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\kgmRender.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\kgmRender.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmRender.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\kgmRender.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmMaterial.obj" \
	"$(INTDIR)\kgmMesh.obj" \
	"$(INTDIR)\kgmRender.obj" \
	"$(INTDIR)\kgmShader.obj" \
	"$(INTDIR)\kgmTerrain.obj" \
	"$(INTDIR)\kgmTexture.obj"

"$(OUTDIR)\kgmRender.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "kgmRender - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\kgmRender.lib"


CLEAN :
	-@erase "$(INTDIR)\kgmMaterial.obj"
	-@erase "$(INTDIR)\kgmMesh.obj"
	-@erase "$(INTDIR)\kgmRender.obj"
	-@erase "$(INTDIR)\kgmShader.obj"
	-@erase "$(INTDIR)\kgmTerrain.obj"
	-@erase "$(INTDIR)\kgmTexture.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\kgmRender.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\kgmRender.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmRender.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:".\kgmRender.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmMaterial.obj" \
	"$(INTDIR)\kgmMesh.obj" \
	"$(INTDIR)\kgmRender.obj" \
	"$(INTDIR)\kgmShader.obj" \
	"$(INTDIR)\kgmTerrain.obj" \
	"$(INTDIR)\kgmTexture.obj"

".\kgmRender.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("kgmRender.dep")
!INCLUDE "kgmRender.dep"
!ELSE 
!MESSAGE Warning: cannot find "kgmRender.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "kgmRender - Win32 Release" || "$(CFG)" == "kgmRender - Win32 Debug"
SOURCE=.\kgmMaterial.cpp

"$(INTDIR)\kgmMaterial.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmMesh.cpp

"$(INTDIR)\kgmMesh.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmRender.cpp

"$(INTDIR)\kgmRender.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmShader.cpp

"$(INTDIR)\kgmShader.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmTerrain.cpp

"$(INTDIR)\kgmTerrain.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmTexture.cpp

"$(INTDIR)\kgmTexture.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

