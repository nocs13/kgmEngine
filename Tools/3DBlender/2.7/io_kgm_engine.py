#
#
#"""
#Created on Oct 3, 2010
#@author: goga
#Name: 'Kgm (.kgm)...'
#Blender: 242
#Group: 'Export'
#"""

#__author__ = "Karakal"
#__version__ = "0.1"
#__bpydoc__ = """ This script Exports Kgm format."""

bl_info = {
    "name": "Export Kgm Model Format (.kgm)",
    "author": "Karakal",
    "version": (0, 1),
    "blender": (2, 5, 3),
    "api": 31343,
    "location": "File > Export",
    "description": "Export to the Kgm Model Format (.kgm)",
    "warning": "",
    "category": "Import-Export"}

import bpy

from bpy.props import StringProperty, BoolProperty
from bpy_extras.io_utils import ExportHelper

import os
from math import radians
from mathutils import *
from bpy.props import *

def toGrad(a):
 return a * 180.0 / 3.1415

class kgm_object(bpy.types.Operator):
  ''' Add kgm object '''
  bl_idname = "object.kgm_object"
  bl_label = "Add kgm object"
  bl_options = {'REGISTER', 'UNDO'}

  bpy.types.Object.kgm_object = bpy.props.EnumProperty( items = ( ('NONE',  "None", ""),
                                                                  ('UNIT',  "Unit", ""),
                                                                  ('DUMMY', "Dummy", ""),
                                                                  ('ACTOR', "Actor", "") ),
                                                        default='NONE' )

  def __init__(self):
    pass

  def __del__(self):
    pass

class kgm_dummy(bpy.types.Operator):
  ''' Add kgmDummy '''
  bl_idname = "object.kgm_dummy"
  bl_label = "Add kgmDummy"
  bl_options = {'REGISTER', 'UNDO'}

  bpy.types.Object.kgm_dummy = bpy.props.BoolProperty(name = "kgm_dummy", default = False)

  def __init__(self):
    self.kgm_class = 'Dummy'
    print("start")

  def __del__(self):
    print("end")

  def execute(self, context):
    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmDummy"
    a.kgm_dummy = True
    a.kgm_class = 'Dummy'
    return {'FINISHED'}

  def modal(self, context, event):
    print("Modal dummy.\n")
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmDummy"
    a.kgm_dummy = True
    a.kgm_class = 'Dummy'
    return {'RUNNING_MODAL'}

  def draw(self, context):
    layout = self.layout

class kgm_unit(bpy.types.Operator):
  ''' Add kgmUnit '''
  bl_idname = "object.kgm_unit"
  bl_label = "Add kgmUnit"
  bl_options = {'REGISTER', 'UNDO'}

  bpy.types.Object.kgm_unit = bpy.props.BoolProperty(name = "kgm_unit", default = False)
  bpy.types.Object.kgm_logic = bpy.props.StringProperty(name = "kgm_logic")
  bpy.types.Object.kgm_state = bpy.props.StringProperty(name = "kgm_state")

  def __init__(self):
    self.kgm_class = 'Unit'
    pass

  def __del__(self):
    pass

  def execute(self, context):
    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmUnit"
    a.kgm_unit = True
    return {'FINISHED'}

  def modal(self, context, event):
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmUnit"
    a.kgm_unit = True
    return {'RUNNING_MODAL'}

  def draw(self, context):
    layout = self.layout

class kgm_actor(kgm_object):
  ''' Add kgmActor '''
  bl_idname = "object.kgm_actor"
  bl_label = "Add kgmActor"
  bl_options = {'REGISTER', 'UNDO'}

  def __init__(self):
    pass

  def __del__(self):
    pass

  def execute(self, context):
    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmActor"
    a.kgm_object = "ACTOR"
    return {'FINISHED'}

  def modal(self, context, event):
    print("Modal actor.\n")
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmActor"
    a.kgm_object = "Actor"
    return {'RUNNING_MODAL'}

