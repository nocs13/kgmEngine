# Microsoft Developer Studio Generated NMAKE File, Based on kgmGame.dsp
!IF "$(CFG)" == ""
CFG=kgmGame - Win32 Debug
!MESSAGE No configuration specified. Defaulting to kgmGame - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "kgmGame - Win32 Release" && "$(CFG)" != "kgmGame - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kgmGame - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\kgmGame.lib"


CLEAN :
	-@erase "$(INTDIR)\kgmActor.obj"
	-@erase "$(INTDIR)\kgmAI.obj"
	-@erase "$(INTDIR)\kgmBarrier.obj"
	-@erase "$(INTDIR)\kgmBomb.obj"
	-@erase "$(INTDIR)\kgmCase.obj"
	-@erase "$(INTDIR)\kgmController.obj"
	-@erase "$(INTDIR)\kgmDeformer.obj"
	-@erase "$(INTDIR)\kgmDestruction.obj"
	-@erase "$(INTDIR)\kgmDoor.obj"
	-@erase "$(INTDIR)\kgmEffect.obj"
	-@erase "$(INTDIR)\kgmGameAI.obj"
	-@erase "$(INTDIR)\kgmGameApp.obj"
	-@erase "$(INTDIR)\kgmGameBase.obj"
	-@erase "$(INTDIR)\kgmGameGraphics.obj"
	-@erase "$(INTDIR)\kgmGameLogic.obj"
	-@erase "$(INTDIR)\kgmGamePhysics.obj"
	-@erase "$(INTDIR)\kgmGameResources.obj"
	-@erase "$(INTDIR)\kgmGameSpawner.obj"
	-@erase "$(INTDIR)\kgmGameWindow.obj"
	-@erase "$(INTDIR)\kgmInventory.obj"
	-@erase "$(INTDIR)\kgmItem.obj"
	-@erase "$(INTDIR)\kgmKey.obj"
	-@erase "$(INTDIR)\kgmLock.obj"
	-@erase "$(INTDIR)\kgmParticles.obj"
	-@erase "$(INTDIR)\kgmPawn.obj"
	-@erase "$(INTDIR)\kgmSky.obj"
	-@erase "$(INTDIR)\kgmSurface.obj"
	-@erase "$(INTDIR)\kgmSwitch.obj"
	-@erase "$(INTDIR)\kgmTransporter.obj"
	-@erase "$(INTDIR)\kgmTrigger.obj"
	-@erase "$(INTDIR)\kgmWeapon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\kgmGame.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\kgmGame.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmGame.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\kgmGame.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmActor.obj" \
	"$(INTDIR)\kgmAI.obj" \
	"$(INTDIR)\kgmBarrier.obj" \
	"$(INTDIR)\kgmBomb.obj" \
	"$(INTDIR)\kgmCase.obj" \
	"$(INTDIR)\kgmController.obj" \
	"$(INTDIR)\kgmDestruction.obj" \
	"$(INTDIR)\kgmDoor.obj" \
	"$(INTDIR)\kgmEffect.obj" \
	"$(INTDIR)\kgmGameAI.obj" \
	"$(INTDIR)\kgmGameApp.obj" \
	"$(INTDIR)\kgmGameBase.obj" \
	"$(INTDIR)\kgmGameGraphics.obj" \
	"$(INTDIR)\kgmGameLogic.obj" \
	"$(INTDIR)\kgmGamePhysics.obj" \
	"$(INTDIR)\kgmGameResources.obj" \
	"$(INTDIR)\kgmGameSpawner.obj" \
	"$(INTDIR)\kgmGameWindow.obj" \
	"$(INTDIR)\kgmInventory.obj" \
	"$(INTDIR)\kgmItem.obj" \
	"$(INTDIR)\kgmKey.obj" \
	"$(INTDIR)\kgmLock.obj" \
	"$(INTDIR)\kgmParticles.obj" \
	"$(INTDIR)\kgmPawn.obj" \
	"$(INTDIR)\kgmSky.obj" \
	"$(INTDIR)\kgmSurface.obj" \
	"$(INTDIR)\kgmSwitch.obj" \
	"$(INTDIR)\kgmTransporter.obj" \
	"$(INTDIR)\kgmTrigger.obj" \
	"$(INTDIR)\kgmWeapon.obj" \
	"$(INTDIR)\kgmDeformer.obj"

