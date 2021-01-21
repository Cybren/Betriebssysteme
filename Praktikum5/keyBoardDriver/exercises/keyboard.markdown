\page keyboard Informationen zur PC-Tastatur
\section keycodes Zeichen- und Tastaturcodes

Für die Tastaturabfrage sind mehrere verschiedene Codes von Bedeutung:

## ASCII Code ##

Der "American Standard Code for Information Interchange" ist eine in der
Computerwelt sehr weit verbreitete Zuordnungstabelle für die Darstellung
von Buchstaben, Ziffern und Sonderzeichen. Ursprünglich waren pro Zeichen 7
Bits vorgesehen, mittlerweile haben sich 8 Bits, also ein Byte
durchgesetzt. Die folgende Tabelle zeigt einen kleinen Ausschnitt aus der
ASCII-Tabelle:

<center><table>
<tr><th>Zeichen</th><th>ASCII-Code</th></tr>
<tr><td>(</td><td>40</td></tr>
<tr><td>0</td><td>48</td></tr>
<tr><td>1</td><td>49</td></tr>
<tr><td>2</td><td>50</td></tr>
<tr><td>A</td><td>65</td></tr>
<tr><td>B</td><td>66</td></tr>
<tr><td>a</td><td>97</td></tr>
<tr><td colspan=2 align="center">...</td></tr>
</table></center>

Zeichen und Zeichenketten werden üblicherweise im ASCII Code abgespeichert.

## Scancode ##

Mit dem Scancode werden den Tasten der PC Tastatur eindeutige Nummern
zugeordnet. Damit ist es auch möglich, Tasten, wie die Cursortasten, denen
kein druckbares Zeichen entspricht, zu identifizieren. Bei Scancodes wird
nicht zwischen Groß- und Kleinbuchstaben unterschieden, da beide mit
derselben Taste erreicht werden.

<center><table>
<tr><th>Taste</th><th>Scancode</th></tr>
<tr><td>A</td><td>30</td></tr>
<tr><td>S</td><td>31</td></tr>
<tr><td>D</td><td>32</td></tr>
<tr><td>Cursor hoch</td><td>72</td></tr>
<tr><td>Cursor runter</td><td>80</td></tr>
<tr><td colspan=2 align="center">...</td></tr>
</table></center>

Im Laufe der PC Entwicklungsgeschichte wurden unterschiedliche Tastaturen
mit einer unterschiedlichen Anzahl und Bedeutung von Tasten herausgebracht.
Gerade bei den Funktions- und Spezialtasten gibt es daher auch
unterschiedliche Scancodes. Da PC Tastaturen nur wenig mehr als 100 Tasten
besitzen, genügen 7 Bits, um den Scancode darzustellen.

## Make- und Breakcodes ##

Programme müssen nicht nur feststellen können, welche der "normalen" Tasten
gedrückt wurden, sondern auch, ob gleichzeitig die Shift (Umschalt)-Taste,
die Control (Steuerungs)-Taste oder die ALT-Taste festgehalten wurden.
Daher sendet die Tastatur statt eines einfachen Scancodes einen oder
mehrere sogenannte Makecodes für jedes Drücken und einen oder mehrere
Breakcodes für jedes Loslassen einer Taste. Wenn eine Taste länger als eine
bestimmte Zeitspanne festgehalten wird, werden darüberhinaus zusätzliche
Makecodes gesendet, durch die die Wiederholungsfunktion realisiert wird.
Bei den meisten Tasten entspricht der Makecode dem Scancode und der
Breakcode dem Scancode mit gesetztem 7. Bit. Einige Tasten erzeugen jedoch
aus historischen Gründen schon beim einmaligen Drücken und Loslassen
mehrere Make- und Breakcodes. Der Tastaturtreiber (in der 3. Aufgabe des
Übungssystems Keyboard::prologue()) muss aus den Make- und Breakcodes der
gedrückten Tasten die gewünschten Zeichen ermitteln.

<em>Anmerkung: Da die Interpretation der Make- und Breakcodes ziemlich mühsam,
langweilig und wenig lehrreich ist, haben wir euch die Dekodierung
abgenommen. Es kann allerdings sein, dass diese Funktion bei eurer Tastatur
nicht alle Zeichen, insbesondere die deutschen Umlaute korrekt erkennt. In
diesem Fall müsst ihr entweder mit ein paar falschen Zeichen leben oder die
Tabellen der Dekodierfunktion entsprechend anpassen.</em>

\section ablauf Ablauf beim Drücken einer Taste

Wenn bei einer PC Tastatur eine Taste gedrückt wird, werden zwei sich
kreuzende Leitungen der Scan-Matrix innerhalb der Tastatur verbunden. Der
Tastaturprozessor (8042 für PC/XT-, 8048 für AT und MF II-Tastaturen)
ermittelt die Position der gedrückten Taste und daraus den Scancode. Über
eine serielle Schnittstelle wird der Code zum Motherboard des PCs gesendet.

\image html keyboard.gif

Auf dem Motherboard des PCs befindet sich ein Tastaturcontroller, der auf
der einen Seite mit der Tastatur über einen Eingabeport und einen
Ausgabeport (Kommandos an die Tastatur) kommuniziert. Auf der anderen Seite
hat der Controller Register, die mit Hilfe von \c in- und \c out-Befehlen
über den Systembus gelesen und beschrieben werden können.

<table>
<tr><th>Port</th><th>lesen(R),
schreiben(W)</th><th>Register</th><th>Bedeutung</th></tr>
<tr><td>0x60</td><td>R</td><td>Ausgabepuffer</td><td>Make/Breakcode von der
Tastatur</td></tr>
<tr><td>0x60</td><td>W</td><td>Eingabepuffer</td><td>Befehle für den
Tastatur<b>prozessor</b> (z.B. LEDs setzen)</td></tr>
<tr><td>0x64</td><td>W</td><td>Steuerregister</td><td>Befehle für den
Tastatur<b>controller</b> (von uns nicht verwendet)</td></tr>
<tr><td>0x64</td><td>R</td><td>Statusregister</td><td>Zustand des
Tastaturcontrollers (z.B. Ausgabepuffer voll?)</td></tr>
</table>

Immer wenn ein Byte vom Tastaturcontroller in seinen Ausgabepuffer
geschrieben wird, signalisiert er das durch Setzen einer
Interruptanforderung. Der Prozessor muss darauf reagieren, indem er das
angekommene Byte aus dem Ausgabepuffer ausliest. Im Statusregister wird
dann vermerkt, daß der Ausgabepuffer wieder leer ist. Erst jetzt können
neue Zeichen von der Tastatur entgegengenommen werden. Wenn die Tastatur im
Pollingbetrieb benutzt wird, kann durch Bit 0 (outb) überprüft werden, ob sich tatsächlich
Zeichen im Ausgabepuffer des Tastaturcontrollers befinden. In die
Gegenrichtung muss immer gewartet werden, bis der Eingabepuffer des
Tastaturcontrollers leer ist (== Bit 1, inpb, den Wert 0 hat), bevor ein neuer
Befehlscode geschrieben wird.

Bei PS/2 PCs ist die Maus ebenfalls an den Tastaturcontroller
angeschlossen. Dadurch landen im Ausgabepuffer sowohl Codes von der
Tastatur als auch von der Maus. Damit die Quelle des Bytes unterschieden
werden, steht im Statusregister das Bit 5 (AUXB) zur Verfügung (1 = Maus, 0
= Tastatur).

<table>
<tr><th>Bit</th><th>Maske</th><th>Name</th><th>Bedeutung</th></tr>
<tr><td>0</td><td>0x01</td><td>outb</td><td>Gesetzt, wenn ein Zeichen im
Ausgabepuffer des Tastaturcontrollers zum Lesen bereit steht</td></tr>
<tr><td>1</td><td>0x02</td><td>inpb</td><td>Gesetzt, solange der
Tastaturcontroller ein von der CPU geschriebenes Zeichen noch nicht
abgeholt hat</td></tr>
<tr><td>5</td><td>0x20</td><td>auxb</td><td>Gesetzt, wenn der Wert im
Ausgabepuffer von der Maus und nicht von der Tastatur stammt</td></tr>
</table>

\section keyboard_processor Tastaturprozessor programmieren

Der Tastaturprozessor kann durch das Schicken von Befehlscodes an
den Eingabepuffer konfiguriert werden. Dazu wird der Befehlscode
(siehe Tabelle) an den Datenport geschrieben. Eigentlich sollte
man danach darauf warten, bis die Tastatur geantwortet
hat und der Ausgabepuffer das Bestätigungsbyte 0xfa (ACK)
enthält. Allerdings ist es schwierig eine wirklich
standardkonforme Implementierung zu erreichen.
Inbesondere ist das korrekte Warten auf ein ACK schwierig, da
nachfolgende Tastendrücke das ACK Zeichen aus dem Ausgabepuffer des
Tastaturcontrollers verdrängen können. Wir werden von den etwa 20
Befehlen, die eine PS/2-Tastatur versteht, nur zwei verwenden:

<table>
<tr><th>Befehlscode</th><th>Name</th><th>Beschreibung</th></tr>
<tr><td>0xed</td><td>set_led</td><td>Ein-/Ausschalten der Tastatur-LEDs.
Nach dem bestätigten Befehlscode muss ein weiteres Byte an den Datenport
geschrieben werden, das den Zustand der LEDs steuert. Der Aufbau ist in
einer eigenen Tabelle beschrieben.</td></tr>
<tr><td>0xf3</td><td>set_speed</td><td>Wiederholungsrate und Verzögerung
setzen durch ein zweites Byte, das nach dem Befehlsbyte folgt. Der Aufbau
ist in einer eigenen Tabelle beschrieben.</td></tr>
</table>

Die folgende Tabelle zeigt den Aufbau des Steuerbytes von set_led zum
setzen der Tastatur-LEDs. MSB bedeutet hierbei most significant bit
(entspricht also 0x80 in Hexadezimal-Darstellung) und LSB least significant
bit (also 0x01).

<table>
<tr><th>MSB</th>
<th></th>
<th></th>
<th></th>
<th></th>
<th></th>
<th></th>
<th>LSB</th></tr>
<tr><td>Always 0</td>
<td>Always 0</td>
<td>Always 0</td>
<td>Always 0</td>
<td>Always 0</td>
<td>Caps Lock</td>
<td>Num Lock</td>
<td>Scroll Lock</td>
</tr>
</table>

Der Aufbau des Konfigurationsbyte von set_speed ist in diesen zwei Tabellen
beschrieben. Die Wiederholungsrate wird durch die Bits 0-4 spezifiziert,
die Verzögerung durch Bit 5 und 6. Für die Wiederholrate gilt: je höher der
Wert desto geringer die Wiederholrate. Eine vollständige Tabelle dieser
Abbildung befindet sich im weiter unten verlinkten Dokument zur
PS/2-Schnittstelle. Die Zuordnung der Verzögerung ist wie folgt:

<table>
<tr><th>Bits 0-4 (hex)</th><th>Wiederholungsrate (Zeichen pro Sekunde) </th></tr>
<tr><td>0x00</td><td>30 </td></tr>
<tr><td>0x02</td><td>25 </td></tr>
<tr><td>0x04</td><td>20 </td></tr>
<tr><td>0x08</td><td>15 </td></tr>
<tr><td>0x0c</td><td>10 </td></tr>
<tr><td>0x10</td><td>7 </td></tr>
<tr><td>0x14</td><td>5 </td></tr>
</table>

<table>
<tr><th>Bits 5 und 6 (hex)</th><th>Verzögerung (in Sekunden)</th></tr>
<tr><td>0x00</td><td>0.25</td></tr>
<tr><td>0x01</td><td>0.5</td></tr>
<tr><td>0x02</td><td>0.75</td></tr>
<tr><td>0x03</td><td>1.0</td></tr>
</table>

Der Empfang des Konfigurationsbytes wird vom Tastaturcontroller ebenfalls
mit einem ACK-Byte quittiert.

## Weiterführende Informationen und Referenzen ##

- Messmer, Hans Peter: PC-Hardwarebuch - Aufbau, Funktionsweise,
Programmierung. Addison-Wesley 1994
- [The AT keyboard controller](http://homepages.cwi.nl/~aeb/linux/kbd/scancodes-8.html)
- [The PS/2 keyboard interface](http://web.archive.org/web/20080822115611/http://www.computer-engineering.org/ps2keyboard/)
