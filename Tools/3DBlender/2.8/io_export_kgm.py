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
    "blender": (2, 80, 0),
    "location": "File > Export",
    "description": "Export to the karakal game map file (.kgm)",
    "warning": "",
    "category": "Import-Export"
}

import os
import threading
from math import radians
import bpy
from mathutils import *
import xml.etree.ElementTree


def toGrad(a):
  return a * 180.0 / 3.1415


def toSnum(a):
  return str("%.6f" % a)


kgm_animaniacs = [( "Unit",    "Unit",    "kgmUnit"    ),
                  ( "Actor",   "Actor",   "kgmActor"   )]



kgm_project_parameters = ''
kgm_unit_types = [('a1', 'a1 a1', 'a1 a1 a1'), ('a2', 'a2 a2', 'a2 a2 a2')]
kgm_actor_types = [('u1', 'a1 u1', 'a1 a1 u1'), ('u2', 'a1 u2', 'a1 a1 u2')]

def hasModifier(o, mname):
  if o is None:
    return False

  if len(o.modifiers) < 1:
    return False

  for m in o.modifiers:
    if m.name == mname:
      return True

  return False


def getModifier(o, mname):
  if o is None:
    return None

  if len(o.modifiers) < 1:
    return None

  for m in o.modifiers:
    if m.name == mname:
      return m

  return None


def choose_unit_types(self, context):
  print('choosing unit type')
  o = bpy.context.active_object

  if o.kgm_type is 'Actor':
    print('is actor')
    return kgm_actor_types

  print('is unit')
  return kgm_unit_types


class kgm_unit_settings(bpy.types.PropertyGroup):
  kgm_unit = bpy.props.BoolProperty()
  kgm_player = bpy.props.BoolProperty()
  kgm_type = bpy.props.EnumProperty(name='kgm unit', items=kgm_animaniacs, description='select kgm unit')
  kgm_state = bpy.props.StringProperty()
  kgm_object = bpy.props.StringProperty()


class kgm_panel(bpy.types.Panel):
  bl_idname = "KGM_PANEL"
  bl_label  = "kgm_panel"
  bl_space_type = 'VIEW_3D'
  bl_region_type = 'TOOLS'
  bl_category = 'CTools'
  #bl_context = "object"

  @classmethod
  def poll(cls, context):
    return (context.object is not None)

  def draw_header(self, context):
    layout = self.layout

  def draw(self, context):
    layout = self.layout
    obj = context.object

    obj = context.object

    #layout.label(text="Export/Import")
    #row = layout.row()
    #box = layout.box()

    #col = box.column(align=True)
    #col.operator(operator='export_scene.kgm_group_export', text='export group')
    #col.operator(operator='export_scene.kgm',              text='export scene')
    #col.operator(operator='import_scene.kgm_settings',     text='import setting')

    if hasattr(obj, 'kgm_unit') and obj.kgm_unit is True:
      self.draw_unit(obj)
    elif hasattr(obj, 'kgm_dummy') and obj.kgm_unit is True:
      self.draw_dummy(obj)
    else:
      pass
      #self.draw_none()

  def draw_none(self):
    print('draw_none')
    pass

  def draw_unit(self, obj):
    print('draw_unit')
    layout = self.layout
    layout.label(text="kgm_unit")

    scene = bpy.context.scene

    row = layout.row()
    row.prop(obj, "kgm_type")
    row = layout.row()
    row.prop(obj, "kgm_state")
    row = layout.row()
    row.prop(obj, "kgm_player")
    row = layout.row()
    row.prop_search(obj, "kgm_capture", scene, "objects")

  def draw_dummy(self, obj):
    print('draw_dummy')
    layout = self.layout
    layout.label(text="kgm_dummy")


class kgm_settings(bpy.types.Operator):
  ''' Import kgm settings '''
  bl_label = "kgm settings"
  bl_idname = 'import_scene.kgm_settings'

  def draw(self, context):
    pass


class kgm_menu(bpy.types.Menu):
  bl_label = "kgm Menu"
  bl_idname = 'view3D.kgm_menu'

  def draw(self, context):
    layout = self.layout
    op = layout.operator("import_scene.kgm_settings")


class kgm_unit(bpy.types.Operator):
  ''' Add kgmObject '''
  bl_idname = "object.kgm_unit"
  bl_label = "Add kgmUnit"
  bl_options = {'REGISTER'}  # , 'UNDO'

  width = bpy.props.FloatProperty(
    name="Width",
    description="Box Width",
    min=0.01, max=100.0,
    default=1.0,
  )

  def __init__(self):
    print("start")

  def __del__(self):
    print("end")

  def execute(self, context):
    print("Execute unit.\n")

    bpy.types.Object.kgm_unit = bpy.props.BoolProperty()
    bpy.types.Object.kgm_type = bpy.props.StringProperty()
    bpy.types.Object.kgm_state = bpy.props.StringProperty()
    bpy.types.Object.kgm_player = bpy.props.BoolProperty()

    bpy.types.Object.kgm_capture = bpy.props.StringProperty()

    bpy.ops.object.add()
    a = bpy.context.object

    a.kgm_unit = True

    a.name = "kgmUnit"
    a.kgm_player = False
    a.kgm_type = "kgmUnit";
    a.kgm_state = "Idle"
    return {'FINISHED'}

  def modal(self, context, event):
    print("Modal object \n")
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    print("Invoke unit.\n")

    bpy.types.Object.kgm_unit = bpy.props.BoolProperty()
    bpy.types.Object.kgm_type = bpy.props.StringProperty()
    bpy.types.Object.kgm_state = bpy.props.StringProperty()
    bpy.types.Object.kgm_player = bpy.props.BoolProperty()

    bpy.types.Object.kgm_capture = bpy.props.StringProperty()

    bpy.ops.object.add()
    a = bpy.context.object

    a.kgm_unit = True

    a.name = "kgmUnit"
    a.kgm_player = False
    a.kgm_type = "kgmUnit";
    a.kgm_state = "Idle"
    return {'FINISHED'}

  def draw(self, context):
    print('draw kgm_unit')


class kgm_dummy(bpy.types.Operator):
  ''' Add kgmDummy '''
  bl_idname = "object.kgm_dummy"
  bl_label = "Add kgmDummy"
  bl_options = {'REGISTER', 'UNDO'}

  width = bpy.props.FloatProperty(
    name="Width",
    description="Box Width",
    min=0.01, max=100.0,
    default=5.0,
  )

  def __init__(self):
    print("start")

  def __del__(self):
    print("end")

  def execute(self, context):
    print("Execute dummy.\n")

    bpy.types.Object.kgm_dummy = bpy.props.BoolProperty(options={'HIDDEN'})

    bpy.ops.object.add()
    a            = bpy.context.object
    a.name       = "kgmDummy"
    a.kgm_dummy = True

    return {'FINISHED'}

  def modal(self, context, event):
    print("Modal dummy.\n")
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    print("Invoke dummy.\n")

    bpy.types.Object.kgm_dummy = bpy.props.BoolProperty(options={'HIDDEN'})

    bpy.ops.object.add()
    a            = bpy.context.object
    a.name       = "kgmDummy"
    a.kgm_dummy = True

    return {'FINISHED'}

  def draw(self, context):
    layout = self.layout

class kgm_sensor(bpy.types.Operator):
  ''' Add kgmSensor '''
  bl_idname = "object.kgm_sensor"
  bl_label = "Add kgmSensor"
  bl_options = {'REGISTER', 'UNDO'}

  width = bpy.props.FloatProperty(
    name="Width",
    description="Box Width",
    min=0.01, max=100.0,
    default=5.0,
  )

  def __init__(self):
    print("kgm_sensor start")

  def __del__(self):
    print("kgm_sensor end")

  def execute(self, context):
    print("Execute sensor.\n")

    bpy.types.Object.kgm_sensor = bpy.props.BoolProperty(options={'HIDDEN'})

    bpy.ops.object.add()

    a            = bpy.context.object
    a.name       = "kgmSensor"
    a.kgm_sensor = True

    return {'FINISHED'}

  def modal(self, context, event):
    print("Modal sensor.\n")
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    print("Invoke sensor.\n")

    bpy.types.Object.kgm_sensor = bpy.props.BoolProperty(options={'HIDDEN'})

    bpy.ops.object.add()

    a            = bpy.context.object
    a.name       = "kgmSensor"
    a.kgm_sensor = True

    return {'FINISHED'}

  def draw(self, context):
    layout = self.layout

