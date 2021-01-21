\page apictimer Informationen zum APIC-Timer

Die APIC-Architektur beinhaltet auf jedem der CPU-lokalen APICs einen
hochauflösenden Timer, der zum Generieren von Interrupts genutzt werden
kann.

Da seine Frequenz nicht in der Spezifikation festgelegt ist, muss sie zunächst durch
Kalibration mit einer anderen Zeitquelle deren Frequenz bekannt ist, beispielsweise
dem Programmable Interrupt Timer (PIT), bestimmt werden.
In der Vorgabe zu OOStuBS ist dieser Code bereits enthalten, er nutzt dazu
jedoch eine generische Methode zur Timerkonfiguration, die noch zu
implementieren ist.

## Programmierung des Timers ##

Vier 32bit-Register (memory-mapped) sind für die Programmierung des APIC-Timers verantwortlich:

<table>
 <tr><th>Addresse</th><th>Registername</th></tr>
 <tr><td>0xfee00320</td><td>LVT Timer Register (aka Timer Control Register)</td></tr>
 <tr><td>0xfee00380</td><td>Initial Count Register (ICR)</td></tr>
 <tr><td>0xfee00390</td><td>Current Count Register (CCR)</td></tr>
 <tr><td>0xfee003e0</td><td>Divide Configuration Register</td></tr>
</table>

Die beiden Count-Register (ICR und CCR) dienen dazu, den Startwert des Timers zu setzen
beziehungsweise den aktuellen Wert auszulesen. Zusätzlich kann mit einem
ICR-Wert von 0 der Timer vor einer Umprogrammierung komplett gestoppt werden.
Ein anschließendes Schreiben des ICR mit einem Wert ungleich 0 startet den Timer.

Das Verhalten des Timers kann über das Kontrollregister und den Divisor beeinflusst werden.
Der Aufbau des Kontrollregisters ist:

 <table>
 <tr><th>Bit(s)</th><th>Wert</th><th>Bedeutung</th></tr>
 <tr><td style="color: lightgray">31-19</td><td style="color: lightgray" colspan="2" align="CENTER">(reserviert)</td></tr>
 <tr><td rowspan="5"><b>18-17</b></td><td colspan="2" align="CENTER"><b>Betriebsmodus</b></td></tr>
 <tr><td>0 </td><td>one-shot (stoppt bei 0)</td></tr>
 <tr><td>1 </td><td>periodic (startet wieder mit ICR-Wert)</td></tr>
 <tr><td>2 </td><td>TSC-Deadline (absolute Zeit über ein spezielles Register IA32_TSC_DEADLINE vergleichen, ICR und CCR ignorieren)</td></tr>
 <tr><td>3 </td><td>reserviert</td></tr>
 <tr><td rowspan="3"><b>16</b></td><td colspan="2" align="CENTER"><b>Interruptmaske</b></td></tr>
 <tr><td>0 </td><td>Interrupts werden zugestellt</td></tr>
 <tr><td>1 </td><td>Interrupts sind deaktiviert</td></tr>
 <tr><td style="color: lightgray">15-13</td><td style="color: lightgray" colspan="2" align="CENTER">(reserviert)</td></tr>
 <tr><td rowspan="3">12</td><td colspan="2" align="CENTER">Interrupt-Status (read-only)</td></tr>
 <tr><td>0 </td><td>idle</td></tr>
 <tr><td>1 </td><td>Interrupt pending</td></tr>
 <tr><td style="color: lightgray">11-8</td><td style="color: lightgray" colspan="2" align="CENTER">(reserviert)</td></tr>
 <tr><td rowspan="1"><b>7-0</b></td><td colspan="2" align="CENTER"><b>Vektornummer</b><br> des auszulösenden Interrupts</td></tr>
 </table>

Die Zuordnung des Divisors auf entsprechende Registerwerte ist dem
Intel-Manual zu entnehmen, aber auch bereits durch die Hilfsfunktion
LAPIC::timer_div() als Teil der Vorgabe implementiert.
