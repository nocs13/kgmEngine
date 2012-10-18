#
bl_addon_info = {
    "name": "kgmActor",
    "author": "Karakal_13",
    "version": (0, 1),
    "blender": (2, 5, 7),
    "api": 31345,
    "location": "View3D > Add",
    "description": "Add kgmActor",
    "warning": "Use only for kgmEngine",
    "category": "Object"}

import bpy
from bpy.ops import *

class kgmActor(bpy.types.Operator):
    ''' Add kgmActor '''   
    bl_idname = "object.kgm_actor_add"
    bl_label = "Add kgmActor"
    bl_options = {'REGISTER', 'UNDO'}
 
 
    def execute(self, context):
        bpy.ops.object.add()
        return {'FINISHED'}

# Add to a menu
#---------------------------        
def menu_func(self, context):
    self.layout.operator(kgmActor.bl_idname, text="kgmActor", icon='PLUGIN')

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_add.append(menu_func)

def unregister():
    bpy.types.INFO_MT_add.remove(menu_func)

if __name__ == "__main__":
    register()
