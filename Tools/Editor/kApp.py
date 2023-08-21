from fltk import *
from kWindow import *
import sys


class kApp():
    def __init__(self):
        self.wnd = kWindow(0, 0, 800, 600, 'kEditor')
        self.wnd.end()
        self.wnd.show(sys.argv)
        Fl_run()


if __name__ == "__main__":
    app = kApp()
