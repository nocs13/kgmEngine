#!/bin/bash

OS=$(uname -s)

echo "Current OS is $OS"

SDR=''

if [ $OS == 'Linux' ]; then
  SDR='nix'
else
  SDR='win'
fi

BWD=$(pwd)

echo "Current working folder is $BWD"

echo "Target folder is $sdr"

CWD=$(dirname `which $0`)

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
      glslangValidator -V $f -o "$SDR/$file.vspv"
    elif [ $ext == 'frag' ]; then
      glslangValidator -V $f -o "$SDR/$file.fspv"
    else
      echo "Wrong extention!"
    fi
done

cd $BWD
