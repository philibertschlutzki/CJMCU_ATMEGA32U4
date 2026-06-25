# CJMCU ATMEGA32U4 Keyboard Automator (de-CH)

Dieses Repository verwandelt ein **CJMCU ATMEGA32U4** Board mit integriertem MicroSD-Kartenleser in einen voll funktionsfähigen Rubber Ducky Klon. Im Gegensatz zu herkömmlichen Rubber Duckies müssen Payloads hier nicht hartkodiert und bei jeder Änderung neu kompiliert werden – sie werden stattdessen einfach von einer SD-Karte gelesen. 

Diese Version ist speziell für das **Schweizerdeutsche Tastaturlayout (de-CH)** optimiert und entspricht **strengen Architektur- und Fail-Safe-Richtlinien** für den Headless-Betrieb. Dynamische Speicherallokationen (wie die Arduino `String`-Klasse) sind vollständig eliminiert, um maximale Stabilität bei begrenztem SRAM (2,5 KB) zu garantieren.

## 🌟 Features
- **MicroSD-Karten-Support:** Führt Ducky Script Payloads direkt aus dem Speicher der SD-Karte aus.
- **Plug & Play:** Keine Treiber auf dem Zielsystem notwendig. Wird als reguläre USB-Tastatur erkannt.
- **Schweizerdeutsches Layout (`de-CH`):** Vermeidet fehlerhafte Tastenanschläge (wie vertauschtes Z und Y), die bei US-Layout-Scripts oft auftreten.
- **Headless Fail-Safe:** Ausgeklügeltes Error-Handling via LED-Blink-Codes für fehlerfreie Diagnose ohne Serial Monitor.
- **Automatisierte Builds (CI/CD):** Kompiliert bei jedem Push automatisch in eine fertige `.hex`-Firmware und stellt diese für Web-Flasher bereit.

## 🛠 Voraussetzungen
1. **Hardware:** Ein ATMEGA32U4 Arduino Ducky von CJMCU mit SD-Karten-Slot.
2. **Speichermedium:** Eine MicroSD-Karte (FAT32 formatiert).
3. **Software:** Ein moderner, WebSerial-fähiger Webbrowser **ODER** das mitgelieferte `flash.sh` Skript / Arduino IDE.

## 🚀 Installation & Flash-Vorgang

### Schritt 1: SD-Karte vorbereiten
1. Formatiere deine MicroSD-Karte im Dateisystem **FAT32**.
2. Erstelle eine Textdatei im Hauptverzeichnis der SD-Karte mit dem exakten Namen **`script.txt`**.
3. Schreibe deinen Ducky Script Payload in diese Datei (siehe unterstützte Befehle unten).

### Schritt 2: Firmware auf den CJMCU flashen

Für das Aufspielen der Firmware stehen drei Methoden zur Verfügung:

#### Methode A: Direkt über den Web-Loader (Empfohlen)
Dank der CI/CD-Pipeline wird die aktuelle `.hex`-Datei automatisch unter einer statischen URL publiziert.
1. Schließe das Board per USB an.
2. Öffne einen WebSerial-kompatiblen Online-Flasher für den `Caterina`-Bootloader.
3. Wähle als Controller-Typ: **Arduino Leonardo / ATmega32U4**.
4. Wähle das Board aus (Serieller Port) und lade die generierte `firmware.hex` aus der GitHub-Pipeline.
5. Klicke auf **Flash/Program**. Das Board wird automatisch zurückgesetzt und geflasht.

#### Methode B: Über das lokale Flash-Skript (Linux / avrdude)
1. Mache das Skript ausführbar: `chmod +x flash.sh`
2. Starte das Skript: `./flash.sh <pfad_zur_firmware.hex>`
3. **Bootloader-Modus aktivieren:** Schließe die Pins `RST` und `GND` am Board kurz (oder drücke den Reset-Button), sobald das Skript danach fragt. Das Skript erkennt den `/dev/ttyACM*` Port automatisch und startet den Flash-Vorgang.

#### Methode C: Arduino IDE (Für Entwickler)
1. Öffne `Keyboard_automator_ch.ino` in der Arduino IDE.
2. Installiere die Bibliotheken `SD` und `Keyboard`.
3. Wähle das Board **"Arduino Leonardo"** und den entsprechenden COM-Port.
4. Klicke auf **Hochladen**.

## 🚨 Error-Handling & Debugging (Headless)
Da das Board ohne verbundenen Bildschirm oder Serial Monitor ("headless") agiert, werden sämtliche Fehlerzustände über die **interne LED (Pin 13)** signalisiert. Achte auf folgende Blink-Codes:

| Blink-Code | Fehlerursache | Lösung |
| :--- | :--- | :--- |
| **Langsames Blinken**<br>(500ms AN / 500ms AUS) | **SD-Karte nicht gefunden** | Prüfe, ob die SD-Karte richtig im Slot sitzt, FAT32 formatiert ist und ob der SPI-Bus (Pin 4) funktioniert. |
| **Schnelles Blinken**<br>(100ms AN / 100ms AUS) | **Payload-Datei fehlt** | Die SD-Karte wurde erkannt, aber die Datei `script.txt` fehlt im Hauptverzeichnis. Erstelle die Datei. |
| **Dreifach-Blinken**<br>(3x kurz AN, dann Pause) | **Syntax- / Parser-Fehler** | Ein Kommando in der `script.txt` wurde nicht erkannt oder ist fehlerhaft geschrieben. Prüfe dein Ducky Script auf Tippfehler. |

## 📜 Unterstützte Befehle (Ducky Script)
Das Skript wertet folgende Kommandos innerhalb der `script.txt` zeilenweise aus:
- `STRING <Text>` - Tippt den angegebenen Text.
- `DELAY <Millisekunden>` - Pausiert die Ausführung (Achtung: Interne 5ms Hardware-Delays bleiben zwecks HID-Stabilität unangetastet).
- `REM <Text>` - Kommentar (wird ignoriert).
- **Sondertasten:** `ENTER`, `CTRL`, `SHIFT`, `ALT`, `GUI` (Windows/Mac), `UP`, `DOWN`, `LEFT`, `RIGHT`, `DELETE`, `PAGEUP`, `PAGEDOWN`, `HOME`, `ESC`, `INSERT`, `TAB`, `END`, `CAPSLOCK`, `SPACE` sowie `F1` bis `F12`.
