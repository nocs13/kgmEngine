#include "main.h"

#ifdef WIN32
#pragma comment(lib, "Foundation.lib")
#pragma comment(lib, "OpenMaya.lib")
#pragma comment(lib, "OpenMayaFX.lib")
#pragma comment(lib, "OpenMayaUI.lib")
#pragma comment(lib, "OpenMayaAnim.lib")
#pragma comment(lib, "OpenMayaRender.lib")
#endif


class kgmExport : public MPxCommand 
{ 
public: 
    MStatus        doIt( const MArgList& args ); 
    static void*   creator(); 
}; 
MStatus kgmExport::doIt( const MArgList& args ) { 
 	MString s;
	s = "-a";
	if(s == args.asString(0)){
		s = args.asString(1);
		kgmExportAnimation(s);
	}
	s = "-g";
	if(s == args.asString(0)){
		s = args.asString(1);
		kgmExportGeometry(s);
	}
	s = "-s";
	if(s == args.asString(0)){
		s = args.asString(1);
		kgmExportSkeleton(s);
	}
	s = "-p";
	if(s == args.asString(0)){
		s = args.asString(1);
		kgmExportPhysics(s);
	}
	s = "-m";
	if(s == args.asString(0)){
		s = args.asString(1);
		kgmExportMesh(s);
	}
    return MS::kSuccess; 
} 
void* kgmExport::creator() { 
    return new kgmExport(); 
} 


class kgmImport : public MPxCommand 
{ 
public: 
    MStatus        doIt( const MArgList& args ); 
    static void*   creator(); 
}; 
MStatus kgmImport::doIt( const MArgList& args ) { 
 	MString s;
	s = "-a";
	if(s == args.asString(0)){
		s = args.asString(1);
//		kgmExportAnimation(s);
	}
	s = "-g";
	if(s == args.asString(0)){
		s = args.asString(1);
//		kgmExportGeometry(s);
	}
	s = "-s";
	if(s == args.asString(0)){
		MessageBox(0, args.asString(1).asChar(), args.asString(0).asChar(), 0);
		s = args.asString(1);
		kgmImportSkeleton(s);
	}
	s = "-p";
	if(s == args.asString(0)){
		s = args.asString(1);
//		kgmExportPhysics(s);
	}
	s = "-m";
	if(s == args.asString(0)){
		s = args.asString(1);
//		kgmExportMesh(s);
	}
    return MS::kSuccess; 
} 
void* kgmImport::creator() { 
    return new kgmImport(); 
} 


MStatus 
#ifdef WIN32
__declspec(dllexport) 
#endif
initializePlugin( MObject obj ) { 
    MFnPlugin plugin( obj, "karakal_13", "1.0", "Any" ); 
    plugin.registerCommand( "kgmexport", kgmExport::creator ); 
    plugin.registerCommand( "kgmimport", kgmImport::creator ); 
    return MS::kSuccess; 
} 

MStatus 
#ifdef WIN32
__declspec(dllexport) 
#endif
uninitializePlugin( MObject obj ) { 
    MFnPlugin plugin( obj ); 
    plugin.deregisterCommand( "kgmabout" ); 
    return MS::kSuccess; 
} 


#ifdef WIN32
HINSTANCE hInstance = NULL;
BOOL CALLBACK DllMain(HANDLE handle, DWORD, LPVOID){
 hInstance = (HINSTANCE)handle;
 return TRUE;
}
#endif

//////////////////////////////////////////////
/////////////////////////////////////////////
////////////////////////////////////////////
//////////////////////////////////////////
////////////////////////////////////////
typedef std::vector<kVertex>	kVertexList;
typedef std::vector<kJoint>		kJointList;
typedef std::vector<kFrame>		kFrameList;

std::vector<kMaterial>			gMaterials;
std::vector<kJoint>				gJoints;
/*
bool kgmFileDialog(bool save = true, char* ext = "*.*\0\0", char* path = 0){
#ifdef WIN32
 OPENFILENAME ofn = {0};

 ofn.lStructSize = sizeof(OPENFILENAME);
 ofn.Flags = OFN_EXPLORER;
 ofn.hInstance = hInstance;
 ofn.lpstrFile = path;
 ofn.lpstrFilter = "All Files\0*.*\0\0";
// ofn.lpstrDefExt = ext;
 ofn.nMaxFile = 128;

 if((save == true)  && (GetSaveFileName(&ofn) == FALSE))
	 return false;
 if((save == false) && (GetOpenFileName(&ofn) == FALSE))
	 return false;
#endif
 return true;
}
*/

