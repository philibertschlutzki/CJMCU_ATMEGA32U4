# CJMCU ATMEGA32U4 BadUSB Automator (de-CH)

Dieses Repository verwandelt ein **CJMCU ATMEGA32U4** Board mit integriertem MicroSD-Kartenleser in einen voll funktionsfähigen BadUSB / Rubber Ducky Klon. Im Gegensatz zu herkömmlichen Rubber Duckies müssen Payloads hier nicht hartkodiert und bei jeder Änderung neu kompiliert werden – sie werden stattdessen einfach von einer SD-Karte gelesen. 

Diese Version ist speziell für das **Schweizerdeutsche Tastaturlayout (de-CH)** optimiert und basiert auf den Prinzipien des bekannten [asciiterminal/CJMCU_ATMEGA32U4_BADUSB](https://github.com/asciiterminal/CJMCU_ATMEGA32U4_BADUSB) Repositories. Sie wurde zusätzlich um eine automatisierte CI/CD-Pipeline via GitHub Actions erweitert.

## 🌟 Features (Sämtliche Funktionalitäten integriert)
- **MicroSD-Karten-Support:** Führt Ducky Script Payloads direkt aus dem Speicher der SD-Karte aus.
- **Plug & Play:** Keine Treiber auf dem Zielsystem notwendig. Wird als reguläre USB-Tastatur erkannt.
- **Schweizerdeutsches Layout (`de-CH`):** Vermeidet fehlerhafte Tastenanschläge (wie vertauschtes Z und Y), die bei US-Layout-Scripts oft auftreten.
- **Automatisierte Builds & Web-Flasher-Support:** Dank GitHub Actions kompiliert das Projekt bei jedem Push automatisch in eine fertige `.hex`-Firmware. Diese wird direkt bereitgestellt und kann ohne lokale Software-Installation via Web-Loader auf das Board geflasht werden.
- **Ducky Script Interpreter:** Unterstützt `STRING`, `DELAY`, `REM` und alle gängigen Sondertasten.

## 🛠 Voraussetzungen
1. **Hardware:** Ein ATMEGA32U4 Arduino Ducky von CJMCU mit SD-Karten-Slot. Kein Löten erforderlich.
2. **Speichermedium:** Eine MicroSD-Karte (beliebige Größe) sowie ein SD-Kartenleser.
3. **Software:** Ein moderner, WebSerial-fähiger Webbrowser (z. B. Google Chrome, Microsoft Edge oder Opera) für den Web-Loader **ODER** optional die [Arduino IDE](https://www.arduino.cc/en/Main/Software) für manuelle, lokale Anpassungen.

## 🚀 Installation & Nutzung

### Schritt 1: SD-Karte vorbereiten
1. Formatiere deine MicroSD-Karte im Dateisystem **FAT32**.
2. Erstelle eine einfache Textdatei im Hauptverzeichnis der SD-Karte mit dem exakten Namen **`script.txt`**.
3. Schreibe deinen gewünschten Ducky Script Payload in diese Datei und speichere sie. (Starte am besten mit einem simplen "Hello World"-Skript).

### Schritt 2: Firmware auf den CJMCU flashen

Für das Aufspielen der Firmware stehen zwei Methoden zur Verfügung:

#### Methode A: Direkt über den Web-Loader (Empfohlen – Keine Installation notwendig)
Da die GitHub Actions Pipeline bei jeder Codeänderung automatisch ein stabiles Produktions-Binary erzeugt und via GitHub Pages/Releases publiziert, kannst du das Board direkt über den Browser flashen:
1. Schließe das CJMCU-Board über ein USB-Kabel an deinen Rechner an.
2. Öffne einen WebSerial-kompatiblen Online-Flasher (z. B. einen webbasierten AVR/Arduino-Flasher mit Support für den `Caterina`-Bootloader).
3. Wähle im Web-Loader das passende Board-Profil bzw. den Controller-Typ aus: **Arduino Leonardo / ATmega32U4**.
4. Klicke auf **Connect** (Verbinden) und wähle den seriellen Port des angeschlossenen Boards aus.
5. Nutze den Direktlink zur aktuellen `.hex`-Datei aus der GitHub-Pipeline (CORS-konforme Raw-URL) oder lade das kompilierte `.hex`-File herunter und ziehe es per Drag-and-Drop in den Web-Loader.
6. Starte den Flash-Vorgang über die Schaltfläche **Program** / **Flash**. Das Board wird automatisch zurückgesetzt, in den Bootloader-Modus versetzt und mit der Schweizer Version bespielt.

#### Methode B: Manuell über die Arduino IDE (Für Entwickler)
Falls du den Quellcode modifizieren oder lokal bauen möchtest:
1. Öffne die Datei `Keyboard_automator_ch/Keyboard_automator_ch.ino` in der Arduino IDE.
2. Stelle sicher, dass die Bibliotheken `SD` und `Keyboard` installiert sind.
3. Wähle im Menü unter Tools > Board **"Arduino Leonardo"** aus.
4. Schließe das CJMCU Board über USB an, wähle den passenden Port und klicke auf **"Hochladen"**.

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
