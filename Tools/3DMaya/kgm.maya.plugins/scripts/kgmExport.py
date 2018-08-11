#from maya.api import *
import os, sys, time 
import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaAnim as OpenMayaAnim
import maya.OpenMayaRender as OpenMayaRender


glRenderer = OpenMayaRender.MHardwareRenderer.theRenderer()
gl = glRenderer.glFunctionTable()

nFps = 1
dFrm = 1

eFlags = {'bin':0, 'txt':0, 'xml':0}
eElems= {'mesh':0, 'material':0, 'skeleton':0, 'animation':0}

#kgm Game Node Class
#class kgmNode(OpenMayaMPx.MPxNode):
class kgmNode( OpenMayaMPx.MPxLocatorNode ):
 id = OpenMaya.MTypeId( 0x7022 )

 def __init__( self ):
  OpenMayaMPx.MPxLocatorNode.__init__( self )

 def draw( self, view, path, style, status):
  view.beginGL( )
  gl.glBegin(OpenMayaRender.MGL_LINES)
  gl.glVertex3f(0.0, -0.5, 0.0)
  gl.glVertex3f(0.0, 0.5, 0.0)
  gl.glEnd()
  view.endGL( )

 @staticmethod
 def create( ):
  return OpenMayaMPx.asMPxPtr( kgmNode( ) )

 @staticmethod
 def initialize( ):
  return OpenMaya.MStatus.kSuccess

#class for maya materials
class kMaterial():
 name = ''   
 color = [1.0, 1.0, 1.0]
 specular = [1.0, 1.0, 1.0]
 emmision = [1.0, 1.0, 1.0]
 transparncy = 0.0
 shininess = 1.0
 mapcolor=''
 mapnormal=''
 mapspecular=''
 
 def __init__(self, o):
  self.name = OpenMaya.MFnDependencyNode(o).name()

  shader = None   
  if o.hasFn(OpenMaya.MFn.kLambert):
   shader = OpenMaya.MFnLambertShader(o)
  elif o.hasFn(OpenMaya.MFn.Blinn):   
   shader = OpenMaya.MFnBlinnShader(o)
  elif o.hasFn(OpenMaya.MFn.Phong):   
   shader = OpenMaya.MFnPhongShader(o)
  elif o.hasFn(OpenMaya.MFn.kSurfaceShader):   
   shader = OpenMaya.MFnDependencyNode(o)
     
  if shader:
   self.color = [shader.color().r, shader.color().g, shader.color().b]
   self.transparency = shader.transparency().r    

  plugs = OpenMaya.MPlugArray()
  dnode = OpenMaya.MFnDependencyNode(o) 
  stat  = dnode.getConnections(plugs)
#  for plug in plugs:
#   print ''
  plug = OpenMaya.MFnDependencyNode(o).findPlug('color')   
  dgIt = OpenMaya.MItDependencyGraph(plug, 
                                     OpenMaya.MFn.kFileTexture,
                                     OpenMaya.MItDependencyGraph.kUpstream,
                                     OpenMaya.MItDependencyGraph.kBreadthFirst,
                                     OpenMaya.MItDependencyGraph.kNodeLevel)
  if(not dgIt.isDone()):
   texFile = OpenMaya.MFnDependencyNode(dgIt.thisNode()).findPlug('fileTextureName').asString()
   mapdiffuse = texFile    
   
  plug = OpenMaya.MFnDependencyNode(o).findPlug("normalCamera")
  dgIt = OpenMaya.MItDependencyGraph(plug, 
                                     OpenMaya.MFn.kFileTexture, 
                                     OpenMaya.MItDependencyGraph.kUpstream, 
                                     OpenMaya.MItDependencyGraph.kBreadthFirst, 
                                     OpenMaya.MItDependencyGraph.kNodeLevel);
  if(not dgIt.isDone()):
   texFile = OpenMaya.MFnDependencyNode(dgIt.thisNode()).findPlug("fileTextureName").asString();
   self.mapcolor = texFile    
  
#class for frame
class kFrame:
 frame = 0
 pos = OpenMaya.MVector()
 rot = OpenMaya.MQuaternion()
 def __init__(self):
  pass
  