class kgmMaterial:
 def __init__(self, mtl):
  self.name = mtl.name
  self.diffuse = mtl.diffuse_color
  self.emmision = [1.0, 1.0, 1.0]
  self.specular = mtl.specular_color
  self.shine = 1.0
  self.alpha = mtl.alpha
  self.use_alpha = mtl.use_transparency
  self.map_color = ''
  self.map_normal = ''
  self.map_specular = ''
  self.images = ()
  #Refine a new list with only image textures that have a file source
  #self.images = [os.path.basename(Texture.image.filepath) for Texture in self.textures if Texture.image.source == "FILE"]
  for TS in mtl.texture_slots.keys():
    TextureSlot = mtl.texture_slots[TS]
    if TextureSlot.texture.type == "IMAGE" and TextureSlot.texture.image.source == "FILE":
      print("TPATH: " + TextureSlot.texture.image.filepath)
      tf_path = os.path.basename(TextureSlot.texture.image.filepath)
      if tf_path != "":
        if TextureSlot.use_map_normal != 0.0:
          self.map_normal = tf_path
          self.map_normal_strength = TextureSlot.normal_factor
        elif TextureSlot.use_map_specular != 0.0:
          self.map_specular = tf_path
          self.map_specular_strength = TextureSlot.normal_factor
        else:
          self.map_color = tf_path

  if mtl.name in bpy.data.materials and 'Shader' in bpy.data.materials[mtl.name]:
   self.shader = bpy.data.materials[mtl.name]['Shader']
  #if ImageFiles:
  #  return ImageFiles[0]
  pass

#light
class kgmLight:
 def __init__(self, o):
  self.name = o.name
  self.pos  = o.matrix_local.to_translation()
  self.rot  = o.matrix_local.to_euler()
  lamp = o.data
  self.intensity = lamp.energy
  self.shadows = 'No' if lamp.shadow_method == 'NOSHADOW' else 'Yes'
  self.range = 2 * lamp.distance
  self.color = lamp.color
  self.type = 'Point' if lamp.type == 'POINT' else 'Spot'

#camera
class kgmCamera:
 def __init__(self, o):
  self.name = o.name
  self.pos  = o.matrix_local.to_translation()
  self.rot  = o.matrix_local.to_euler()
  self.angle = o.data.angle
  self.near = o.data.clip_start
  self.far = o.data.clip_end

#skeleton
class kgmBone:
 def __init__(self, bone, aname):
  self.parent = bone.parent.name if bone.parent else 'None'
  self.name = bone.name
  self.mtx = bone.matrix_local

  #take bone armature relative position
  self.pos   = bone.matrix_local.to_translation()
  self.quat  = bone.matrix_local.to_quaternion()
  self.euler = self.quat.to_euler()

  #take bone reltive to parent position
  pose_bone = bpy.context.scene.objects[aname].pose.bones[bone.name]
  if bone.parent:
    mtx_bone = pose_bone.parent.matrix.inverted()
  else:
    mtx_bone = Matrix()
  mtx_bone  *= pose_bone.matrix
  mtx_bone   = pose_bone.matrix
  self.pos   = mtx_bone.to_translation()
  self.quat  = mtx_bone.to_quaternion()
  self.euler = mtx_bone.to_euler()

class kgmSkeleton:
 def __init__(self, o):
  self.name = o.name
  self.bones = []

  #if bpy.ops.object.mode_set.poll():
  #bpy.ops.object.mode_set(mode='POSE')

  self.arm = armature = o.data
  self.mtx = o.matrix_local
  self.pos = o.matrix_local.to_translation() # * mathutils.Vector((0, 0, 0))
  self.quat = o.matrix_local.to_quaternion()
  self.euler = o.matrix_local.to_euler()
  self.scale = o.matrix_local.to_scale()
  list = [bone for bone in armature.bones if bone.parent == None]
  for o in list:
   self.collect(o)

  #if bpy.ops.object.mode_set.poll():
  #bpy.ops.object.mode_set(mode='OBJECT')


 def collect(self, o):
  self.bones.append(kgmBone(o, self.name))
  for bone in o.children:
   self.collect(bone)

