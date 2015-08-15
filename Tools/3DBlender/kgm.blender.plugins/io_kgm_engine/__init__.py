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

if "bpy" in locals():
    import importlib
    if "kgm_objects" in locals():
        importlib.reload(kgm_objects)
    if "kgm_export" in locals():
        importlib.reload(kgm_export)

import os
from math import radians
import bpy
from mathutils import *

from . import *

#---------------------------
def menu_func(self, context):
    self.layout.operator(kgm_export.kgmExport.bl_idname, text="Karakal game (.kgm)", icon='PLUGIN')

def menu_func_a(self, context):
    self.layout.operator(kgm_objects.kgm_unit.bl_idname, text="kgmUnit", icon='OUTLINER_OB_EMPTY')
    self.layout.operator(kgm_objects.kgm_dummy.bl_idname, text="kgmDummy", icon='OUTLINER_OB_EMPTY')
    #self.layout.operator_menu_enum(kgm_object.bl_idname, "type", text="kgmObject", icon='OUTLINER_OB_EMPTY')
    pass

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)
    bpy.types.INFO_MT_add.append(menu_func_a)
    #bpy.ops.wm.call_menu(name=kgm_object_menu.bl_idname)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)
    bpy.types.INFO_MT_add.remove(menu_func_a)

if __name__ == "__main__":
  register()