"$(OUTDIR)\kgmGame.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\kgmGame.lib" "$(OUTDIR)\kgmGame.bsc"


CLEAN :
	-@erase "$(INTDIR)\kgmActor.obj"
	-@erase "$(INTDIR)\kgmActor.sbr"
	-@erase "$(INTDIR)\kgmAI.obj"
	-@erase "$(INTDIR)\kgmAI.sbr"
	-@erase "$(INTDIR)\kgmBarrier.obj"
	-@erase "$(INTDIR)\kgmBarrier.sbr"
	-@erase "$(INTDIR)\kgmBomb.obj"
	-@erase "$(INTDIR)\kgmBomb.sbr"
	-@erase "$(INTDIR)\kgmCase.obj"
	-@erase "$(INTDIR)\kgmCase.sbr"
	-@erase "$(INTDIR)\kgmController.obj"
	-@erase "$(INTDIR)\kgmController.sbr"
	-@erase "$(INTDIR)\kgmDeformer.obj"
	-@erase "$(INTDIR)\kgmDeformer.sbr"
	-@erase "$(INTDIR)\kgmDestruction.obj"
	-@erase "$(INTDIR)\kgmDestruction.sbr"
	-@erase "$(INTDIR)\kgmDoor.obj"
	-@erase "$(INTDIR)\kgmDoor.sbr"
	-@erase "$(INTDIR)\kgmEffect.obj"
	-@erase "$(INTDIR)\kgmEffect.sbr"
	-@erase "$(INTDIR)\kgmGameAI.obj"
	-@erase "$(INTDIR)\kgmGameAI.sbr"
	-@erase "$(INTDIR)\kgmGameApp.obj"
	-@erase "$(INTDIR)\kgmGameApp.sbr"
	-@erase "$(INTDIR)\kgmGameBase.obj"
	-@erase "$(INTDIR)\kgmGameBase.sbr"
	-@erase "$(INTDIR)\kgmGameGraphics.obj"
	-@erase "$(INTDIR)\kgmGameGraphics.sbr"
	-@erase "$(INTDIR)\kgmGameLogic.obj"
	-@erase "$(INTDIR)\kgmGameLogic.sbr"
	-@erase "$(INTDIR)\kgmGamePhysics.obj"
	-@erase "$(INTDIR)\kgmGamePhysics.sbr"
	-@erase "$(INTDIR)\kgmGameResources.obj"
	-@erase "$(INTDIR)\kgmGameResources.sbr"
	-@erase "$(INTDIR)\kgmGameSpawner.obj"
	-@erase "$(INTDIR)\kgmGameSpawner.sbr"
	-@erase "$(INTDIR)\kgmGameWindow.obj"
	-@erase "$(INTDIR)\kgmGameWindow.sbr"
	-@erase "$(INTDIR)\kgmInventory.obj"
	-@erase "$(INTDIR)\kgmInventory.sbr"
	-@erase "$(INTDIR)\kgmItem.obj"
	-@erase "$(INTDIR)\kgmItem.sbr"
	-@erase "$(INTDIR)\kgmKey.obj"
	-@erase "$(INTDIR)\kgmKey.sbr"
	-@erase "$(INTDIR)\kgmLock.obj"
	-@erase "$(INTDIR)\kgmLock.sbr"
	-@erase "$(INTDIR)\kgmParticles.obj"
	-@erase "$(INTDIR)\kgmParticles.sbr"
	-@erase "$(INTDIR)\kgmPawn.obj"
	-@erase "$(INTDIR)\kgmPawn.sbr"
	-@erase "$(INTDIR)\kgmSky.obj"
	-@erase "$(INTDIR)\kgmSky.sbr"
	-@erase "$(INTDIR)\kgmSurface.obj"
	-@erase "$(INTDIR)\kgmSurface.sbr"
	-@erase "$(INTDIR)\kgmSwitch.obj"
	-@erase "$(INTDIR)\kgmSwitch.sbr"
	-@erase "$(INTDIR)\kgmTransporter.obj"
	-@erase "$(INTDIR)\kgmTransporter.sbr"
	-@erase "$(INTDIR)\kgmTrigger.obj"
	-@erase "$(INTDIR)\kgmTrigger.sbr"
	-@erase "$(INTDIR)\kgmWeapon.obj"
	-@erase "$(INTDIR)\kgmWeapon.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\kgmGame.bsc"
	-@erase ".\kgmGame.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\kgmGame.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\kgmGame.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\kgmActor.sbr" \
	"$(INTDIR)\kgmAI.sbr" \
	"$(INTDIR)\kgmBarrier.sbr" \
	"$(INTDIR)\kgmBomb.sbr" \
	"$(INTDIR)\kgmCase.sbr" \
	"$(INTDIR)\kgmController.sbr" \
	"$(INTDIR)\kgmDestruction.sbr" \
	"$(INTDIR)\kgmDoor.sbr" \
	"$(INTDIR)\kgmEffect.sbr" \
	"$(INTDIR)\kgmGameAI.sbr" \
	"$(INTDIR)\kgmGameApp.sbr" \
	"$(INTDIR)\kgmGameBase.sbr" \
	"$(INTDIR)\kgmGameGraphics.sbr" \
	"$(INTDIR)\kgmGameLogic.sbr" \
	"$(INTDIR)\kgmGamePhysics.sbr" \
	"$(INTDIR)\kgmGameResources.sbr" \
	"$(INTDIR)\kgmGameSpawner.sbr" \
	"$(INTDIR)\kgmGameWindow.sbr" \
	"$(INTDIR)\kgmInventory.sbr" \
	"$(INTDIR)\kgmItem.sbr" \
	"$(INTDIR)\kgmKey.sbr" \
	"$(INTDIR)\kgmLock.sbr" \
	"$(INTDIR)\kgmParticles.sbr" \
	"$(INTDIR)\kgmPawn.sbr" \
	"$(INTDIR)\kgmSky.sbr" \
	"$(INTDIR)\kgmSurface.sbr" \
	"$(INTDIR)\kgmSwitch.sbr" \
	"$(INTDIR)\kgmTransporter.sbr" \
	"$(INTDIR)\kgmTrigger.sbr" \
	"$(INTDIR)\kgmWeapon.sbr" \
	"$(INTDIR)\kgmDeformer.sbr"

