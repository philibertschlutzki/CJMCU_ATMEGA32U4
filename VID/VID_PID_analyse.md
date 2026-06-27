Hier ist die maßgeschneiderte, detaillierte Anleitung zur Analyse und Manipulation der VID/PID, exakt abgestimmt auf die Architektur deines `CJMCU_ATMEGA32U4`-Repositories.

Da dein Projekt auf den Standard-Arduino-Core über die `arduino-cli` setzt und den Caterina-Bootloader verwendet, ergeben sich spezifische Verhaltensweisen beim Auslesen der IDs.

---

# Leitfaden: VID/PID Analyse und Spoofing (CJMCU ATmega32U4)

Dieser Leitfaden beschreibt, wie die Vendor ID (VID) und Product ID (PID) im vorliegenden Keyboard-Automator-Projekt ausgelesen, systemseitig analysiert und für Security-Tests (z.B. Whitelist-Bypasses) modifiziert werden können.

## 1. Statische Analyse: Woher kommen VID und PID in diesem Repo?

In diesem Projekt sind VID und PID **nicht** direkt im Quellcode (`Keyboard_automator_ch.ino`) hardcodiert.

Ein Blick in deine CI/CD-Pipeline (`.github/workflows/build.yml`) zeigt den entscheidenden Kompilierungsbefehl:

```yaml
arduino-cli compile --fqbn arduino:avr:leonardo ./Keyboard_automator_ch ...

```

Durch das Target `arduino:avr:leonardo` erbt die Firmware automatisch die Default-IDs der Arduino-Plattform für dieses Board:

* **Runtime-Modus (Sketch läuft, HID aktiv):** * VID: `0x2341` (Arduino SA)
* PID: `0x8036` (Leonardo)


* **Bootloader-Modus (Caterina aktiv, bereit für avrdude):**
* VID: `0x2341`
* PID: `0x0036`



### Wie man VID/PID für Security-Zwecke anpasst (Spoofing)

Um das Gerät gegenüber Endpoint-Protection-Systemen als vertrauenswürdige Hardware (z.B. eine Dell- oder Logitech-Tastatur) auszugeben, musst du den Code nicht umschreiben. Du kannst die IDs direkt beim Build-Prozess in der `.github/workflows/build.yml` überschreiben.

Passe den Compile-Schritt wie folgt an, um Build-Properties zu injizieren:

```yaml
      - name: Compile with custom VID/PID
        run: |
          arduino-cli compile --fqbn arduino:avr:leonardo \
          --build-property build.vid=0x046D \
          --build-property build.pid=0xC31C \
          --build-property usb.vid=0x046D \
          --build-property usb.pid=0xC31C \
          ./Keyboard_automator_ch --output-dir ./build-output

```

*(Das Beispiel nutzt `0x046D` und `0xC31C`, was einer regulären Logitech-Tastatur entspricht).*

## 2. Dynamische Analyse auf dem Host-System (Ubuntu / Linux)

Um das tatsächliche Verhalten des geflashten Boards bei der USB-Enumeration zu analysieren, eignen sich native Linux-Tools. Der CJMCU wechselt beim Einstecken die PID, was genau beobachtet werden muss.

### A. Den Caterina-Bootloader-Wechsel auslesen

Dein eigenes Flash-Skript (`flash.sh`) macht sich diese PID-Änderung bereits zunutze. Wenn das Board über den RST-Pin zurückgesetzt wird, wechselt es für ca. 8 Sekunden in den Bootloader.

Du kannst dieses Verhalten direkt in der Konsole überwachen, analog zur Logik in deinem Skript:

```bash
# Überwacht Systemereignisse in Echtzeit
udevadm monitor --environment --udev | grep -E "ID_VENDOR_ID|ID_MODEL_ID"

```

Wenn du nun den Reset-Button drückst, siehst du den Wechsel von `ID_MODEL_ID=8036` (Keyboard) auf `ID_MODEL_ID=0036` (Bootloader) und wieder zurück.

### B. USB-Deskriptor des HID-Payloads analysieren

Sobald das Board im Runtime-Modus ist (die Ducky-Scripts von der SD-Karte können gelesen werden), meldet es sich als Composite Device an. Um den vollständigen USB-Deskriptor auszulesen:

```bash
# Suche den CJMCU in der USB-Baumstruktur
lsusb -t

# Detaillierte Ausgabe aller Endpunkte für das spezifische Gerät
sudo lsusb -v -d 2341:8036

```

In der Ausgabe von `lsusb -v` solltest du gezielt auf folgende Attribute achten, um zu prüfen, wie das System das Gerät kategorisiert:

* `bInterfaceClass`: `0x03` (Human Interface Device)
* `bInterfaceProtocol`: `0x01` (Keyboard)

### C. Kernel-Ring-Buffer für Timing-Analysen

Um zu prüfen, ob die in `AGENTS.md` erwähnten Timing-Restriktionen und das HID-Report-Timing vom Kernel sauber geparst werden, hilft ein Blick in `dmesg`:

```bash
sudo dmesg -w | grep -i usb

```

Hier siehst du den exakten Millisekunden-Zeitstempel der Enumeration und ob der Linux-Kernel den HID-Deskriptor des ATmega32U4 ohne Parser-Fehler akzeptiert hat.

## 3. Programmatische Validierung

Wenn du nach dem Spoofing der CI/CD-Pipeline automatisiert verifizieren möchtest, ob die neue `firmware.hex` sich mit den korrekten manipulierten IDs meldet, kann ein kurzes Skript helfen.

Dies ist besonders nützlich, um sicherzustellen, dass die Custom-IDs vom System auch wirklich als "Keyboard" und nicht als generisches Serial-Device interpretiert werden:

```bash
#!/bin/bash
# Prüft angeschlossene Eingabegeräte auf spezifische VIDs
# Nützlich zur Validierung nach einem Firmware-Update

TARGET_VID="2341" # Anpassen an gespoofte VID

for sysdevpath in $(find /sys/bus/usb/devices/usb*/ -name dev); do
    syspath="${sysdevpath%/dev}"
    if [ -f "$syspath/idVendor" ]; then
        VID=$(cat "$syspath/idVendor")
        PID=$(cat "$syspath/idProduct")
        
        if [ "$VID" == "$TARGET_VID" ]; then
            echo "[!] CJMCU / Target Device gefunden:"
            echo "    VID: $VID"
            echo "    PID: $PID"
            echo "    Hersteller: $(cat "$syspath/manufacturer" 2>/dev/null)"
            echo "    Produkt: $(cat "$syspath/product" 2>/dev/null)"
        fi
    fi
done

```