class kAnimated:
 name = ''   
 frames = []
 
 def __init__(self, ft):
  self.name = ft.name()   
  self.frames = list()
  ac = OpenMayaAnim.MAnimControl()
  tm = OpenMaya.MTime()
  at = ac.minTime().value()
  ac.setCurrentTime(ac.minTime())                       
  while at < ac.maxTime().value():
   mat = OpenMaya.MTransformationMatrix()
   path = OpenMaya.MDagPath()
   OpenMaya.MDagPath.getAPathTo(ft.object(), path)
   frame = kFrame()
   tm.setValue(at)
   ac.setCurrentTime(tm)
#   mat = path.inclusiveMatrix()
   mat = ft.transformation()
   frame.pos = mat.getTranslation(OpenMaya.MSpace.kWorld)
   frame.rot = mat.rotation()
   frame.frame = at
   self.frames.append(frame)   
   at = at + dFrm      
  ac.setCurrentTime(ac.minTime())
  
#class for Joint
class kJoint:
 name = ''   
 parent = ''
 iparent = -1
 pos = OpenMaya.MVector(0, 0, 0)
 rot = OpenMaya.MQuaternion()
 def __init__(self):
  pass
        
#class for skeleton
class kSkeleton():
 joints = []   
 def __init__(self):
  self.joints = list()   
  it = OpenMaya.MItDag()
  fnTran = OpenMaya.MFnTransform()
  stat = OpenMaya.MStatus()
  it.reset()
  i = 0
  while not it.isDone():
   o = OpenMaya.MObject(it.item())
   fnBone = OpenMaya.MFnTransform()
   if not o.hasFn(OpenMaya.MFn.kJoint):
    it.next()      
    continue      
   if not o.hasFn(OpenMaya.MFn.kTransform):
    it.next()      
    continue
   stat = fnBone.setObject(o)      
   path = OpenMaya.MDagPath()
   it.getPath(path)
   #mat = path.inclusiveMatrix()
   print 'count: ', i
   i = i + 1
   mat = fnBone.transformation()
   joint = kJoint()
   joint.pos = mat.getTranslation(OpenMaya.MSpace.kWorld)
   joint.rot = mat.rotation()
   joint.name = fnBone.name()
   if fnBone.parentCount() > 0 and fnBone.parent(0).hasFn(OpenMaya.MFn.kTransform):
    fnPBone = OpenMaya.MFnTransform()
    fnPBone.setObject(fnBone.parent(0))
    joint.parent = fnPBone.name()    
   self.joints.append(joint)        
   it.next()      
 
#class or mesh vertex
class kVertex():
 v = OpenMaya.MPoint()
 n = OpenMaya.MVector()
 uv0 = [0.0, 0.0]
 uv1 = [0.0, 0.0]
 joints = [-1, -1, -1, -1]
 weights = [0.0, 0.0, 0.0, 0.0]
 iraw = -1
     
 def __init__(self):
  pass
  
 @staticmethod
 def exist(verts, v):
  #and v.n == cv.n and v.uv0 == cv.uv0   
  vi = 0
  for vi in range(0, len(verts)):
   if v.v == verts[vi].v:
    return vi
   else:
    vi = vi + 1            
  return -1
      
#class or mesh vertex
class kFace():
 f = [0, 0, 0]
 n = OpenMaya.MVector()
 def __init__(self):
  pass
  
#class for maya mesh
class kMesh():
 name = ''   
 vertices = []
 faces = []
 
 def __init__(self, o):
  print 'build mesh...'
  self.vertices = list()
  self.faces = list()
  #Geometry   
  points = OpenMaya.MPointArray()
  upoints = OpenMaya.MFloatArray()
  vpoints = OpenMaya.MFloatArray()
  #uvfamily = OpenMaya.MStringArray()
  tricount = OpenMaya.MIntArray()
  triverts = OpenMaya.MIntArray()

  self.name = OpenMaya.MFnDependencyNode(o).name()
  mesh = OpenMaya.MFnMesh(o)
  mesh.getPoints(points)
  mesh.getUVs(upoints, vpoints)
  #mesh.getUVSetNames(uvfamily)
  mesh.getTriangles(tricount, triverts)
  
  ti = 0
  for i in range(tricount.length()):
   n = OpenMaya.MVector()
   mesh.getPolygonNormal(triverts[i], n)   
   for j in range(tricount[i]):
    f = kFace()
    f.f = [triverts[3 * ti], triverts[3 * ti + 1],triverts[3 * ti + 2]]
    f.n = n
    self.faces.append(f)
    ti = ti + 1
    
  for f in self.faces:
   for j in range(0, 3):   
    v  = kVertex()
    v.iraw = f.f[j]
    v.n = f.n
    v.v = points[v.iraw]
    #v.uv0 = [upoints[v.iraw], vpoints[v.iraw]]
    ivt = kVertex.exist(self.vertices, v)
    print 'iraw: ', v.iraw, ' ivt: ', ivt, ' v: ', v.v.x, ' vlen: ', len(self.vertices)
    #ivt = -1
    if ivt < 0:
     self.vertices.append(v)
     f.f[j] = len(self.vertices) - 1
    else:
     f.f[j] = ivt

  #Skin   
