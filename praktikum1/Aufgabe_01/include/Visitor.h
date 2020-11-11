#pragma once

#include <string>
#include <vector>

class Book;
class Visitor
{
public:
    Visitor(int id, std::string name);

    int getId();
    std::string getName();
    const std::vector<Book*>& getLentBooks();
    bool addToLentBooks(Book* book);
    bool removeFromLentBooks(Book* book);

private:
    int id;
    std::string name;
    std::vector<Book*> lentBooks;
};