MObject kgmRootJoint()
{
 MItDag it; //Iterator
 for(; !it.isDone(); it.next()){
  if(it.depth() != 1)
	  continue;
  MObject o = it.item();	
  if(!o.hasFn(MFn::kJoint)){
   MFnDagNode fn(o);
   MDagPath path;
   fn.getPath(path);
  }else{
   return o;
  }
 }
 return MObject::kNullObj;
}

//SKELETON
int kgmCollectSkeleton(){
 MItDag				it;
 MFnTransform       fnTran;
 MStatus			stat;

 gJoints.clear();
 for(it.reset(); !it.isDone(); it.next()){
  MObject o(it.item());
  MObject oparent;
  if(!o.hasFn(MFn::kJoint))
	  continue;
   MFloatArray weights;
   MDagPath	   path;
   MSelectionList sels;
   MObject comps;
   MFnTransform fnBone;
   MTransformationMatrix mat;
   kJoint		joint = {0};
   memset(&joint, 0, sizeof(kJoint));
   
   if(!o.hasFn(MFn::kTransform))
	   continue;
   stat = fnBone.setObject(o);
   if(!stat)
	   continue;
   it.getPath(path);


   //mat = fnBone.transformation(&stat);
   mat = path.inclusiveMatrix();
   MVector pos = mat.getTranslation(MSpace::kWorld, &stat);
//   MQuaternion rot = mat.rotation();
   MMatrix mrot = mat.asRotateMatrix();
   MEulerRotation er = mat.eulerRotation();
   double rx = er.x, ry = er.y, rz = er.z;
   er = MEulerRotation(rx, ry, rz);
   MQuaternion rot = er.asQuaternion();//mat.eulerRotation().asQuaternion();
   joint.pos[0] = pos.x, joint.pos[1] = pos.y, joint.pos[2] = pos.z;
   joint.rot[0] = rot.x, joint.rot[1] = rot.y, joint.rot[2] = rot.z, joint.rot[3] = rot.w;
   strcpy(joint.name, fnBone.name().asChar());

   if((fnBone.parentCount() > 0) && (fnBone.parent(0).hasFn(MFn::kTransform))){
    MFnTransform fnPBone;
    stat = fnPBone.setObject(fnBone.parent(0));
    if(stat)
     strcpy(joint.nparent, fnPBone.name().asChar());
   }
   joint.parent = -1;
   gJoints.push_back(joint);
 }

 for(int i = 0; i < gJoints.size(); i++){
  for(int j = 0; j < gJoints.size(); j++){
   if(!strcmp(gJoints[i].nparent, gJoints[j].name)){
    gJoints[i].parent = j;
	break;
   }
  }
 }
 return gJoints.size();
}
//EXPORT SKELETON
bool kgmExportSkeleton(MString& path){
 gJoints.clear();
 if( 1 > kgmCollectSkeleton())
	 return false;

 FILE* file = fopen(path.asChar(), "w");
 if(!file){
  gJoints.clear();
  return false;
 }

 for(int i =0; i < gJoints.size(); i++){
  fprintf(file, "Bone %s\n", gJoints[i].name);
  fprintf(file, "\t\t%i %.5f %.5f %.5f %.5f %.5f %.5f %.5f\n",
		  gJoints[i].parent,	
	      gJoints[i].pos[0], gJoints[i].pos[1], gJoints[i].pos[2],
		  gJoints[i].rot[0], gJoints[i].rot[1], gJoints[i].rot[2], gJoints[i].rot[3]);
 }
 fclose(file);
 gJoints.clear();
 return true;
}

