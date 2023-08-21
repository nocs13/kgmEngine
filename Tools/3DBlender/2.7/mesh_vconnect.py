# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; version 2
#  of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

bl_info = {
    "name": "Vertex Connect",
    "author": "Karakal_13",
    "version": (1,0),
    "blender": (2, 63, 0),
    "api": 45996,
    "location": "View3D > EditMode > ToolShelf",
    "description": "Modeling tool.",
    "wiki_url": "http://wiki.blender.org/index.php",
    "tracker_url": "http://projects.blender.org/tracker/index.php",
    "category": "Mesh"
}

import bpy
import bmesh
import math
import mathutils
import operator

from math import *

class VConnect_toolbar(bpy.types.Panel):
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'
    bl_context = "mesh_edit"
    bl_label = "VConnect"

    @classmethod
    def poll(cls, context):
        return context.active_object

    def draw(self, context):
        layout = self.layout

        scn = context.scene
        ob = context.object
        col = layout.column(align=True)
        row = layout.row()
        row.separator()
        col.operator("vconnect.edit_connect", text="Connect")
        col.prop(scn, "VConnect_distance")

# Edit strokes operator.
class VConnect_edit_connect(bpy.types.Operator):
    bl_idname = "vconnect.edit_connect"
    bl_label = "VConnect edit connect"
    bl_description = "Connect near vertices."

    def execute(self, context):
      if self.main_object is None:
        return
      self.connect_vertices()
      return

    def invoke (self, context, event):
#        self.main_object = bpy.context.active_object
        if bpy.context.edit_object is None or bpy.context.edit_object.type != 'MESH':
          print("Wrong edit object")
          return {"FINISHED"}

        self.main_object = bpy.context.active_object
        self.distance = bpy.context.scene.VConnect_distance
        self.execute(context)
        return {"FINISHED"}

    def connect_vertices(self):
      self.main_object.update_from_editmode()
      mbmesh = bmesh.from_edit_mesh(self.main_object.data)
      mbmesh.faces.active = None

      verts = [v for v in mbmesh.verts if v.select]

      for i in range(0, len(verts) - 1):
        for j in range(i + 1, len(verts)):
          v = verts[i].co - verts[j].co
          dist = math.sqrt(v.x*v.x + v.y*v.y + v.z*v.z)
          if dist < self.distance:
            vx = 0.5 * (verts[i].co.x + verts[j].co.x)
            vy = 0.5 * (verts[i].co.y + verts[j].co.y)
            vz = 0.5 * (verts[i].co.z + verts[j].co.z)
            vi = verts[i].index
            vj = verts[j].index
            mbmesh.verts[vi].co.x = mbmesh.verts[vj].co.x = vx
            mbmesh.verts[vi].co.y = mbmesh.verts[vj].co.y = vy
            mbmesh.verts[vi].co.z = mbmesh.verts[vj].co.z = vz

      bmesh.update_edit_mesh(self.main_object.data, True)
      bpy.ops.mesh.remove_doubles(threshold=0.00001, use_unselected=False)
      self.main_object.update_from_editmode()

def register():
    bpy.utils.register_class(VConnect_toolbar)
    bpy.utils.register_class(VConnect_edit_connect)
    bpy.types.Scene.VConnect_distance = bpy.props.FloatProperty(
      name="distance",
      description = "Maximux distance for connect",
      default = 1.0)


def unregister():
    bpy.utils.unregister_class(VConnect_toolbar)
    bpy.utils.unregister_class(VConnect_edit_connect)
    del bpy.types.Scene.VConnect_distance

if __name__ == "__main__":
    register()

