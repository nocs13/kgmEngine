#pragma once
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h> 
#include <vector>
#include <iostream>

#define _BOOL bool

#include <maya/MGlobal.h> 
#include <maya/MObject.h> 
#include <maya/MString.h> 
#include <maya/MVector.h> 
#include <maya/MMatrix.h> 
#include <maya/MPoint.h> 
#include <maya/MQuaternion.h> 
#include <maya/MEulerRotation.h> 
#include <maya/MObjectArray.h> 
#include <maya/MStringArray.h> 
#include <maya/MPointArray.h> 
#include <maya/MFloatArray.h> 
#include <maya/MIntArray.h> 
#include <maya/MDagPathArray.h> 
#include <maya/MArgList.h> 
#include <maya/MPxCommand.h> 
#include <maya/MIOStream.h> 
#include <maya/MPxFileTranslator.h> 
#include <maya/MDagPath.h> 
#include <maya/MItDag.h> 
#include <maya/MItGeometry.h> 
#include <maya/MItDependencyNodes.h> 
#include <maya/MItDependencyGraph.h> 
#include <maya/MItSelectionList.h> 
#include <maya/MPlugArray.h> 
#include <maya/MFn.h> 
#include <maya/MFnDagNode.h> 
#include <maya/MFnMesh.h> 
#include <maya/MFnTransform.h> 
#include <maya/MFnSkinCluster.h> 
#include <maya/MFnAttribute.h> 
#include <maya/MSelectionList.h> 
#include <maya/MFnBlinnShader.h> 
#include <maya/MFnLambertShader.h> 
#include <maya/MMaterial.h> 
#include <maya/MAnimControl.h> 
#include <maya/MTime.h> 
#include <maya/MFnPlugin.h> 


#ifdef WIN32
extern HINSTANCE hInstance;
#endif

//#ifdef WIN32
//#define Msg(x) MessageBox(0, (x), "kgmExport", 0)
#define Msg(x) cout << (x) << endl
//#endif

//   Structs
typedef struct{ 
 MVector v;		//vertex
 MVector n;		//normal
 MVector uv0;	//texcoord 1 - 4
 MVector uv1;
 MVector uv2;
 MVector uv3;	 
 int	iraw;		//raw index in real mesh
 char  ibones[4];	//affected joints indices
 float wbones[4];	//affected joints weights
} kVertex;
typedef struct{
 int f[3];
} kFace;

typedef struct{
 
} kMesh;

typedef struct{
 char  name[64];
 char  nparent[64];
 int   parent;
 float pos[3];
 float rot[4];
} kJoint;

typedef struct{
 char name[64]; //name id
 int a[4];	//ambient
 int d[4];	//diffuse
 int e[4];	//emmision
 int s[4];	//specular
 int o;		//opacity
 int sh;	//shininess

 char mc[64];	//map color file
 char mn[64];	//map normal file
} kMaterial;

typedef struct{
 int frame;
 float pos[3];
 float rot[4];
} kFrame;


//Functions
bool kgmExportAnimation(MString& path);
bool kgmExportSkeleton(MString& path);
bool kgmExportGeometry(MString& path);
bool kgmExportPhysics(MString& path);
bool kgmExportMesh(MString& path);

bool kgmImportSkeleton(MString& path);
