# AI & Developer Agents Instruction File (AGENTS.md)

Dieses Dokument dient als **absolut wasserdichtes und unmissverständliches, striktes Regelwerk** für sämtliche zukünftigen KI-Agenten, Large Language Models (LLMs) und menschliche Entwickler, die bei der Erweiterung, Analyse oder Modifikation dieses Repositories assistieren.

**Jegliche Anpassungen an der Codebase, der Dokumentation oder der CI/CD-Pipeline müssen diesen Richtlinien ausnahmslos folgen.**

---

## 1. Pipeline & Web-Loader Architektur (Deployment)
Die CI/CD-Pipeline in `.github/workflows/build.yml` ist der Kernbestandteil unserer Web-Loader Bereitstellungsstrategie.
- **Zweck:** Der Workflow generiert aus dem Arduino-Code eine kompilierte `.hex`-Datei und stellt diese öffentlich via GitHub Pages (`gh-pages` Branch) zur Verfügung.
- **Regel:** Künftige Agents müssen wissen, dass die Pipeline die `.hex`-Datei als `firmware.hex` öffentlich zugänglich macht (statische Raw-URL ohne CORS-Beschränkungen).
- **Verbot:** Jegliche Modifikation an der `build.yml`, welche die Bereitstellungskette, die Dateibenennung (`firmware.hex`) oder den Public Access via GitHub Pages unterbricht oder abändert, ist **strikt untersagt**. Der Web-Loader (WebSerial API) verlässt sich auf eine stabile, ungehinderte Download-URL.

---

## 2. SRAM & Memory Management (Hardware-Restriktionen)
Der verwendete CJMCU ATmega32U4 Microcontroller verfügt über extrem limitierte **2,5 KB SRAM**.
- **Striktes Verbot:** Es herrscht ein **kategorisches, ausnahmsloses Verbot für jegliche dynamische Speicherallozierungen, ganz besonders die Nutzung, Instanziierung und dynamische Reallokation der Arduino `String`-Klasse**. Dies gilt unter allen Umständen, insbesondere jedoch innerhalb von (Endlos-)Schleifen oder Parsern.
- **Vorschrift:** Zukünftige Algorithmus-Anpassungen (z.B. beim Parsen von Ducky Scripts oder anderen Serial-Daten) **müssen zwingend** auf statisch allozierte Char-Arrays (`char buffer[MAX_LEN]`) umgestellt werden.
- **Grund:** Die Verwendung von dynamischen Speicherallozierungen (und den damit verknüpften Objekten wie `String`) führt auf AVR-Architekturen unvermeidlich zu Heap-Fragmentierung und unvorhersehbaren Abstürzen zur Laufzeit (Out-of-Memory).

---

## 3. Hardware-Interaktion & Timing (HID-Stabilität)
Dieses Board agiert primär als Human Interface Device (HID). Das Timing auf dem SPI-Bus sowie das HID-Report-Timing ist hochkritisch.
- **Regel:** Die hardcodierten Delays im Quellcode – insbesondere die **5ms Pause zwischen einzelnen Keystrokes** – sind geschäftskritisch für die HID-Stabilität über den SPI-Bus.
- **Verbot:** Diese Pausen (Delays) dürfen weder reduziert noch entfernt werden. Eine Unterschreitung führt dazu, dass Host-Betriebssysteme Tastaturanschläge verschlucken oder das Gerät als fehlerhaft auswerfen.

---

## 4. Strict CI/CD Dependency Rule (Hard Fail-Kriterium)
Da die Firmware ausschließlich über die CI/CD-Pipeline an die Endbenutzer ausgeliefert wird, müssen Abhängigkeiten absolut konsistent sein.
- **Regel:** Sobald neue Bibliotheken im `.ino`-Code (oder anderen C++-Dateien) per `#include` referenziert werden, **müssen** diese zwingend im Schritt `Install dependencies` (`arduino-cli lib install "LibraryName"`) in der Datei `.github/workflows/build.yml` nachgetragen werden.
- **Durchsetzung:** Dies ist ein **"Hard Fail"-Kriterium** für jeden Pull Request. Fehlt der entsprechende Installationsschritt im Workflow, ist die Code-Änderung von Agenten/Entwicklern als unvollständig und fehlerhaft abzulehnen.

---

## 5. Fail-Safe & Debugging (Headless Operation)
Das ATmega32U4 Board wird "headless" (ohne Serial Monitor) betrieben. Klassisches `Serial.println()` ist daher nutzlos für das Debugging durch den Endnutzer.
- **Standard für alle künftigen Tasks:** Jede Art von Fehlerbehandlung – insbesondere fehlende Peripherie (z.B. wenn die SD-Karte am CS Pin 4 nicht gefunden wird) oder Parse-Fehler – **muss zwingend** über Hardware-Indikatoren signalisiert werden. Hardware-Fehler im Headless-Betrieb müssen ausnahmslos über visuelle Blink-Codes abgefangen und signalisiert werden.
- **Umsetzung:** Implementiere bei Fehlerschleifen (`while(1)`) immer visuelle **Blink-Codes** (z. B. auf Pin 13 oder den RX / TX LEDs). Dies ermöglicht eine Fehlerdiagnose ("Debugging without Monitor") für den User.