avrdude -c USBasp \
        -p t85 \
        -D \
        -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m  \
        -U flash:w:blink.ino.hex.bak 
