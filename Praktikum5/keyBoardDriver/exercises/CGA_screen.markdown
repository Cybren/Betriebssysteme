\page CGA-Screen Informationen zur Ansteuerung der CGA-Grafikkarte

\section vram Video-RAM
Jede PC Grafikkarte verfügt über einen eigenen Speicherbereich, das
Video-RAM, mit dessen Hilfe der Bildschirminhalt programmiert werden kann.
Im Grafikmodus müssen dazu für jeden Bildpunkt je nach der Anzahl der
möglichen Farben ein oder mehrere Bits im Video-RAM gesetzt bzw. gelöscht
werden. Im Textmodus genügt es dagegen, den ASCII-Code des gewünschten
Zeichens, zusammen mit dessen Darstellungsattributen in das Video-RAM zu
übertragen. Die Umsetzung des ASCII-Codes in eine Pixelmatrix wird dann von
der Grafikkarte eigenständig vorgenommen.

In der Rechnerübung zu Betriebssysteme sollen alle Ausgaben im CGA
Textmodus erfolgen. Dieser Modus wird von fast allen Grafikkarten
unterstützt, ist einfach zu programmieren und für das Übungsbetriebssystem
vollkommen ausreichend.

Im CGA Textmodus belegt jede Bildschirmposition zwei Bytes im Video-RAM.
Das erste der beiden Bytes (gerade Offsetadresse) nimmt den ASCII-Code des
darzustellenden Zeichens auf, das zweite (ungerade Offsetadresse) die
gewünschte Vorder- und Hintergrundfarbe. Die Abbildung der
Bildschirmpositionen auf die Einträge im Video-RAM erfolgt nach Zeilen und
Spalten geordnet. Bei einer Auflösung von 80 Zeichen pro Zeile und 25
Zeilen belegt das Zeichen in der linken oberen Ecke die Bytes 0 und 1, das
Zeichen rechts daneben die Bytes 2 und 3 und das Zeichen am Ende der ersten
Zeile die Bytes 158 und 159. Die Zählung wird dann mit dem ersten Zeichen
der zweiten Zeile fortgesetzt.

![Das Mapping im CGA Screen von Zeichen auf Speicher. Im Textmodus wir der Bildschirm in 80x25 Zeichen eingeteilt, wovon jedes von 2 Bytes im CGA Speicher abgebildet wird.](img/cga.svg)

Da das Video-RAM in den Hauptspeicher des PCs eingeblendet ist, kann es mit
Hilfe normaler Speicherzugriffe beschrieben werden. Offset 0 im Video-RAM
wird mit (hexadezimal) \b b8000 addressiert.

\section attribs Darstellungsattribute
Zu jedem Zeichen können die Merkmale Vordergrundfarbe, Hintergrundfarbe und
Blinken einzeln festgelegt werden. Für diese Attribute steht pro Zeichen
ein Byte zur Verfügung, dessen Bits folgende Bedeutung haben:

<center>
<table>
<tr><th colspan="2">Darstellungsattribute</th></tr>
<tr><td>Bits 0-3</td><td>Vordergrundfarbe</td></tr>
<tr><td>Bits 4-6</td><td>Hintergrundfarbe</td></tr>
<tr><td>Bit 7</td><td>Blinken</td></tr>
</table>
</center>

Im CGA Textmodus stehen die folgenden 16 Farben zur Verfügung:

<center><table>
<tr><th colspan="4">Farbpalette</th></tr>
<tr><td align="RIGHT">0</td><td>Schwarz</td><td align="RIGHT">8</td><td>Dunkelgrau</td></tr>
<tr><td align="RIGHT">1</td><td>Blau</td><td align="RIGHT">9</td><td>Hellblau</td></tr>
<tr><td align="RIGHT">2</td><td>Grün</td><td align="RIGHT">10</td><td>Hellgrün</td></tr>
<tr><td align="RIGHT">3</td><td>Cyan</td><td align="RIGHT">11</td><td>Hellcyan</td></tr>
<tr><td align="RIGHT">4</td><td>Rot</td><td align="RIGHT">12</td><td>Hellrot</td></tr>
<tr><td align="RIGHT">5</td><td>Magenta</td><td align="RIGHT">13</td><td>Hellmagenta</td></tr>
<tr><td align="RIGHT">6</td><td>Braun</td><td align="RIGHT">14</td><td>Gelb</td></tr>
<tr><td align="RIGHT">7</td><td>Hellgrau</td><td align="RIGHT">15</td><td>Weiß</td></tr>
</table></center>

Da für die Hintergrundfarbe im Attributbyte nur drei Bits zur Verfügung
stehen, können auch nur die ersten acht Farben zur Hintergrundfarbe gewählt
werden.

\section cursor Kontrolle des Cursors
Um die aktuelle Cursorposition abfragen oder setzen zu können, muß der
Videocontroller der Grafikkarte programmiert werden. Der Videocontroller
der CGA-Karte stellt insgesamt 18 Steuerregister (8-Bit Wortbreite) zur
Verfügung. Für die Cursorsteuerung sind hier nur die Steuerregister 14 und
15 interessant:

<center><table>
<tr><th>Index</th><th>Register</th><th>Bedeutung</th></tr>
<tr><td>14</td><td>Cursor (high)</td>
<td rowspan="2">Zeichenoffset der Cursorposition</td></tr>
<tr><td>15</td><td>Cursor (low)</td></tr>
</table></center>

Die Steuerregister können jedoch nicht direkt angesprochen werden. Der
Zugriff erfolgt vielmehr indirekt über ein Index- und ein Datenregister.
Dazu wird in das Indexregister zunächst die Nummer des Steuerregisters
geschrieben, das gelesen oder geschrieben werden soll. Der eigentliche
Zugriff auf den Inhalt des Steuerregisters (lesen/schreiben) erfolgt dann
über das Datenregister.
Daten- und Indexregister können direkt mit Hilfe der in und out Befehle
angesprochen werden, da ihnen eigenständige Portadressen zugeordnet sind:

<center>
  Index  | Register      | Zugriffsart
  -------|---------------|---------------------
   0x3d4 | Indexregister | nur schreiben
   0x3d5 | Datenregister | lesen und schreiben
</center>
