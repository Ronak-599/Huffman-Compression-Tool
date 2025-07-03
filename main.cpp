#include "HuffmanCoding.h"
#include <iostream>

using namespace std;

int main() {
    HuffmanCoding huffman;
    string inputFile, outputFile;
    int choice;

    cout << "Huffman Compression Tool" << endl;
    cout << "1. Compress File" << endl;
    cout << "2. Decompress File" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    cout << "Enter input file path: ";
    getline(cin, inputFile);

    cout << "Enter output file path: ";
    getline(cin, outputFile);

    if (choice == 1) {
        huffman.compress(inputFile, outputFile);
        cout << "File compressed successfully." << endl;
    } else if (choice == 2) {
        huffman.decompress(inputFile, outputFile);
        cout << "File decompressed successfully." << endl;
    } else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
