#include <string>
#include <iostream>
#include <vector>

#include "Vector.h"

class Project {

public:

    std::string some_variable;
    double some_other_variable;

    Project(const std::string& some_variable, double some_other_variable) : some_variable(some_variable), 
        some_other_variable(some_other_variable) {}   


    explicit Project() = default;
};

template <typename T>
void test_vector(Vector<T> test_vector) {

    std::cout << "Capacity: " << test_vector.capacity() << "\n";
    std::cout << "Size: " << test_vector.size() << "\n";

    for (const auto& project : test_vector) {
        std::cout << project.some_other_variable << " " << project.some_variable << "\n";
    }
}

int main() {
    
    Vector<Project> test_projects;

    test_projects.push_back(Project("aaaa", 3.634));
    test_projects.push_back(Project("bbbbb", 6.2345));
    test_projects.insert(1, Project("aaaaaaaaaaa", 69));
    test_projects.erase(2);

    std::cout << test_projects[1].some_variable << "\n";

    test_vector(test_projects);

    return 0;
}