class kgmVertex:
 def __init__(self):
  self.v = [0, 0, 0]
  self.n = [0, 0, 0]
  self.uv = [0, 0]
  self.ib = [-1, -1, -1, -1]
  self.wb = [0, 0, 0, 0]
  self.idx = -1

class kgmFace:
 def __init__(self, a, b, c):
  self.f = [a, b, c]


class kgmMesh:
 def __init__(self, o):
  mesh = o

  print('Current mesh: ' + mesh.name)
  self.name = mesh.name
  self.vertices = []
  self.faces = []
  self.skin = False
  self.mtls = []
  self.hasvgroups = False

  try:
    self.hasvgroups = True if len(o.vertex_groups) > 0 else False
  except:
    self.hasvgroups = False

  if self.hasvgroups:
    self.vgroups = o.vertex_groups
  else:
    self.vgroups = None

  armatures = None

  try:
    armatures = [modifier for modifier in o.modifiers if modifier.type == 'ARMATURE']
  except:
    armatures = None

  if armatures:
   self.armature = armatures[0].object
   self.bones = self.armature.data.bones

  uvcoord = len(mesh.uv_textures)

  mesh_faces = None

  if hasattr(mesh, 'faces'):
    mesh_faces = mesh.faces
  elif hasattr(mesh, 'polygons'):
    mesh_faces = mesh.polygons

  if mesh_faces:
    print('Faces: ' + str(len(mesh_faces)))
    for i in range(0, len(mesh_faces)):
     face = mesh_faces[i]
     #iface = [-1, -1, -1, -1]
     iface = []

     for j in range(0, len(face.vertices)):
      v = kgmVertex();
      vi = face.vertices[j]
      c = mesh.vertices[vi].co
      n = face.normal
      v.v = [c[0], c[1], c[2]]
      v.n = [n[0], n[1], n[2]]

      if self.hasvgroups == True:
       for g in range(0, len(mesh.vertices[vi].groups)):
        if g < 4:
         #print(mesh.vertices[vi].groups[g].id)
         v.ib[g] = self.getBoneIndex(self.vgroups[mesh.vertices[vi].groups[g].group].name)
         v.wb[g] = mesh.vertices[vi].groups[g].weight

      if uvcoord:
        if hasattr(mesh, 'uv_layers'):
          uv = mesh.uv_layers.active.data[face.loop_start + j].uv
          v.uv = [uv[0], uv[1]]
        else:
          uvface = mesh.uv_textures.active.data[i] if uvcoord else None
          if uvface:
            uv = uvface.uv[j]
            v.uv = [uv[0], uv[1]]

      v.idx = vi
      iface.append(self.addVertex(v))

     for k in range(2, len(iface)):
       self.faces.append(kgmFace(iface[0], iface[k - 1], iface[k]))

 def addVertex(self, vx):
  iv = -1
  #check if such vertex already exist
  for i in range(0, len(self.vertices)):
   v = self.vertices[i]
   if (v.v[0] == vx.v[0]) and (v.v[1] == vx.v[1]) and (v.v[2] == vx.v[2]):
    if (v.uv[0] == vx.uv[0]) and (v.uv[1] == vx.uv[1]):
     iv = i
     if (v.n[0] != vx.n[0]) or (v.n[1] != vx.n[1]) or (v.n[2] != vx.n[2]):
      n1 = Vector((v.n[0], v.n[1], v.n[2]))
      n2 = Vector((vx.n[0], vx.n[1], vx.n[2]))
      n = n1 + n2
      n.normalize()
      v.n = [n.x, n.y, n.z]
     break

  if iv < 0:
   self.vertices.append(vx)
   iv = (len(self.vertices) - 1)
  return iv

 def skinVertex(self, o, m, v):
  armatures = [modifier for modifier in o.modifiers if modifier.type == 'ARMATURE']
  if armatures:
   armature = armatures[0].object
   bones = armature.data.bones

 def skin(self, o, m):
  armatures = [modifier for modifier in o.modifiers if modifier.type == 'ARMATURE']
  if armatures:
   armature = armatures[0].object
   bones = armature.data.bones
   poseBones = armature.pose.bones
   maxInfluences = 0
   usedBones = set()
   vertexGroups = {}

   for v in m.vertices:
    boneInfluences = [poseBones[o.vertex_groups[group.group].name] for group in v.goups if o.vertex_groups[group.group].name in poseBones]
    if len(boneInfluences) > maxInfuences:
     maxInfluennces = len(boneInfluences)
    for bone in boneInfluences:
     usedBones.add(bone)

 def getBoneIndex(self, name):
  if self.armature == None:
    return -1

  for i in range(0, len(self.armature.data.bones)):
    if name == self.armature.data.bones[i].name:
      return i

  return -1

