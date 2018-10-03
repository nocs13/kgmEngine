#!/usr/bin/env python
# -*- coding: utf-8 -*-
from gimpfu import *

def cartoon(r, g, b):
  z = 255 / 3
  color = [r, g, b]
  for c in range(3):
    for i in range(3):
      if (color[c] >= i * z) and (color[c] < (i + 1) * z):
        color[c] = (i + 1) * z
        break

  return color

def kgm_cartoon(image, layer) :
  # Indicates that the process has started.
  gimp.progress_init("kgm cartoonizing " + layer.name + "...")

  # Set up an undo group, so the operation will be undone in one step.
  pdb.gimp_image_undo_group_start(image)

  pc = 1.0 / (layer.width + 1)

  try:
    for x in range(layer.width):
      for y in range(layer.height):
        pixel = layer.get_pixel(x, y)

        if(len(pixel) >= 3):
          sum = pixel[0] + pixel[1] + pixel[2]
          gray = int(sum/3)
          #newColor = (gray, gray, gray) + pixel[3:]
          #newColor = (0, gray, 0)
          newColor = cartoon(pixel[0], pixel[1], pixel[2])
          layer.set_pixel(x,y, newColor)
        gimp.progress_update(pc * x)

    # Update the layer.
    layer.update(0, 0, layer.width, layer.height)
  except Exception as err:
    gimp.message("Unexpected error: " + str(err))

  # Close the undo group.
  pdb.gimp_image_undo_group_end(image)

  # End progress.
  pdb.gimp_progress_end()

register(
  "python_fu_kgm_cartoon",
  "Cartoon image",
  "Image cartoonize for kgm",
  "karakal_13",
  "karakal_13",
  "2018",
  "KgmCartoon...",
  "RGB*, RGB*",
  [
  (PF_IMAGE,   "image", "takes current image", None),
  (PF_DRAWABLE,"layer", "input layer", None),
  ],
  [],
  kgm_cartoon,
  menu="<Image>/Filters/Artistic")

main()