class kgm_trigger(bpy.types.Operator):
  ''' Add kgmSensor '''
  bl_idname = "object.kgm_trigger"
  bl_label = "Add kgmTrigger"
  bl_options = {'REGISTER', 'UNDO'}

  width = bpy.props.FloatProperty(
    name="Width",
    description="Box Width",
    min=0.01, max=100.0,
    default=5.0,
  )

  def __init__(self):
    print("kgm_trigger start")

  def __del__(self):
    print("kgm_trigger end")

  def execute(self, context):
    print("Execute trigger.\n")

    bpy.types.Object.kgm_trigger = bpy.props.BoolProperty(options={'HIDDEN'})

    bpy.ops.object.add()

    a            = bpy.context.object
    a.name       = "kgmTrigger"
    a.kgm_trigger = True

    return {'FINISHED'}

  def modal(self, context, event):
    print("Modal trigger.\n")
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    print("Invoke trigger.\n")

    bpy.types.Object.kgm_trigger = bpy.props.BoolProperty(options={'HIDDEN'})

    bpy.ops.object.add()

    a            = bpy.context.object
    a.name       = "kgmTrigger"
    a.kgm_trigger = True

    return {'FINISHED'}

  def draw(self, context):
    layout = self.layout

scene_materials = []


class kgmMaterial:
  def __init__(self, mtl):
    self.name     = mtl.name

    self.diffuse  = [1.0, 1.0, 1.0, 1.0]
    self.specular = [1.0, 1.0, 1.0, 1.0]
    self.shine    = 1.0
    self.alpha    = 1.0
    self.map_color = ''
    self.map_normal = ''
    self.map_specular = ''
    self.map_environment = False
    self.map_normal_strength = 1.0
    self.map_specular_strength = 1.0
    self.images = ()
    self.depth = True
    self.blend = None
    self.shader = ''
    self.env_source = 'Static'
    self.env_mapping = 'Cube'
    self.env_viewpoint = ''
    self.env_image = ''
    self.env_intensity = 0.0
    self.sh_off  = False
    self.sh_cast = False
    self.sh_recv = False
    self.mir_fresnel = 0.0;
    self.trn_fresnel = 0.0;
    self.tex_move    = None
    self.distortion  = 0.0

    self.diffuse  = mtl.diffuse_color
    self.specular = mtl.specular_color * mtl.specular_intensity
    self.shine    = mtl.specular_intensity * mtl.metallic
    self.alpha    = 1.0

    if mtl.shadow_method is not "NONE":
      self.sh_off  = True
      self.sh_recv = True

    if mtl.blend_method == 'OPAQUE':
      pass
    elif mtl.blend_method == 'BLEND':
      self.blend = 'Mix'

    if mtl.use_nodes is True:
      for x in mtl.node_tree.nodes:
        if x.name == 'Image Texture' and x.image is not None:
          if (len(x.outputs) > 0 and x.outputs[0] is not None) and len(x.outputs[0].links) > 0:
            for lk in x.outputs[0].links:
              if lk.to_socket is not None and lk.to_socket.name == 'Base Color':
                self.map_color = x.image.name
              elif lk.to_socket is not None and lk.to_socket.name == 'Normal':
                self.map_normal = x.image.name
              elif lk.to_socket is not None and lk.to_socket.name == 'Specular':
                self.map_specular = x.image.name

        '''elif TextureSlot.texture.type == "DISTORTED_NOISE":
          self.distortion = TextureSlot.texture.distortion
        elif TextureSlot.texture.type == "ENVIRONMENT_MAP":
          offset = TextureSlot.offset
          self.map_environment = True
          self.env_intensity = TextureSlot.diffuse_factor
          self.tex_move = str(offset[0]) + " " + str(offset[1]) + " " + str(offset[2])
          print("env Mapping is " + TextureSlot.texture.environment_map.mapping)
          if TextureSlot.texture.environment_map.mapping == "PLANE":
            self.env_mapping = 'Plane'
            self.env_viewpoint = TextureSlot.texture.environment_map.viewpoint_object

          if TextureSlot.texture.environment_map.source == "ANIMATED":
            self.env_source = 'Animated'
          elif TextureSlot.texture.environment_map.source == "IMAGE_FILE":
            self.env_source = 'Image'
            if TextureSlot.texture.image is not None:
              self.env_image = TextureSlot.texture.image.name'''


class kgmLight:
  def __init__(self, o):
    lamp = o.data
    self.name = o.name
    self.pos = o.matrix_local.to_translation()
    self.rot = o.matrix_local.to_euler()
    self.intensity = lamp.energy
    self.shadows = 'No' if lamp.shadow_method == 'NOSHADOW' else 'Yes'
    self.range = 2 * lamp.distance
    self.color = lamp.color

    self.type = 'Point'

    if lamp.type == 'SPOT':
      self.type = 'Spot'
    elif lamp.type == 'SUN':
      self.type = 'Sun'
      self.intensity = -1
    else:
      self.type = 'Point'

    if lamp.type == 'SPOT':
      end = o.matrix_world * Vector((0, 0, -lamp.shadow_buffer_clip_end))
      nor = end - o.matrix_world.to_translation()
      nor.normalize()
      self.dir = nor
      self.angle = lamp.spot_size


class kgmCamera:
  def __init__(self, o):
    self.name = o.name
    self.pos = o.matrix_local.to_translation()
    self.rot = o.matrix_local.to_euler()
    self.angle = o.data.angle
    self.near = o.data.clip_start
    self.far = o.data.clip_end


class kgmBone:
  def __init__(self, bone, aname):
    self.parent = bone.parent.name if bone.parent else 'None'
    self.name = bone.name
    self.mtx = bone.matrix_local

    # take bone armature relative position
    self.pos = bone.matrix_local.to_translation()
    self.quat = bone.matrix_local.to_quaternion()
    self.euler = self.quat.to_euler()

    # take bone reltive to parent position
    pose_bone = bpy.context.scene.objects[aname].pose.bones[bone.name]
    if bone.parent:
      mtx_bone = pose_bone.parent.matrix.inverted()
    else:
      mtx_bone = Matrix()
    mtx_bone *= pose_bone.matrix
    mtx_bone = pose_bone.matrix
    self.pos = mtx_bone.to_translation()
    self.quat = mtx_bone.to_quaternion()
    self.euler = mtx_bone.to_euler()


class kgmSkeleton:
  def __init__(self, o):
    self.name = o.name
    self.bones = []

    # if bpy.ops.object.mode_set.poll():
    # bpy.ops.object.mode_set(mode='POSE')

    self.arm = armature = o.data
    self.mtx = o.matrix_local
    self.pos = o.matrix_local.to_translation()  # * mathutils.Vector((0, 0, 0))
    self.quat = o.matrix_local.to_quaternion()
    self.euler = o.matrix_local.to_euler()
    self.scale = o.matrix_local.to_scale()

    list = [bone for bone in armature.bones if bone.parent == None]

    for o in list:
      self.collect(o)

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
    #  mesh = o.to_mesh(bpy.context.scene, False, "PREVIEW")
    mesh = o.data
    mtx  = o.matrix_local

    self.position = mtx.to_translation()
    self.rotation = mtx.to_euler()
    self.scale    = mtx.to_scale()

    self.mtl_name = ""

    if len(mesh.materials) > 0:
        self.mtl_name = mesh.materials[0].name

    print('Current mesh: ' + mesh.name)
    self.name = mesh.name
    self.vertices = []
    self.faces = []
    self.skin = False
    self.mtls = []
    self.hasvgroups = True if len(o.vertex_groups) > 0 else False

    if self.hasvgroups:
      self.vgroups = o.vertex_groups
    else:
      self.vgroups = None

    armatures = [modifier for modifier in o.modifiers if modifier.type == 'ARMATURE']

    if armatures:
      self.armature = armatures[0].object
      self.bones = self.armature.data.bones

    for m in mesh.materials:
      b = False
      self.mtls.append(m.name)

      for n in scene_materials:
        if n.name == m.name:
          b = True
          break

      if b == False:
        scene_materials.append(kgmMaterial(m))
        print("add material to scene_materials")

    self.smooth = kgmMesh.isSmoothed(mesh)

    if self.smooth is True:
      print('Current mesh is smoothed.')
    else:
      print('Current mesh is not smoothed.')

    mesh_faces = None

    mesh_faces = mesh.polygons

    if mesh_faces:
      print('Faces: ' + str(len(mesh_faces)))

      for i in range(0, len(mesh_faces)):
        face = mesh_faces[i]
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
                v.ib[g] = self.getBoneIndex(self.vgroups[mesh.vertices[vi].groups[g].group].name)
                v.wb[g] = mesh.vertices[vi].groups[g].weight

          if hasattr(mesh, 'uv_layers'):
            uv = mesh.uv_layers.active.data[face.loop_start + j].uv
            v.uv = [uv[0], uv[1]]

          v.idx = vi

          iface.append(self.addVertex(v))

        for k in range(2, len(iface)):
          self.faces.append(kgmFace(iface[0], iface[k - 1], iface[k]))

  def addVertex(self, vx):
    iv = -1
    nx = Vector((vx.n[0], vx.n[1], vx.n[2]))

    # check if such vertex already exist

    for i in range(0, len(self.vertices)):
      v = self.vertices[i]
      if (v.v[0] == vx.v[0]) and (v.v[1] == vx.v[1]) and (v.v[2] == vx.v[2]):
        if (v.uv[0] == vx.uv[0]) and (v.uv[1] == vx.uv[1]):
          nc = Vector((v.n[0], v.n[1], v.n[2]))

          if self.smooth is False:
            if nx == nc:
              iv = i
              break
          else:
            if nc.dot(nx) > 0.866:
              n = nc + nx
              n.normalize()
              v.n = n

              self.vertices[i] = v
              iv = i
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
        boneInfluences = [poseBones[o.vertex_groups[group.group].name] for group in v.goups if
                          o.vertex_groups[group.group].name in poseBones]
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

  @staticmethod
  def isSmoothed(m):
    s = 0

    if m is None:
      return False

    for f in m.polygons:
      if f.use_smooth == True:
        s = s + 1
      else:
        s = s - 1

    if s > 0:
      return True

    return False





