#include "Library.h"
#include "Book.h"
#include "Visitor.h"
#include <sstream>
// add shelf include for line:13, 16
#include "Shelf.h"

Library::Library(std::string name)
{
    this->name = name;
}

bool Library::addShelf(Shelf* shelf)
{
    if (this->shelfs.find(shelf->getId()) != this->shelfs.end()) {
        return false;
    }
    this->shelfs.emplace(shelf->getId(), shelf);
}

bool Library::addBook(Book* book)
{
    if (this->books.find(book->getIsbn()) != this->books.end()) {
        return false;
    }
    this->books.emplace(book->getIsbn(), book);
}

void Library::lentBook(Book* book, Visitor* visitor)
{
    if (book == nullptr) {
        return;
    }
    if (visitor == nullptr) {
        return;
    }
    visitor->addToLentBooks(book);
    book->setLent(true);
    this->lendings.emplace(book, visitor);

}

void Library::returnBook(Book* book, Visitor* visitor)
{
    if (book == nullptr) {
        return;
    }
    if (visitor == nullptr) {
        return;
    }
    // check if book is lent
    if (book->isLent() == false){
        return;
    }
    if (visitor->removeFromLentBooks(book)) {
        book->setLent(false);
        // changed "this->getLendings()" to lendings cause return value of getLendings is const and its impossible to erase
        lendings.erase(book);
    }
}

std::string Library::toString()
{
    std::stringstream strStream;
    strStream << this->name << std::endl;
    strStream << "Shelfs" << std::endl;
    for (std::pair<std::string, Shelf*> shelf : this->shelfs){
        strStream << shelf.second->getId() << std::endl;
        for(Book* book:shelf.second->getBooks()){
            strStream << "\t" << book->getName() << " | ";
            strStream << book->getIsbn() << " | ";
            strStream << book->getAuthor() << " | ";
            if (book->isLent()){
                strStream << this->lendings.at(book)->getName();
                strStream << " has lent" << std::endl;
            }else{
                strStream << "available" << std::endl;
            }
        }
    }
    return strStream.str();
}

const std::map<Book*, Visitor*>& Library::getLendings() const
{
    return lendings;
}
