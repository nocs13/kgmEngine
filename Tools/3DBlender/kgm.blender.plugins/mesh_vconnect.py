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
        self.main_object = bpy.context.active_object
        self.distance = bpy.context.scene.VConnect_distance
        
        self.execute(context)
        
        return {"FINISHED"}
      
    def connect_vertices(self):
      self.main_object.update_from_editmode()
      mverts = self.main_object.data.vertices
      medges = self.main_object.data.edges
      mfaces = self.main_object.data.polygons
      
      verts = [v for v in mverts if v.select]
      
      for i in range(0, len(verts) - 1):
        print(str(verts[i].co))
        for j in range(i + 1, len(verts)):
          v = verts[i].co - verts[j].co
          dist = math.sqrt(v.x*v.x + v.y*v.y + v.z*v.z)
          print(dist)
          if dist < self.distance:
            print("merging " + str(i) + " " + str(j))
            vx = 0.5 * (verts[i].co.x + verts[j].co.x)
            vy = 0.5 * (verts[i].co.y + verts[j].co.y)
            vz = 0.5 * (verts[i].co.z + verts[j].co.z)
            vi = verts[i].index
            vj = verts[j].index
            mverts[vi].co.x = mverts[vj].co.x = vx
            mverts[vi].co.y = mverts[vj].co.y = vy
            mverts[vi].co.z = mverts[vj].co.z = vz
            #self.main_object.data.update()

      #self.main_object.data.remove_doubles(0.00001);
      bpy.ops.object.mode_set(mode = 'OBJECT')
      self.main_object.data.from_pydata([v.co for v in mverts], [f.vertices for f in medges], [])
      bpy.ops.object.mode_set(mode = 'EDIT')
      bpy.ops.mesh.remove_doubles(threshold=0.0001, use_unselected=False)
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
    
    