class kgmFrame:
  def __init__(self, t, x, y, z, rx, ry, rz, rw):
    self.key = t
    self.pos = [x, y, z]
    self.quat = [rx, ry, rz, rw]
    self.euler = [x, y, z]


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
      #   currentScene.frame_current = frame
      currentScene.frame_set(frame)
      allBones = currentScene.objects[a.name].pose.bones
      for bone in allBones:
        if bone.name == o.name:
          if bone.parent:
            # mtx   = bone.matrix
            mtx = bone.matrix * bone.parent.matrix.inverted()
            # mtx  *= Matrix.Rotation(radians(-90), 4, "X")
            # mtx  *= Matrix.Scale(-1, 4, Vector((0, 1, 0)))
          else:
            mtx = Matrix()
            mtx = bone.matrix
          mtx = bone.matrix
          pos = mtx.to_translation()
          quat = mtx.to_quaternion()
          euler = quat.to_euler()

          f = kgmFrame(frame, pos.x, pos.y, pos.z, quat.x, quat.y, quat.z, quat.w)
          f.euler = quat.to_euler()
          self.frames.append(f)
    currentScene.frame_set(startFrame)

class kgmBaseObject:
  def __init__(self, o):
    self.name = o.name
    self.mtx = o.matrix_world
    self.pos = o.matrix_local.to_translation()
    self.quat = self.mtx.to_quaternion()
    self.euler = self.mtx.to_euler()
    self.linked = 'None'
    self.props = {}

    if o.parent != None:
      self.linked = o.parent.name

class kgmDummy(kgmBaseObject):
  def __init__(self, o):
    super().__init__(o)
    self.kgm_dummy = True

class kgmParticles(kgmBaseObject):
  def __init__(self, o):
    super().__init__(o)

    print("Create kgmParticles object")

    p = o.particle_systems[0]
    ps = p.settings

    tpf = 1.0 / bpy.context.scene.render.fps

    self.count   = ps.count
    self.start   = ps.frame_start * tpf
    self.stop    = ps.frame_end   * tpf
    self.life    = ps.lifetime    * tpf
    self.rlife   = ps.lifetime_random
    self.speed   = ps.normal_factor
    self.size    = ps.particle_size
    self.rsize   = ps.size_random
    self.mass    = ps.mass
    self.dir     = [ps.object_align_factor[0], ps.object_align_factor[1], ps.object_align_factor[2]]
    self.gravity = ps.effector_weights.gravity
    self.bound   = [o.dimensions.x, o.dimensions.y, o.dimensions.z]

    f_r = ps.factor_random

    if f_r > self.speed:
        f_r = self.speed

    self.noise   = f_r / self.speed

    if ps.frame_end == bpy.context.scene.frame_end:
      self.stop = -1

    mesh = o.data

    if len(mesh.materials) > 0:
      self.material = mesh.materials[0].name
    else:
      self.material = 'None'

    print("Particles matrial is " + self.material)


class kgmUnit:
  def __init__(self, o):
    print('Init unit')
    self.name = o.name
    self.gtype = o.get('kgm_type')
    self.state = o.get('kgm_state')
    self.mtx = o.matrix_world
    self.pos = o.matrix_local.to_translation()
    self.quat = self.mtx.to_quaternion()
    self.euler = self.mtx.to_euler()
    #self.size = o.dimensions
    self.size = o.scale * o.empty_draw_size
    self.linked = 'None'
    self.capture = o.kgm_capture
    self.props = {}

    if self.gtype == None:
      self.gtype = "kgmUnit"
    if self.state == None:
      self.state = "Idle"

    if o.parent != None:
      self.linked = o.parent.name

    if o.get('kgm_player'):
      self.player = "on"
    else:
      self.player = "off"

    preds = ['kgm', 'kgm_type', 'kgm_object', 'kgm_player', 'kgm_state']

    for k in o.keys():
      tk = str(k)
      print('object option: ' + tk + '\n')
      # tk.find('kgm_') == -1 or
      if tk.find('_') == 0 or tk in preds:
        print('object option: ' + tk + ' in use\n')
        pass
      else:
        self.props[tk] = o.get(k)


class kgmObstacle:
  def __init__(self, o):
    mesh = o.data
    mtx = o.matrix_local
    self.name = o.name
    self.faces = []
    self.absorption = o.collision.absorption

    if hasattr(mesh, 'polygons'):
      for i in range(0, len(mesh.polygons)):
        face = mesh.polygons[i]
        iface = []

        for j in range(0, len(face.vertices)):
          vi = face.vertices[j]
          c = mtx * mesh.vertices[vi].co
          iface.append(c)

        self.faces.append(iface)


class kgmProxy:
  def __init__(self, o):
    self.mtx = o.matrix_local
    self.name = o.name
    self.type = o.type
    self.object = o.proxy.name
    self.pos = Vector((0, 0, 0)) * self.mtx
    self.quat = self.mtx.to_quaternion()
    self.euler = self.mtx.to_euler()

class kgmTerrain:
  def  __init__(self, o):
    self.mtx = o.matrix_local
    self.name = o.name
    self.type = o.type
    self.object = o.name
    self.pos = Vector((0, 0, 0)) * self.mtx
    self.quat = self.mtx.to_quaternion()
    self.euler = self.mtx.to_euler()
    self.image = ""
    self.strength = 1.0
    self.direction = [0, 0, 0]
    self.width  = o.dimensions[0]
    self.length = o.dimensions[1]
    self.height = o.dimensions[2]
    self.map_bump   = None
    self.map_spec   = None
    self.map_blend  = None
    self.map_colors = list()
    self.uv_scale_u = 1.0
    self.uv_scale_v = 1.0

    disp = getModifier(o, 'Displace')

    if disp is None:
      return

    if disp.texture is not None and disp.texture.image is not None:
      self.image = disp.texture.image.name

    self.strength = disp.strength
    self.direction = disp.direction

    mtl = o.material_slots[0].material

    if mtl is not None:
      for ts in mtl.texture_slots:
        if ts is None:
          break

        print('ts is: ' + str(ts))
        print('ts type is ' + str(ts.texture.type))
        print('ts image is ' + str(ts.texture.image))
        print('ts source is ' + str(ts.texture.image.source))
        print('ts file name is ' + str(ts.texture.image.name))

        if ts.texture is not None and ts.texture.type == 'IMAGE' and ts.texture.image is not None \
           and ts.texture.image.source == "FILE":

          if ts.use_stencil is True and self.map_blend is None:
            self.map_blend = ts.texture.image.name
          elif ts.use_map_specular is True and self.map_spec is None:
            self.map_spec = ts.texture.image.name
          elif ts.use_map_normal is True and self.map_bump is None:
            self.map_bump = ts.texture.image.name
          elif ts.use_map_color_diffuse is True:
            self.map_colors.append(ts.texture.image.name)

            if len(self.map_colors) == 1:
              self.uv_scale_u = ts.scale[0]
              self.uv_scale_v = ts.scale[1]