class kgmFrame:
 def __init__(self, t, x, y, z, rx, ry, rz, rw):
  self.key = t
  self.pos = [x,  y,  z]
  self.quat = [rx, ry, rz, rw]
  self.euler = [x,  y,  z]

class kgmAnimation:
 def __init__(self, o):
  self.name = o.name
  self.type = o.type
  self.frames = []
  frameCount = bpy.context.scene.frame_end - bpy.context.scene.frame_start + 1

  for frame in range(0, frameCount):
   bpy.context.scene.frame_set(frame)
   pos = o.matrix_world.to_translation()
   rot = o.matrix_world.to_quaternion()
   f = kgmFrame(frame, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, rot.w)
   f.euler = o.matrix.to_euler()
   self.frames.append(f)


class kgmBoneAnimation(kgmAnimation):
 def __init__(self, o, a):
  self.name = o.name
  self.type = 'BONE'
  self.frames = []

  currentScene = bpy.context.scene
  startFrame = currentScene.frame_start
  endFrame = currentScene.frame_end

  for frame in range(startFrame, endFrame + 1, 1):
   currentScene.frame_set(frame)
   allBones = currentScene.objects[a.name].pose.bones
   for bone in allBones:
    if bone.name == o.name:
     if bone.parent:
       mtx   = bone.matrix * bone.parent.matrix.inverted()
     else:
       mtx = Matrix()
       mtx = bone.matrix
     mtx   = bone.matrix
     pos   = mtx.to_translation()
     quat  = mtx.to_quaternion()
     euler = quat.to_euler()


     f = kgmFrame(frame, pos.x, pos.y, pos.z, quat.x, quat.y, quat.z, quat.w)
     f.euler = quat.to_euler()
     self.frames.append(f)
  currentScene.frame_set(startFrame)

class kgmObstacle:
 def __init__(self, o):
  mesh = o.data
  mtx = o.matrix_local
  self.name = mesh.name
  self.faces = []

  mesh_faces = None

  if hasattr(mesh, 'faces'):
    mesh_faces = mesh.faces
  elif hasattr(mesh, 'polygons'):
    mesh_faces = mesh.polygons

  if mesh_faces is not None:
    for f in mesh_faces:
     iface = []

     for v in f.vertices:
      c = mtx * mesh.vertices[v].co
      iface.append(c)

     self.faces.append(iface)

class kgmProxy:
 def __init__(self, o):
  self.mtx    = o.matrix_local
  self.name   = o.name
  self.type   = o.type
  self.object = o.proxy.name
  self.pos    = self.mtx.to_translation()
  self.quat   = self.mtx.to_quaternion()
  self.euler  = self.mtx.to_euler()

class kgmVisual:
 def __init__(self, o):
  self.mtx      = o.matrix_local
  self.name     = o.name
  self.type     = ''
  self.data     = ''
  self.pos      = self.mtx.to_translation()
  self.rot      = self.mtx.to_euler()
  self.quat     = self.mtx.to_quaternion()
  self.material = ''

  if len(o.material_slots) > 0:
    self.material = o.material_slots[0].name

  if o.type == 'MESH':
    self.type = 'Mesh'
    self.data = o.data.name

