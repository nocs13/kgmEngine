#
#
#"""
#Created on Oct 3, 2021
#@author: goga
#Name: 'Kgm (.kgm)...'
#Blender: 282
#Group: 'Import/Export'
#"""

#__author__ = "Karakal"
#__version__ = "0.1"
#__bpydoc__ = """ This script Exports Kgm format. """

bl_info = {
    "name": "Export Kgm Format (.kgm)",
    "author": "Karakal_13",
    "version": (0, 1),
    "blender": (2, 80, 0),
    "api": 31343,
    "location": "File > Export",
    "description": "Export to the Kgm Format (.kgm)",
    "warning": "",
    "category": "Import-Export"}

import bpy
from .kgm_anim_hair import KgmHairPanel
from .kgm_anim_hair import KgmHairCircleButtonOperator
from .kgm_anim_hair import KgmHairSemiCircleButtonOperator
from .kgm_anim_hair import KgmHairOvalButtonOperator
from .kgm_anim_hair import KgmHairingButtonOperator

#import kgm_anim_hair
#import kgm_objects

from bpy.props import StringProperty, BoolProperty
from bpy_extras.io_utils import ExportHelper

import os
from math import radians
from mathutils import *
from bpy.props import *

def toGrad(a):
 return a * 180.0 / 3.1415

class kgmExport(bpy.types.Operator):
  #from . import kgm_objects
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
    materials  = [kgm_objects.kgmMaterial(ob) for ob in bpy.data.materials]
    meshes     = [kgm_objects.kgmMesh(ob) for ob in bpy.data.meshes]

    visuals    = [kgm_objects.kgmVisual(ob) for ob in objects if self.exp_meshes and ob.type == 'MESH' and ob.collision.use != True]
    obstacles  = [kgm_objects.kgmObstacle(ob) for ob in objects if ob.type == 'MESH' and self.exp_kgmphysics and ob.collision.use == True]
    lights     = [kgm_objects.kgmLight(ob) for ob in objects if ob.type == 'LAMP' and self.exp_lights]
    cameras    = [kgm_objects.kgmCamera(ob) for ob in objects if ob.type == 'CAMERA' and self.exp_cameras]
    skeletons  = [kgm_objects.kgmSkeleton(ob) for ob in objects if ob.type == 'ARMATURE' and self.exp_armatures]
    units      = [kgm_objects.kgmUnit(ob) for ob in objects if ob.type == 'EMPTY' and self.exp_kgmobjects and ob.get('kgm_unit')]
    actors     = [kgm_objects.kgmActor(ob) for ob in objects if ob.type == 'EMPTY' and self.exp_kgmobjects and ob.get('kgm_actor')]
    effects    = [kgm_objects.kgmEffect(ob) for ob in objects if ob.type == 'EMPTY' and self.exp_kgmobjects and ob.get('kgm_effect')]
    sensors    = [kgm_objects.kgmSensor(ob) for ob in objects if ob.type == 'EMPTY' and self.exp_kgmobjects and ob.get('kgm_sensor')]
    triggers   = [kgm_objects.kgmTrigger(ob) for ob in objects if ob.type == 'EMPTY' and self.exp_kgmobjects and ob.get('kgm_trigger')]
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

    #path = self.filepath
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
              "' scale='" + str("%.5f" % s.scale.x) + " " + str("%.5f" % s.scale.y) + " " + str("%.5f" % s.scale.z) +
              "'>\n")

      for b in s.bones:
        file.write("  <Bone name='" + b.name + "' parent='" + b.parent + "'")
        file.write(" position='" + str("%.5f" % b.pos.x) + " " + str("%.5f" % b.pos.y) + " " + str("%.5f" % b.pos.z) + "'")
        file.write(" quaternion='" + str("%.5f" % b.quat[1]) + " " + str("%.5f" % b.quat[2]) + " " + str("%.5f" % b.quat[3]) + " " + str("%.5f" % b.quat[0]) + "'")
        file.write("/>\n")

      file.write(" </kgmSkeleton>\n")

    #lights
    for o in lights:
      file.write(" <kgmLight name='" + o.name + "'>\n")
      file.write("  <Type value='" + o.type + "'/>\n")
      file.write("  <Position value='" + str(o.pos[0]) + " " + str(o.pos[1]) + " " + str(o.pos[2]) + "'/>\n")
      file.write("  <Rotation value='" + str(o.rot[0]) + " " + str(o.rot[1]) + " " + str(o.rot[2]) + "'/>\n")
      file.write("  <Color value='" + str(o.color[0]) + " " + str(o.color[1]) + " " + str(o.color[2]) + "'/>\n")
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

        if o.type == 'Mesh':
          file.write("  <Mesh id='" + o.data + "'/>\n")

        if o.material != '':
          file.write("  <Material id='" + o.material + "'/>\n")
        file.write(" </kgmVisual>\n")

    #collisions
    for obstacle in obstacles:
      file.write(" <kgmObstacle polygons='" + str(len(obstacle.faces)) + "'>\n")
      for face in collision.faces:
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

