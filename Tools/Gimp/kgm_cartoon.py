#!/usr/bin/env python
# -*- coding: utf-8 -*-
from gimpfu import *

def kgm_cartoon(img, layer) :
  try:
    for x in range(layer.width):
      for y in range(layer.heigh):
        pixel = layer.get_pixel(x, y)

        if(len(pixel) >= 3):
          sum = pixel[0] + pixel[1] + pixel[2]
          gray = int(sum/3)
          newColor = (gray,gray,gray) + pixel[3:]
          layer.set_pixel(x,y, newColor)
  except Exception as err:
    gimp.message("Unexpected error: " + str(err))

register(
  "python_fu_kgm_cartoon",
  "Cartoon image",
  "Image cartoonize for kgm",
  "karakal_13",
  "karakal_13",
  "2018",
  "KgmCartoon...",
  "*",
  [],
  [],
  kgm_cartoon,
  menu="<Image>/Filters/Artistic")

main()