class KgmOperator(bpy.types.Operator):
  bl_label = "Kgm Operator"
  bl_idname = "object.kgm_operator"

  kgm_logic = bpy.props.StringProperty(name="AI")
  kgm_state = bpy.props.StringProperty(name="State")

  def execute(self, context):
    self.report({'INFO'}, self.kgm_state)
    self.report({'INFO'}, self.kgm_logic)
    return {'FINISHED'}


class kgmPanel(bpy.types.Panel):
  """kgm object panel."""
  bl_label = "Kgm Object Panel"
  bl_idname = "OBJECT_PT_KGM"
  bl_space_type = "PROPERTIES"
  bl_region_type = "WINDOW"
  bl_context = "object"

  bpy.types.Object.kgm_state  = bpy.props.StringProperty(name = "kgm_state",  default = "None")
  bpy.types.Object.kgm_object = bpy.props.StringProperty(name = "kgm_object", default = "None")
  bpy.types.Object.kgm_player = bpy.props.BoolProperty(name = "kgm_player", default = False)

  def draw(self, context):
    obj = context.object

    if hasattr(obj, 'kgm_dummy') and obj.kgm_dummy is True:
      self.draw_dummy(context)
    elif hasattr(obj, 'kgm_unit') and obj.kgm_unit is True:
      self.draw_unit(context)
    elif hasattr(obj, 'kgm_actor') and obj.kgm_actor is True:
      self.draw_actor(context)

  def draw_dummy(self, context):
    obj = context.object
    layout = self.layout
    row = layout.row()
    row.label(text = "Dummy", icon = "WORLD_DATA")

  def draw_unit(self, context):
    obj = context.object
    layout = self.layout
    row = layout.row()
    row.label(text = "Unit", icon = "WORLD_DATA")
    row = layout.row()
    row.prop(obj, 'kgm_logic')
    row = layout.row()
    row.prop(obj, 'kgm_state')

  def draw_actor(self, context):
    obj = context.object
    layout = self.layout
    row = layout.row()
    row.label(text = "Actor", icon = "WORLD_DATA")
    row = layout.row()
    row.prop(obj, 'kgm_object')

    #props = layout.operator(KgmOperator.bl_idname)
    #props.kgm_logic = obj.kgm_unit_logic
    #props.kgm_state = obj.kgm_unit_state

