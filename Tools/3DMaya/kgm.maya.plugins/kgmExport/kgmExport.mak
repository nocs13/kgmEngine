# Microsoft Developer Studio Generated NMAKE File, Based on kgmExport.dsp
!IF "$(CFG)" == ""
CFG=kgmExport - Win32 Debug
!MESSAGE No configuration specified. Defaulting to kgmExport - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "kgmExport - Win32 Release" && "$(CFG)" != "kgmExport - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "kgmExport.mak" CFG="kgmExport - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "kgmExport - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "kgmExport - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kgmExport - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\kgmExport.mll"


CLEAN :
	-@erase "$(INTDIR)\kgmDialog.obj"
	-@erase "$(INTDIR)\kgmExport.obj"
	-@erase "$(INTDIR)\kgmExportSkeleton.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Script.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\kgmExport.exp"
	-@erase "$(OUTDIR)\kgmExport.lib"
	-@erase "$(OUTDIR)\kgmExport.mll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "KGMEXPORT_EXPORTS" /Fp"$(INTDIR)\kgmExport.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Script.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmExport.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\kgmExport.pdb" /machine:I386 /def:".\kgmExport.def" /out:"$(OUTDIR)\kgmExport.mll" /implib:"$(OUTDIR)\kgmExport.lib" 
DEF_FILE= \
	".\kgmExport.def"
LINK32_OBJS= \
	"$(INTDIR)\kgmExport.obj" \
	"$(INTDIR)\kgmExportSkeleton.obj" \
	"$(INTDIR)\kgmDialog.obj" \
	"$(INTDIR)\Script.res" \
	"$(INTDIR)\main.obj"

"$(OUTDIR)\kgmExport.mll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "kgmExport - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\kgmExport.dll"


CLEAN :
	-@erase "$(INTDIR)\kgmDialog.obj"
	-@erase "$(INTDIR)\kgmExport.obj"
	-@erase "$(INTDIR)\kgmExportSkeleton.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Script.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\kgmExport.dll"
	-@erase "$(OUTDIR)\kgmExport.exp"
	-@erase "$(OUTDIR)\kgmExport.ilk"
	-@erase "$(OUTDIR)\kgmExport.lib"
	-@erase "$(OUTDIR)\kgmExport.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "KGMEXPORT_EXPORTS" /Fp"$(INTDIR)\kgmExport.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Script.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmExport.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\kgmExport.pdb" /debug /machine:I386 /def:".\kgmExport.def" /out:"$(OUTDIR)\kgmExport.dll" /implib:"$(OUTDIR)\kgmExport.lib" /pdbtype:sept 
DEF_FILE= \
	".\kgmExport.def"
LINK32_OBJS= \
	"$(INTDIR)\kgmExport.obj" \
	"$(INTDIR)\kgmExportSkeleton.obj" \
	"$(INTDIR)\kgmDialog.obj" \
	"$(INTDIR)\Script.res" \
	"$(INTDIR)\main.obj"

"$(OUTDIR)\kgmExport.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
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
!IF EXISTS("kgmExport.dep")
!INCLUDE "kgmExport.dep"
!ELSE 
!MESSAGE Warning: cannot find "kgmExport.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "kgmExport - Win32 Release" || "$(CFG)" == "kgmExport - Win32 Debug"
SOURCE=.\kgmDialog.cpp

"$(INTDIR)\kgmDialog.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmExport.cpp

"$(INTDIR)\kgmExport.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kgmExportSkeleton.cpp

"$(INTDIR)\kgmExportSkeleton.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Script.rc

"$(INTDIR)\Script.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

