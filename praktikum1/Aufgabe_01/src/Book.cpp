#include "Book.h"
#include "Shelf.h"

Book::Book(std::string isbn, std::string name, std::string author, bool lent) {
    this->isbn = isbn;
    this->name = name;
    this->author = author;
    this->lent = lent;
}

void Book::setShelf(Shelf* shelf) {
    if(shelf == nullptr) {
        return;
    }
    if(this->shelf == shelf) {
        return;
    }
    if(this->shelf != nullptr) {
        this->shelf->removeBook(this);
    }
    this->shelf = shelf;
    this->shelf->addBook(this);
}

void Book::removeShelf() {
    if(this->shelf != nullptr) {
        this->shelf->removeBook(this);
    }
    this->shelf = nullptr;
}

std::string Book::getIsbn() {
    return this->isbn;
}

std::string Book::getName() {
    return this->name;
}

std::string Book::getAuthor()  {
    return this->author;
}

bool Book::isLent()  {
    return this->lent;
}

void Book::setLent(bool lent) {
    this->lent = lent;
}