#  fnSkin = None
#  splug = mesh.findPlug('inMesh')
#  try:
#   itDg = OpenMaya.MItDependencyGraph(plugInMesh, OpenMaya.MFn.kSkinClusterFilter, OpenMaya.MItDependencyGraph.kUpstream, OpenMaya.MItDependencyGraph.kDepthFirst, OpenMaya.MItDependencyGraph.kNodeLevel)
#   while not itDg.isDone():
#    oNode = itDg.currentItem()
#    fnSkin = OpenMayaAnim.MFnSkinCuster(oNode)
#    break
#  except:
#   pass
  return
  itDeps = OpenMaya.MItDependencyNodes()
  fnSkin = OpenMayaAnim.MFnSkinCluster()    
  fnTran = OpenMaya.MFnTransform()
  stat   = OpenMaya.MStatus()
  itDeps.reset(OpenMaya.MFn.kSkinClusterFilter)
   
  while not itDeps.isDone():
   bnPaths = OpenMaya.MDagPathArray()
   stat = fnSkin.setObject(itDeps.item())  
   if not stat:
    continue
   fnSkin.indexForOutputShape(mesh.object(), stat)
   if not state:
    continue
   fnSkin.influenceObjects(bnPaths, stat)
   if not state:
    continue
   for ibone in range(bnPaths.length()):
    weights = OpenMaya.MFloatArra()               
    path = OpenMaya.MDagPath()
    sels = OpenMaya.MSelectionList()
    comps = OpenMaya.MObject()
    fnBone = OpenMaya.MFnTransform()
    mat = OpenMaya.MTransformationMatrix()
    joint = kJoint()
    isbone = -1
    
    stat = fnSkin.getPointsAsAffectedByInfluence(bnPaths[ibone], sels, weights)
    if 0 == weights.length():
     continue   
    if 1 != sels.length():
     continue
    sels.getDagPath(0, path, comps)
    #
    if not bnPaths[ibone].hasFn(OpenMaya.MFn.kTransform):
     continue
    stat = fnBone.setObject(bnPaths[ibone])
    if not stat:
     continue
    mat = fnBone.transformation(stat)
    pos = mat.getTranslation(openMaya.MSpace.kWorld, stat)
    rot = mat.getRotation()
    joint.name = fnBone.name() 
    itPoints = OpenMaya.MItGeometry(mdpmesh, comps)
    while not itPoints.isDone():
     iraw = itPoints.index()
     for v in self.vertices:
      if v.iraw == iraw:
       for ib in range(4):
        if (v.joints[ib] == -1) and (weights[iraw] > 0.0):
         v.joints[ib] = ibone
         v.weights[ib] = weights[iraw]                        
     itPoints.next()     
   itDeps.next()           
  
#exporter base class
class kgmExport():
 def open(self, path):
  pass
 
 def close(self):
  pass
     
 def mesh(self, mesh):
  pass
  
 def material(self, material):
  pass    
 
 def skeleton(self, skeleton):
  pass
  
 def animation(self, animation):
  pass                

