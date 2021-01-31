#
#
#"""
#Created on Oct 3, 2010
#@author: goga
#Name: 'Kgm hair'
#Blender: 242
#Group: 'Export'
#"""

#__author__ = "Karakal"
#__version__ = "0.1"
#__bpydoc__ = """ This script some kgm utils."""

bl_info = {
    "name": "Kgm Utils",
    "author": "Karakal",
    "version": (0, 1),
    "blender": (2, 80, 0),
    "description": "Simple utilites for kgm scene.",
    "warning": "",
    "category": "Object"
}

import bpy
import re

from bpy.types import PropertyGroup

class KgmArmatureSymmetryPropertyGroup(PropertyGroup):
    boneTo = bpy.props.StringProperty(  # bl 2.80 use testint: bpy.props
        name="boneTo",
        description="Bone should symmetry.",
    )
    boneFrom = bpy.props.StringProperty(  # bl 2.80 use testint: bpy.props
        name="boneFrom",
        description="Bone symmetry source.",
    )
    
class KgmUtilsOperator(bpy.types.Operator):
	def getNameIndex(self, o):
		name = o.name
		num = re.findall(r'\.\d+$', name)

		if num is not None and len(num) > 0:
			return num[0]

		return ""
		
	def haveNameIndex(self, o):
		name = o.name
		num = re.findall(r'\.\d+$', name)

		if num is not None and len(num) > 0 and  num[0].isdigit():
			return True

		return False

class KgmArmatureSymmetryButtonOperator(KgmUtilsOperator):
    bl_idname = "scene.kgm_armature_symmetry_button_operator"
    bl_label = "Armature symmetry"

    src = bpy.props.StringProperty(default='Left');
    dst = bpy.props.StringProperty(default='Right');

    def getBone(self, arm, name):
        if arm is None or arm.type != 'ARMATURE':
            return None
        
        for b in arm.data.edit_bones:
            if b.name == name:
                return b
            
        return None

    def execute(self, context):
        #row = layout.row()
        #row.operator('object.simple_operator')

        arm = None
        
        print("Start symmetry armature...")
        
        if len(bpy.context.selected_objects) < 1:
            print("No selected objects.")
            return {'FINISHED'}
        else:
            arm = bpy.context.selected_objects[0]

            if arm.type != "ARMATURE":
                print("Selected objects is not armature.")
                return {'FINISHED'}

        for b in arm.data.edit_bones:
            print("Current bone " + b.name)
            
            if self.src in b.name:
                bs = self.getBone(arm, b.name.replace(self.src, self.dst))

                if bs is not None:
                    print("Symmetry bone " + bs.name)
                    bs.head = b.head
                    bs.tail = b.tail

                    bs.head[0] *= -1
                    bs.tail[0] *= -1
                    
            
        print("Pressed button.")
        return {'FINISHED'}

    
class KgmArmaturePanel(bpy.types.Panel):
    """Creates a Panel in the Object properties window"""
    bl_label = "Kgm Armature Panel"
    bl_idname = "OBJECT_PT_kgm_armature"
    
    bl_space_type  = 'VIEW_3D'
    bl_region_type = 'UI'

    def draw(self, context):
        layout = self.layout

        obj = context.object

        row = layout.row()
        row.label(text="Symmetry armature", icon='WORLD_DATA')
        
        row = layout.row()
        row.operator("scene.kgm_armature_symmetry_button_operator")

        print("Panel symmetry armature.")
        #row = layout.row()
        #scene = context.scene               
        #layout.prop(scene, "theChosenObject", scene, "objects")        
        #row.operator("mesh.primitive_cube_add")


def register():
    
    bpy.utils.register_class(KgmArmatureSymmetryPropertyGroup)
    bpy.utils.register_class(KgmArmaturePanel)
    bpy.utils.register_class(KgmArmatureSymmetryButtonOperator)


def unregister():
    bpy.utils.unregister_class(KgmArmatureSymmetryButtonOperator)
    bpy.utils.unregister_class(KgmArmaturePanel)


if __name__ == "__main__":
    register()

