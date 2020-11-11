#pragma once

#include <string>
#include <vector>

class Book;
class Shelf {
public:
    Shelf(std::string id);
    std::string getId();

    void addBook(Book* book);
    void removeBook(Book* book);

    const std::vector<Book *>& getBooks();

private:
    std::string id;
    std::vector<Book*> books;
};