#export to kgm xml file
class kgmExportXml(kgmExport):
 def __init__(self, file):
  self.file = file
  
 def mesh(self, mesh):
  print >> self.file,"<kgmMesh name='" + mesh.name + "'>"   
  print >> self.file," <Vertices length='" + str(len(mesh.vertices))+ "'>"
  for v in mesh.vertices:
   print >> self.file,"   " + str(v.v[0]) + " " + str(v.v[1]) + " " + str(v.v[2]) + " " + str(v.n[0]) + " " + str(v.n[1]) + " " + str(v.n[2]) + " " + str(v.uv0[0]) + " " + str(v.uv0[1]) + " "     
   #print >> self.file,"   <UV1 x='" + str(v.uv1[0]) + "' y='" + str(v.uv1[1]) + "' />"     
   #print >> self.file,"   <Joints a='" + str(v.joints[0]) + "' b='" + str(v.joints[1]) + "' c='" + str(v.joints[2]) + "'  d='" + str(v.joints[3]) + "' />"     
   #print >> self.file,"   <Weights a='" + str(v.weights[0]) + "' b='" + str(v.weights[1]) + "' c='" + str(v.weights[2]) + "'  d='" + str(v.weights[3]) + "' />"     

  print >> self.file," </Vertices>"
     
  print >> self.file," <Faces length='" + str(len(mesh.faces)) + "'>"
  for f in mesh.faces:
   print >> self.file,"  " + str(f.f[0]) + " " + str(f.f[1]) + " " + str(f.f[2]) + " "     
  print >> self.file," </Faces>"
     
  print >> self.file,"</kgmMesh>"   
  print ''          
  pass
  
 def material(self, material):
  print >> self.file," <kgmMaterial name='" + material.name + "'>"   
  print >> self.file,"  <Color value='" + str(material.color[0]) + " " + str(material.color[1]) + " " + str(material.color[2]) + "' />"   
  print >> self.file,"  <Specular value='" + str(material.specular[0]) + " " + str(material.specular[1]) + " " + str(material.specular[2]) + "' />"   
  print >> self.file,"  <Emmision value='" + str(material.emmision[0]) + " " + str(material.emmision[1]) + " " + str(material.emmision[2]) + "' />"   
  print >> self.file,"  <Transparency value='" + str(material.transparency) + "' />"   
  print >> self.file,"  <Shininess value='" + str(material.shininess) + "' />"   
  if( material.mapcolor not in ''):
   print >> self.file,"  <MapColor value='" + material.mapcolor + "' />"   
  print >> self.file," </kgmMaterial>"   
  print ''     
  pass    
 
 def skeleton(self, skeleton):
  print >> self.file, " <kgmSkeleton>"  
  for j in skeleton.joints: 
   print >> self.file,"  <Joint name='" + j.name + "' parent='" + j.parent + "' />"   
   print >> self.file,"   <Position x='" + str(j.pos.x) + "' y='" + str(j.pos.y) + "' z='" + str(j.pos.z) + "' />"   
   print >> self.file,"   <Rotation x='" + str(j.rot.x) + "' y='" + str(j.rot.y) + "' z='" + str(j.rot.z) + "' w='" + str(j.rot.w) + "' />"   
   print >> self.file,"  </Joint>"   
  print >> self.file," </kgmSkeleton>"
  pass
  
 def animated(self, animated):
  print >> self.file, "  <Frames name = '" + self.name + "'>"
  for f in animated.frames:
   print >> self.file,"   <Frame id='" + str(f.frame) + "' />"
   print >> self.file,"    <Position x='" + str(f.pos.x) + "' y='" + str(f.pos.y) + "' z='" + str(f.pos.z) + "' />"   
   print >> self.file,"    <Rotation x='" + str(f.rot.x) + "' y='" + str(f.rot.y) + "' z='" + str(f.rot.z) + "' w='" + str(f.rot.w) + "' />"   
   print >> self.file,"   </Frame>"
  print >> self.file, "  </Frames>"   
  pass                