class kgmThread(threading.Thread):
  def __init__(self, c):
    self.context = c

  def run(self):
    pass


def export_animation(file, o):
  file.write("  <Animation name='" + o.name + "'>\n")
  for f in o.frames:
    file.write("   <Frame key='" + str(f.key) + "' position='" + str("%.5f" % f.pos[0]) + " " + str(
      "%.5f" % f.pos[1]) + " " + str("%.5f" % f.pos[2]) + "'")
    file.write(" quaternion='" + str("%.5f" % f.quat[0]) + " " + str("%.5f" % f.quat[1]) + " " + str(
      "%.5f" % f.quat[2]) + " " + str("%.5f" % f.quat[3]) + "'")
    file.write(" euler='" + str("%.5f" % toGrad(f.euler[0])) + " " + str("%.5f" % toGrad(f.euler[1])) + " " + str(
      "%.5f" % toGrad(f.euler[2])) + "'")
    file.write(" />\n")
  file.write("  </Animation>\n")


def export_material(file, o):
  file.write(" <Material name='" + o.name + "'>\n")
  file.write("  <Color value='" + str("%.5f" % o.diffuse[0]) + " " + str("%.5f" % o.diffuse[1]) + " " + str(
    "%.5f" % o.diffuse[2]) + "'/>\n")
  file.write("  <Specular value='" + str("%.5f" % o.specular[0]) + " " + str("%.5f" % o.specular[1]) + " " + str(
    "%.5f" % o.specular[2]) + "'/>\n")
  file.write("  <Shininess value='" + str("%.5f" % o.shine) + "'/>\n")
  file.write("  <Alpha value='" + str("%.5f" % o.alpha) + "'/>\n")
  file.write("  <Shadow shadeless='" + str(o.sh_off) + "' cast='" + str(o.sh_cast) + "' receive='" + str(o.sh_recv) + "'/>\n")
  file.write("  <Fresnel mirror='" + str(o.mir_fresnel) + "' transparency='" + str(o.trn_fresnel) + "'/>\n")

  if o.depth is False:
    file.write("  <Depth value='0'/>\n")

  if o.blend is not None:
      file.write("  <Blend value='" + str(o.blend.lower()) + "'/>\n")

  if o.tex_move is not None:
    file.write("  <TexMove value='" + str(o.tex_move) + "'/>\n")

  if o.distortion > 0.0:
    file.write("  <Distortion value='" + str(o.distortion) + "'/>\n")

  if (hasattr(o, 'shader')):
    file.write("  <Shader value='" + o.shader + "'/>\n")

  if o.map_color != "":
    file.write("  <map_color value='" + o.map_color + "'/>\n")
  if o.map_normal != "":
    file.write("  <map_normal value='" + o.map_normal + "' strength='" + str(o.map_normal_strength) + "' />\n")
  if o.map_specular != "":
    file.write("  <map_specular value='" + o.map_specular + "' strength='" + str(o.map_specular_strength) + "' />\n")
  if hasattr(o, 'map_environment') and o.map_environment is not False:
    file.write("  <map_environment type='" + o.env_source + "' mapping='" + o.env_mapping + "' viewpoint='" + str(o.env_viewpoint) \
                  + "' image='" + str(o.env_image) + "' intensity='" + toSnum(o.env_intensity) + "' />\n")
  file.write(" </Material>\n")


def export_light(file, o):
  file.write(" <Light name='" + o.name + "'>\n")
  file.write("  <Type value='" + o.type + "'/>\n")
  file.write("  <Color value='" + str(o.color[0]) + " " + str(o.color[1]) + " " + str("%.5f" % o.color[2]) + "'/>\n")
  file.write("  <Position value='" + str("%.5f" % o.pos[0]) + " " + str("%.5f" % o.pos[1]) + " " + str(
    "%.5f" % o.pos[2]) + "'/>\n")
  file.write("  <Rotation value='" + str("%.5f" % o.rot[0]) + " " + str("%.5f" % o.rot[1]) + " " + str(
    "%.5f" % o.rot[2]) + "'/>\n")
  if o.type == 'Spot':
    file.write("  <Direction value='" + str("%.5f" % o.dir[0]) + " " + str("%.5f" % o.dir[1]) + " " + str(
               "%.5f" % o.dir[2]) + "'/>\n")
    file.write("  <Spot value='" + str("%.5f" % o.angle) + "'/>\n")

  file.write("  <Intensity value='" + str("%.5f" % o.intensity) + "'/>\n")
  file.write("  <Shadows value='" + str(o.shadows) + "'/>\n")
  file.write(" </Light>\n")


def export_camera(file, o):
  file.write(" <Camera name='" + o.name + "'>\n")
  file.write("  <Position value='" + str("%.5f" % o.pos[0]) + " " + str("%.5f" % o.pos[1]) + " " + str(
    "%.5f" % o.pos[2]) + "'/>\n")
  file.write("  <Rotation value='" + str("%.5f" % o.rot[0]) + " " + str("%.5f" % o.rot[1]) + " " + str(
    "%.5f" % o.rot[2]) + "'/>\n")
  file.write("  <Clip angle='" + str("%.5f" % o.angle) + "' zfar='" + str("%.5f" % o.far) + "' znear='" + str(
    "%.5f" % o.near) + "'/>\n")
  file.write(" </Camera>\n")


def export_terrain(file, o):
  file.write(" <Terrain name='" + o.name + "' dimensions='" + toSnum(o.width) + " " + toSnum(o.length) + " " + toSnum(o.strength) + "'>\n")
  file.write("  <UVScale u='" + str(toSnum(o.uv_scale_u)) + "' v='" + str(toSnum(o.uv_scale_v)) + "' />\n")
  file.write("  <HeightMap value='" + o.image + "'/>\n")

  if o.map_bump is not None:
    file.write("  <NormalMap value='" + o.map_bump + "'/>\n")

  if o.map_blend is not None:
    file.write("  <BlendMap value='" + o.map_blend + "'/>\n")

  if o.map_spec is not None:
    file.write("  <SpecularMap value='" + o.map_spec + "'/>\n")

  for cm in o.map_colors:
    if cm is not None:
      file.write("  <ColorMap value='" + cm + "'/>\n")

  file.write(" </Terrain>\n")

def export_mesh_data(file, o):
  v = o.position
  r = o.rotation
  s = o.scale

  file.write(" <Mesh name='" + o.name + "'")

  if o.mtl_name != "":
    file.write(" material='" + o.mtl_name + "'")

  file.write(">\n");

  file.write("  <Position value='" + str("%.5f" % v.x) + " " + str("%.5f" % v.y) + " " + str("%.5f" % v.z) + "'/>\n")
  file.write("  <Rotation value='" + str("%.5f" % r.x) + " " + str("%.5f" % r.y) + " " + str("%.5f" % r.z) + "'/>\n")
  file.write("  <Scale value='"    + str("%.5f" % s.x) + " " + str("%.5f" % s.y) + " " + str("%.5f" % s.z) + "'/>\n")

  file.write("  <Vertices length='" + str(len(o.vertices)) + "'>\n")
  for v in o.vertices:
    file.write("   " + str("%.6f" % v.v[0]) + " " + str("%.6f" % v.v[1]) + " " + str("%.6f" % v.v[2]))
    file.write(" " + str("%.6f" % v.n[0]) + " " + str("%.6f" % v.n[1]) + " " + str("%.6f" % v.n[2]))
    file.write(" " + str("%.6f" % v.uv[0]) + " " + str("%.6f" % v.uv[1]))
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
  file.write(" </Mesh>\n")


