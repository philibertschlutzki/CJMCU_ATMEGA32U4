# CJMCU ATMEGA32U4 BadUSB Automator (de-CH)

Dieses Repository verwandelt ein **CJMCU ATMEGA32U4** Board mit integriertem MicroSD-Kartenleser in einen voll funktionsfähigen BadUSB / Rubber Ducky Klon. Im Gegensatz zu herkömmlichen Rubber Duckies müssen Payloads hier nicht hartkodiert und bei jeder Änderung neu kompiliert werden – sie werden stattdessen einfach von einer SD-Karte gelesen. 

Diese Version ist speziell für das **Schweizerdeutsche Tastaturlayout (de-CH)** optimiert und basiert auf den Prinzipien des bekannten [asciiterminal/CJMCU_ATMEGA32U4_BADUSB](https://github.com/asciiterminal/CJMCU_ATMEGA32U4_BADUSB) Repositories. Sie wurde zusätzlich um eine automatisierte CI/CD-Pipeline via GitHub Actions erweitert.

## 🌟 Features (Sämtliche Funktionalitäten integriert)
- **MicroSD-Karten-Support:** Führt Ducky Script Payloads direkt aus dem Speicher der SD-Karte aus.
- **Plug & Play:** Keine Treiber auf dem Zielsystem notwendig. Wird als reguläre USB-Tastatur erkannt.
- **Schweizerdeutsches Layout (`de-CH`):** Vermeidet fehlerhafte Tastenanschläge (wie vertauschtes Z und Y), die bei US-Layout-Scripts oft auftreten.
- **Automatisierte Builds:** Dank GitHub Actions kompiliert das Projekt bei jedem Push automatisch in eine fertige `.hex`-Firmware.
- **Ducky Script Interpreter:** Unterstützt `STRING`, `DELAY`, `REM` und alle gängigen Sondertasten.

## 🛠 Voraussetzungen
1. **Hardware:** Ein ATMEGA32U4 Arduino Ducky von CJMCU mit SD-Karten-Slot. Kein Löten erforderlich.
2. **Speichermedium:** Eine MicroSD-Karte (beliebige Größe) sowie ein SD-Kartenleser.
3. **Software:** Die [Arduino IDE](https://www.arduino.cc/en/Main/Software) zum anfänglichen Flashen.

## 🚀 Installation & Nutzung

### Schritt 1: SD-Karte vorbereiten
1. Formatiere deine MicroSD-Karte im Dateisystem **FAT32**.
2. Erstelle eine einfache Textdatei im Hauptverzeichnis der SD-Karte mit dem exakten Namen **`script.txt`**.
3. Schreibe deinen gewünschten Ducky Script Payload in diese Datei und speichere sie. (Starte am besten mit einem simplen "Hello World"-Skript).

### Schritt 2: Firmware auf den CJMCU flashen
Du musst dem Board einmalig beibringen, die SD-Karte auszulesen und als Tastatur zu agieren:
1. Öffne die Datei `Keyboard_automator_ch/Keyboard_automator_ch.ino` in der Arduino IDE.
2. Stelle sicher, dass die Bibliotheken `SD` und `Keyboard` installiert sind.
3. Wähle im Menü unter Tools > Board **"Arduino Leonardo"** aus.
4. Schließe das CJMCU Board über USB an, wähle den passenden Port und klicke auf **"Hochladen"**.

*(Alternativ: Lade die fertig kompilierte `.hex`-Datei direkt aus den "Actions" (Artifacts) dieses GitHub-Repos herunter und flashe sie manuell).*

### Schritt 3: Ausführung ("The Magic")
1. Stecke die SD-Karte in den Slot des CJMCU Boards.
2. Schließe das Board an einen beliebigen Zielcomputer an.
3. Das Skript wird automatisch und unmittelbar ausgeführt!

## 📜 Unterstützte Befehle (Ducky Script)
Das Skript wertet folgende Kommandos innerhalb der `script.txt` zeilenweise aus:
- `STRING <Text>` - Tippt den angegebenen Text.
- `DELAY <Millisekunden>` - Pausiert die Ausführung.
- `REM <Text>` - Kommentar (wird ignoriert).
- **Sondertasten:** `ENTER`, `CTRL`, `SHIFT`, `ALT`, `GUI` (Windows/Mac-Taste), `UP`, `DOWN`, `LEFT`, `RIGHT`, `DELETE`, `PAGEUP`, `PAGEDOWN`, `HOME`, `ESC`, `INSERT`, `TAB`, `END`, `CAPSLOCK`, `SPACE` sowie `F1` bis `F12`.
