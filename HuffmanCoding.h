#ifndef HUFFMANCODING_H
#define HUFFMANCODING_H

#include "HuffmanNode.h"
#include <string>
#include <unordered_map>

using namespace std;

class HuffmanCoding {
public:
    HuffmanCoding();
    ~HuffmanCoding();

    void compress(const string& inputFile, const string& outputFile);
    void decompress(const string& inputFile, const string& outputFile);

private:
    HuffmanNode* root;
    unordered_map<char, string> huffmanCodes;
    unordered_map<char, int> frequencyMap;

    void buildFrequencyMap(const string& filename);
    void buildHuffmanTree();
    void generateCodes(HuffmanNode* node, const string& code);
    void freeTree(HuffmanNode* node);

    void serializeTree(HuffmanNode* node, ofstream& outFile);
    HuffmanNode* deserializeTree(ifstream& inFile);
};

#endif
