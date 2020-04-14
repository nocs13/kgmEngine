#!/bin/bash

OWD=$PWD

echo "current folder is $OWD"

CWD=`dirname $0`

echo "need to go $CWD"

cd $CWD

for f in *; do
    echo "Current file: $f"

    ext=`echo $f | awk -F. '{print $3}'`
    file=`echo $f | awk -F. '{print $1"."$2}'`
    echo "Current ext: $ext"
    echo "Current file part: $file"

    [ ! -z "$ext" ] || continue

    if [ $ext == 'vert' ]; then
      glslangValidator -V $f -o "nix/$file.vspv"
    elif [ $ext == 'frag' ]; then
      glslangValidator -V $f -o "nix/$file.fspv"
    else
      echo "Wrong extention!"
    fi
done
