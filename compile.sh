rm -rf ./build

if [ -z $1 ] 
then
  sketchfile=distance/distance.ino
else
  sketchfile=$1
fi

arduino                     \
  --verify                  \
  --pref build.path=./build \
  --board attiny:avr:ATtinyX5:cpu=attiny85,clock=internal8 \
  $sketchfile

cp build/*.hex . 
