#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

if [ ! -f "$DIR/sausage.tar.gz" ]; then
  wget https://github.com/jlipps/sausage/archive/0.17.0.tar.gz -O "$DIR/sausage.tar.gz"
  tar -C "$DIR" -zxvf "$DIR/sausage.tar.gz" > /dev/null
  cp -R "$DIR/sausage-0.17.0" "$DIR/../sausage"
  rm -rf "$DIR/sausage-0.17.0"
fi