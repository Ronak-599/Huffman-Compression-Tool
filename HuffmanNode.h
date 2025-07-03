

#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

struct HuffmanNode {
    char ch;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, int freq) : ch(c), frequency(freq), left(nullptr), right(nullptr) {}
};

#endif
