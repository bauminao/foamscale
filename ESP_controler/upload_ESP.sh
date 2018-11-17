if [ -z $1 ] 
then
  hexfile=`ls *.bin | head -n 1`
else
  hexfile=$1
fi

esptool.py --baud 460800 write_flash 0x0000 $hexfile
