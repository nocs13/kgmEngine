#!/bin/bash

OS=$(uname -s)

echo "Current OS is $OS"

SDR=''

if [ $OS == 'Linux' ]; then
  SDR='nix'
else
  SDR='win'
fi

for f in *.vk; do
    echo "Current file: " ${f}
    
    #[ ! -e $f ] || continue
    
    file=`echo $f | rev | cut -c4- | rev`
    
    echo "Current module: " ${file}

    csplit -z $f /\/\/Fragment\ Shader/ '{*}'

    if [[ $? -eq 0 ]]; then
	echo "Split succeeded"
    else
	echo "Split failed"
	continue
    fi
    
    [ ! -z xx00 ] || continue
    [ ! -z xx01 ] || continue

    mv xx00 xx00.vert
    mv xx01 xx01.frag

    glslangValidator -V 'xx00.vert' -o "$file.$SDR.vspv"
    glslangValidator -V 'xx01.frag' -o "$file.$SDR.fspv"
    
    rm -f xx00.vert xx01.frag
done

rm -f xx00* xx01*

cd $BWD
