# Bug: Lokaler Kompilierungs-Test schlägt fehl (Missing KeyboardLayout_de_CH.h)

## Beschreibung
Der Build-Prozess bricht sofort ab, da die Datei `Keyboard_automator_ch.ino` die Bibliothek `"KeyboardLayout_de_CH.h"` lokal inkludiert, diese Datei sich jedoch nicht im Repository befindet.

## Fehlermeldung
`The local compilation test failed due to a missing KeyboardLayout_de_CH.h file in the original repository`

## Auswirkung
Die CI/CD Pipeline in `.github/workflows/build.yml` scheitert beim Schritt "Compile". Es kann keine `firmware.hex` für den Web-Loader erstellt werden.

## Lösung
1. Die fehlende Datei `KeyboardLayout_de_CH.h` in das Verzeichnis `Keyboard_automator_ch/` hochladen.
2. Commit & Push ausführen, um die Pipeline erneut anzustoßen.


# Kritisch: Verstoß gegen AGENTS.md - Nutzung der String-Klasse

## Beschreibung
Im Quellcode `Keyboard_automator_ch.ino` wird die Arduino `String`-Klasse extensiv für das Parsen der Payload-Dateien genutzt (z. B. `String line`, `String remain`).

## Problem
Dies verstößt massiv gegen Abschnitt 2 ("SRAM & Memory Management") der `AGENTS.md`. Der ATmega32U4 verfügt nur über 2,5 KB SRAM. Die dynamische Speicherallozierung durch `String`-Objekte in einer Schleife führt unweigerlich zu Heap-Fragmentierung und unvorhersehbaren Laufzeitabstürzen ("Out-of-Memory").

## Lösung
- Striktes Refactoring des Ducky-Script-Parsers.
- Vollständige Entfernung aller `String`-Objekte.
- Umstellung auf statisch allozierte Char-Arrays (`char buffer[MAX_LEN]`) und C-String Funktionen (`strtok`, `strcmp` aus `<string.h>`).


# Feature/Bug: Fehlendes visuelles Error-Handling für Headless-Betrieb

## Beschreibung
Das Board agiert ohne Serial Monitor. Aktuell fängt das Script einen Ausfall der SD-Karte (`!SD.begin(4)`) einfach mit einem leeren `return;` ab.

## Problem
Abschnitt 5 der `AGENTS.md` ("Fail-Safe & Debugging") wird missachtet. Da kein Serial Monitor verfügbar ist, haben Benutzer keine Möglichkeit zu diagnostizieren, warum das Skript nicht startet (z. B. SD-Karte fehlt, Lesefehler).

## Lösung
- Ersetzen des stillen `return;` durch eine Endlosschleife (`while(1)`).
- Implementierung eines visuellen Blink-Codes (z. B. RX/TX LEDs oder Pin 13), um Hardware-Fehler zu signalisieren.

# Enhancement: Automatisierte GitHub Releases in der CI/CD Pipeline ergänzen

## Beschreibung
Die aktuelle CI/CD Pipeline (`build.yml`) publiziert die kompilierte Firmware lediglich als Artifact und auf dem `gh-pages` Branch für den Web-Loader. Es wird jedoch kein formelles GitHub Release angelegt.

## Problem
Benutzer, die das Repository aufsuchen, finden im "Releases"-Reiter auf GitHub keine kompilierte Firmware oder Historie, da der Schritt in der Pipeline fehlt.

## Lösung
Die `.github/workflows/build.yml` um eine Action (wie z. B. `softprops/action-gh-release`) erweitern. Sobald ein Tag gepusht wird oder ein Push auf den `main` Branch erfolgt, sollte automatisch ein offizielles GitHub Release inkl. angehängter `firmware.hex` erstellt werden.
