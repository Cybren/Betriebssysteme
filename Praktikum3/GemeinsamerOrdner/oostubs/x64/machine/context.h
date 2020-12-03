/*! \file
 *  \brief Enthält die Funktionen zum Kontextwechsel.
 */

#pragma once

/*! \brief Die Struktur toc dient dazu, bei einem Koroutinenwechsel die Werte
 *  der nicht-flüchtigen Register zu sichern.
 *
 *  \note Für den Zugriff auf die Elemente der Struktur ::stack aus einer
 *  Assemblerfunktion heraus werden in der `context.asm` Namen für die
 *  benötigten Abstände der einzelnen Elemente zum Anfang der Struktur
 *  definiert. Damit dann auch auf die richtigen Elemente zugegriffen wird,
 *  müssen sich die Angaben von *context.h* und *context.asm* exakt entsprechen.
 *  Wer also *context.h* ändert, muss auch *context.asm* anpassen (und umgekehrt).
 */
struct stack {
	void *kernel;
	void *user;
} __attribute__((packed));

class Thread;

/*! \brief Diese Funktion bereitet einen Kontext für die erste Aktivierung vor.
 *
 *  Dazu muss der Stack so initialisiert werden, dass bei
 *  der ersten Aktivierung die Ausführung mit der Funktion context_kickoff beginnt,
 *  die wiederum den Zeiger object als ersten Parameter auf dem Stack
 *  vorfinden muss.
 *
 *  context_prepare kann in C++ in der Datei context.cc implementiert werden und
 *  deshalb muss sie nicht als extern "C" deklariert werden.
 *
 *  \param target Stack (für Kontext), der auf eine Aktivierung vorbereitet werden soll
 *  \param tos Zeiger auf die oberste Adresse des Stackbereichs.
 *  \param that Zeiger auf das zu startende Threadobjekt (Parameter für kickoff)
 */
void context_prepare(struct stack * target, void* tos, Thread * that);

extern "C" {
	/*! \brief Diese Funktion führt einen Kontextwechsel durch. Dazu müssen
	 *  die aktuellen Registerwerte auf dem Stack und der Stackpointer selbst
	 *  in `current` gesichert werden und durch die Werte von `next` ersetzt
	 *  werden.
	 *
	 *  Die Funktion wird in Assembler in der Datei context.asm implementiert.
	 *  Sie muss als extern "C" deklariert werden, da sie als
	 *  Assemblerfunktion nicht dem C++ Namemangeling unterliegt.
	 *  \param current Zeiger auf ein struct stack, in den der
	 *  aktuelle Stackpointer gesichert werden soll.
	 *  \param next Zeiger auf ein struct struct, der den Stackpointer mit dem
	 *  zu ladenden Kontext enthält.
	 */
	void context_switch(struct stack * current, struct stack * next);

	/*! \brief Funktion zum Starten eines Threads.
	 *
	 *  Für die allererste Aktivierung eines Threads muss eine "Rücksprungadresse"
	 *  zu einer Funktion bekannt sein, von der aus dann der Übergang von der C- zur
	 *  C++-Ebene erfolgen kann. Hierfür dient die Funktion kickoff.
	 *
	 *  <b>Aktivierung von kickoff</b>
	 *  Bei der Initialisierung eines Threads mit Hilfe der Funktion toc_settle()
	 *  wird nicht nur die Struktur toc für den ersten Threadwechsel
	 *  vorbereitet, sondern auch die Adresse der Funktion kickoff als
	 *  Rücksprungadresse auf den Stack gelegt. Beim ersten Threadwechsel
	 *  mit toc_switch wird daher an den Anfang von kickoff "zurück"gesprungen,
	 *  womit die Ausführung beginnt.
	 *
	 *  Diese Methode realisiert den Sprung von der C-Ebene zur C++-Ebene, indem sie
	 *  ganz einfach die Methode action() des als Parameter vorgefundenen
	 *  Threadobjektes aufruft.
	 *
	 *  \b Hinweis
	 *  Da diese Funktion nicht wirklich aufgerufen, sondern nur durch eine
	 *  geschickte Initialisierung des Stacks der Koroutine angesprungen wird, darf
	 *  sie nie terminieren. Anderenfalls würde ein sinnloser Wert als
	 *  Rücksprungadresse interpretiert werden und der Rechner abstürzen.
	 *
	 *  \param object Thread, der gestartet werden soll.
	 */
 	void context_kickoff(Thread* object);

}