def export_mesh_node(file, o):
  name = o.name
  mtx = o.matrix_local
  v = mtx.to_translation()
  r = mtx.to_euler()
  s = mtx.to_scale()
  file.write(" <Visual type='mesh' name='" + o.name + "' proxy='" + o.data.name + "'>\n")
  for m in o.data.materials:
    file.write("  <Material name='" + m.name + "'/>\n")

  file.write("  <Position value='" + str("%.5f" % v.x) + " " + str("%.5f" % v.y) + " " + str("%.5f" % v.z) + "'/>\n")
  file.write("  <Rotation value='" + str("%.5f" % r.x) + " " + str("%.5f" % r.y) + " " + str("%.5f" % r.z) + "'/>\n")
  file.write("  <Scale value='"    + str("%.5f" % s.x) + " " + str("%.5f" % s.y) + " " + str("%.5f" % s.z) + "'/>\n")
  file.write(" </Visual>\n")
  pass


def export_sceleton(file, o):
  file.write(" <Skeleton name='" + s.name +
             "' position='" + str("%.5f" % s.pos.x) + " " + str("%.5f" % s.pos.y) + " " + str("%.5f" % s.pos.z) +
             "' quaternion='" + str("%.5f" % s.quat.x) + " " + str("%.5f" % s.quat.y) + " " + str("%.5f" % s.quat.z) + " " + str("%.5f" % s.quat.w) +
             "' scale='" + str("%.5f" % s.scale.x) + " " + str("%.5f" % s.scale.y) + " " + str("%.5f" % s.scale.z) +
             "'>\n")
  for b in s.bones:
    file.write("  <Bone name='" + b.name + "' parent='" + b.parent + "'")
    file.write(" position='" + str("%.5f" % b.pos.x) + " " + str("%.5f" % b.pos.y) + " " + str("%.5f" % b.pos.z) + "'")
    # file.write(" rotation='" + str(b.rot.x) + " " + str(b.rot.y) + " " + str(b.rot.z) + "'")
    file.write(" quaternion='" + str("%.5f" % b.quat[1]) + " " + str("%.5f" % b.quat[2]) + " " + str(
      "%.5f" % b.quat[3]) + " " + str("%.5f" % b.quat[0]) + "'")
    file.write(" euler='" + str("%.5f" % toGrad(b.euler.x)) + " " + str("%.5f" % toGrad(b.euler.y)) + " " + str(
      "%.5f" % toGrad(b.euler.z)) + "'")
    # file.write(" euler='" + str(b.euler.x) + " " + str(b.euler.y) + " " + str(b.euler.z) + "'")
    file.write("/>\n")
  file.write(" </Skeleton>\n")


def export_obstacle(file, o):

  file.write(" <Obstacle polygons='" + str(len(o.faces)) + "' absorption='" + toSnum(o.absorption) + "'>\n")
  for face in o.faces:
    file.write("  <Polygon vertices='" + str(len(face)) + "'>\n")
    for v in face:
      file.write("   " + toSnum(v[0]) + " " + toSnum(v[1]) + " " + toSnum(v[2]) + "\n")
    file.write("  </Polygon>\n")
  file.write(" </Obstacle>\n")


def export_particles(file, o):
  file.write(" <Particles name='" + o.name + "' material='" + o.material + "'>\n")
  file.write("  <Position value='" + str("%.5f" % o.pos[0]) + " " + str("%.5f" % o.pos[1]) + " " + str("%.5f" % o.pos[2]) + "'/>\n")
  file.write("  <Rotation value='" + str("%.5f" % o.euler[0]) + " " + str("%.5f" % o.euler[1]) + " " + str("%.5f" % o.euler[2]) + "'/>\n")
  file.write("  <PrData count='" + str(o.count) + "' speed='" + str("%.5f" % o.speed) + "' noise='" + str("%.5f" % o.noise) + "'/>\n")
  file.write("  <PrData life='" + str("%.5f" % o.life) + "' dlife='" + str("%.5f" % o.rlife) + "'/>\n")
  file.write("  <PrData start='" + str(o.start) + "' stop='" + str(o.stop) + "'/>\n")
  file.write("  <PrData size='" + str("%.5f" % o.size) + "' dsize='" + str("%.5f" % o.rsize) + "'/>\n")
  file.write("  <PrData mass='" + str("%.5f" % o.mass) + "' gravity='" + str("%.5f" % o.gravity) + "'/>\n")
  file.write("  <PrData direction='" + str("%.5f" % o.dir[0]) + " " + str("%.5f" % o.dir[1]) + " " + str("%.5f" % o.dir[2]) + "'/>\n")
  file.write("  <PrData volume='" + str("%.5f" % o.bound[0]) + " " + str("%.5f" % o.bound[1]) + " " + str("%.5f" % o.bound[2]) + "'/>\n")
  file.write(" </Particles>\n")


def export_dummy(file, o):
  d = kgmDummy(o)

  file.write(" <Dummy name='" + d.name + "' parent='" + d.linked + "'>\n")
  file.write("  <Position value='" + toSnum(d.pos[0]) + " " + toSnum(d.pos[1]) + " " + toSnum(d.pos[2]) + "'/>\n")
  file.write("  <Rotation value='" + toSnum(d.euler[0]) + " " + toSnum(d.euler[1]) + " " + toSnum(d.euler[2]) + "'/>\n")
  file.write(" </Dummy>\n")


def export_unit(file, o):
  file.write(" <Unit name='" + o.name + "' type='" + o.gtype + "'>\n")
  file.write("  <Position value='" + toSnum(o.pos[0]) + " " + toSnum(o.pos[1]) + " " + toSnum(o.pos[2]) + "'/>\n")
  file.write("  <Rotation value='" + toSnum(o.euler[0]) + " " + toSnum(o.euler[1]) + " " + toSnum(o.euler[2]) + "'/>\n")
  file.write("  <Dimension value='" + toSnum(o.size[0]) + " " + toSnum(o.size[1]) + " " + toSnum(o.size[2]) + "'/>\n")

  if o.capture is not None and o.capture != "":
    file.write("  <Capture id='" + o.capture + "'/>\n")

  if o.linked is not None and o.linked != "":
    file.write("  <Parent id='" + o.linked + "'/>\n")

  file.write("  <State value='" + o.state + "'/>\n")

  for k in o.props.keys():
   file.write("  <Property id='" + str(k) + "' value='" + str(o.props[k]) + "'/>\n")

  file.write(" </Unit>\n")


def export_kgmobject(file, o):
  if hasattr(o, "kgm_dummy") and o.kgm_dummy is True:
    export_kgmdummy(file, o)
  elif hasattr(o, "kgm_unit") and o.kgm_unit is True:
    export_kgmunit(file, o)


from bpy.props import *
from bpy_extras.io_utils import ImportHelper


class kgmImport(bpy.types.Operator, ImportHelper):
  '''Kgm settings import'''
  bl_idname = "import_scene.kgm_settings"
  bl_label = "Import Kgm settings"

  filename_ext = ".kgs"
  filter_glob = StringProperty(default="*.kgs", maxlen=1024, options={'HIDDEN'}, )

  @classmethod
  def poll(cls, context):
    return context.active_object != None

  def execute(self, context):
    from xml.dom.minidom import parse
    global kgm_project_parameters

    print("Kgm settings file: " + self.filepath)

    try:
      # kgm_project_parameters = xml.etree.ElementTree.parse(self.filepath).getroot()
      kgs_dom = parse(self.filepath)
      kgs_dom_root = kgs_dom.firstChild
      print('kgs_dom' + str(kgs_dom))
      print('kgs_dom root tag ' + str(kgs_dom_root.nodeName))
      for node in kgs_dom_root.childNodes:
        if node.nodeType is xml.dom.Node.ELEMENT_NODE:
          print('kgs nodes ' + str(node))
    except xml.parsers.expat.ExpatError as err:
      print('xml error: ' + str(err))
      return {'FINISHED'}
    except Exception as err:
      print('kgs load error ' + str(err))
      print('cannot load ' + self.filepath)

    kgm_project_parameters = kgs_dom_root
    return {'FINISHED'}

from bpy_extras.io_utils import ExportHelper