#kgm Engine Plugin Class
class kgmEngine( OpenMayaMPx.MPxCommand ):
 def __init__( self ):
  OpenMayaMPx.MPxCommand.__init__( self )
	
 def doIt( self, arglist ):
  wnd = cmds.window( "kgmEngineWindow_" + str(time.time()), widthHeight = ( 200, 200 ) )
  #cmds.columnLayout( adjustableColumn = True )
  #cmds.button( label = 'Export Map',          command = kgmEngine.exportMap )
  #cmds.button( label = 'Export Mesh',         command = kgmEngine.exportMesh )
  #cmds.button( label = 'Export Skeleton',     command = kgmEngine.exportSkeleton )
  #cmds.button( label = 'Export Animation',    command = kgmEngine.exportAnimation )
  #cmds.button( label = 'Insert Node',         command = kgmEngine.insertNode )
  cmds.columnLayout()
  cmds.rowColumnLayout( numberOfColumns = 2 )
  cmds.checkBox( "Mesh", label="Mesh", value=False, onCommand=kgmEngine.enableMesh, offCommand=kgmEngine.enableMesh )
  cmds.checkBox( "Material", label="Material", value=False, onCommand=kgmEngine.enableMaterial, offCommand=kgmEngine.enableMaterial )
  cmds.checkBox( "Skeleton", label="Skeleton", value=False, onCommand=kgmEngine.enableSkeleton, offCommand=kgmEngine.enableSkeleton )
  cmds.checkBox( "Animation", label="Animation", value=False, onCommand=kgmEngine.enableAnimation, offCommand=kgmEngine.enableAnimation )
  
  cmds.columnLayout()
  cmds.rowColumnLayout( numberOfColumns = 2 )
  cmds.columnLayout( adjustableColumn=True )
  cmds.button( label = 'Export',         command = kgmEngine.export )
  cmds.button( label = 'Insert Node',    command = kgmEngine.insertNode )
  
  cmds.columnLayout()
  cmds.rowColumnLayout( numberOfColumns = 3 )
  cmds.columnLayout( adjustableColumn=True )
  cmds.radioCollection()
  cmds.radioButton( label='Bin', align='left', select=False )
  cmds.radioButton( label='Xml', align='left', select=True  )
  
  #cmds.radioButtonGrp( label='Format',   labelArray3=['Bin', 'Txt', 'Xml'], numberOfRadioButtons=3 )
  #cmds.radioButton( 'Format', label='Bin',   )
  cmds.showWindow( wnd )

 
 @staticmethod
 def enableMesh( value ):
  eElems['mesh'] = value

 @staticmethod
 def enableMaterial( value ):
  eElems['material'] = value      
         
 @staticmethod
 def enableSkeleton( value ):
  eElems['skeleton'] = value
        
 @staticmethod
 def enableAnimation( value ):
  eElems['animation'] = value
        
 @staticmethod
 def exportMap( arg ):
  fname = '/home/goga/z.xml' 
#  fname = cmds.fileDialog(mode=1)
  if fname == '':
   return    
  
  
  materials = []
  it = OpenMaya.MItDependencyNodes()
  it.reset()
  while not it.isDone():
   o = OpenMaya.MObject(it.item())
   if o.hasFn(OpenMaya.MFn.kLambert) or o.hasFn(OpenMaya.MFn.kBlinn) or o.hasFn(OpenMaya.MFn.kPhong) or o.hasFn(OpenMaya.MFn.kSurfaceShader):
    m = kMaterial() 
    if o.hasFn(OpenMaya.MFn.kLambert):
     sh = OpenMaya.MFnLambertShader(o)
    elif o.hasFn(OpenMaya.MFn.Blinn):   
     sh = OpenMaya.MFnBlinnShader(o)
    elif o.hasFn(OpenMaya.MFn.Phong):   
     sh = OpenMaya.MFnPhongShader(o)
    elif o.hasFn(OpenMaya.MFn.kSurfaceShader):   
     sh = OpenMaya.MFnDependencyNode(o)
     
    if not o.hasFn(OpenMaya.MFn.kSurfaceShader):
     m.color = [sh.color().r, sh.color().g, sh.color().b]
     m.transparency = sh.transparency().r    
    else: 
     pass
     
    materials.append(m)     
   it.next()
   
  export = kgmExportXml()
  export.open(fname)
   
  for m in materials:
   export.material(m)

  meshes = []  
  it = OpenMaya.MItDag() 
  it.reset()
  while( not it.isDone()):
   o = OpenMaya.MObject(it.item())
   if(o.hasFn(OpenMaya.MFn.kMesh)):
    m = kMesh(OpenMaya.MFnMesh(o)) 
    meshes.append(m)     
   it.next()
  for m in meshes:
   m.export_xml(file)
  print >> file, '</kgmEngine>'
  file.close()           
  print 'export map'




 @staticmethod
 def export( arg ):
  fname = '/home/goga/z.xml' 
#  fname = cmds.fileDialog(mode=1)
  if fname == '':
   return    
  file = open(fname, 'w')
  print >> file, '<?xml version="1.0" ?>'
  print >> file, '<kgmEngine version=\'0.0.1\'>'
  xml = kgmExportXml(file)
  #sel = OpenMaya.MSelectionList();
  #OpenMaya.MGlobal.getActiveSelectionList(sel)
  #itsel = OpenMaya.MItSelectionList(sel, OpenMaya.MFn.kMesh)
  #itsel.reset()

  if eElems['material']: 
   it = OpenMaya.MItDependencyNodes()
   it.reset()
   while not it.isDone():
    o = OpenMaya.MObject(it.item())
    if o.hasFn(OpenMaya.MFn.kLambert) or o.hasFn(OpenMaya.MFn.kBlinn) or o.hasFn(OpenMaya.MFn.kPhong) or o.hasFn(OpenMaya.MFn.kSurfaceShader):
     xml.material( kMaterial(o) ) 
    it.next()

  if eElems['mesh']:
   it = OpenMaya.MItDag() 
   it.reset()   
   while( not it.isDone() ):
    o = OpenMaya.MObject(it.item())
    if(o.hasFn(OpenMaya.MFn.kMesh)):
     xml.mesh(kMesh(o))
    it.next()    

  print >> file, '</kgmEngine>'
  file.close()           
  print 'export selected mesh'
  
 @staticmethod
 def exportSkeleton( arg ):
  fname = '/home/goga/z.xml' 
