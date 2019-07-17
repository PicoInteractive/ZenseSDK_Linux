#!/bin/sh

ConfigPath=$HOME/.config/PicoZense

if [ ! -d $ConfigPath ]; then

  mkdir $ConfigPath

fi

cp -rf ./Config $ConfigPath