class kgmProject(bpy.types.Operator):
  '''This appiers in the tooltip of '''
  # this is important since its how bpy.ops.export.kgm_export() is constructed
  bl_idname = "import_scene.kgm"
  bl_label = "Set Kgm Project"

  # TODO, add better example props
  filepath = StringProperty(name="File Path",
                            description="File path used for set the Kgm project",
                            maxlen=1024,
                            default="~/")

  def invoke(self, context, event):
    print('kgmProject: invoke')
    wm = context.window_manager

    if True:
      wm.fileselect_add(self)
      return {'RUNNING_MODAL'}
    elif True:
      wm.invoke_search_popup(self)
      return {'RUNNING_MODAL'}
    elif False:
      return wm.invoke_props_popup(self, event) #
    elif False:
      return self.execute(context)

    return {'RUNNING_MODAL'}

  @classmethod
  def poll(self, context):
    wm = context.window_manager

    if True:
      wm.fileselect_add(self)
      return {'RUNNING_MODAL'}
    elif True:
      wm.invoke_search_popup(self)
      return {'RUNNING_MODAL'}
    elif False:
      return wm.invoke_props_popup(self, event) #
    elif False:
      return self.execute(context)
    print('kgmProject: poll')
    return context.active_object != None

  def execute(self, context):
    #from .kgm_project import parse
    #print('kgmProject: execute')
    #print('kgmProject: setting kgm project from file: ' + self.filepath)
    #parse(self.filepath)

    return {'FINISHED'}

  def prepare():
    #from .kgm_project import parse
    #print('kgmProject: execute')
    #parse(os.getenv("HOME") + "/" + ".kgmEngine/project")
    pass
#---------------------------

def menu_func(self, context):
  self.layout.operator(kgmExport.bl_idname, text="Karakal game (.kgm)", icon='NONE')

def menu_func_a(self, context):
  self.layout.operator(kgm_objects.kgm_dummy.bl_idname,  text="kgmDummy", icon='OUTLINER_OB_EMPTY')

def register():
  kgmProject.prepare()

  #bpy.utils.register_module(__name__)
  #bpy.types.INFO_MT_file_export.append(menu_func)
  #bpy.types.INFO_MT_add.append(menu_func_a)
  bpy.types.TOPBAR_MT_file_export.append(menu_func)
  bpy.utils.register_class(kgmExport)
  bpy.utils.register_class(KgmHairPanel)
  bpy.utils.register_class(KgmHairCircleButtonOperator)
  bpy.utils.register_class(KgmHairSemiCircleButtonOperator)
  bpy.utils.register_class(KgmHairOvalButtonOperator)
  bpy.utils.register_class(KgmHairingButtonOperator)

def unregister():
  bpy.types.TOPBAR_MT_file_export.remove(menu_func)
  #bpy.utils.unregister_module(__name__)
  #bpy.types.INFO_MT_file_export.remove(menu_func)
  #bpy.types.INFO_MT_add.remove(menu_func_a)
  bpy.utils.unregister_class(kgmExport)
  bpy.utils.unregister_class(KgmHairPanel)
  bpy.utils.unregister_class(KgmHairCircleButtonOperator)
  bpy.utils.unregister_class(KgmHairSemiCircleButtonOperator)
  bpy.utils.unregister_class(KgmHairOvalButtonOperator)
  bpy.utils.unregister_class(KgmHairingButtonOperator)

if __name__ == "__main__":
  register()
