#pragma once

#include <string>

class Shelf;
class Book {
public:
    //added default value to lent so it accepts constructions with three arguments
    Book(std::string isbn, std::string name, std::string author, bool lent = false);

    std::string getIsbn();
    std::string getName();
    std::string getAuthor();
    bool isLent();
    void setLent(bool lent);

    void setShelf(Shelf* shelf);
    void removeShelf();

private:
    std::string isbn;
    std::string name;
    std::string author;
    bool lent;
    Shelf* shelf;
};