class kgmExport(bpy.types.Operator, ExportHelper):
  ''' Kgm scene export '''
  bl_idname = "export_scene.kgm"
  bl_label = "Export Kgm"

  # List of operator properties, the attributes will be assigned
  # to the class instance from the operator settings before calling.

  # TODO, add better example props
  filename_ext = "" #".kgm"
  filter_glob = StringProperty(default="*.*", maxlen=1024, options={'HIDDEN'}, )
  # filepath = StringProperty(name="File Path", description="File path used for exporting the Kgm file", maxlen=1024, default="~/")
  exp_meshes     : BoolProperty(name="Export Meshes", description="", default=True)
  exp_lights     : BoolProperty(name="Export Lights", description="", default=True)
  exp_materials  : BoolProperty(name="Export Materials", description="", default=True)
  exp_cameras    : BoolProperty(name="Export Cameras", description="", default=False)
  exp_armatures  : BoolProperty(name="Export Armatures", description="", default=False)
  exp_animations : BoolProperty(name="Export Animations", description="", default=False)
  exp_particles  : BoolProperty(name="Export Particles", description="", default=False)
  exp_obstacles  : BoolProperty(name="Export Obstacles", description="", default=False)
  exp_kgmobjects : BoolProperty(name="Export Units", description="", default=False)
  is_selection   : BoolProperty(name="Selected only", description="", default=False)

  type = bpy.props.EnumProperty(items=(('OPT_A', "Xml", "Xml format"), ('OPT_B', "Bin", "Binary format")),
                                name="Format", description="Choose between two items", default='OPT_A')

  kgmobjects = []
  mesh_datas = []
  mesh_nodes = []

  @classmethod
  def poll(cls, context):
    return context.active_object != None

  def execute(self, context):
    # Bug, currently isnt working
    # if not self.is_property_set("filepath"):
    # raise Exception("filename not set")

    scene = context.scene
    for o in scene.objects:
      print(o.name + ':' + o.type)

    self.objects = []

    if self.is_selection:
      self.objects = [ob for ob in scene.objects if ob.visible_get() and ob.select]
    else:
      self.objects = [ob for ob in scene.objects if ob.visible_get()]

    objects = self.objects

    #cFrame = bpy.context.scene.frame_current

    self.mesh_datas = []
    self.mesh_nodes = []
    self.obstacles  = []
    self.lights     = []
    self.cameras    = []
    self.skeletons  = []
    self.animations = []
    self.particles  = []
    self.kgmobjects = []
    self.dummies    = []
    self.units      = []
    self.terrain    = None

    print("Collect Objects...")

    print("Filter objects to remove particles")

    for o in self.objects:
      if (o.type == 'MESH') and (len(o.particle_systems) > 0):
        self.particles.append(kgmParticles(o))
        self.objects.remove(o)

    '''meshes     = [kgmMesh(ob) for ob in objects if ob.type == 'MESH' and self.exp_meshes and ob.collision.use != True and ob.proxy is None]
    obstacles  = [kgmObstacle(ob) for ob in objects if ob.type == 'MESH' and self.exp_obstacles and ob.collision.use == True]
    lights     = [kgmLight(ob) for ob in objects if ob.type == 'LAMP' and self.exp_lights]
    cameras    = [kgmCamera(ob) for ob in objects if ob.type == 'CAMERA' and self.exp_cameras]
    skeletons  = [kgmSkeleton(ob) for ob in objects if ob.type == 'ARMATURE' and self.exp_armatures]
    #gobjects   = [kgmObject(ob) for ob in objects if ob.type == 'EMPTY' and self.exp_kgmobjects and ob.get('kgm')]
    animations = []'''

    threads    = list(range(8))
    threads[0] = threading.Thread(target=self.collect_meshes)
    threads[1] = threading.Thread(target=self.collect_obstacles)
    threads[2] = threading.Thread(target=self.collect_lights)
    threads[3] = threading.Thread(target=self.collect_cameras)
    threads[4] = threading.Thread(target=self.collect_skeletons)
    threads[5] = threading.Thread(target=self.collect_animations)
    threads[6] = threading.Thread(target=self.collect_kgmobjects)
    threads[7] = threading.Thread(target=self.collect_materials)

    for thread in threads:
      thread.start()

    for thread in threads:
      thread.join()

    '''if self.exp_animations:
      armatures = [ob for ob in objects if ob.type == 'ARMATURE']
      for armature in armatures:
        print("scan armature")
        for bone in armature.data.bones:
          animations.append(kgmBoneAnimation(bone, armature))'''

    print("Animations: " + str(len(self.animations)))
    print("Objects: " + str(len(self.objects)))
    print("Mehses: " + str(len(self.mesh_nodes)))
    print("Lights: " + str(len(self.lights)))
    print("Materials: " + str(len(scene_materials)))

    # path = self.filepath
    #if not self.filepath.lower().endswith(".kgm"):
    #  self.filepath += ".kgm"

    file = open(self.filepath, "w")
    file.write("<?xml version='1.0'?>\n")

    # scene data
    frames = bpy.context.scene.frame_end - bpy.context.scene.frame_start + 1
    fstart = bpy.context.scene.frame_start
    fend = bpy.context.scene.frame_end
    file.write("<kgm>\n")

    # animations
    if self.exp_animations:
      file.write(" <Animations fstart='" + str(fstart) + "' fend='" + str(fend) + "'>\n")
      for o in self.animations:
        export_animation(file, o)
      file.write(" </Animations>\n")

    # materials
    if self.exp_materials:
      for o in scene_materials:
        export_material(file, o)

    # lights
    for o in self.lights:
      export_light(file, o)

    # cameras
    for o in self.cameras:
      export_camera(file, o)

    # meshes
    if self.exp_meshes:
      if self.terrain is not None:
        print("Export terrain")
        export_terrain(file, self.terrain)

      print("Export mesh library")
      for o in self.mesh_datas:
        export_mesh_data(file, o)

    #print("Export mesh nodes")
    #for o in self.mesh_nodes:
    #  export_mesh_node(file, o)

    # skeletons
    for o in self.skeletons:
      export_sceleton(file, o)

    # particles
    for o in self.particles:
      export_particles(file, o)

    # obstacle
    for o in self.obstacles:
      export_obstacle(file, o)

    for o in self.dummies:
      export_dummy(file, o)

    for o in self.units:
      export_unit(file, o)

    #for o in self.kgmobjects:
    #  export_kgmobject(file, o)

    '''#kgm_objects
    for a in gobjects:
     print('kgmObject: ' + a.name)
     if a.gtype == "actor" or a.gtype == 1:
      file.write(" <kgmActor name='" + a.name + "' object='" + a.gobject + "' parent='" + a.linked + "' player='" + a.player + "'>\n")
      file.write("  <Position value='" + str(a.pos[0]) + " " + str(a.pos[1]) + " " + str(a.pos[2]) + "'/>\n")
      file.write("  <Rotation value='" + str(a.euler[0]) + " " + str(a.euler[1]) + " " + str(a.euler[2]) + "'/>\n")
      file.write("  <State value='" + a.state + "'/>\n")
      for k in a.props.keys():
       file.write("  <" + str(k) + " value='" + str(a.props[k]) + "'/>\n")
      file.write(" </kgmActor>\n")
     elif a.gtype == "dummy" or a.gtype == 0:
      file.write(" <kgmDummy name='" + a.name + "' parent='" + a.linked + "'>\n")
      file.write("  <Position value='" + str(a.pos[0]) + " " + str(a.pos[1]) + " " + str(a.pos[2]) + "'/>\n")
      file.write("  <Rotation value='" + str(a.euler[0]) + " " + str(a.euler[1]) + " " + str(a.euler[2]) + "'/>\n")
      file.write(" </kgmDummy>\n")
     else:
      file.write(" <kgmGameObject name='" + a.name + "' object='" + a.gobject + "' parent='" + a.linked + "'>\n")
      file.write("  <Position value='" + str(a.pos[0]) + " " + str(a.pos[1]) + " " + str(a.pos[2]) + "'/>\n")
      file.write("  <Rotation value='" + str(a.euler[0]) + " " + str(a.euler[1]) + " " + str(a.euler[2]) + "'/>\n")
      file.write(" </kgmGameObject>\n")'''

    file.write("</kgm>\n")
    file.close()
    return {'FINISHED'}

  def invoke(self, context, event):
    wm = context.window_manager

    if True:
      # File selector
      wm.fileselect_add(self)  # will run self.execute()
      return {'RUNNING_MODAL'}
    elif True:
      # search the enum
      wm.invoke_search_popup(self)
      return {'RUNNING_MODAL'}
    elif False:
      # Redo popup
      return wm.invoke_props_popup(self, event)  #
    elif False:
      return self.execute(context)

  def collect_meshes(self):
    if self.exp_meshes is not True:
      print("Export meshes are not enabled.")
      return

    self.mesh_nodes = [ob for ob in self.objects if ob.type == 'MESH' and ob.collision.use != True]
    print("Collected mesh nodes: " + str(len(self.mesh_nodes)))

    terrains = []

    for i in reversed(range(len(self.mesh_nodes))):
      if hasModifier(self.mesh_nodes[i], 'Displace') is True:
        print("Removing mesh: " + self.mesh_nodes[i].name + " as had displace modifier.")
        terrains.append(self.mesh_nodes[i])
        del self.mesh_nodes[i]

    if len(terrains) > 0:
      self.terrain = kgmTerrain(terrains[-1])

    for n in self.mesh_nodes:
      self.mesh_datas.append(kgmMesh(n))

    print("Collected mesh datas: " + str(len(self.mesh_datas)))

  def collect_obstacles(self):
    self.obstacles = [kgmObstacle(ob) for ob in self.objects if
                      ob.type == 'MESH' and self.exp_obstacles and ob.collision.use == True]

  def collect_lights(self):
    self.lights = [kgmLight(ob) for ob in self.objects if ob.type == 'LAMP' and self.exp_lights]

  def collect_cameras(self):
    self.cameras = [kgmCamera(ob) for ob in self.objects if ob.type == 'CAMERA' and self.exp_cameras]

  def collect_skeletons(self):
    self.skeletons = [kgmSkeleton(ob) for ob in self.objects if ob.type == 'ARMATURE' and self.exp_armatures]

  def collect_kgmobjects(self):
    for ob in self.objects:
      #if ((hasattr(ob, "kgm_dummy") and ob.kgm_dummy is True) or (hasattr(ob, "kgm_sensor") and ob.kgm_sensor is True)) \
      #    or (hasattr(ob, "kgm_trigger") and ob.kgm_trigger is True) or (hasattr(ob, "kgm_unit") and ob.kgm_unit is True):
      #  self.kgmobjects.append(ob)
      if (hasattr(ob, "kgm_dummy") and ob.kgm_dummy is True):
        self.dummies.append(kgmDummy(ob))
      elif (hasattr(ob, "kgm_unit") and ob.kgm_unit is True):
        self.units.append(kgmUnit(ob))

  def collect_materials(self):
    for m in bpy.data.materials:
      exist = False
      for o in scene_materials:
        if o.name == m.name:
          exist = True
          break
      if exist is False:
        scene_materials.append(kgmMaterial(m))


  def collect_animations(self):
    if self.exp_animations:
      armatures = [ob for ob in self.objects if ob.type == 'ARMATURE']
      for armature in armatures:
        print("scan armature")
        for bone in armature.data.bones:
          self.animations.append(kgmBoneAnimation(bone, armature))


