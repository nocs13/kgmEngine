import os
from math import radians
import bpy
from mathutils import *

class kgm_dummy(bpy.types.Operator):
  ''' Add kgmDummy '''
  bl_idname = "object.kgm_dummy"
  bl_label = "Add kgmDummy"
  bl_options = {'REGISTER', 'UNDO'}

  bpy.types.Object.kgm_dummy = bpy.props.BoolProperty(name = "kgm_dummy", default = False)

  def __init__(self):
    print("start")

  def __del__(self):
    print("end")

  def execute(self, context):
    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmDummy"
    a.kgm_dummy = True
    return {'FINISHED'}

  def modal(self, context, event):
    print("Modal dummy.\n")
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmDummy"
    a.kgm_dummy = True
    return {'RUNNING_MODAL'}

  def draw(self, context):
    layout = self.layout

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

  if hasattr(mesh, 'faces'):
    for i in range(0, len(mesh.faces)):
     face = mesh.faces[i]
     iface = []

     for j in range(0, len(face.vertices)):
      vi = face.vertices[j]
      c = mtx * mesh.vertices[vi].co
      iface.append(c)

     self.faces.append(iface)

class kgmProxy:
 def __init__(self, o):
  self.mtx    = o.matrix_local
  self.name   = o.name
  self.type   = o.type
  self.object = o.proxy.name
  self.pos    = Vector((0, 0, 0)) * self.mtx
  self.quat   = self.mtx.to_quaternion()
  self.euler  = self.mtx.to_euler()

class kgmVisual:
 def __init__(self, o):
  self.mtx      = o.matrix_local
  self.name     = o.name
  self.type     = ''
  self.data     = ''
  self.pos      = Vector((0, 0, 0)) * self.mtx
  self.rot      = self.mtx.to_euler()
  self.quat     = self.mtx.to_quaternion()
  self.material = ''

  if len(o.material_slots) > 0:
    self.material = o.material_slots[0].name

  if o.type == 'MESH':
    self.type = 'Mesh'
    self.data = o.data.name

import threading
class kgmThread(threading.Thread):
  def __init__(self, c):
    self.context = c

  def run(self):
    pass

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
  bpy.types.Object.kgm_units  = bpy.props.EnumProperty(name = "Units", items=Units)

  def draw(self, context):
    obj = context.object

    if hasattr(obj, 'kgm_dummy') and obj.kgm_dummy is True:
      self.draw_dummy(context)
    elif hasattr(obj, 'kgm_obstacle') and obj.kgm_obstacle is True:
      self.draw_obstacle(context)

  def draw_dummy(self, context):
    obj = context.object
    layout = self.layout
    row = layout.row()
    row.label(text = "Dummy", icon = "WORLD_DATA")

  def draw_obstacle(self, context):
    obj = context.object
    layout = self.layout
    row = layout.row()
    row.label(text = "Obstacle", icon = "WORLD_DATA")
    row = layout.row()
    row.prop(obj, "kgm_surface")
