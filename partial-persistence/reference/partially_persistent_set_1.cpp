#include <iostream>
#include <string>

#include "partially_persistent_set_1.hpp"

std::ostream& operator<<(std::ostream& os, const std::optional<std::string>& s)
{
    return os << (s ? *s : "<none>");
}

int main()
{
    partially_persistent_set_1<std::string> pps;

    std::cout << "Version: " << pps.currentVersion() << std::endl;
    std::cout << "Insert 'Hello'" << std::endl;
    pps.insert("Hello");
    std::cout << "Version: " << pps.currentVersion() << std::endl;

    std::cout << "Find 'Hello' at version 0: " << pps.find(0, "Hello") << std::endl;
    std::cout << "Find 'Hello' at version 1: " << pps.find(1, "Hello") << std::endl;
    std::cout << "Find 'World' at version 1: " << pps.find(1, "World") << std::endl;

    std::cout << "Insert 'World'" << std::endl;
    pps.insert("World");
    std::cout << "Version: " << pps.currentVersion() << std::endl;

    std::cout << "Find 'World' at version 1: " << pps.find(1, "World") << std::endl;
    std::cout << "Find 'World' at version 2: " << pps.find(2, "World") << std::endl;
    std::cout << "Find 'Hello' at version 2: " << pps.find(2, "Hello") << std::endl;

    pps.insert("Foo");
    pps.insert("Bar");
    pps.insert("Baz");
    std::cout << pps.currentVersion() << std::endl;
    std::cout << "Find 'Foo' at version 100: " << pps.find(100, "Foo") << std::endl;

    return 0;
}
