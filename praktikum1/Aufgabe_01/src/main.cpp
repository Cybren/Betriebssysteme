#include "Library.h"
#include "Shelf.h"
#include "Book.h"
#include "Visitor.h"

#include <iostream>

int main() {

    Library library("Universitaetsbibliothek Kassel");

    Shelf* rShelf = new Shelf("R");
    Shelf* sShelf = new Shelf("S");
    Shelf* tShelf = new Shelf("T");
    Shelf* aShelf = new Shelf("A");

    library.addShelf(rShelf);
    library.addShelf(sShelf);
    library.addShelf(tShelf);
    library.addShelf(aShelf);

    Book* book1 = new Book("978-3868942705", "Moderne Betriebssysteme", "Andrew Tannenbaum");
    Book* book2 = new Book("978-3827372932", "Verteilte Systeme: Prinzipien und Paradigmen", "Andrew Tannenbaum");
    Book* book3 = new Book("978-3827418241", "Theoretische Informatik - kurz gefasst", "Uwe Schöning");
    Book* book4 = new Book("978-3834803887", "Höhere Mathematik I. Lineare Algebra", "Walter Strampp");
    Book* book5 = new Book("978-3868940985", "Künstliche Intelligenz", "Stuart Russel");

    library.addBook(book1);
    library.addBook(book2);
    library.addBook(book3);
    library.addBook(book4);
    library.addBook(book5);

    rShelf->addBook(book5);
    sShelf->addBook(book3);
    sShelf->addBook(book4);
    tShelf->addBook(book1);
    tShelf->addBook(book2);

    Visitor* alice = new Visitor(1, "Alice");
    Visitor* bob = new Visitor(2, "Bob");
    Visitor* carol = new Visitor(3, "Carol");
    Visitor* dan = new Visitor(4, "Dan");

    library.lentBook(book1, alice);
    library.lentBook(book2, alice);
    library.lentBook(book5, carol);
    library.lentBook(book4, bob);
    library.lentBook(book4, dan);

    std::cout << library.toString() << std::endl;

    library.returnBook(book4, bob);
    library.returnBook(book3, bob);
    library.lentBook(book4, dan);

    std::cout << library.toString() << std::endl;
    
    delete bob;
    delete carol;
    delete dan;
    delete alice;

    return 0;
}