bool kgmImportSkeleton(MString& path){
 std::vector<kJoint> joints;
 FILE* file = fopen(path.asChar(), "r");
 if(!file){
  return false;
 }
/*
 for(int i =0; i < gJoints.size(); i++){
  fprintf(file, "Bone %s\n", gJoints[i].name);
  fprintf(file, "\t\t%i %.5f %.5f %.5f %.5f %.5f %.5f %.5f\n",
		  gJoints[i].parent,	
	      gJoints[i].pos[0], gJoints[i].pos[1], gJoints[i].pos[2],
		  gJoints[i].rot[0], gJoints[i].rot[1], gJoints[i].rot[2], gJoints[i].rot[3]);
 }
*/
 char* buf = new char[1024];
 while(!feof(file))
 {
  memset(buf, 0, 1024);
  fgets(buf, 1024, file);

  if(feof(file) || ferror(file))
	  break;

  kJoint joint = {0};
  char* p = strchr(buf, '\n'); if(p) *p = '\0';
        p = strchr(buf, '\r'); if(p) *p = '\0';
  
  char  key[64] = {0};
  sscanf(buf, "%s", key);

  if(!strcmp("Bone", key)){
   strcpy(joint.name, &buf[5]);
   for(int i = 0; i < strlen(joint.name); i++){
    if(joint.name[i] == ' ') joint.name[i] = '_';
   }
  }

  memset(buf, 0, 1024);
  fgets(buf, 1024, file);

  sscanf(buf, "%i %f %f %f %f %f %f %f",
		 &joint.parent,	
	     &joint.pos[0], &joint.pos[1], &joint.pos[2],
		 &joint.rot[0], &joint.rot[1], &joint.rot[2], &joint.rot[3]);
  joints.push_back(joint);
 }
 delete [] buf;
 fclose(file);

 int i;
 for(i = 0; i < joints.size(); i++){
  MQuaternion quat(joints[i].rot[0], joints[i].rot[1], joints[i].rot[2], joints[i].rot[3]);
  MVector	  vec(joints[i].pos[0], joints[i].pos[1], joints[i].pos[2]);
  quat.invertIt();
  MEulerRotation eul = quat.asEulerRotation();
  float rx = -1.0f * eul.x * 180.0 / 3.1415;
  float ry = eul.y * 180.0 / 3.1415;
  float rz = eul.z * 180.0 / 3.1415;
  if(joints[i].parent < 0){
   MGlobal::executeCommand(MString("joint -absolute -radius 0.2 -position ") + 
						   vec.x + " " + vec.y + " " + vec.z + " -angleX " +
						   rx + " -angleY " + ry + " -angleZ " + rz + " -name " + joints[i].name);
  
  }else{
   int parent = joints[i].parent;
   MGlobal::executeCommand(MString("select ") + joints[parent].name);
//   MGlobal::executeCommand(MString("joint -absolute -radius 0.2 -position ") + 
//						   vec.x + " " + vec.y + " " + vec.z + " -angleX " +
//						   rx + " -angleY " + ry + " -angleZ " + rz + " -name " + joints[i].name);
   MGlobal::executeCommand(MString("joint -absolute -radius 0.2 -position ") + 
						   vec.x + " " + vec.y + " " + vec.z + 
						   " -rotateQuaternionX " + quat.x + 
						   " -rotateQuaternionY " + quat.y + 
						   " -rotateQuaternionZ " + quat.z + 
						   " -rotateQuaternionW " + quat.w + 
						   " -name " + joints[i].name);
   MGlobal::executeCommand(MString("select -d"));
  }
 }

 joints.clear();
 return true;
}
//////////////////////
//EXPORT MATERIAL LIST
int kgmCollectMaterials(){
 MItDependencyNodes it;
 MStatus stat;

 for(it.reset(); !it.isDone(); it.next()){
  MObject o(it.item());
  if(o.hasFn(MFn::kLambert)){
   MFnLambertShader sh(o);
   MColor a, d, s, t;
   kMaterial m = {0};

   strcpy(m.name, sh.name().asChar());
   d = sh.color();
   a = sh.ambientColor();
   t = sh.transparency();
   m.a[0] = m.e[0] = 255 * a.r;
    m.a[1] = m.e[1] = 255 * a.g;
     m.a[2] = m.e[2] = 255 * a.b;
   m.d[0] = 255 * d.r;
    m.d[1] = 255 * d.g;
     m.d[2] = 255 * d.b;
   m.o = 100 - 100 * t.r;

   ////
   MPlugArray plugs;
   stat = MFnDependencyNode(sh).getConnections(plugs);
   for(int i = 0; i < plugs.length(); i++){
//	   MessageBox(0, plugs[i].name().asChar(), "", 0);
   }
   //////////	
   MPlug pc = MFnDependencyNode(sh).findPlug("color");
   MItDependencyGraph dgIt(pc, MFn::kFileTexture, MItDependencyGraph::kUpstream, MItDependencyGraph::kBreadthFirst, MItDependencyGraph::kNodeLevel);
   MString msTextureFile;
   MFnDependencyNode(dgIt.thisNode()).findPlug("fileTextureName").getValue(msTextureFile);
   if(!dgIt.isDone()){
    Msg(msTextureFile.asChar());
    char* pc = strrchr(msTextureFile.asChar(), '/');
    if(pc) 
     strcpy(m.mc, ++pc);
    else 
     strcpy(m.mc, msTextureFile.asChar());
   }
   MPlug pb = MFnDependencyNode(sh).findPlug("normalCamera", &stat);
   MItDependencyGraph dggIt(pb, MFn::kFileTexture, MItDependencyGraph::kUpstream, MItDependencyGraph::kBreadthFirst, MItDependencyGraph::kNodeLevel);
   MFnDependencyNode(dggIt.thisNode()).findPlug("fileTextureName").getValue(msTextureFile);
   if(!dggIt.isDone()){
	Msg(msTextureFile.asChar());
    char* pc = strrchr(msTextureFile.asChar(), '/');
    if(pc) 
     strcpy(m.mn, ++pc);
    else 
     strcpy(m.mn, msTextureFile.asChar());
   }
   gMaterials.push_back(m);
  }
  /*
  if(o.hasFn(MFn::kBlinn)){
   MFnBlinnShader sh(o);
   MColor a, d, s, t;
   int opacity, shininess;
   int c[4]; 
   kMaterial m = {0};
  }
  //*/
 }
 return gMaterials.size();
}
bool kgmExportMaterials(FILE* file){
 for(int i = 0; i < gMaterials.size(); i++){
  kMaterial& m = gMaterials[i];
  fprintf(file, "Material %s\n", m.name);
  fprintf(file, "\tDiffuse %i %i %i\n", m.d[0], m.d[1], m.d[2]);
  fprintf(file, "\tOpacity %i\n", m.o);
  if(strlen(m.mc) > 0)
   fprintf(file, "\tDiffuseMap %s\n", m.mc);
  if(strlen(m.mn) > 0)
   fprintf(file, "\tNormalMap %s\n", m.mn);
 }
 return true;
}
//////////////////
//MESH
int kgmExistVertex(std::vector<kVertex>& verts, kVertex& v){
 for(int i = 0; i < verts.size(); i++){
  if(verts[i].v != v.v) continue;
  if(verts[i].uv0 != v.uv0) continue;
  if(verts[i].uv1 != v.uv1) continue;
  if(verts[i].n != v.n) verts[i].n += v.n;
  return i;
 }
 return -1;
}
//EXPORT MESH
int  instance = 0;
bool kgmExportMesh(FILE* file, MFnMesh& mesh){
 MObjectArray mshaders;
 MStringArray uvfamily;
 MPointArray points;
 MFloatArray upoints;
 MFloatArray vpoints;
 MIntArray   tricount;
 MIntArray   triverts;
 MIntArray   mindices;
 MDagPath    mdpmesh;

 std::vector<kVertex> verts;
 std::vector<kFace>   faces;
 std::vector<kJoint>  bones;

 int  numUVS = 0;
 bool skin = false;

 mesh.getPath(mdpmesh);

 mesh.getPoints(points);
 mesh.getUVs(upoints, vpoints);
 mesh.getUVSetNames(uvfamily);
 mesh.getTriangles(tricount, triverts);

 numUVS = mesh.numUVSets();
 mesh.getConnectedShaders(instance, mshaders, mindices);

 char s[256] = {0};
 int  i = 0, triangles = 0;

// for(i = 0; i < uvfamily.length(); i++)
//	 Msg(uvfamily[i].asChar());

 for(i = 0; i < tricount.length(); i++){
  for(int j = 0; j < tricount[i]; j++){
   kFace f;
   mesh.getPolygonTriangleVertices(i, j, f.f);
   faces.push_back(f);
  }
 }

 for(i = 0; i < faces.size(); i++){
  for(int j = 0; j < 3; j++){
   kVertex v;
   int ipt = faces[i].f[j];
   int ivt = -1;
   float tu, tv;

   v.ibones[0] = -1;
    v.ibones[1] = -1;
    v.ibones[2] = -1;
    v.ibones[3] = -1;
   v.wbones[0] = 0.0;
    v.wbones[1] = 0.0;
    v.wbones[2] = 0.0;
    v.wbones[3] = 0.0;
   v.v = points[ipt];
   v.iraw = ipt;
   mesh.getUV(ipt, tu, tv); v.uv0.x = tu, v.uv0.y = tv;
   mesh.getVertexNormal(ipt, v.n);

   ivt = kgmExistVertex(verts, v);
   if(ivt < 0){
    verts.push_back(v);
	faces[i].f[j] = verts.size() - 1;
   }else{
    faces[i].f[j] = ivt;
   }
  }
 }


 //Fill Skin data if such exist
 MItDependencyNodes itDeps;
 MFnSkinCluster     fnSkin;
 MFnTransform       fnTran;
 MStatus	    stat;

 itDeps.reset(MFn::kSkinClusterFilter);
 for(; !itDeps.isDone(); itDeps.next()){
  MDagPathArray bnPaths;
  stat = fnSkin.setObject(itDeps.item());
  if(!stat)
	  continue;

  fnSkin.indexForOutputShape(mesh.object(), &stat);
  if(!stat)
	  continue;

  fnSkin.influenceObjects(bnPaths, &stat);
  if(!stat)
	  continue;

  skin = true;
  for(int ibone = 0; ibone < bnPaths.length(); ibone++){
   MFloatArray weights;
   MDagPath	   path;
   MSelectionList sels;
   MObject comps;
   MFnTransform fnBone;
   MTransformationMatrix mat;
   kJoint		joint;
   int			isbone = -1; //skeleton bone index

   stat = fnSkin.getPointsAffectedByInfluence(bnPaths[ibone], sels, weights);
   if(0 == weights.length())
	   continue;
   if(1 != sels.length())
	   continue;
   sels.getDagPath(0, path, comps);
   if(!(path == mdpmesh))
	   continue;
   if(!bnPaths[ibone].hasFn(MFn::kTransform))
	   continue;
   stat = fnBone.setObject(bnPaths[ibone]);
   if(!stat)
	   continue;

   mat = fnBone.transformation(&stat);
   MVector pos = mat.getTranslation(MSpace::kWorld, &stat);
   MQuaternion rot = mat.rotation();
   memset(joint.name, 0, sizeof(joint.name));
   strcpy(joint.name, fnBone.name().asChar());

//   stat = fnSkin.getWeights(mdpmesh, comps, ibone, weights);
   stat = fnSkin.getPointsAffectedByInfluence(bnPaths[ibone], sels, weights);	
   if(!stat)
	   continue;
   char s[64];
   sprintf(s, "weights: %i", weights.length());
//   MessageBox(0, s, "", 0);
   for(int i = 0; i < weights.length(); i++)
	if(weights[i] > 0.0){
 		char s[64]; sprintf(s, "VIndex: %i %0.2f\0", i, weights[i]);
//        MessageBox(0, s, "", 0);
	}

   MItGeometry itPoints(mdpmesh, comps);
   for(; !itPoints.isDone(); itPoints.next()){
    int iraw = itPoints.index();
	char str[64]; sprintf(str, "VIndex: %i\0", iraw);

    for(int iverts = 0; iverts < verts.size(); iverts++){
     if(verts[iverts].iraw == iraw){
      for(int ib = 0; ib < 4; ib++){
       if((verts[iverts].ibones[ib] == -1) && (weights[iraw] > 0.0)){
        verts[iverts].ibones[ib] = ibone;
	verts[iverts].wbones[ib] = weights[iraw];
	break;
       }
      }
     }
    }
   }
  }
 }
 //////////////////////////////
 strcpy(s, mesh.name().asChar());
// Msg(s);
 char* pc = s;
 for(pc = s; *pc != '\0'; pc++){
  if(*pc == ' ' || *pc == '|')
	  *pc = '_';
 }
 fprintf(file, "Mesh %s\n", s);
 fprintf(file, "\tVertices %i\n", verts.size());
 for(i = 0; i < verts.size(); i++){
  kVertex v = verts[i];
  fprintf(file, "\t\t%f %f %f\t%f %f %f\t%f %f\n", 
				v.v.x, v.v.y, v.v.z, 
				v.n.x, v.n.y, v.n.z,
				v.uv0.x, v.uv0.y);
 }
 fprintf(file, "\tFaces %i\n", faces.size());
 for(i = 0; i < faces.size(); i++){
  kFace f = faces[i];
  fprintf(file, "\t\t%i %i %i\n", f.f[0], f.f[1], f.f[2]);
 }

 if(skin){
 fprintf(file, "\tSkin %i\n", verts.size());
  for(i = 0; i < verts.size(); i++){
   kVertex v = verts[i];
   int abones = 0;
   for(int ib = 0; ib < 4; ib++)
	   if(v.wbones[ib] > 0.0)
		   abones++;
   fprintf(file, "\t\t%i %i %i %i %i %f %f %f %f\n", abones,
		   v.ibones[0], v.ibones[1], v.ibones[2], v.ibones[3], 
		   v.wbones[0], v.wbones[1], v.wbones[2], v.wbones[3]);
  }
 }

 verts.clear();
 faces.clear();
 return true;
}

