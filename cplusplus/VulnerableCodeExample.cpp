#include <iostream>
#include <cstring>

void copyInput(char* input) {
    char buffer[10];  // Small buffer size
    strcpy(buffer, input); // Unsafe copy operation
    std::cout << "Copied data: " << buffer << std::endl;
}

int main() {
    char userInput[50];
    std::cout << "Enter some input: ";
    std::cin >> userInput;
    copyInput(userInput);
    return 0;
}
