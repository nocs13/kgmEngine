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

kgm_animaniacs = [  ( "Unit",    "Unit",    "kgmUnit"    ),
                    ( "Actor",   "Actor",   "kgmActor"   )]

kgm_project_parameters = ''
kgm_unit_types = [('a1', 'a1 a1', 'a1 a1 a1'), ('a2', 'a2 a2', 'a2 a2 a2')]
kgm_actor_types = [('u1', 'a1 u1', 'a1 a1 u1'), ('u2', 'a1 u2', 'a1 a1 u2')]


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

    layout.label(text="Export/Import")
    row = layout.row()
    box = layout.box()

    col = box.column(align=True)
    col.operator(operator='export_scene.kgm_group_export', text='export group')
    col.operator(operator='export_scene.kgm',              text='export scene')
    col.operator(operator='import_scene.kgm_settings',     text='import setting')

    if hasattr(obj, 'kgm_unit'):
      self.draw_unit(obj)
    elif hasattr(obj, 'kgm_dummy'):
      self.draw_dummy(obj)

  def draw_unit(self, obj):
    layout = self.layout
    layout.label(text="kgm_object unit")

    row = layout.row()
    row.prop(obj, "kgm_unit")
    row = layout.row()
    row.prop(obj, "kgm_player")
    row = layout.row()
    row.prop(obj, "kgm_state")
    row = layout.row()
    row.prop(obj, "kgm_object")

  def draw_dummy(self, obj):
    layout = self.layout
    layout.label(text="kgm_object")


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
    bpy.types.Object.kgm_player = bpy.props.BoolProperty()
    bpy.types.Object.kgm_type = bpy.props.EnumProperty(name='kgm unit', items=kgm_animaniacs,
                                                     description='select kgm unit')
    bpy.types.Object.kgm_state = bpy.props.StringProperty()
    bpy.types.Object.kgm_object = bpy.props.StringProperty()

    bpy.types.Object.kgm_unit_type = bpy.props.EnumProperty(items=choose_unit_types, name='unit types')

    bpy.ops.object.add()
    a = bpy.context.object

    if not hasattr(a, 'kgm_unit'):
      return {'FINISHED'}

    a.name = "kgmUnit"
    a.kgm_unit = True
    a.kgm_player = False
    a.kgm_type = "Unit";
    a.kgm_state = "Idle"
    a.kgm_object = "None"
    return {'FINISHED'}

  def modal(self, context, event):
    print("Modal object \n")
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    print("Invoke unit.\n")

    bpy.types.Object.kgm_unit = bpy.props.BoolProperty(options={'HIDDEN'})
    bpy.types.Object.kgm_player = bpy.props.BoolProperty()
    bpy.types.Object.kgm_type = bpy.props.EnumProperty(name='kgm unit', items=kgm_animaniacs,
                                                     description='select kgm unit')
    bpy.types.Object.kgm_state = bpy.props.StringProperty()
    bpy.types.Object.kgm_object = bpy.props.StringProperty()

    bpy.types.Object.kgm_unit_type = bpy.props.EnumProperty(items=choose_unit_types, name='unit types')

    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmUnit"
    a.kgm_unit = True
    a.kgm_player = False
    a.kgm_type = "Unit";
    a.kgm_state = "Idle"
    a.kgm_object = "None"
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

    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmDummy"
    return {'FINISHED'}

  def modal(self, context, event):
    print("Modal dummy.\n")
    return {'RUNNING_MODAL'}

  def invoke(self, context, event):
    print("Invoke dummy.\n")

    bpy.ops.object.add()
    a = bpy.context.object

    a.name = "kgmDummy"
    return {'FINISHED'}

  def draw(self, context):
    layout = self.layout


scene_materials = []


class kgmMaterial:
  def __init__(self, mtl):
    self.name = mtl.name
    self.diffuse = mtl.diffuse_color
    self.emmision = [1.0, 1.0, 1.0]
    self.specular = mtl.specular_color
    self.shine = 1.0
    self.alpha = mtl.alpha
    self.map_color = ''
    self.map_normal = ''
    self.map_specular = ''
    self.images = ()

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
    self.type = 'Point' if lamp.type == 'POINT' else 'Spot'


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
    mtx = o.matrix_local

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
        iface = []
        for j in range(0, len(face.vertices)):
          v = kgmVertex();
          vi = face.vertices[j]
          c = mtx * mesh.vertices[vi].co
          n = mtx.to_3x3() * face.normal
          v.v = [c[0], c[1], c[2]]
          v.n = [n[0], n[1], n[2]]
          if self.hasvgroups == True:
            for g in range(0, len(mesh.vertices[vi].groups)):
              if g < 4:
                # print(mesh.vertices[vi].groups[g].id)
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
    # check if such vertex already exist
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