class kgmExport(bpy.types.Operator):
 '''This appiers in the tooltip of '''
 bl_idname = "export_scene.kgm" # this is important since its how bpy.ops.export.kgm_export() is constructed
 bl_label = "Export Kgm"

 # List of operator properties, the attributes will be assigned
 # to the class instance from the operator settings before calling.

   # TODO, add better example props
 filepath = StringProperty(name="File Path", description="File path used for exporting the Kgm file", maxlen=1024, default="~/")
   #use_setting = BoolProperty(name="Example Boolean", description="Example Tooltip", default= True)
 exp_meshes = BoolProperty(name="Export Meshes", description="", default=True)
 exp_lights = BoolProperty(name="Export Lights", description="", default=False)
 exp_materials = BoolProperty(name="Export Materials", description="", default=False)
 exp_cameras = BoolProperty(name="Export Cameras", description="", default=False)
 exp_armatures = BoolProperty(name="Export Armatures", description="", default=False)
 exp_animations = BoolProperty(name="Export Animations", description="", default=False)
 exp_kgmobjects = BoolProperty(name="Export kgmObjects", description="", default=False)
 exp_kgmphysics = BoolProperty(name="Export kgmPhysics", description="", default=False)
 is_selection = BoolProperty(name="Selected only", description="", default=False)
 type = bpy.props.EnumProperty(items=(('OPT_A', "Xml", "Xml format"), ('OPT_B', "Bin", "Binary format")), name="Format", description="Choose between two items", default='OPT_A')


 @classmethod
 def poll(cls, context):
  return context.active_object != None

 def execute(self, context):
 #Bug, currently isnt working
 #if not self.is_property_set("filepath"):
 #raise Exception("filename not set")

  scene = context.scene
  for o in scene.objects:
   print(o.name + ':' + o.type)

  if self.is_selection:
   objects = [ob for ob in scene.objects if ob.is_visible(scene) and ob.select]
  else:
   objects = [ob for ob in scene.objects if ob.is_visible(scene)]

  #cFrame = bpy.context.scene.frame_current

  print("Collect Objects...")

  #meshes     = [kgm_objects.kgmMesh(ob) for ob in objects if ob.type == 'MESH' and self.exp_meshes and ob.collision.use != True and ob.proxy is None]
  materials  = [kgmMaterial(ob) for ob in bpy.data.materials]
  meshes     = [kgmMesh(ob) for ob in bpy.data.meshes]

  visuals    = [kgmVisual(ob) for ob in objects if self.exp_meshes and ob.type == 'MESH' and ob.collision.use != True]
  obstacles  = [kgmObstacle(ob) for ob in objects if ob.type == 'MESH' and self.exp_kgmphysics and ob.collision.use == True]
  lights     = [kgmLight(ob) for ob in objects if ob.type == 'LAMP' and self.exp_lights]
  cameras    = [kgmCamera(ob) for ob in objects if ob.type == 'CAMERA' and self.exp_cameras]
  skeletons  = [kgmSkeleton(ob) for ob in objects if ob.type == 'ARMATURE' and self.exp_armatures]
  animations = []

  if self.exp_animations:
    armatures = [ob for ob in objects if ob.type == 'ARMATURE']
    for armature in armatures:
      print("scan armature")
      for bone in armature.data.bones:
        animations.append(kgmBoneAnimation(bone, armature))

  print("Animations: " + str(len(animations)))
  print("Mehses: "     + str(len(meshes)))
  print("Lights: "     + str(len(lights)))
  print("Materials: "  + str(len(materials)))
  print("Visuals: "    + str(len(visuals)))

