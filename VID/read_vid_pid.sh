#!/bin/bash

# Farben für die Ausgabe
GREEN='\\033[0;32m'
RED='\\033[0;31m'
YELLOW='\\033[1;33m'
NC='\\033[0m' # No Color

echo -e "${YELLOW}=== USB VID/PID Scanner ===${NC}"

# Verzeichnisse definieren (relativ zum Skript)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
JSON_FILE="$SCRIPT_DIR/target_pid_vid.json"

# Prüfen, ob die JSON-Datei existiert
if [ ! -f "$JSON_FILE" ]; then
    echo -e "${RED}[FEHLER] Die Datei target_pid_vid.json wurde nicht gefunden.${NC}"
    echo -e "Bitte stelle sicher, dass sie im gleichen Verzeichnis wie das Skript liegt ($SCRIPT_DIR)."
    exit 1
fi

# VID und PID aus der JSON-Datei auslesen
# Unterstützt 'jq', fällt aber sicherheitshalber auf 'grep' & 'awk' zurück, falls 'jq' nicht installiert ist.
# Zudem werden Präfixe wie "0x" entfernt und alles in Kleinbuchstaben konvertiert, um den Linux-Standard in /sys/ zu matchen.
if command -v jq &> /dev/null; then
    TARGET_VID=$(jq -r '.VID' "$JSON_FILE" | sed -E 's/^0[xX]//' | tr '[:upper:]' '[:lower:]')
    TARGET_PID=$(jq -r '.PID' "$JSON_FILE" | sed -E 's/^0[xX]//' | tr '[:upper:]' '[:lower:]')
else
    TARGET_VID=$(grep -i '"VID"' "$JSON_FILE" | awk -F'"' '{print $4}' | sed -E 's/^0[xX]//' | tr '[:upper:]' '[:lower:]')
    TARGET_PID=$(grep -i '"PID"' "$JSON_FILE" | awk -F'"' '{print $4}' | sed -E 's/^0[xX]//' | tr '[:upper:]' '[:lower:]')
fi

if [ -z "$TARGET_VID" ] || [ -z "$TARGET_PID" ]; then
    echo -e "${RED}[FEHLER] Konnte VID oder PID nicht aus der JSON-Datei auslesen.${NC}"
    exit 1
fi

echo "Suche nach Ziel-Gerät mit:"
echo "  Gesuchte VID : $TARGET_VID"
echo "  Gesuchte PID : $TARGET_PID"
echo "----------------------------------------"

# USB-Geräte scannen über /sys/bus/usb/devices/
FOUND=0

for syspath in /sys/bus/usb/devices/*; do
    if [ -f "$syspath/idVendor" ] && [ -f "$syspath/idProduct" ]; then
        VID=$(cat "$syspath/idVendor")
        PID=$(cat "$syspath/idProduct")

        if [ "$VID" == "$TARGET_VID" ] && [ "$PID" == "$TARGET_PID" ]; then
            FOUND=1
            MANUFACTURER="Unbekannt (Nicht hinterlegt)"
            PRODUCT="Unbekannt (Nicht hinterlegt)"
            
            [ -f "$syspath/manufacturer" ] && MANUFACTURER=$(cat "$syspath/manufacturer")
            [ -f "$syspath/product" ] && PRODUCT=$(cat "$syspath/product")

            echo -e "${GREEN}[ERFOLG] Zielgerät gefunden!${NC}"
            echo "  Hersteller: $MANUFACTURER"
            echo "  Produkt   : $PRODUCT"
            echo "  VID       : $VID"
            echo "  PID       : $PID"
            echo "  Sys-Pfad  : $syspath"
            echo "----------------------------------------"
        fi
    fi
done

if [ $FOUND -eq 0 ]; then
    echo -e "${RED}[FEHLSCHLAG] Es ist aktuell kein Gerät mit der gespooften VID $TARGET_VID und PID $TARGET_PID angeschlossen.${NC}"
fi
"""

file_path = "read_vid_pid.sh"
with open(file_path, "w", encoding="utf-8") as f:
    f.write(script_content)

print(f"File saved to {file_path}")