class kgmExportGroup(bpy.types.Operator, ExportHelper):
  ''' Kgm scene export '''
  bl_idname = "export_shape.kgm"
  bl_label  = "Export Kgm (.kgm)"

  filename_ext  = ".kgm"
  filter_glob   = StringProperty(default="*.kgm", maxlen=1024, options={'HIDDEN'}, subtype='DIR_PATH')
  exp_meshes    = BoolProperty(name="Export Meshes",    description="", default=True)
  exp_materials = BoolProperty(name="Export Materials", description="", default=True)
  exp_obstacles = BoolProperty(name="Export Obstacles", description="", default=False)

  mesh_datas = []

  @classmethod
  def poll(cls, context):
    return context.active_object != None

  def execute(self, context):
    dirpath = os.path.dirname(self.filepath)

    if self.exp_materials is True:
      for m in bpy.data.materials:
        mpath = os.path.join(dirpath, m.name + '.mtl')

        if os.path.isfile(mpath) is True:
          print('File ' + mpath + ' already exists')
          continue

        try:
          file = open(mpath, "w")
          file.write("<?xml version='1.0'?>\n")
          file.write("<kgm>\n")
          mtl = kgmMaterial(m)
          export_material(file, mtl)
          file.write("</kgm>\n")
          file.close()
        except Exception as err:
          print('Cannot export material ' + m.name + ' to ' + mpath)
          print('Error is: ' + str(err))

    if self.exp_meshes is True:
      self.collect_meshes()

      for m in self.mesh_datas:
        mpath = os.path.join(dirpath, m.name + '.msh')

        if os.path.isfile(mpath) is True:
          print('File ' + mpath + ' already exists')
          continue

        try:
          file = open(mpath, "w")
          file.write("<?xml version='1.0'?>\n")
          file.write("<kgm>\n")
          export_mesh_data(file, m)
          file.write("</kgm>\n")
          file.close()
        except Exception as err:
          print('Cannot export material ' + m.name + ' to ' + mpath)
          print('Error is: ' + str(err))

      print('kgm group export started')
      return {'FINISHED'}

  def collect_meshes(self):
    mesh_nodes = [ob for ob in bpy.context.scene.objects if
                  ob.type == 'MESH' and self.exp_meshes and ob.collision.use != True]
    print("Collected mesh nodes: " + str(len(mesh_nodes)))

    for n in mesh_nodes:
      exist = False
      for m in self.mesh_datas:
        if n.name == m.name:
          exist = True
          break
      if exist is False:
        self.mesh_datas.append(kgmMesh(n))
    print("Collected mesh datas: " + str(len(self.mesh_datas)))

class KgmTerrainNodeTree(bpy.types.NodeTree):
    '''Povray Material Nodes'''

    bl_idname = 'KgmTerrainNodeTree'
    bl_label = 'Kgm Terrain Object Nodes'
    bl_icon = 'PLUGIN'

    @classmethod
    def poll(cls, context):
        return context.scene.render.engine == 'EEVEE'

    @classmethod
    def get_from_context(cls, context):
        ob = context.active_object
        if ob and ob.type not in {'LIGHT'}:
            ma = ob.active_material
            if ma is not None:
                nt_name = ma.node_tree
                if nt_name != '':
                    return nt_name, ma, ma
        return (None, None, None)

    def update(self):
        self.refresh = True

def create_kgm_terrain_node_group(context, operator, name):
    bpy.context.scene.use_nodes = True
    group = bpy.data.node_groups.new(name, 'CompositorNodeTree')

    x_pos = -500
    x_len = 150

    g_in = group.nodes.new('NodeGroupInput')
    g_in.location = (x_pos, 0)
    group.inputs.new('NodeSocketColor', 'Layer 1')
    group.inputs.new('NodeSocketColor', 'Layer 2')
    group.inputs.new('NodeSocketColor', 'Layer 3')
    group.inputs.new('NodeSocketColor', 'Layer 4')
    group.inputs.new('NodeSocketColor', 'Mask')

    g_out = group.nodes.new('NodeGroupOutput')
    g_out.location = (500, 0)
    group.outputs.new('NodeSocketColor', 'Result')

    n_srgba = group.nodes.new(type='CompositorNodeSepRGBA')
    x_pos = x_pos + x_len
    n_srgba.location = (x_pos, 0)
    n_srgba.label = "Mask"

    n_mix1 = group.nodes.new(type='CompositorNodeMixRGB')
    x_pos = x_pos + x_len
    n_mix1.location = (x_pos, -200)
    n_mix1.blend_type = 'MULTIPLY'
    n_mix1.label = "Mask R to Layer 1"

    n_mix2 = group.nodes.new(type='CompositorNodeMixRGB')
    n_mix2.location = (x_pos, -100)
    n_mix2.blend_type = 'MULTIPLY'
    n_mix2.label = "Mask G to Layer 2"

    n_mix3 = group.nodes.new(type='CompositorNodeMixRGB')
    n_mix3.location = (x_pos, 100)
    n_mix3.blend_type = 'MULTIPLY'
    n_mix3.label = "Mask B to Layer 3"

    n_mix4 = group.nodes.new(type='CompositorNodeMixRGB')
    n_mix4.location = (x_pos, 200)
    n_mix4.blend_type = 'MULTIPLY'
    n_mix4.label = "Mask A to Layer 4"

    n_mix5 = group.nodes.new(type='CompositorNodeMixRGB')
    x_pos = x_pos + x_len
    n_mix5.location = (x_pos, -100)
    n_mix5.blend_type = 'ADD'
    n_mix5.label = "Layer 1 and Layer 2"

    n_mix6 = group.nodes.new(type='CompositorNodeMixRGB')
    n_mix6.location = (x_pos, 0)
    n_mix6.blend_type = 'ADD'
    n_mix6.label = "Layer 3 and Layer 4"

    n_mix7 = group.nodes.new(type='CompositorNodeMixRGB')
    x_pos = x_pos + x_len
    n_mix7.location = (x_pos, 100)
    n_mix7.blend_type = 'ADD'
    n_mix7.label = "ALayer 1_2 and Layer 3_4"

    link = group.links.new

    link(g_in.outputs[4], n_srgba.inputs[0])

    link(g_in.outputs[0], n_mix1.inputs[1])
    link(g_in.outputs[1], n_mix2.inputs[1])
    link(g_in.outputs[2], n_mix3.inputs[1])
    link(g_in.outputs[3], n_mix4.inputs[1])

    link(n_srgba.outputs[0], n_mix1.inputs[2])
    link(n_srgba.outputs[1], n_mix2.inputs[2])
    link(n_srgba.outputs[2], n_mix3.inputs[2])
    link(n_srgba.outputs[3], n_mix4.inputs[2])

    link(n_mix1.outputs[0], n_mix5.inputs[1])
    link(n_mix2.outputs[0], n_mix5.inputs[2])
    link(n_mix3.outputs[0], n_mix6.inputs[1])
    link(n_mix4.outputs[0], n_mix6.inputs[2])

    link(n_mix5.outputs[0], n_mix7.inputs[1])
    link(n_mix6.outputs[0], n_mix7.inputs[2])

    link(n_mix7.outputs[0], g_out.inputs[0])

    return group

