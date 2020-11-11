//Ben Gerhards ben-gerhards@t-online.de 35438258
//Till Nebel tillnebel@googlemail.com 35375771

#include <algorithm>
#include "Visitor.h"

Visitor::Visitor(int id, std::string name) {
    this->id = id;
    this->name = name;
}

int Visitor::getId() {
    return id;
}

std::string Visitor::getName() {
    return name;
}

const std::vector<Book *>& Visitor::getLentBooks() {
    return lentBooks;
}

bool Visitor::addToLentBooks(Book *book) {
    if(book == nullptr) {
        return false;
    }
    if(std::find(this->lentBooks.begin(), this->lentBooks.end(), book) != this->lentBooks.end())
    {
        return false;
    }
    this->lentBooks.push_back(book);
    return true;
}

bool Visitor::removeFromLentBooks(Book *book) {
    if(book == nullptr) {
        return false;
    }
    //check if visitor has lent the book
    if(std::find(this->getLentBooks().begin(), this->getLentBooks().end(), book) == this->getLentBooks().end()){
        return false;
    }
    this->lentBooks.erase(std::find(this->lentBooks.begin(), this->lentBooks.end(), book));
    return true;
}