"$(OUTDIR)\kgmGame.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:".\kgmGame.lib" 
LIB32_OBJS= \
	"$(INTDIR)\kgmActor.obj" \
	"$(INTDIR)\kgmAI.obj" \
	"$(INTDIR)\kgmBarrier.obj" \
	"$(INTDIR)\kgmBomb.obj" \
	"$(INTDIR)\kgmCase.obj" \
	"$(INTDIR)\kgmController.obj" \
	"$(INTDIR)\kgmDestruction.obj" \
	"$(INTDIR)\kgmDoor.obj" \
	"$(INTDIR)\kgmEffect.obj" \
	"$(INTDIR)\kgmGameAI.obj" \
	"$(INTDIR)\kgmGameApp.obj" \
	"$(INTDIR)\kgmGameBase.obj" \
	"$(INTDIR)\kgmGameGraphics.obj" \
	"$(INTDIR)\kgmGameLogic.obj" \
	"$(INTDIR)\kgmGamePhysics.obj" \
	"$(INTDIR)\kgmGameResources.obj" \
	"$(INTDIR)\kgmGameSpawner.obj" \
	"$(INTDIR)\kgmGameWindow.obj" \
	"$(INTDIR)\kgmInventory.obj" \
	"$(INTDIR)\kgmItem.obj" \
	"$(INTDIR)\kgmKey.obj" \
	"$(INTDIR)\kgmLock.obj" \
	"$(INTDIR)\kgmParticles.obj" \
	"$(INTDIR)\kgmPawn.obj" \
	"$(INTDIR)\kgmSky.obj" \
	"$(INTDIR)\kgmSurface.obj" \
	"$(INTDIR)\kgmSwitch.obj" \
	"$(INTDIR)\kgmTransporter.obj" \
	"$(INTDIR)\kgmTrigger.obj" \
	"$(INTDIR)\kgmWeapon.obj" \
	"$(INTDIR)\kgmDeformer.obj"