#  path = self.filepath
  if not self.filepath.lower().endswith(".kgm"):
   self.filepath += ".kgm"

  file = open(self.filepath, "w")
  file.write("<?xml version='1.0'?>\n")

  #scene data
  frames = bpy.context.scene.frame_end - bpy.context.scene.frame_start + 1
  fstart = bpy.context.scene.frame_start
  fend = bpy.context.scene.frame_end
  file.write("<kgm>\n")

  #animations
  if self.exp_animations:
   file.write(" <kgmAnimation fstart='" + str(fstart) + "' fend='" + str(fend) + "'>\n")
   for o in animations:
    file.write("  <Animation name='" + o.name + "'>\n")
    for f in o.frames:
     file.write("   <Frame key='" + str(f.key) + "' position='" + str("%.5f" % f.pos[0]) + " " + str("%.5f" % f.pos[1]) + " " + str("%.5f" % f.pos[2]) + "'")
     file.write(" quaternion='" + str("%.5f" % f.quat[0]) + " " + str("%.5f" % f.quat[1]) + " " + str("%.5f" % f.quat[2]) + " " + str("%.5f" % f.quat[3]) + "'")
     file.write(" euler='" + str("%.5f" % toGrad(f.euler[0])) + " " + str("%.5f" % toGrad(f.euler[1])) + " " + str("%.5f" % toGrad(f.euler[2])) + "'")
     file.write(" />\n")
    file.write("  </Animation>\n")
   file.write(" </kgmAnimation>\n")

   #materials
  if self.exp_materials:
     for o in materials:
       print("Materials " + str(o.name))
       file.write(" <kgmMaterial name='" + o.name + "'>\n")
       file.write("  <Color value='" + str(o.diffuse[0]) + " " + str(o.diffuse[1]) + " " + str(o.diffuse[2]) + "'/>\n")
       file.write("  <Emmision value='" + str(o.emmision[0]) + " " + str(o.emmision[1]) + " " + str(o.emmision[2]) + "'/>\n")
       file.write("  <Specular value='" + str(o.specular[0]) + " " + str(o.specular[1]) + " " + str(o.specular[2]) + "'/>\n")
       file.write("  <Shininess value='" + str(o.shine) + "'/>\n")

       if o.use_alpha:
         file.write("  <Alpha value='" + str(o.alpha) + "'/>\n")
       if o.map_color != "":
         file.write("  <map_color value='" + o.map_color + "'/>\n")
       if o.map_normal != "":
         file.write("  <map_normal value='" + o.map_normal + "' strength='" + str(o.map_normal_strength) + "' />\n")
       if o.map_specular != "":
         file.write("  <map_specular value='" + o.map_specular + "' strength='" + str(o.map_specular_strength) + "' />\n")
       if(hasattr(o, 'shader')):
         file.write("  <Shader value='" + o.shader + "'/>\n")
       file.write(" </kgmMaterial>\n")

  #meshes
  for o in meshes:
   file.write(" <kgmMesh name='" + o.name + "'>\n")
   if len(o.mtls) > 0:
    file.write("  <Material name='" + o.mtls[0] + "' />\n")

   file.write("  <Vertices length='" + str(len(o.vertices)) + "'>\n")
   for v in o.vertices:
    file.write("   " + str(v.v[0]) + " " + str(v.v[1]) + " " + str(v.v[2]))
    file.write(" " + str(v.n[0]) + " " + str(v.n[1]) + " " + str(v.n[2]))
    file.write(" " + str(v.uv[0]) + " " + str(v.uv[1]))
    file.write("\n")
   file.write("  </Vertices>\n")

   file.write("  <Faces length='" + str(len(o.faces)) + "'>\n")
   for f in o.faces:
    file.write("   " + str(f.f[0]) + " " + str(f.f[1]) + " " + str(f.f[2]) + "\n")
   file.write("  </Faces>\n")

   if o.hasvgroups == True:
    file.write("  <Skin length='" + str(len(o.vertices)) + "'>\n")
    for v in o.vertices:
     file.write("   " + str(v.ib[0]) + " " + str(v.ib[1]) + " " + str(v.ib[2]) + " " + str(v.ib[3]))
     file.write(" " + str(v.wb[0]) + " " + str(v.wb[1]) + " " + str(v.wb[2]) + " " + str(v.wb[3]))
     file.write("\n")
    file.write("  </Skin>\n")
   file.write(" </kgmMesh>\n")

  #skeletons
  for s in skeletons:
   file.write(" <kgmSkeleton name='" + s.name +
              "' position='" + str("%.5f" % s.pos.x) + " " + str("%.5f" % s.pos.y) + " " + str("%.5f" % s.pos.z) +
              "' quaternion='" + str("%.5f" % s.quat.x) + " " + str("%.5f" % s.quat.y) + " " + str("%.5f" % s.quat.z) + " " + str("%.5f" % s.quat.w) +
#              "' euler='" + str(toGrad(s.euler.x)) + " " + str(toGrad(s.euler.y)) + " " + str(toGrad(s.euler.z)) +
              "' scale='" + str("%.5f" % s.scale.x) + " " + str("%.5f" % s.scale.y) + " " + str("%.5f" % s.scale.z) +
              "'>\n")
   for b in s.bones:
    file.write("  <Bone name='" + b.name + "' parent='" + b.parent + "'")
    file.write(" position='" + str("%.5f" % b.pos.x) + " " + str("%.5f" % b.pos.y) + " " + str("%.5f" % b.pos.z) + "'")
    #file.write(" rotation='" + str(b.rot.x) + " " + str(b.rot.y) + " " + str(b.rot.z) + "'")
    file.write(" quaternion='" + str("%.5f" % b.quat[1]) + " " + str("%.5f" % b.quat[2]) + " " + str("%.5f" % b.quat[3]) + " " + str("%.5f" % b.quat[0]) + "'")
    file.write(" euler='" + str("%.5f" % toGrad(b.euler.x)) + " " + str("%.5f" % toGrad(b.euler.y)) + " " + str("%.5f" % toGrad(b.euler.z)) + "'")
    #file.write(" euler='" + str(b.euler.x) + " " + str(b.euler.y) + " " + str(b.euler.z) + "'")
    file.write("/>\n")
   file.write(" </kgmSkeleton>\n")