class kgmObject:
  def __init__(self, o):
    self.name = o.name
    self.gtype = o.get('kgm_type')
    self.state = o.get('kgm_state')
    self.gobject = o.get('kgm_object')
    self.mtx = o.matrix_world
    self.pos = o.matrix_local.to_translation()
    self.quat = self.mtx.to_quaternion()
    self.euler = self.mtx.to_euler()
    self.linked = 'None'
    self.props = {}

    if self.gtype == None:
      self.gtype = ""
    if self.gobject == None:
      self.gobject = ""
    if self.state == None:
      self.state = ""

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
    self.mtx = o.matrix_local
    self.name = o.name
    self.type = o.type
    self.object = o.proxy.name
    self.pos = Vector((0, 0, 0)) * self.mtx
    self.quat = self.mtx.to_quaternion()
    self.euler = self.mtx.to_euler()


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
  file.write("  <Emmision value='" + str("%.5f" % o.emmision[0]) + " " + str("%.5f" % o.emmision[1]) + " " + str(
    "%.5f" % o.emmision[2]) + "'/>\n")
  file.write("  <Specular value='" + str("%.5f" % o.specular[0]) + " " + str("%.5f" % o.specular[1]) + " " + str(
    "%.5f" % o.specular[2]) + "'/>\n")
  file.write("  <Shininess value='" + str("%.5f" % o.shine) + "'/>\n")
  file.write("  <Alpha value='" + str("%.5f" % o.alpha) + "'/>\n")

  if o.map_color != "":
    file.write("  <map_color value='" + o.map_color + "'/>\n")
  if o.map_normal != "":
    file.write("  <map_normal value='" + o.map_normal + "' strength='" + str(o.map_normal_strength) + "' />\n")
  if o.map_specular != "":
    file.write("  <map_specular value='" + o.map_specular + "' strength='" + str(o.map_specular_strength) + "' />\n")
  if (hasattr(o, 'shader')):
    file.write("  <Shader value='" + o.shader + "'/>\n")
  file.write(" </Material>\n")


def export_light(file, o):
  file.write(" <Light name='" + o.name + "'>\n")
  file.write("  <Type value='" + o.type + "'/>\n")
  file.write("  <Color value='" + str(o.color[0]) + " " + str(o.color[1]) + " " + str("%.5f" % o.color[2]) + "'/>\n")
  file.write("  <Position value='" + str("%.5f" % o.pos[0]) + " " + str("%.5f" % o.pos[1]) + " " + str(
    "%.5f" % o.pos[2]) + "'/>\n")
  file.write("  <Rotation value='" + str("%.5f" % o.rot[0]) + " " + str("%.5f" % o.rot[1]) + " " + str(
    "%.5f" % o.rot[2]) + "'/>\n")
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


def export_mesh_data(file, o):
  file.write(" <Mesh name='" + o.name + "'>\n")
  if len(o.mtls) > 0:
    file.write("  <Material name='" + o.mtls[0] + "' />\n")

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
  file.write(" <Visual type='mesh' name='" + o.name + "' proxy='" + o.data.name + "'>\n")
  for m in o.data.materials:
    file.write("  <Material name='" + m.name + "'/>\n")

  file.write("  <Position='" + str("%.5f" % v.x) + " " + str("%.5f" % v.y) + " " + str("%.5f" % v.z) + "'/>\n")
  file.write("  <Rotation='" + str("%.5f" % r.x) + " " + str("%.5f" % r.y) + " " + str("%.5f" % r.z) + "'/>\n")
  file.write(" </Visual>\n")
  pass


