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
#__bpydoc__ = """ This script generate Kgm hair."""

bl_info = {
    "name": "Kgm Hair Operations",
    "author": "Karakal",
    "version": (0, 1),
    "blender": (2, 80, 0),
    "description": "Kgm hair operations",
    "warning": "",
    "category": "Object"
}

import bpy
import re

class KgmHairOperator(bpy.types.Operator):
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

class KgmHairCircleButtonOperator(KgmHairOperator):
    bl_idname = "scene.kgm_hair_circle_button_operator"
    bl_label = "Circle hair"

    id = bpy.props.IntProperty()

    def execute(self, context):
        bpy.ops.curve.primitive_bezier_circle_add()
        o_shape = bpy.context.selected_objects[0]
        if self.haveNameIndex(o_shape):
            o_shape.name = "kgm_hair_shape." + self.getNameIndex(o_shape)
        else:
            o_shape.name = "kgm_hair_shape"
        bpy.ops.curve.primitive_nurbs_path_add()
        o_path = bpy.context.selected_objects[0]
        if self.haveNameIndex(o_path):
            o_path.name = "kgm_hair_path." + self.getNameIndex(o_path)
        else:
            o_path.name = "kgm_hair_path"
        o_path.data.bevel_object = o_shape
        o_path.data.bevel_mode = 'OBJECT'
        
        bpy.ops.object.mode_set(mode='EDIT')
        o_path.data.splines[0].points[0].co = [-2, 0, 2, 1]
        o_path.data.splines[0].points[1].co = [-1, 0, 2, 1]
        o_path.data.splines[0].points[2].co = [1, 0,  1, 1]
        o_path.data.splines[0].points[3].co = [3, 0, -3, 1]
        o_path.data.splines[0].points[4].co = [3, 0, -5, 1]
        
        o_path.data.splines[0].points[0].radius = 0.1
        o_path.data.splines[0].points[1].radius = 1.0
        o_path.data.splines[0].points[2].radius = 0.7
        o_path.data.splines[0].points[3].radius = 0.3
        o_path.data.splines[0].points[4].radius = 0.02
        bpy.ops.object.mode_set(mode='OBJECT')
        
        print("Pressed button ", self.id)
        return {'FINISHED'}

class KgmHairSemiCircleButtonOperator(KgmHairOperator):
    bl_idname = "scene.kgm_hair_semi_circle_button_operator"
    bl_label = "Semi Circle hair"

    id = bpy.props.IntProperty()

    def execute(self, context):
        bpy.ops.curve.primitive_bezier_circle_add()
        o_shape = bpy.context.selected_objects[0]
        if self.haveNameIndex(o_shape):
            o_shape.name = "kgm_hair_shape." + self.getNameIndex(o_shape)
        else:
            o_shape.name = "kgm_hair_shape"
        o_shape.data.splines[0].bezier_points[2].co = [0, 0, 0]
        bpy.ops.curve.primitive_nurbs_path_add()
        o_path = bpy.context.selected_objects[0]
        if self.haveNameIndex(o_path):
            o_path.name = "kgm_hair_path." + self.getNameIndex(o_path)
        else:
            o_path.name = "kgm_hair_path"
        o_path.data.bevel_object = o_shape
        o_path.data.bevel_mode = 'OBJECT'
        
        bpy.ops.object.mode_set(mode='EDIT')
        o_path.data.splines[0].points[0].co = [-2, 0, 2, 1]
        o_path.data.splines[0].points[1].co = [-1, 0, 2, 1]
        o_path.data.splines[0].points[2].co = [1, 0,  1, 1]
        o_path.data.splines[0].points[3].co = [3, 0, -3, 1]
        o_path.data.splines[0].points[4].co = [3, 0, -5, 1]
        
        o_path.data.splines[0].points[0].radius = 0.1
        o_path.data.splines[0].points[1].radius = 1.0
        o_path.data.splines[0].points[2].radius = 0.7
        o_path.data.splines[0].points[3].radius = 0.3
        o_path.data.splines[0].points[4].radius = 0.02
        bpy.ops.object.mode_set(mode='OBJECT')
        
        print("Pressed button ", self.id)
        return {'FINISHED'}