#lights
  for o in lights:
    file.write(" <kgmLight name='" + o.name + "'>\n")
    file.write("  <Type value='" + o.type + "'/>\n")
    file.write("  <Color value='" + str(o.color[0]) + " " + str(o.color[1]) + " " + str(o.color[2]) + "'/>\n")
    file.write("  <Position value='" + str(o.pos[0]) + " " + str(o.pos[1]) + " " + str(o.pos[2]) + "'/>\n")
    file.write("  <Rotation value='" + str(o.rot[0]) + " " + str(o.rot[1]) + " " + str(o.rot[2]) + "'/>\n")
    file.write("  <Intensity value='" + str(o.intensity) + "'/>\n")
    file.write("  <Shadows value='" + str(o.shadows) + "'/>\n")
    file.write(" </kgmLight>\n")

#cameras
  for o in cameras:
    file.write(" <kgmCamera name='" + o.name + "'>\n")
    file.write("  <Position value='" + str(o.pos[0]) + " " + str(o.pos[1]) + " " + str(o.pos[2]) + "'/>\n")
    file.write("  <Rotation value='" + str(o.rot[0]) + " " + str(o.rot[1]) + " " + str(o.rot[2]) + "'/>\n")
    file.write("  <Clip angle='" + str(o.angle) + "' zfar='" + str(o.far) + "' znear='" + str(o.near) + "'/>\n")
    file.write(" </kgmCamera>\n")

#visuals
  for o in visuals:
    if o.type != '':
      file.write(" <kgmVisual name='" + o.name + "' >\n")
      file.write("  <Position value='" + str(o.pos[0]) + " " + str(o.pos[1]) + " " + str(o.pos[2]) + "'/>\n")
      file.write("  <Rotation value='" + str(o.rot[0]) + " " + str(o.rot[1]) + " " + str(o.rot[2]) + "'/>\n")
      file.write("  <Quaternion value='" + str(o.quat[1]) + " " + str(o.quat[2]) + " " + str(o.quat[3]) + " " + str(o.quat[0]) + "'/>\n")

      if o.type is 'Mesh':
        file.write("  <Mesh id='" + o.data + "'/>\n")

      if o.material != '':
        file.write("  <Material id='" + o.material + "'/>\n")
      file.write(" </kgmVisual>\n")

  #collisions
  for obstacle in obstacles:
   file.write(" <kgmObstacle polygons='" + str(len(obstacle.faces)) + "'>\n")
   for face in obstacle.faces:
     file.write("  <Polygon vertices='" + str(len(face)) + "'>\n")
     for v in face:
      file.write("   " + str(v[0]) + " " + str(v[1]) + " " + str(v[2]) + "\n")
     file.write("  </Polygon>\n")
   file.write(" </kgmObstacle>\n")

  file.write("</kgm>\n")
  file.close()
  return {'FINISHED'}

 def invoke(self, context, event):
  wm = context.window_manager

  if True:
   # File selector
   wm.fileselect_add(self) # will run self.execute()
   return {'RUNNING_MODAL'}
  elif True:
   # search the enum
   wm.invoke_search_popup(self)
   return {'RUNNING_MODAL'}
  elif False:
   # Redo popup
   return wm.invoke_props_popup(self, event) #
  elif False:
   return self.execute(context)

def menu_func(self, context):
  self.layout.operator(kgmExport.bl_idname, text="Karakal game (.kgm)", icon='NONE')

def register():
  bpy.utils.register_module(__name__)
  bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
  bpy.utils.unregister_module(__name__)
  bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
  register()