".\kgmGame.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("kgmGame.dep")
!INCLUDE "kgmGame.dep"
!ELSE 
!MESSAGE Warning: cannot find "kgmGame.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "kgmGame - Win32 Release" || "$(CFG)" == "kgmGame - Win32 Debug"
SOURCE=.\kgmActor.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmActor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmActor.obj"	"$(INTDIR)\kgmActor.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmAI.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmAI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmAI.obj"	"$(INTDIR)\kgmAI.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmBarrier.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmBarrier.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmBarrier.obj"	"$(INTDIR)\kgmBarrier.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmBomb.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmBomb.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmBomb.obj"	"$(INTDIR)\kgmBomb.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmCase.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmCase.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmCase.obj"	"$(INTDIR)\kgmCase.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmController.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmController.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmController.obj"	"$(INTDIR)\kgmController.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmDeformer.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmDeformer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmDeformer.obj"	"$(INTDIR)\kgmDeformer.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmDestruction.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmDestruction.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmDestruction.obj"	"$(INTDIR)\kgmDestruction.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmDoor.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmDoor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmDoor.obj"	"$(INTDIR)\kgmDoor.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmEffect.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmEffect.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmEffect.obj"	"$(INTDIR)\kgmEffect.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmGameAI.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmGameAI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmGameAI.obj"	"$(INTDIR)\kgmGameAI.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmGameApp.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmGameApp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmGameApp.obj"	"$(INTDIR)\kgmGameApp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmGameBase.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmGameBase.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmGameBase.obj"	"$(INTDIR)\kgmGameBase.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmGameGraphics.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmGameGraphics.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmGameGraphics.obj"	"$(INTDIR)\kgmGameGraphics.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmGameLogic.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmGameLogic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmGameLogic.obj"	"$(INTDIR)\kgmGameLogic.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmGamePhysics.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmGamePhysics.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmGamePhysics.obj"	"$(INTDIR)\kgmGamePhysics.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmGameResources.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmGameResources.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmGameResources.obj"	"$(INTDIR)\kgmGameResources.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmGameSpawner.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmGameSpawner.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmGameSpawner.obj"	"$(INTDIR)\kgmGameSpawner.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmGameWindow.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmGameWindow.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmGameWindow.obj"	"$(INTDIR)\kgmGameWindow.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmInventory.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmInventory.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmInventory.obj"	"$(INTDIR)\kgmInventory.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmItem.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmItem.obj"	"$(INTDIR)\kgmItem.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmKey.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmKey.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmKey.obj"	"$(INTDIR)\kgmKey.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmLock.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmLock.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmLock.obj"	"$(INTDIR)\kgmLock.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmParticles.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmParticles.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmParticles.obj"	"$(INTDIR)\kgmParticles.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmPawn.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmPawn.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmPawn.obj"	"$(INTDIR)\kgmPawn.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmSky.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmSky.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmSky.obj"	"$(INTDIR)\kgmSky.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmSurface.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmSurface.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmSurface.obj"	"$(INTDIR)\kgmSurface.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmSwitch.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmSwitch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmSwitch.obj"	"$(INTDIR)\kgmSwitch.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmTransporter.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmTransporter.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmTransporter.obj"	"$(INTDIR)\kgmTransporter.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmTrigger.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmTrigger.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmTrigger.obj"	"$(INTDIR)\kgmTrigger.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kgmWeapon.cpp

!IF  "$(CFG)" == "kgmGame - Win32 Release"


"$(INTDIR)\kgmWeapon.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "kgmGame - Win32 Debug"


"$(INTDIR)\kgmWeapon.obj"	"$(INTDIR)\kgmWeapon.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