class KgmHairOvalButtonOperator(KgmHairOperator):
    bl_idname = "scene.kgm_hair_oval_button_operator"
    bl_label = "Oval hair"

    id = bpy.props.IntProperty()

    def execute(self, context):
        bpy.ops.curve.primitive_bezier_circle_add()
        o_shape = bpy.context.selected_objects[0]
        if self.haveNameIndex(o_shape):
            o_shape.name = "kgm_hair_shape." + self.getNameIndex(o_shape)
        else:
            o_shape.name = "kgm_hair_shape"
        o_shape.data.splines[0].bezier_points[0].co = [-.5, 0, 0]
        o_shape.data.splines[0].bezier_points[2].co = [.5, 0, 0]
        bpy.ops.curve.primitive_nurbs_path_add()
        o_path = bpy.context.selected_objects[0]
        if self.haveNameIndex(o_path):
            o_path.name = "kgm_hair_path." + self.getNameIndex(o_path)
        else:
            o_path.name = "kgm_hair_path"
        o_path.data.bevel_object = o_shape
        o_path.data.bevel_mode = 'OBJECT'
        
        bpy.ops.object.mode_set(mode='EDIT')
        o_path.data.splines[0].points[0].co = [-2, 0, 2, 1]
        o_path.data.splines[0].points[1].co = [-1, 0, 2, 1]
        o_path.data.splines[0].points[2].co = [1, 0,  1, 1]
        o_path.data.splines[0].points[3].co = [3, 0, -3, 1]
        o_path.data.splines[0].points[4].co = [3, 0, -5, 1]
        
        o_path.data.splines[0].points[0].radius = 0.1
        o_path.data.splines[0].points[1].radius = 1.0
        o_path.data.splines[0].points[2].radius = 0.7
        o_path.data.splines[0].points[3].radius = 0.3
        o_path.data.splines[0].points[4].radius = 0.02
        bpy.ops.object.mode_set(mode='OBJECT')
        print("Pressed button ", self.id)
        return {'FINISHED'}
    
class KgmHairingButtonOperator(bpy.types.Operator):
    bl_idname = "scene.kgm_hairing_button_operator"
    bl_label = "Hairing"

    id = bpy.props.IntProperty()

    def execute(self, context):
        amode = bpy.context.object.mode
        print("Current mode is ", str(amode))

        o_path = bpy.context.selected_objects[0]

        print("Splines count is ", str(len(o_path.data.splines)))

        for i in range(1, len(o_path.data.splines)):
            print("Modify spline index is ", str(i))
            print("Modify spline points count is ", str(len(o_path.data.splines[i].bezier_points)))
            for j in range(0, len(o_path.data.splines[i].bezier_points)):
                if j < len(o_path.data.splines[0].bezier_points):
                    o_path.data.splines[i].bezier_points[j].radius = o_path.data.splines[0].bezier_points[j].radius
                else:
                    o_path.data.splines[i].bezier_points[j].radius = o_path.data.splines[0].bezier_points[-1].radius
        bpy.ops.object.mode_set(mode=amode)
        print("Pressed button ", self.id)
        return {'FINISHED'}

class KgmHairPanel(bpy.types.Panel):
    """Creates a Panel in the Object properties window"""
    bl_label = "Kgm Hair Panel"
    bl_idname = "OBJECT_PT_kgm_hair"
    
    bl_space_type  = 'VIEW_3D'
    bl_region_type = 'UI'

    def draw(self, context):
        layout = self.layout

        obj = context.object

        row = layout.row()
        row.label(text="KgmHairs!", icon='WORLD_DATA')

        #row = layout.row()
        #row.label(text="Active object is: " + obj.name)
        #row = layout.row()
        #row.prop(obj, "name")

        row = layout.row()
        #row.operator("mesh.primitive_cube_add")
        row.operator("scene.kgm_hair_circle_button_operator")
        row = layout.row()
        row.operator("scene.kgm_hair_semi_circle_button_operator")
        row = layout.row()
        row.operator("scene.kgm_hair_oval_button_operator")
        row = layout.row()
        row.operator("scene.kgm_hairing_button_operator")
        
    def add_hair_circle(self):
        pass


def register():
    bpy.utils.register_class(KgmHairPanel)
    bpy.utils.register_class(KgmHairCircleButtonOperator)
    bpy.utils.register_class(KgmHairSemiCircleButtonOperator)
    bpy.utils.register_class(KgmHairOvalButtonOperator)
    bpy.utils.register_class(KgmHairingButtonOperator)


def unregister():
    bpy.utils.unregister_class(KgmHairPanel)
    bpy.utils.unregister_class(KgmHairCircleButtonOperator)
    bpy.utils.unregister_class(KgmHairSemiCircleButtonOperator)
    bpy.utils.unregister_class(KgmHairOvalButtonOperator)
    bpy.utils.unregister_class(KgmHairingButtonOperator)


if __name__ == "__main__":
    register()