bool kgmExportGeometry(MString& path){
 FILE* file = fopen(path.asChar(), "w");
 if(!file)
	 return false;
 //PREPARE
 kgmCollectMaterials();
 kgmCollectSkeleton();
 //EXPORT MATERIALS
 kgmExportMaterials(file);
 //EXPORT MESHES
 MItDag it;
 for(it.reset(); !it.isDone(); it.next()){
  MObject o(it.item());
  if(o.hasFn(MFn::kMesh)){
   MFnMesh mesh(o);
//   MFnDagNode dn(o);
   MDagPath dp;
   mesh.getPath(dp);
   instance = 0;
   if(dp.isInstanced()) 
	   instance = dp.instanceNumber();
   kgmExportMesh(file, mesh);
  }
 }
 fclose(file);
 gMaterials.clear();
 gJoints.clear();
 return true;
}

bool kgmExportMesh(MString& path)
{
 FILE* file = fopen(path.asChar(), "w");
 if(!file)
	 return false;
 //PREPARE
// kgmCollectMaterials();
 kgmCollectSkeleton();
 //EXPORT MATERIALS
 //kgmExportMaterials(file);
 //EXPORT MESHES
 MSelectionList selection;
 MGlobal::getActiveSelectionList(selection);
 MItSelectionList it(selection, MFn::kMesh);
 for(; !it.isDone(); it.next()){
  MDagPath dagPath;
  it.getDagPath(dagPath);
  MFnMesh mesh(dagPath.node());
  MDagPath dp;
  mesh.getPath(dp);
  instance = 0;
  if(dp.isInstanced()) 
   instance = dp.instanceNumber();
  kgmExportMesh(file, mesh);
 }
 fclose(file);
// gMaterials.clear();
 gJoints.clear();
 return true;
}

