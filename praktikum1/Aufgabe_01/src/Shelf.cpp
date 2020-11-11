#include <algorithm>
#include "Shelf.h"
#include "Book.h"

Shelf::Shelf(std::string id) {
    this->id = id;
}

std::string Shelf::getId() {
    return id;
}

void Shelf::addBook(Book* book) {
    if (book  == nullptr)
        return;
    if(std::find(books.begin(), this->books.end(), book) != this->books.end()) {
        return;
    }
    this->books.push_back(book);
    book->setShelf(this);
}

void Shelf::removeBook(Book* book) {
    if(book == nullptr)
        return;
    if (std::find(books.begin(), this->books.end(), book) == this->books.end()) {
        return;
    }
    books.erase(std::find(books.begin(), this->books.end(), book));
    book->removeShelf();
}

const std::vector<Book *>& Shelf::getBooks() {
    return books;
}
