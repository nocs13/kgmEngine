# -*- coding: utf-8 -*-
import bpy
import xml.etree.ElementTree as ET

Units   = []
Actors  = []
Sensors = []

def parse(path):
  tree = NULL

  try:
    tree = ET.parse('country_data.xml')
  except:
    print('kgm project ' + path + 'cannot load as xml file.')
    return

  root = tree.getroot()

  for elem in root.iter(NULL):
    print('kgm project element: ' + elem.attrib)

def run(cmd):
  pass