/////////////////////////
/////////ANIMATION
int nFPS = 1;
int dFRM = 1;

int kgmCollectFrames(MFnTransform& ft, kFrameList& fl){
 MAnimControl ac;
 MTime        tm;
 for(double at = ac.minTime().value(); at < ac.maxTime().value(); at += (double)dFRM){
  MTransformationMatrix mat;
  MDagPath path; MDagPath::getAPathTo(ft.object(), path);
  kFrame		frame = {0};
  MStatus stat;

  tm.setValue(at);
  ac.setCurrentTime(tm);
//  mat = ft.transformation(&stat);
  mat = path.inclusiveMatrix();
  MVector pos = mat.getTranslation(MSpace::kWorld, &stat);
  MQuaternion rot = mat.rotation();
  frame.frame = at;
  frame.pos[0] = pos.x, frame.pos[1] = pos.y, frame.pos[2] = pos.z;
  frame.rot[0] = rot.x, frame.rot[1] = rot.y, frame.rot[2] = rot.z, frame.rot[3] = rot.w;
  fl.push_back(frame);
 }
 ac.setCurrentTime(ac.minTime());
 return fl.size();
}
//ANIMATION
bool kgmExportAnimation(MString& path){
 FILE* file = fopen(path.asChar(), "w");
 if(!file)
	 return false;
 //PREPARE
 MItDag it;
 MAnimControl ac;
 MTime		  tm; 
 int		  sec = 1;
 int		  fps = 1;
 int		  st  = ac.minTime().value();
 int		  et  = ac.maxTime().value();
 switch(MTime::uiUnit()){
 case MTime::kMilliseconds:	// 1000 fps
		nFPS = 1000;
		break;
 case MTime::kGames:		// 15 fps
			nFPS = 15;
			break;
 case MTime::kFilm:			// 24 fps
			nFPS = 24;
			break;
 case MTime::kPALFrame:		// 25 fps
			nFPS = 25;
			break;
 case MTime::kNTSCFrame:	// 30 fps
			nFPS = 30;
			break;
 case MTime::kShowScan:		// 48 fps
			nFPS = 48;
			break;
 case MTime::kPALField:		// 50 fps
			nFPS = 50;
			break;
 case MTime::kNTSCField:	// 60 fps
			nFPS = 60;
			break;
 default:
			nFPS = 1;
 }
 fps = nFPS;
 sec = (et - st) / fps; if(sec < 1) sec = 1;
 dFRM = 1.0; //sec / (10 * sec);

 fprintf(file, "FrameStart %i\n", st);
 fprintf(file, "FrameEnd %i\n",   et);
 fprintf(file, "FPS %i\n",        fps);

 //MGlobal glob;
 MSelectionList selection;
 MGlobal::getActiveSelectionList(selection);

 for(int isel = 0; isel < selection.length(); isel++){
  MObject o;
  MDagPath dp;
  
  selection.getDagPath(isel, dp);
  o = dp.node();

  if(o.hasFn(MFn::kTransform)){
   MFnTransform ft(o);
   kFrameList fl;
   kgmCollectFrames(ft, fl);
   if(fl.size() < 1)
	   continue;
   fprintf(file, "Frames %s\n", ft.name().asChar());
   for(int i = 0; i < fl.size(); i++){
    kFrame f = fl[i];
	fprintf(file, "\tKey %i %f %f %f %f %f %f %f\n", 
		    f.frame, f.pos[0], f.pos[1], f.pos[2],
			f.rot[0], f.rot[1], f.rot[2], f.rot[3]);
   }
   fl.clear();
  }

 }
 fclose(file);
 return true;
}
///////////
//PHYSICS
void kgmExportPhysics(FILE* file, MFnMesh& mesh)
{
 MPointArray points;
 MIntArray   tricount;
 MIntArray	 triverts;


 mesh.getPoints(points);
 mesh.getTriangles(tricount, triverts);

 int i = 0;
 for(i = 0; i < triverts.length(); i += 3)
 {
  MPoint pta = points[triverts[i + 0]];
  MPoint ptb = points[triverts[i + 1]];
  MPoint ptc = points[triverts[i + 2]];

  fprintf(file, "%f %f %f %f %f %f %f %f %f\n", 
		  pta.x, pta.y, pta.z, 
		  ptb.x, ptb.y, ptb.z, 
		  ptc.x, ptc.y, ptc.z);
 }
 points.clear();
 tricount.clear();
 triverts.clear();
}

bool kgmExportPhysics(MString& path){
 FILE* file = fopen(path.asChar(), "w");
 if(!file)
	 return false;
 //EXPORT MESHES
 MItDag it;
 for(it.reset(); !it.isDone(); it.next()){
  MObject o(it.item());
  if(o.hasFn(MFn::kMesh)){
   MFnMesh mesh(o);
   MFnAttribute attr(o);
   if(attr.isHidden())
	   continue;
   kgmExportPhysics(file, mesh);
  }
 }
 fclose(file);
 return true;
}
///////////