def create_kgm_terrain_node_group(context, operator, name):
  bpy.context.scene.use_nodes = True
  group = bpy.data.node_groups.new(name, 'CompositorNodeTree')

  x_pos = -500
  x_len = 150

  g_in = group.nodes.new('NodeGroupInput')
  g_in.location = (x_pos, 0)
  group.inputs.new('NodeSocketColor', 'Diffuse')
  group.inputs.new('NodeSocketColor', 'Normal')
  group.inputs.new('NodeSocketColor', 'Specular')
  group.inputs.new('NodeSocketFloat', 'Shininess')
  group.inputs.new('NodeSocketFloat', 'Alpha')

  g_out = group.nodes.new('NodeGroupOutput')
  g_out.location = (500, 0)
  group.outputs.new('NodeSocketColor', 'Result')

  n_srgba = group.nodes.new(type='CompositorNodeSepRGBA')
  x_pos = x_pos + x_len
  n_srgba.location = (x_pos, 0)
  n_srgba.label = "Mask"

  n_mix1 = group.nodes.new(type='CompositorNodeMixRGB')
  x_pos = x_pos + x_len
  n_mix1.location = (x_pos, -200)
  n_mix1.blend_type = 'MULTIPLY'
  n_mix1.label = "Mask R to Layer 1"

  n_mix2 = group.nodes.new(type='CompositorNodeMixRGB')
  n_mix2.location = (x_pos, -100)
  n_mix2.blend_type = 'MULTIPLY'
  n_mix2.label = "Mask G to Layer 2"

  n_mix3 = group.nodes.new(type='CompositorNodeMixRGB')
  n_mix3.location = (x_pos, 100)
  n_mix3.blend_type = 'MULTIPLY'
  n_mix3.label = "Mask B to Layer 3"

  n_mix4 = group.nodes.new(type='CompositorNodeMixRGB')
  n_mix4.location = (x_pos, 200)
  n_mix4.blend_type = 'MULTIPLY'
  n_mix4.label = "Mask A to Layer 4"

  n_mix5 = group.nodes.new(type='CompositorNodeMixRGB')
  x_pos = x_pos + x_len
  n_mix5.location = (x_pos, -100)
  n_mix5.blend_type = 'ADD'
  n_mix5.label = "Layer 1 and Layer 2"

  n_mix6 = group.nodes.new(type='CompositorNodeMixRGB')
  n_mix6.location = (x_pos, 0)
  n_mix6.blend_type = 'ADD'
  n_mix6.label = "Layer 3 and Layer 4"

  n_mix7 = group.nodes.new(type='CompositorNodeMixRGB')
  x_pos = x_pos + x_len
  n_mix7.location = (x_pos, 100)
  n_mix7.blend_type = 'ADD'
  n_mix7.label = "ALayer 1_2 and Layer 3_4"

  link = group.links.new

  link(g_in.outputs[4], n_srgba.inputs[0])

  link(g_in.outputs[0], n_mix1.inputs[1])
  link(g_in.outputs[1], n_mix2.inputs[1])
  link(g_in.outputs[2], n_mix3.inputs[1])
  link(g_in.outputs[3], n_mix4.inputs[1])

  link(n_srgba.outputs[0], n_mix1.inputs[2])
  link(n_srgba.outputs[1], n_mix2.inputs[2])
  link(n_srgba.outputs[2], n_mix3.inputs[2])
  link(n_srgba.outputs[3], n_mix4.inputs[2])

  link(n_mix1.outputs[0], n_mix5.inputs[1])
  link(n_mix2.outputs[0], n_mix5.inputs[2])
  link(n_mix3.outputs[0], n_mix6.inputs[1])
  link(n_mix4.outputs[0], n_mix6.inputs[2])

  link(n_mix5.outputs[0], n_mix7.inputs[1])
  link(n_mix6.outputs[0], n_mix7.inputs[2])

  link(n_mix7.outputs[0], g_out.inputs[0])

  return group

class KgmTerrainNodeGroup(bpy.types.Operator):
  """ Kgm Terrain Node """
  bl_label = "KgmTerrainNodeGroup"
  bl_idname = 'node.kgm_terrain_node_group'

  def execute(self, context):
      node_name = 'Kgm Terrain Node Group'
      group = create_kgm_terrain_node_group(self, context, node_name)
      node = context.scene.node_tree.nodes.new('CompositorNodeGroup')
      node.node_tree = bpy.data.node_groups[group.name]
      node.use_custom_color = True
      node.color = (0.3, 0.6, 0.9)

      return {'FINISHED'}

class KgmMaterialNodeGroup(bpy.types.Operator):
  """ Kgm Terrain Node """
  bl_label = "KgmMaterialNodeGroup"
  bl_idname = 'node.kgm_material_node_group'

  def execute(self, context):
      node_name = 'Kgm Material Node Group'
      group = create_kgm_material_node_group(self, context, node_name)
      node = context.scene.node_tree.nodes.new('CompositorNodeGroup')
      node.node_tree = bpy.data.node_groups[group.name]
      node.use_custom_color = True
      node.color = (0.3, 0.6, 0.9)

      return {'FINISHED'}


class KgmTerrainNodeTreePanel(bpy.types.Panel):
    """Creates a Panel in the Object properties window"""
    bl_label = "Kgm terrain node tree Panel"
    bl_idname = "OBJECT_PT_KGM_TERRAIN_NODE_TREE"
    bl_space_type = 'NODE_EDITOR'
    bl_region_type = 'UI'
    bl_category = "Kgm Tab"

    def draw(self, context):
        layout = self.layout

        row = layout.row()
        row.operator("node.kgm_terrain_node_group")

def menu_func(self, context):
    print("Apply menu.")
    self.layout.operator(kgmExport.bl_idname, text="Karakal game map")

def menu_kgm_imp_set_func(self, context):
    self.layout.operator(kgmImport.bl_idname, text="Karakal game settings (.kgs)", icon='PLUGIN')

def menu_func_a(self, context):
    self.layout.operator(kgm_unit.bl_idname, text="kgmUnit", icon='OUTLINER_OB_EMPTY')
    self.layout.operator(kgm_dummy.bl_idname, text="kgmDummy", icon='OUTLINER_OB_EMPTY')

classes = [
    kgmExport,
    KgmTerrainNodeTree,
    KgmTerrainNodeGroup,
    KgmMaterialNodeGroup,
    KgmTerrainNodeTreePanel
]

def register():
    print("Registering kgm module.")

    for c in classes:
      bpy.utils.register_class(c)

    bpy.types.TOPBAR_MT_file_export.append(menu_func)
    #bpy.types.TOPBAR_MT_file_import.append(menu_kgm_imp_set_func)
    #bpy.types.TOPBAR_MT_add.append(menu_func_a)

def unregister():
    bpy.types.TOPBAR_MT_file_export.remove(menu_func)

    for c in reversed(classes):
      bpy.utils.unregister_class(c)
    #bpy.types.TOPBAR_MT_file_import.remove(menu_kgm_imp_set_func)
    #bpy.types.TOPBAR_MT_add.remove(menu_func_a)


if __name__ == "__main__":
  register()
