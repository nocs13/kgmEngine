#!/bin/bash

OWD=$PWD

echo "current folder is $OWD"

CWD=`dirname $0`

echo "need to go $CWD"

cd $CWD

for i in *.vert; do
    file="${i%.*}"

    glslangValidator -V "$file.vert" -o "$file.vspv"
    echo "Vertex convert result is $?"
    glslangValidator -V "$file.frag" -o "$file.fspv"
    echo "Fragment convert result is $?"
    
    base64 "$file.vspv" > "$file.spv"
    echo "//Fragment shader" >>  "$file.spv"
    base64 "$file.fspv" >> "$file.spv"

    rm -f "$file.vspv"
    rm -f "$file.fspv"
done
