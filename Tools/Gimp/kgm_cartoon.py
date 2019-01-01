#!/usr/bin/env python
# -*- coding: utf-8 -*-
from gimpfu import *

class Colormap:
  def __init__(self):
    self.ranges = [None] * 9
    self.ranges[0] = [0,   33]
    self.ranges[1] = [14,  90]
    self.ranges[2] = [37,  115]
    self.ranges[3] = [91,  151]
    self.ranges[4] = [121, 160]
    self.ranges[5] = [151, 180]
    self.ranges[6] = [181, 210]
    self.ranges[7] = [211, 255]

    self.colors = [None] * 9
    self.colors[0] = [0, 0, 0]
    self.colors[1] = [143, 56, 143]
    self.colors[2] = [212, 53, 39]
    self.colors[3] = [229, 120, 40]
    self.colors[4] = [57, 170, 216]
    self.colors[5] = [121, 199, 70]
    self.colors[6] = [245, 194, 38]
    self.colors[7] = [255, 255, 255]

  def rgb(self, gray):
    for i in range(0, len(self.ranges)):
      if gray >= self.ranges[i][0] and gray <= self.ranges[i][1]:
        return self.colors[i]
    return [0, 0, 0]

def cartoon(r, g, b):
  z = 255 / 3
  color = [r, g, b]
  for c in range(3):
    for i in range(3):
      if (color[c] >= i * z) and (color[c] < (i + 1) * z):
        color[c] = (i + 1) * z
        break

  return color

def to_gray(r, g, b):
  gray = 0.299 * r + 0.587 * g + 0.114 * b
  return int(gray)

def to_rgb(gray):
  rgb = []

  #if (gray < 1):
  #  rgb = [0, 0, 0]
  #else if gray > 254:
  #  rgb = [255, 255, 255]

  return rgb


def kgm_cartoon(image, layer) :
  # Indicates that the process has started.
  gimp.progress_init("kgm cartoonizing " + layer.name + "...")

  # Set up an undo group, so the operation will be undone in one step.
  pdb.gimp_image_undo_group_start(image)

  pc = 1.0 / (layer.width + 1)

  colormap = Colormap()

  try:
    for x in range(layer.width):
      for y in range(layer.height):
        pixel = layer.get_pixel(x, y)

        if(len(pixel) >= 3):
          #sum = pixel[0] + pixel[1] + pixel[2]
          #gray = int(sum/3)
          gray = to_gray(pixel[0], pixel[1], pixel[2])
          rgb = colormap.rgb(gray)
          newColor = (rgb[0], rgb[1], rgb[2])
          #newColor = (gray, gray, gray)
          #newColor = (0, gray, 0)
          #newColor = cartoon(pixel[0], pixel[1], pixel[2])
          #newColor = cartoon(pixel[0], pixel[1], pixel[2])
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