def export_sceleton(file, o):
  file.write(" <Skeleton name='" + s.name +
             "' position='" + str("%.5f" % s.pos.x) + " " + str("%.5f" % s.pos.y) + " " + str("%.5f" % s.pos.z) +
             "' quaternion='" + str("%.5f" % s.quat.x) + " " + str("%.5f" % s.quat.y) + " " + str(
    "%.5f" % s.quat.z) + " " + str("%.5f" % s.quat.w) +
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
  file.write(" <Obstacle polygons='" + str(len(o.faces)) + "'>\n")
  for face in collision.faces:
    file.write("  <Polygon vertices='" + str(len(face)) + "'>\n")
    for v in face:
      file.write("   " + str(v[0]) + " " + str(v[1]) + " " + str(v[2]) + "\n")
    file.write("  </Polygon>\n")
  file.write(" </Obstacle>\n")


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
  filename_ext = ".kgm"
  filter_glob = StringProperty(default="*.kgm", maxlen=1024, options={'HIDDEN'}, )
  # filepath = StringProperty(name="File Path", description="File path used for exporting the Kgm file", maxlen=1024, default="~/")
  # use_setting = BoolProperty(name="Example Boolean", description="Example Tooltip", default= True)
  exp_meshes = BoolProperty(name="Export Meshes", description="", default=True)
  exp_lights = BoolProperty(name="Export Lights", description="", default=False)
  exp_materials = BoolProperty(name="Export Materials", description="", default=False)
  exp_cameras = BoolProperty(name="Export Cameras", description="", default=False)
  exp_armatures = BoolProperty(name="Export Armatures", description="", default=False)
  exp_animations = BoolProperty(name="Export Animations", description="", default=False)
  # exp_kgmobjects = BoolProperty(name="Export kgmObjects", description="", default=False)
  exp_obstacles = BoolProperty(name="Export Obstacles", description="", default=False)
  is_selection = BoolProperty(name="Selected only", description="", default=False)
  type = bpy.props.EnumProperty(items=(('OPT_A', "Xml", "Xml format"), ('OPT_B', "Bin", "Binary format")),
                                name="Format", description="Choose between two items", default='OPT_A')

  objects = []
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

    if self.is_selection:
      self.objects = [ob for ob in scene.objects if ob.is_visible(scene) and ob.select]
    else:
      self.objects = [ob for ob in scene.objects if ob.is_visible(scene)]

    objects = self.objects

    # cFrame = bpy.context.scene.frame_current

    self.mesh_datas = []
    self.mesh_nodes = []
    self.obstacles = []
    self.lights = []
    self.cameras = []
    self.skeletons = []
    self.animations = []

    print("Collect Objects...")

    '''meshes     = [kgmMesh(ob) for ob in objects if ob.type == 'MESH' and self.exp_meshes and ob.collision.use != True and ob.proxy is None]
    obstacles  = [kgmObstacle(ob) for ob in objects if ob.type == 'MESH' and self.exp_obstacles and ob.collision.use == True]
    lights     = [kgmLight(ob) for ob in objects if ob.type == 'LAMP' and self.exp_lights]
    cameras    = [kgmCamera(ob) for ob in objects if ob.type == 'CAMERA' and self.exp_cameras]
    skeletons  = [kgmSkeleton(ob) for ob in objects if ob.type == 'ARMATURE' and self.exp_armatures]
    #gobjects   = [kgmObject(ob) for ob in objects if ob.type == 'EMPTY' and self.exp_kgmobjects and ob.get('kgm')]
    animations = []'''

    threads = list(range(6))
    threads[0] = threading.Thread(target=self.collect_meshes)
    threads[1] = threading.Thread(target=self.collect_obstacles)
    threads[2] = threading.Thread(target=self.collect_lights)
    threads[3] = threading.Thread(target=self.collect_cameras)
    threads[4] = threading.Thread(target=self.collect_skeletons)
    threads[5] = threading.Thread(target=self.collect_animations)

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
    if not self.filepath.lower().endswith(".kgm"):
      self.filepath += ".kgm"

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
    print("Export mesh library")
    for o in self.mesh_datas:
      export_mesh_data(file, o)

    print("Export mesh nodes")
    for o in self.mesh_nodes:
      export_mesh_node(file, o)

    # skeletons
    for s in self.skeletons:
      export_sceleton(file, o)

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

    # obstacle
    for o in self.obstacles:
      export_obstacle(file, o)

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
    self.mesh_nodes = [ob for ob in self.objects if ob.type == 'MESH' and self.exp_meshes and ob.collision.use != True]
    print("Collected mesh nodes: " + str(len(self.mesh_nodes)))

    for n in self.mesh_nodes:
      exist = False
      for m in self.mesh_datas:
        if n.name == m.name:
          exist = True
          break
      if exist is False:
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

  def collect_animations(self):
    if self.exp_animations:
      armatures = [ob for ob in self.objects if ob.type == 'ARMATURE']
      for armature in armatures:
        print("scan armature")
        for bone in armature.data.bones:
          self.animations.append(kgmBoneAnimation(bone, armature))


class kgmExportGroup(bpy.types.Operator, ExportHelper):
  ''' Kgm scene export '''
  bl_idname = "export_scene.kgm_group_export"
  bl_label = "Kgm export group"

  filename_ext = ".kgm"
  filter_glob = StringProperty(default="*.kgm", maxlen=1024, options={'HIDDEN'}, subtype='DIR_PATH')
  exp_meshes = BoolProperty(name="Export Meshes", description="", default=True)
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


def menu_func(self, context):
    self.layout.operator(kgmExport.bl_idname, text="Karakal game map (.kgm)", icon='PLUGIN')

def menu_kgm_imp_set_func(self, context):
    self.layout.operator(kgmImport.bl_idname, text="Karakal game settings (.kgs)", icon='PLUGIN')

def menu_func_a(self, context):
    self.layout.operator(kgm_unit.bl_idname, text="kgmUnit", icon='OUTLINER_OB_EMPTY')
    self.layout.operator(kgm_dummy.bl_idname, text="kgmDummy", icon='OUTLINER_OB_EMPTY')

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)
    bpy.types.INFO_MT_file_import.append(menu_kgm_imp_set_func)
    bpy.types.INFO_MT_add.append(menu_func_a)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)
    bpy.types.INFO_MT_file_import.remove(menu_kgm_imp_set_func)
    bpy.types.INFO_MT_add.remove(menu_func_a)


if __name__ == "__main__":
  register()
