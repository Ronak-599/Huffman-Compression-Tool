#include "HuffmanCoding.h"
#include <fstream>
#include <queue>
#include <bitset>

using namespace std;

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

HuffmanCoding::HuffmanCoding() {
    root = nullptr;
}

HuffmanCoding::~HuffmanCoding() {
    freeTree(root);
}

void HuffmanCoding::freeTree(HuffmanNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

void HuffmanCoding::buildFrequencyMap(const string& filename) {
    ifstream inFile(filename, ios::binary);
    char ch;
    while (inFile.get(ch)) {
        frequencyMap[ch]++;
    }
    inFile.close();
}

void HuffmanCoding::buildHuffmanTree() {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

    for (auto& entry : frequencyMap) {
        minHeap.push(new HuffmanNode(entry.first, entry.second));
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();

        HuffmanNode* merged = new HuffmanNode('\0', left->frequency + right->frequency);
        merged->left = left;
        merged->right = right;

        minHeap.push(merged);
    }

    root = minHeap.top();
}

void HuffmanCoding::generateCodes(HuffmanNode* node, const string& code) {
    if (!node) return;

    if (!node->left && !node->right) {
        huffmanCodes[node->ch] = code;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

void HuffmanCoding::serializeTree(HuffmanNode* node, ofstream& outFile) {
    if (!node) return;

    if (!node->left && !node->right) {
        outFile.put('1');
        outFile.put(node->ch);
    } else {
        outFile.put('0');
        serializeTree(node->left, outFile);
        serializeTree(node->right, outFile);
    }
}

HuffmanNode* HuffmanCoding::deserializeTree(ifstream& inFile) {
    char bit;
    inFile.get(bit);

    if (bit == '1') {
        char ch;
        inFile.get(ch);
        return new HuffmanNode(ch, 0);
    }

    HuffmanNode* left = deserializeTree(inFile);
    HuffmanNode* right = deserializeTree(inFile);
    HuffmanNode* node = new HuffmanNode('\0', 0);
    node->left = left;
    node->right = right;
    return node;
}

void HuffmanCoding::compress(const string& inputFile, const string& outputFile) {
    buildFrequencyMap(inputFile);
    buildHuffmanTree();
    generateCodes(root, "");

    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    serializeTree(root, outFile);

    string encodedStr = "";
    char ch;
    while (inFile.get(ch)) {
        encodedStr += huffmanCodes[ch];
    }

    // Store size of last valid bits to avoid trailing junk
    int extraBits = 8 - (encodedStr.size() % 8);
    if (extraBits != 8) {
        encodedStr.append(extraBits, '0');
    } else {
        extraBits = 0;
    }

    outFile.put(static_cast<char>(extraBits)); // write padding info

    for (size_t i = 0; i < encodedStr.size(); i += 8) {
        bitset<8> byte(encodedStr.substr(i, 8));
        outFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    inFile.close();
    outFile.close();
}

void HuffmanCoding::decompress(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    root = deserializeTree(inFile);

    char paddingByte;
    inFile.get(paddingByte);
    int extraBits = static_cast<int>(paddingByte);

    string encodedStr = "";
    char byte;
    while (inFile.get(byte)) {
        bitset<8> bits(static_cast<unsigned char>(byte));
        encodedStr += bits.to_string();
    }

    if (extraBits > 0)
        encodedStr = encodedStr.substr(0, encodedStr.size() - extraBits);

    HuffmanNode* curr = root;
    for (char bit : encodedStr) {
        if (bit == '0') curr = curr->left;
        else curr = curr->right;

        if (!curr->left && !curr->right) {
            outFile.put(curr->ch);
            curr = root;
        }
    }

    inFile.close();
    outFile.close();
}
