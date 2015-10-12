# -*- coding: utf-8 -*-
import os
#import bpy
import xml.etree.ElementTree as ET
from xml.dom import minidom
import subprocess

#Units   = []
Actors  = []
Sensors = []

Directory  = None
Executable = None

Process = None

def parse(path):
  xmldoc = None

  try:
    xmldoc = minidom.parse(path)
  except Exception as e:
    print('kgm project ' + path + ' cannot load as xml file. error: ' + e)
    return

  Directory = os.path.dirname(path)

  items = xmldoc.getElementsByTagName('Executable');

  if len(items) > 0:
    Executable = items[0].attributes['value'].value
    print('kgm execuable is: ' + Executable);

  from .kgm_objects import kgm_unit
  items = xmldoc.getElementsByTagName('kgmUnit');

  if len(items) > 0:
    for item in items:
      kgm_unit.Units.append(item.attributes['id'].value)
      print('kgm unit: ' + item.attributes['id'].value);

  items = xmldoc.getElementsByTagName('kgmActor');

  if len(items) > 0:
    for item in items:
      Actors.append(item.attributes['id'].value)
      print('kgm actor: ' + item.attributes['id'].value);

  items = xmldoc.getElementsByTagName('kgmSensor');

  if len(items) > 0:
    for item in items:
      Sensors.append(item.attributes['id'].value)
      print('kgm Sensor: ' + item.attributes['id'].value);

def run(map):
  if Executable is not None and Directory is not None:
    cmd = Directory + '/' + Executable + ' map ' + map;
    ls_output=subprocess.Popen([cmd], stdout=subprocess.PIPE)
  pass
