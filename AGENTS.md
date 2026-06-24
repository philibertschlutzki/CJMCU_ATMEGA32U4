# AI & Developer Agents Instruction File (AGENTS.md)

Dieses Dokument dient als Leitfaden für KI-Agenten, Large Language Models (LLMs) und Entwickler, die bei der Erweiterung, Analyse und Code-Generierung für dieses Projekt assistieren.

## 🎯 Projektzweck & Kontext
Wartung und Erweiterung einer BadUSB-Firmware für den CJMCU ATmega32U4 Microcontroller. Das Board liest zur Laufzeit Ducky Scripts (`script.txt`) von einer MicroSD-Karte über SPI und agiert als Human Interface Device (HID). 

## 📂 Systemarchitektur & Repository-Struktur
- **`.github/workflows/build.yml`**: CI/CD Pipeline. Verwendet `arduino-cli`, um Code für das Target `arduino:avr:leonardo` zu kompilieren. Die Output-Hex-Files werden für die Nutzer als Download (Artifact) bereitgestellt.
- **`Keyboard_automator_ch/Keyboard_automator_ch.ino`**: Hauptfirmware. Setzt einen funktionierenden SPI-Bus (SD-Karten CS-Pin ist `4`) und das Schweizer Tastaturlayout voraus.
- **`.gitignore`**: Ignoriert lokale Kompilierungs-Artefakte (`build-output/`).

## 🛠 Technische Randbedingungen & Guidelines für Code-Änderungen

Bei der Modifikation von Code oder dem Beantworten von Issues sind folgende Punkte durch Agents zwingend zu beachten:

1. **Hardware-Limitationen:**
   - Der ATmega32U4 besitzt nur 2,5 KB SRAM.
   - **Regel:** Vermeide exzessive Nutzung der `String`-Klasse von Arduino in Schleifen, da dies schnell zu Heap-Fragmentierung und Abstürzen führt. Wenn möglich, implementiere String-Parsing speichereffizienter (z.B. zeichenweises Lesen mit statischen Puffern).

2. **Ducky Script Parsing (`Line()` & `Press()`):**
   - Befehle werden beim Einlesen beim Zeilenumbruch (`\n`) oder am Ende der Datei evaluiert.
   - Zwischen allen individuellen Tastendrücken liegt eine hardkodierte Pause von `5ms`.
   - Das Hinzufügen neuer Befehle erfordert das Erweitern der if/else-Blöcke in der Methode `Press()` oder `Line()`. 

3. **CI/CD Pipeline Updates:**
   - Wenn der `.ino`-Datei neue externe Bibliotheken hinzugefügt werden, **muss** der Workflow in `.github/workflows/build.yml` im Schritt `Install dependencies` (`arduino-cli lib install "LibraryName"`) zwingend mit aktualisiert werden. Ansonsten scheitert der Build in der Pipeline.

4. **Fehlerbehandlung (Roadmap für künftige Tasks):**
   - Momentan gibt es keine visuelle Indikation, falls die SD-Karte (`!SD.begin(4)`) oder die Datei nicht gefunden wird. Zukünftige Code-Vorschläge sollten LED-Blink-Codes (z. B. auf Pin 13 / RX / TX LEDs) implementieren, um Fehlerbehebung ("Debugging without Monitor") zu ermöglichen.
   - Einbindung dynamischer Befehle wie `DEFAULT_DELAY` oder `REPEAT`.

Bitte generiere Änderungen immer im Stil von Embedded C++ und kommentiere signifikante Algorithmus-Anpassungen im Code.
