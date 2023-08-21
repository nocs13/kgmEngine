from fltk import *

class kWindow(Fl_Gl_Window):
  def __init__(self, xpos, ypos, width, height, label):
    Fl_Gl_Window.__init__(self, xpos, ypos, width, height, label)

  def draw(self):
    pass