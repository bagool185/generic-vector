#include <exception>
#include <string>

struct OutOfBoundsException : public std::exception {

    virtual const char* what() const throw() {
        return "Index out of bounds";
    }
};

struct EmptyVectorException : public std::exception {

    virtual const char* What() const throw() {
        return "Empty vector";
    }
};