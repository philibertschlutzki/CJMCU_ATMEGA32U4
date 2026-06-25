#!/bin/bash

# Check if hex file argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <path_to_hex_file>"
    echo "Please provide the .hex file to flash."
    exit 1
fi

HEX_FILE="$1"

if [ ! -f "$HEX_FILE" ]; then
    echo "Error: File '$HEX_FILE' not found!"
    exit 1
fi

echo "Please reset the board into bootloader mode."
echo "Short RST and GND briefly (or press the reset button)."
echo "Waiting for /dev/ttyACM* device to appear..."

TIMEOUT=30
ELAPSED=0
PORT=""

while [ $ELAPSED -lt $TIMEOUT ]; do
    # Iteriere über alle vorhandenen ttyACM Geräte
    for dev in /dev/ttyACM*; do
        [ -e "$dev" ] || continue
        
        # Prüfe per udevadm, ob das Gerät die exakte PID des Caterina-Bootloaders (0036) hat
        if udevadm info -q property -n "$dev" 2>/dev/null | grep -q "ID_MODEL_ID=0036"; then
            PORT="$dev"
            echo "Found Caterina Bootloader at $PORT"
            break 2
        fi
    done
    
    sleep 1
    ELAPSED=$((ELAPSED + 1))
    echo -n "."
done
echo ""

if [ -z "$PORT" ] || [ ! -e "$PORT" ]; then
    echo "Error: Timeout reached. No /dev/ttyACM* device found."
    echo "Please ensure the board is in bootloader mode and connected."
    exit 1
fi

echo "Flashing $HEX_FILE to ATmega32U4 on port $PORT..."
avrdude -p atmega32u4 -c avr109 -P "$PORT" -b 57600 -U flash:w:"$HEX_FILE":i

if [ $? -eq 0 ]; then
    echo "Flashing successful!"
else
    echo "Flashing failed."
    exit 1
fi
