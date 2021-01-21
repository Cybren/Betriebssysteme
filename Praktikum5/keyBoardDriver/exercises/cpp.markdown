\page CPP-Intro C++ Einführung für Betriebssytembau

[TOC]

Dies ist bei weitem keine vollständige Einführung in C++, sondern
ausschließlich eine Behandlung einzelner Themenkomplexe, die für dieses Fach
von Bedeutung sind. Die Gebiete werden nicht nur in ihrer Verwendung
erläutert, sondern auch in ihrer technischen Auswirkung. Die einzelnen
Mechanismen sollen also entmystifiziert werden. Dies wird anhand von
C++ Codebeispielen, Terminaloutputs und Assemblerlistings geschehen.

Eine gute Quelle für ein umfassenderes C++ Tutorial ist
[cplusplus.com](http://www.cplusplus.com/doc/tutorial/).

# Objekt vs. Pointer vs. Referenz

Jede Variablen<b>definition</b> reserviert ein Stück Speicher und gibt
diesem Speicher einen Namen und einen Datentypen. Über den Namen kann
der Speicher im Programm angesprochen werden, der Datentyp gibt an wie
dieser zu interpretieren ist. Dazu wollen wir uns anschauen, was der
Compiler (`g++ -m32 -fno-PIE test.c -o test`) aus folgendem Stück Code
macht:

\code{.cpp}
int     ganzzahl;
double  fliesskomma;

int main(void) {
   ganzzahl    += 1;
   fliesskomma += 1;
   return 0;
}
\endcode

Zunächst können wir uns mit dem Tool `nm` ansehen, welche globalen
Objekte mit welchen Addressen und Größen angelegt wurden (`...` zeigt
eine Auslassung an):

```console
$ nm --numeric-sort -S test
...
0000052d 0000001b T main
...
00002020 00000008 B fliesskomma
00002028 00000004 B ganzzahl
...
```

Aus dieser Ausgabe sehen wir, dass die beiden Variablen im BSS Segment
(`B`) angelegt wurden, und 4 bzw. 8 Byte groß sind. Das BSS Segment
bedeutet, dass der Speicher der Variablen beim Programmstart mit 0
initialisiert wird. Dies erfolgt, weil beides Variablen sind und bei
der Definition keinen Wert zugewiesen bekommen haben. Wir sehen
also, dass der Name `ganzzahl` einen Speicherbereich bezeichnet, der an
0x2028 liegt und 4 Byte groß ist. Allerdings haben wir im fertig
übersetzten Programm keinerlei Informationen mehr darüber, von welchem
Datentyp dieser Speicher ist. Diese Information wurde nur vom Compiler
benutzt, um den passenden Binärcode zu erzeugen. Dies ist eine der
Stellen an der zu Tage tritt, dass C und C++ keine typsichere Sprache
ist, da man den Speicher, der als `fliesskomma` bekannt ist, auch völlig
anders verwenden könnte. Schauen wir uns nun den erzeugten Assembler an:

```
$ objdump -d test
...
000052d <main>:
 52d:   83 05 28 20 00 00 01    addl   $0x1,0x2028 # Addiere Konstante zur Speicherstelle
 534:   d9 e8                   fld1               # Lege eine 1 auf float-stack
 536:   dc 05 20 20 00 00       faddl  0x2020      # Addiere Speicherstelle auf float-stack
 53c:   dd 1d 20 20 00 00       fstpl  0x2020      # Speichere top-of(float-stack) in Speicherzelle
 542:   b8 00 00 00 00          mov    $0x0,%eax   # Kopiere Konstante zum Register
 547:   c3                      ret
```

Wie wir sehen, steht an der Adresse 0x52d, die wir schon aus der `nm`
Ausgabe kennen, der Code der Funktion `main`. Hier sind unsere drei
Zeilen relativ gut zu erkennen. Zunächst inkrementieren wir `ganzzahl`
ohne den Umweg über ein Register um 1. Dann passiert dasselbe Spiel mit der
Fließkommazahl unter Zuhilfename der x86 Fließkommaeinheit, die einen
eigenen Stack von Fließkommazahlen verwaltet. Und schließlich schieben
wir den Wert 0 in das Register `%eax`, wo der Aufrufer von main der
Rückgabewert erwartet, und kehren mittels `ret` zurück.

Meistens reicht es jedoch nicht, Objekte immer bei ihrem Namen zu
nennen, da dies doch sehr unflexibel ist. In unserem Beispiel sind die
Adressen unserer beiden Variablen beispielsweise direkt in den
Maschinencode eingewoben. Viel schöner wäre es doch, wenn wir eine
Indirektion einbauen würden und einer Funktion einen Namen übergeben,
anstatt das Objekt selbst. Dies ist mittels einem Zeiger/Pointer möglich:

\code{.cpp}
void inkrement(int * zahl) {
    *zahl = *zahl + 1;
}
\endcode

Hier übergeben wir der Funktion einen Zeiger auf einen
Speicherbereich, der einen Integer beinhaltet. Immer wenn wir auf den
Speicher zugreifen wollen, müssen wir den Zeiger dereferenzieren
(`*zahl`), um den eigentlichen Wert zu erhalten. Schauen wir uns nun den Assembler an:

```
00000548 <_Z9inkrementPi>:
 548:   8b 44 24 04             mov    0x4(%esp),%eax  # Lade das erste Argument nach %eax
 54c:   83 00 01                addl   $0x1,(%eax)     # Addiere 1 auf die Speicherzelle auf %eax zeigt
 54f:   c3                      ret
```

Zunächst fällt auf, dass unsere Funktion einen sehr komischen Namen
hat. Dies liegt am C++ Name Mangling, bei dem die Parametertypen (in diesem Fall also `void` und `int *`) in
den **Namen** der Funktion einkodiert werden. Im ersten Befehl laden wir
den Pointer `zahl` vom Stack, auf dem die Argumente übergeben werden,
in das Register `%%eax`. Die nächste Zeile verwendet die
Addressierungsart "Register-Indirekt" (`(%%eax)`) um auf die
dahinterliegende Speicherstelle zuzugreifen und diese zu
inkrementieren. Bei diesem Beispiel hat der Aufrufer der Funktion
Speicherplatz auf dem Stack angelegt, der innerhalb der Funktion unter
dem Namen `zahl` bekannt ist und als `int *` interpretiert wird.
Würden wir also unsere Funktion mit `inkrement(&ganzzahl)` aufrufen,
so hätte das Register `%%eax` in unserem Beispiel den numerischen Wert 0x2020.

Allerdings haben Pointer keine Garantien darüber, ob sie gerade auf
ein valides/vorhandenes Stück Speicher zeigen oder ob ihr Wert
völliger Bogus ist. Ein häufig verwendete Art einen Fehler anzuzeigen
ist es beispielsweise den Nullpointer zurück zu geben. Dieser hat auch
tatsächlich den nummerischen Wert 0 (daher der Name).

<div class="alert alert-danger">In den meisten Umgebungen führt das
dereferenzieren des Nullpointers zu einer Ausnahme. In unserem Fall ist
dies allerdings nicht so, da wir das Betriebssytem sind und die Maschiene
zunächst nicht dazu konfiguriert haben, den Wert 0 als einen invaliden
Zeiger zu behandeln.</div>

C++ bietet noch eine weitere Möglichkeit, Objekte zu adressieren:
Referenzen. Eine Referenz ist zweiter Name bzw. ein Alias für ein
Objekt. Dies bedeutet auch, dass sich hinter einer Referenz **immer**
ein valides Objekt befindet.

Compiler implementieren das Sprachkonzept „Referenz“ meist auf der Basis
von Pointern, ihnen steht allerdings frei, in manchen Situationen andere
Möglichkeiten zu verwenden.

\code{.cpp}
void inkrement_ref(int &zahl) {
    zahl = zahl + 1;
}
\endcode

Wie wir sehen, müssen wir die Referenz, die durch das `&` angezeigt
wird, nicht dereferenzieren, da sie ein Alias des referenzierten
Objekts ist. Der Name `zahl` tut so, als wäre es direkt das Objekt, auf
das die Referenz zeigt. Wenn wir uns den dazu gehörigen Assembler
betrachen, sehen wir, dass der Compiler in diesem Fall die Abbildung
auf Pointer gewählt hat und exakt der gleiche Assembler entsteht.

```
00000550 <_Z13inkrement_refRi>:
 550:   8b 44 24 04             mov    0x4(%esp),%eax
 554:   83 00 01                addl   $0x1,(%eax)
 557:   c3                      ret
```

- [Wikipedia: NM](https://en.wikipedia.org/wiki/Nm_(Unix))
- [Wikipedia: Datensegement#BSS](https://de.wikipedia.org/wiki/Datensegment#BSS)
- [Wikipedia: Name mangling](https://en.wikipedia.org/wiki/Name_mangling)
- [C++ Tutorial: Pointers](http://www.cplusplus.com/doc/tutorial/pointers/)

# `struct`, `class` und Methoden

Nun ist C++ dafür bekannt, dass es (auch) objektorientierte
Programmierung erlaubt. Der erste Schritt in diese Richtung
ist bereits in C unternommen worden, nämlich die Möglichkeit
mehrere Variablen unterschiedlichen Typs in einem **Verbunddatentyp**
zu organisieren. Mit der folgenden Strukturdefinition kann man Objekte
vom Typen `foobar` erzeugen und herumreichen (zum Beispiel per Pointer
oder per Referenz).

\code{.cpp}
struct foobar {
   int    foo;
   double bar;
};
\endcode

Das neue in C++ ist nun, dass man Methoden innerhalb einer Struktur
definieren kann, die auf den Daten der Struktur arbeiten. Dabei werden
die in der Funktion verwendeten Namen zunächst in der umgebenden
Struktur gesucht, bevor der Compiler annimmt, dass es sich um eine globale
Variable handelt.

\code{.cpp}
struct foobar {
   int    foo;
   double bar;

   int calc(int x);
};

int foobar::calc(int x) {
   return (foo * 23) + x;
}
\endcode

In unserem Beispiel sehen wir, wie zunächst eine Methode `calc`
deklariert wird und danach außerhalb der C++-Klasse definiert (mit
Leben gefüllt) wird. Es funktioniert auch, dass man eine Methode
direkt in einer Klasse/Struktur definiert, allerdings ist die Trennung
von Interface und Implementierung in vielen Fällen schöner. Die
Methode `calc()`  greift auf das Feld `foo` zu. Dies bedeutet, dass die
Methode, die wir hier sehen, irgendwie einen impliziten Parameter haben
muss, der auf ein konkretes Objekt zeigt, in dessen Kontext `calc()`
ausgeführt wird. Dieser implizite Parameter ist unter dem Namen `this`
verfügbar und ist ein Pointer auf das Objekt (`foobar *`). Dies
bedeutet auch, dass `this->foo` eine explizite Form ist um auf `foo`
zuzugreifen. Schauen wir uns nun den erzeugten Assembler an:

```
0000055a <_ZN6foobar4calcEi>:
 55a:   8b 44 24 04             mov    0x4(%esp),%eax     # Impliziter Paramter: this
 55e:   6b 00 17                imul   $0x17,(%eax),%eax  # Multiplikation mit 23
 561:   03 44 24 08             add    0x8(%esp),%eax     # Expliziter Paramter: x
 565:   c3                      ret
```

Im Assembler sehen wir hier, dass die gesamte Abstraktion die durch
Methoden eingeführt worden sind, in C++ (in diesem Fall) vollständig
zusammenfällt. Der `this`-Pointer wird einfach als erstes Argument dem
Assembler übergeben und alle anderen Parameter (`x == 0x8(%%esp)`)
rutschen um eins nach hinten. In diesem Fall wäre dies äquivalent zu
einer Funktionssignatur `int calc(foobar * this, int x)`. Was wir
außerdem sehen können, ist, dass der Zugriff auf `foo` eine direkte
Derefenzierung des `this` Pointers ist: `(%%eax)`.

Allerdings ist die Kopplung von Code und Daten nicht das einzige
Merkmal objektorientierter Programmierung. Sondern meistens ist damit
auch die Möglichkeit zur `Vererbung` gemeint. Das bedeutet, dass eine
Klasse von einer anderen Klasse erben kann und diese um Daten und
Funktionalität erweitert.

\code{.cpp}
struct foobar_premium : public foobar {
    bool is_premium() {
        foo -= 1;
        return true;
    };
};
\endcode

Hier sehen wir eine Klasse `foobar_premium` die von `foobar` „public“
erbt. Dadurch können ihre Methoden auf die geerbten Felder zugreifen
und neue Funktionalität anbieten. Dies ist sehr ähnlich zu den
Mechanismen in Java, mit einigen wichtigen Unterschieden:

- Methoden sind per default nicht `virtual` (was äquivalent zu java `final` ist). Es wird daher nicht automatisch „weitervererbt“.
- Mehrfachvererbung ist möglich.
- Objekte in C++ können sowohl auf dem Stack als auch auf dem Heap angelegt werden.
- Die Sichtbarkeit der Vererbung kann einstellt werden (Man kann mit `private` alles erben, aber anderer Code sieht davon nichts).

Nun stellt sich die Frage, wieso hier immer von Klassen geredet wird,
wo doch im Code ständig `struct` steht. Der interessierte Studierende weiß
doch, dass es auch `class` gibt. Der Punkt ist, dass die beiden Schlüsselwörter
beinahe äquivalent sind. Der einzige Unterschied ist, dass in
`struct`s alle Felder per default `public` sind und in einer `class`
`private`.

- [C++ Tutorial](http://www.cplusplus.com/doc/tutorial/classes/)
- [Stackoverflow: When should you use a class vs a struct in C++?](https://stackoverflow.com/questions/54585/when-should-you-use-a-class-vs-a-struct-in-c)

# Speicherverwaltung in Betriebssytembau

In C/C++ gibt es prinzipiell drei Bereiche in denen Daten abgelegt werden können:

1. Globale Variablen. Diese werden im Daten- bzw. dem BSS Segement
   angelegt und werden vor dem Aufruf der `main()` Funktion
   initialisiert. Gibt es keine initiale Zuweisung, so wird der
   Speicher einfach mit Nullen gefüllt.
2. Lokale Variablen auf dem Stack. Diese sind nur innerhalb einer
   Funktion gültig und werden auf dem Laufzeitstack angelegt. Dies
   erklärt auch, wieso sie die angelegten Objekte ihre Gültigkeit nach
   der Rückkehr der Funktion verlieren, da der Funktionsrahmen wieder
   abgebaut wird.
3. Objekte im Heap. In einer Umgebung mit einer vollständigen libc hat
   man mittels `malloc()` zunächst ungetypten
   Speicher (`void *`) vom Heap zu bekommen. Dieser Teil der libc baut
   dabei heutzutage intern auf den `brk()` & `mmap()` Systemaufrufen auf.

Da wir beim Betriebssystembau keine libc haben, müssen wir zunächst auf
den Komfort dynamischer Speicherverwaltung verzichten. Daher sind nur
die Varianten 1 und 2 für uns relevant (in BST schreiben wir uns unsere
eigene Speicherverwaltung).

Besonders bei den lokalen Variablen gibt es allerdings häufig gemachte
Fehler. So wird zum einen gerne vergessen, dass der Stack endlich
(4096 Byte) ist und große Objekte zu einem Stackoverflow führen.

\code{.cpp}
struct huge_object {
  char buffer[5000];
};
void foo() {
   huge_object barfoo;
}
\endcode

Dieser Stackoverflow führt allerdings nicht dazu, dass das
Betriebsystem abstürzt, sondern zu einer Speicherkorruption, welche
meistens recht schwierig zu finden ist. Man schreibt einfach über den
Stack hinaus in den Bereich der globalen Variablen.

Der andere gern gemachte Fehler ist, die Lebensdauer von lokalen
Objekten nicht zu beachten. So wird zum Beispiel gerne die Addresse
einer lokalen Variable zurück gegeben, was ebenfalls zu einer
Speicherkorruption führt. Meistens findet der Compiler diese
fehlerhafte Verwendung lokaler Variablen, allerdings nicht in allen
Fällen. Daher ist Vorsicht geboten!

\code{.cpp}
object_t * foo() {
    object_t ret;
    ret.foo = 80;
    return &ret;
}
\endcode

- [Stackoverflow: Static in C](https://stackoverflow.com/questions/572547/what-does-static-mean-in-c)

# Bitoperationen und Bitfelder

Beim Betriebssystembau hat man häufig direkt mit der Hardware zu tun.
Die Designer dieser Hardware versuchen ihrerseits die
Schaltkreise möglichst effizient zu gestalten. Dies führt dazu, dass
häufig mehrere Bits an Informationen, die nicht direkt
zusammengehören, in einem Speicherwort zusammengepfercht sind. Ein
Beispiel, das in der Aufgabe 1 auftaucht, ist der Speicher
der CGA Graphikkarte. Dort wird jedes Zeichen auf dem Bildschirm
(80x25 Zeichen) von 2 Bytes dargestellt. Das erste der beiden Bytes
ist dabei das dargestellte Zeichen (in ASCII), das zweite Byte ist die
Konfiguration für Vordergrund- und Hintergrundfarbe.

<table class="table table-bordered" style="text-align: center;">
<tr><th class="col"><b>Bit</b></th><th class="col" width="10%">7</th><th class="col" width="10%">6</th><th class="col" width="10%">5</th><th class="col" width="10%">4</th><th class="col" width="10%">3</th><th class="col" width="10%">2</th><th class="col" width="10%">1</th><th class="col" width="10%">0</th></tr>
<tr>
<td><b>Bedeutung</b></td><td>Blinken</td><td colspan="3">Hintergrundfarbe</td><td colspan="4">Vordergrundfarbe</td></tr></table>

Nun kann man solche Attributbytes mittels der Bitoperationen, die C und
C++ bereitstellt, zusammenbauen. Dabei gibt es das binäre UND (`&`),
das binäre ODER (`|`), das binäre NICHT (`~`), und das binäre XOR
(`^`) und die beiden Shift Operationen (`<<`, `>>`). Eine einfache
Option für eine Wrapperfunktion wäre also:

\code{.cpp}
char make_attribute(char foreground, char background, char Blink) {
     foreground &= 0xf; 
     background &= 0x7; 
     Blink      &=   1; 

     background <<=  4; 
     Blink      <<=  7; 

     return foreground | background | Blink; 
}
\endcode

In dieser Funktion werden zuerst die Variablen mit einer Maske
verundet, um alle Bits, die außerhalb des Wertebereich des jeweiligen
Attributs liegen, zu beschneiden. Danach werden die Bits innerhalb von
`background` und `Blink` mittels eines links-shifts noch an die passende
Stelle verschoben und in der Returnanweisung mittels eines binären
ODERs kombiniert.

Zu diesen Operationen gibt es noch diverse Idiome, die man verwendet
um einzelne Bits oder eine Menge von Bits zu löschen, zu setzen oder
auf ihren Wert zu prüfen. Diese funktionieren meist über ein Bitmaske,
in der alle Bits, für die wir uns interessieren, gesetzt sind:

\code{.cpp}
int MASKE = 0x11011;
int value;
 
value &= ~MASKE;
 
value |= MASKE;
 
value |= (1 << 13);
 
if ((value & MASKE) != 0) {...}
 
if ((value1 & MASKE) == (value2 & MASKE)) {...}
\endcode

Diese Art die einzelnen Bits zu manipulieren, ist für kleinere
Anwendungen gut genug. Allerdings neigen Programme, die allzu heftig
davon Gebrauch machen, dazu völlig unlesbar und voll von magischen
Konstanten zu sein. In C++ (und C) gibt es daher noch die Möglichkeit
Bitfelder zu verwenden. Diese weisen den Compiler an, einzelne Bits
eines Speicherbereiches unter einem Namen bekannt zu machen.

\code{.cpp}
struct CGA_Attr {
    char foreground : 4;
    char background : 3;
    char blink      : 1;
} __attribute__((packed));
\endcode

Hierbei ist der oberste Eintrag bei den gängisten Compilern niederwertig
(= näher bei Bit 0).
Das zusätzliche Attribut `packed` weißt den Compiler an, wirklich keine
Padding Bits mehr zwischen den Feldern zu lassen. Verwendet man nun
diese Struktur, wird unsere Funktion vom Eingang deutlich einfacher,
wir können sie sogar in den Konstruktor packen.

\code{.cpp}
struct CGA_Attr {
    char foreground : 4;
    char background : 3;
    char blink      : 1;

    CGA_Attr(char fg, char bg, char B)
      : foreground(fg), background(bg), blink(B) {}
} __attribute__((packed));
\endcode

Im Beispiel wird mittels der Initialisierungsliste den Feldern direkt ein
Wert zugewiesen. Dies erlaubt es dem Compiler, besseren Code für diese
Zuweisungen zu erzeugen.

- [Wikipedia: Magische Zahl](https://de.wikipedia.org/wiki/Magische_Zahl_(Informatik))
- [Wikipedia: Initialisierungsliste](https://de.wikipedia.org/wiki/Initialisierungsliste)

# Was bedeutet `volatile`?

Vor Jahren habe ich in einem Mikrokontroller Forum auf die Frage, wieso
ein Program, dass Interrupts verwendet, nicht funktioniert, die
Gegenfrage gelesen: "Hast du schon überall `volatile` hingeschrieben?`.

Das ist natürlich nicht die richtige Anwort. Denn `volatile` ist nicht
das magische Schlüsselwort, mit dem der Compiler Wettlaufsituationen
verhindert. Seine Bedeutung versteht man, wenn man verinnerlicht hat,
dass der Compiler nicht für jeden Zugriff auf eine Variable aus dem
Speicher ließt. Da der Speicher langsam und die Register deutlich
schneller sind, ist es von großem Vorteil, eine Variable aus dem
Speicher zu lesen, alle nötigen Modifikationen durchzuführen und dann erst zurückzuschreiben. Beispiel:

\code{.asm}
mov foo, %eax
add $1,  %eax
shl $3,  %eax
orl $17, %eax
mov %eax, foo
\endcode

In dem Beispiel lebt Variable `foo` zunächst im Speicher und für die
mittleren drei Instruktionen im Register, bevor sie wieder zurückgeschrieben
wird. Man muss quasi gedanklich den Geist einer Variable
(wo ist der Wert aktuell) von ihrer aktuellen Speicherstelle trennen.

Das Keyword `volatile` verbietet nun dem Compiler diese Optimierung.
Für jedes Auftreten einer `volatile` Variable muss der Compiler den
Wert aus dem Speicher lesen und das Ergebnis zurück schreiben.

- [How to use the C Volatile Keyword](https://barrgroup.com/Embedded-Systems/How-To/C-Volatile-Keyword)
- [Wikipedia: Race Condition](https://en.wikipedia.org/wiki/Race_condition#Example)

# Operatorüberladung

Eins der meist gehassten und am häufigst verteidigten Konzepte, neben
den C++ Templates, ist die Möglichkeit, Operatoren zu überladen. Dies
kann dazu führen, dass ein unschuldig aussehender Code `a + b`
beliebig kaputte Dinge im Hintergrund machen kann. Und weil andere
damit unvernünftige Dinge machen, ist es auch wichtig, dass ihr wisst,
wie man unvernünftige Dinge tut.

Prinzipiell ist Operatorüberladung sehr einfach. Man definiert einfach
eine Funktion die den magischen Namen `operatorOP` hat für den
Operator OP, denn man gerne für seine hübschen Datentypen haben
möchte.

\code{.cpp}
bool operator==(const &foo a, const &foo b) {
   return a.id == b.id;
}
\endcode

Damit definieren wir einen Operator der die Gleichheit zweier selbst
definierter Datentypen regelt. Da es noch andere (eingebaute)
Operatoren gibt, entscheiden die normalen C++ Überladungsregeln,
welche Operator zur Anwendung kommt. Zusätzlich zu frei lebenden
Operatoren kann man auch Operatoren mit einem Argument weniger als
Methoden definieren:

\code{.cpp}
class foo {
   int id;
 public:
   bool operator==(const &foo other) {
      return this->id == other.id;
   }
};
\endcode

In diesem Fall bekommt der Operator die linke Seite durch das
implizite `this` Argument. Da der Operator als Methode definiert wurde,
kann er auf die privaten Felder von `foo` zugreifen, was nicht möglich
mit einem frei schwebenden Operator wäre.

Eine besonders eigenwillige Verwendung einer Operatorüberladung wurde
für die C++ Standardbiblothek für die Ein- und Ausgabe verwendet. Hier
wird der `<<` Operator überladen, um Zahlen und Strings gleichermaßen
in einem Ausgabekanal zu schicken.

\code{.cpp}
std::cout << 12 << "Hallo Welt" << &main << std::endl;
\endcode

Hierbei ist gibt jedes `operator<<` eine Referenz auf den Ausgabekanal zurück:

\code{.cpp}
Stream & operator<<(Stream& out, int x)         {...; return out;}
Stream & operator<<(Stream& out, std::string x) {...; return out;}
Stream & operator<<(Stream& out, void *x)       {...; return out;}
\endcode

- [CPP Referenz: Operatorenüberladung](http://en.cppreference.com/w/cpp/language/operators)

# Virtual auf technischer Ebene

Eine Besonderheit von Java ist es, dass jede Methode einer Klasse per
default zunächst `virtual` ist. Dies bedeutet, dass der Aufruf der
Methode **dynamisch dispatched** wird. Die andere Art den Dispatch
eines Methodenaufrufes durchzuführen, ist der **statische Dispatch**.
Wir wollen uns zunächst (in C++) anschauen, wo der Unterschied im
Verhalten von virtuellem und statischem Dispatch liegt.

\code{.cpp}
Object *obj = AbstractObjectFactory.make_object();
obj->method();
\endcode

In diesem kurzen Codeabschnitt erzeugen wir ein Objekt, dass von einem
`Object *` gehalten werden kann. Dies bedeutet, dass es entweder
direkt ein `Object` ist oder ein Objekt einer abgeleiteten Klasse, die
von `Object` erbt. Ob nun das Objekt, auf das `obj` zeigt, ein `Object`
oder ein `DerivedObject` ist, bestimmt seinen **dynamischen Typ**. Die
Variable, welche die Referenz auf das Objekt hält, bestimmt ihren
**statischen Typen**. Das heißt, dass in unserem Beispiel `*obj` sicher
den statischen Typen `Object` hat, aber der dynamische Typ aus dem
Code nicht direkt ablesbar ist.

Der Unterschied zwischen dynamischen und statischen Dispatch ist nun,
ob der dynamische oder der statische Typ herangezogen wird, um die
Methode auszuwählen. Falls unsere `->method()` statisch dispatched
wird, kann der Compiler direkt eine `call` Instruktion generieren.

\code{.asm}
mov    %eax,(%esp)
call   8048926 <_ZN6Object6methodEv>
\endcode

Zuerst wird der Pointer auf das Objekt auf den Stack gelegt und dann
der Funktionskörper direkt mit einem Aufruf angesprungen.
Wiederrum sehen wir, dass der Funktionsname gemangled ist.

Soll nun ein dynamischer Dispatch an die Stelle des statischen Dispatches
treten, muss unser Programm irgendwie herausfinden können, von welchem
dynamischen Typen das referenzierte Objekt ist. Denn auf diesen kommt
es beim dynamischen Dispatch ja an. Dazu muss das Wissen über den
dynamischen Typen des Objekts überhaupt erstmal irgendwo gespeichert
werden. Denn normalerweise, wenn es keine einzige virtuelle Methode
gibt, ist die Abbildung von C++ Klassen und Strukturen auf den
Speicher direkt. Dazu ein Beispiel, wie unser `Object` aussehen
könnte:

\code{.cpp}
class Object {
    int a;
    int b;
    int c;
public:
    void method();
};
\endcode

In diesem Fall wird `method()` statisch dispatched und es besteht
keinerlei Notwendigkeit den dynamischen Typen solcher Objekte zu
kennen, wieso also dafür wertvolle Bytes ausgeben. Daher spart sich
C++ in diesem Fall auch die Information um den dynamischen Typen und
belegt für unser Objekt genau die 12 Bytes (3 * 4 Bytes für jeden
Integer), die verwendet werden. Dabei werden die Felder der Klasse von
oben nach unten aufsteigend in den Speicher gelegt. In diesem Fall ist daher:

\code{.cpp}
Object * obj = ...;
char *memory = (char*) obj;
obj->a == *(int*)(memory + 0);
obj->b == *(int*)(memory + 4);
obj->c == *(int*)(memory + 8);
\endcode

Ebenfalls könnte man, wenn man glaubt zu verstehen, was der Compiler
tut, ein Objekt direkt aus einem Pointer auf ein Stück Speicher
casten. Im Beispiel wird auch sichtbar, dass unser Beispiel auf einer
Litte-Endian Maschine kompiliert wurde.

\code{.cpp}
char memory[] = {12, 0, 0, 0, 
                 23, 0, 0, 0, 
                 45, 0, 0, 0, 
};
Object *obj = (Object *)memory;
\endcode

Um Informationen über den dynamischen Typen hinzuzufügen, müssen nun im
Speicher des Objekts zusätzliche Informationen untergebracht werden.
Die meisten Compiler fügen dazu vor dem ersten Element der Klasse
einen **Virtual Function Table Pointer** hinzu. Bei allen Objekten des
gleichen dynamischen Typs zeigt dieser Pointer auf die gleiche
virtuelle Funktionstabelle.

\code{.cpp}
class Object {
    int a;
    int b;
    int c;
public:
    virtual void method();
};

int main() {
    Object *obj = new Object();
    
    obj->method();
}
\endcode

Dieses Beispiel nun in einer GDB Session am angegeben Breakpoint.

```
Breakpoint 1, main () at test.c:22
22	    obj->method();

# Das Objekt an sich Inspizieren:
(gdb) p obj
$1 = (Object *) 0x80f1ce0
(gdb) p *obj
$2 = {_vptr.Object = 0x80b7f2c <vtable for Object+8>, a = 0, b = 0, c = 0}
(gdb) x /4x obj
0x80f1ce0:	0x080b7f2c	0x00000000	0x00000000	0x00000000

# Die dazu gehörige VTable:
(gdb) info vtbl obj
vtable for 'Object' @ 0x80b7f2c (subobject @ 0x80f1ce0):
[0]: 0x8048926 <Object::method()>
(gdb) x/a 0x80b7f2c
0x80b7f2c <_ZTV6Object+8>:	0x8048926 <Object::method()>
```

Wir sehen also, dass das eingefügte Feld auf ein Objekt mit dem Namen
`<vtable for Object+8>` zeigt, welches vom Compiler erzeugt wurde. In
dieser VTable ist ein Pointer auf den Funktionskörper von `Object::method()` gespeichert. Da der VTable-Pointer direkt vom dynamischen Typen abhängt, kann über diese Funktionstabelle der dynamische Dispatch durchgeführt werden. Für unseren Methodenaufruf sieht der dazu passende Assembler so aus (Object * in `%%eax`):

\code{.asm}
mov    (%eax),%edx   # VTable-Pointer in %edx
push   %eax          # this Pointer auf den Stack
call   *(%edx)       # Dereferenziere den VTable-Pointer ohne Offset (Slot 0)
                     # und mache einen indirekten Aufruf
\endcode

Jede virtuelle Methode, die wir definieren, bekommt also einen Slot in
der virtuellen Funktionstabelle. Um noch genauer zu betrachten, was
der Compiler in die VTable baut, können wir den Compiler mit `g++
-fdump-class-hierarchy` aufrufen und die Ausgabe betrachten:

```
Vtable for Object
Object::_ZTV6Object: 3 entries
0     (int (*)(...))0
4     (int (*)(...))(& _ZTI6Object)
8     (int (*)(...))Object::method

Class Object
   size=16 align=4
   base size=16 base align=4
Object (0x0x7f6b48574060) 0
    vptr=((& Object::_ZTV6Object) + 8)
```

Wir sehen aus der Ausgabe, dass die VTable eigentlich 8 Byte weiter
vorne anfängt als der vptr, der am Ende in unseren Objekten
gespeichert wird. Zusätzlich zu unseren Methodenpointern wird noch ein
Pointer auf die Run-Time-Type-Information (RTTI) gespeichert.

All dieses Wissen zusammengenommen können wir also auch mal ein
Object und die dazu passende VTable zusammenfaken:

\code{.cpp}
void my_method(Object *obj) {
    printf("X: %d %d %d\n", obj->a, obj->b, obj->c);
}

int main() {
    int vtable[] = {
        0, 0, 
        (int) &my_method
    };
    int memory[] = {
        (int) vtable + 8,
        1, 2, 3
    };
    Object *obj = (Object *)memory;
}
\endcode

Als Ausgabe erhalten wir für dieses Programm (für `-m32`, 32 Bit): `X: 1 2 3`.

Was wir aus dieser technischen Betrachtung von virtuellen Funktionstabellen lernen, ist:

- Der statische Dispatch ist umsonst.
- Der dynamische Dispatch ist ein indirekter Call über einen VTable Pointer.
- C++ zahlt diese Kosten nur, wenn die Funktionalität explizit gefordert wurde.

Links:

- [Wikipedia: Endianess](https://en.wikipedia.org/wiki/Endianness)
- [C++ vtables - Part 1](https://shaharmike.com/cpp/vtable-part1/)

# Vererbung auf technischer Ebene

Wir haben bereits gelernt, wie in C++ Verbundatentypen (struct, union,
class) im Speicher abgelegt werden. Bei einfache Datenstrukturen (POD)
werden die Felder (von oben nach unten) hintereinander in den Speicher
gelegt (von den niedrigen Adressen zu den hohen Adressen. Und wir
haben gelernt, wie Klassen die einen `virtual` Methode enthalten um
einen VTable Pointer erweitert werden.

Wie sieht es aber nun mit Vererbung aus? Dazu ein Beispiel, welches
wir mit `clang -Xclang -fdump-record-layouts` übersetzen:

\code{.cpp}
class Base {
    int a;
    int b;
};

class Derived : public Base {
    int cc;
};

int main() {
    Base x;
    Derived yy;
}
\endcode

Aus der Ausgabe bekommen wir heraus, dass die Base Klasse ganz regulär
in den Speicher abelegt wird. Hierbei ist die Linke Spalte der Offset
vom Pointer zum Objekt:

```
0 | class Base
0 |   int a
4 |   int b
  | [sizeof=8, dsize=8, align=4,
  |  nvsize=8, nvalign=4]
```

Die beiden Integer sind also einfach hintereinander in den Speicher
gelegt. Und wenn wir uns die Klasse Derived anschauen, dann sehen wir,
dass dieses Speicherabbild in den Anfang des Derived Objektes
eingebaut wird. Und dadurch der Pointer zur Variable `cc` einen Offset
von 8 Byte aufweist.

```
0 | class Derived
0 |   class Base (base)
0 |     int a
4 |     int b
8 |   int cc
  | [sizeof=12, dsize=12, align=4,
  |  nvsize=12, nvalign=4]
```

Der Trick bei dieser Art der Einbettung ist nun, dass man den Pointer
der auf ein Derived Objekt zeigt ohne Probleme wie ein Objekt vom
Typen Base verwenden kann. Denn alle Felder sind ja an der gleichen
Stelle. Die selben Regel gelten, wenn wir eine `virtual` Methode zur
Base hinzufügen. In dem Fall, kann jedes Derived Objekt das vtable
Pointer Feld der *primären* Basisklasse verwenden um auf den eigenen
vtable zu zeigen:

```
 0 | class Derived
 0 |   class Base (primary base)
 0 |     (Base vtable pointer)
 8 |     int a
12 |     int b
16 |   int cc
   | [sizeof=24, dsize=20, align=8,
   |  nvsize=20, nvalign=8]

```

Solange man nur Einfachvererbung hat ist dies im Grunde alles was es
über Memory Layouts von Klassen wissen muss. In dem Moment wo man
Mehrfachvererbung hat, wird das ganze eine Ecke komplizierter. Dazu
ein Beispiel **ohne virtual**:

\code{.cpp}
class Base {
    int a;
    int b;
};

class Base2 {
    int c;
    int d;
};

class Derived : public Base, public Base2 {
    int xx;
};
\endcode

Das Speicherlayout der abgeleiteten Klasse sieht dann folgendermassen aus:

```
 0 | class Derived
 0 |   class Base (base)
 0 |     int a
 4 |     int b
 8 |   class Base2 (base)
 8 |     int c
12 |     int d
16 |   int xx
   | [sizeof=20, dsize=20, align=4, nvsize=20, nvalign=4]
```

Auffällig ist, dass wiederum die Speicherlayouts der geerbten Klassen
1:1 in der Derived Klasse zu finden sind. Allerdings besteht nun das
Problem, dass wir den Pointer der auf den Anfang zeigt, nicht mehr
einfach verwenden können um eine Funktion zu füttern die ein `Base2`
Speicherlayout erwartet. In dem Fall muss also eine Anpassung des this
Pointers erfolgen. Um dies zu illustrieren, schauen wir uns den
Assembler der Funktion `Base2 * foo(Derived *x) {return x;}` an:

\code{.asm}
00000000 <_Z3fooP7Derived>:
   mov    0x4(%esp),%ecx   # Lade das erste Argument nach %ecx
   lea    0x8(%ecx),%eax   # %eax = %ecx + 8 (Offset zu Base2)
   test   %ecx,%ecx        # Teste ob %ecx == 0
   cmove  %ecx,%eax        # Falls ja, setze %eax auf 0
   ret                     # Return %eax
\endcode

Hier sehen wir, dass für die Konvertierung der beiden Pointer ein
Offset von 8 aufaddiert wird. Dies führt dazu, dass aus einen Derived
Pointer ein Base2 Pointer wird. Es bedeutet aber auch, dass der
Pointer nun mitten in das Objekt zeigt. Die Sequenz `test; cmove`
verhindert, dass aus einem Nullpointer ein Pointer mit dem Wert 8
wird.

Diese automatische Offsetanpassung ist auch der Grund, wieso man in
C++ mehrere Cast Operationen hat, die diese Offsetanpassung vornehmen
(oder halt nicht). Wen man nun noch `virtual` ins Spiel bringt, wird
die Geschichte noch etwas komplizierter. Zu beidem verweisen wir aber auf
die Links zu diesem Kapitel.

- [Wikipedia: Plain Old Data structure](https://de.wikipedia.org/wiki/Plain_Old_Data_structure)
- [C++ vtables - Part 2](https://shaharmike.com/cpp/vtable-part2/)
- [C++ Tutorial: Type conversions](http://www.cplusplus.com/doc/tutorial/typecasting/)