#  fname = cmds.fileDialog(mode=1)
  if fname == '':
   return    
  file = open(fname, 'w')
  print >> file, '<?xml version="1.0" ?>'
  print >> file, '<kgmEngine>'
  skeleton = kSkeleton()
  skeleton.export_xml(file)
  print >> file, '</kgmEngine>'
  file.close()
  print 'kgm Export Skelenon'

 @staticmethod
 def exportAnimation( arg ):
  it = OpenMaya.MItDag()
  tm = OpenMaya.MTime()
  ac = OpenMayaAnim.MAnimControl()
  sec = 1
  fps = 1
  st = ac.minTime().value()
  et = ac.maxTime().value()
  
  if OpenMaya.MTime.uiUnit() == OpenMaya.MTime.kMilliseconds:
   nFps = 1000
  elif OpenMaya.MTime.uiUnit() == OpenMaya.MTime.kGames:    
   nFps = 15
  elif OpenMaya.MTime.uiUnit() == OpenMaya.MTime.kFilm:    
   nFps = 24
  elif OpenMaya.MTime.uiUnit() == OpenMaya.MTime.kPALFrame:    
   nFps = 25
  elif OpenMaya.MTime.uiUnit() == OpenMaya.MTime.kNTSCFrame:    
   nFps = 30
  elif OpenMaya.MTime.uiUnit() == OpenMaya.MTime.kShowScan:    
   nFps = 48
  elif OpenMaya.MTime.uiUnit() == OpenMaya.MTime.kPALField:    
   nFps = 50
  elif OpenMaya.MTime.uiUnit() == OpenMaya.MTime.kNTSCField:    
   nFps = 60
  else:
   nFps = 1
  
  fps = nFps
  sec = (et - st) / fps
  if sec < 1:
   sec = 1
  dFrm = 1.0
  
  selection = OpenMaya.MSelectionList()            
  OpenMaya.MGlobal.getActiveSelectionList(selection)
  if selection.length() < 1:
   return   
  fname = '/home/goga/z.xml' 
#  fname = cmds.fileDialog(mode=1)
  if fname == '':
   return    
  file = open(fname, 'w')
  print >> file, '<?xml version="1.0" ?>'
  print >> file, '<kgmEngine>'
  print >> file, " <kgmAnimation start='" + str(st) + "' end='" + str(et) + "' fps='" + str(fps) + "'>"   
  for isel in range(selection.length()):
   o = OpenMaya.MObject()   
   dp = OpenMaya.MDagPath()
   selection.getDagPath(isel, dp)
   o = dp.node()   
   if o.hasFn(OpenMaya.MFn.kTransform):
    ft = OpenMaya.MFnTransform(o)  
    an = kAnimated(ft)
    an.export_xml(file)
  print >> file, ' </kgmAnimation>'  
  print >> file, '</kgmEngine>'
  file.close()   
  print 'kgm Export Animation'


 @staticmethod
 def insertNode( arg ):
  cmds.createNode( 'kgmNode', name = 'kgmNode' )

 @staticmethod
 def create( ):
  return OpenMayaMPx.asMPxPtr( kgmEngine( ) )


# Initialize Plugin
def initializePlugin( obj ):
 plugin = OpenMayaMPx.MFnPlugin( obj, "kgmEngine", "1.0", "Any" )
 try:
  plugin.registerCommand( "kgmengine", kgmEngine.create )
  plugin.registerNode( 'kgmNode', kgmNode.id, kgmNode.create, kgmNode.initialize, OpenMayaMPx.MPxNode.kLocatorNode )
 except:
  raise RuntimeError, "Failed to register"

def uninitializePlugin(obj):
 plugin = OpenMayaMPx.MFnPlugin(obj)
 try:
  plugin.deregisterCommand("kgmengine")
  plugin.deregisterNode(kgmNode.id)
 except:
  raise RuntimeError, "Failed to deregister"








