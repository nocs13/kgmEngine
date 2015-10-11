# -*- coding: utf-8 -*-
#import bpy
import xml.etree.ElementTree as ET

Units   = []
Actors  = []
Sensors = []

def parse(path):
  tree = None

  try:
    tree = ET.parse(path)
  except Exception as e:
    print('kgm project ' + path + ' cannot load as xml file. error: ' + e)
    return

  root = tree.getroot()

  for elem in root.iter(None):
    print('kgm project element: ' + elem.attrib)

def run(cmd):
  pass
