#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "HuffmanNode.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <iostream>


class HuffmanTree {
private:
    HuffmanNode* root;

    void buildTree(const std::string& data);
    void storeCodes(HuffmanNode* root, const std::string& str, std::unordered_map<char, std::string>& huffmanCode);
    void destroyTree(HuffmanNode* node);

public:
    std::unordered_map<char, std::string> huffmanCode;
    std::unordered_map<std::string, char> reverseHuffmanCode;

    HuffmanTree(const std::string& data);
    ~HuffmanTree();
};

HuffmanTree::HuffmanTree(const std::string& data) : root(nullptr) {
    buildTree(data);
}

HuffmanTree::~HuffmanTree() {
    destroyTree(root);
}

void HuffmanTree::buildTree(const std::string& data) {
    std::unordered_map<char, unsigned> freq;
    for (char ch : data) {
        freq[ch]++;
    }

    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, compare> minHeap;

    for (auto pair : freq) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() != 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        HuffmanNode* top = new HuffmanNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }

    root = minHeap.top();
    storeCodes(root, "", huffmanCode);

    for (auto pair : huffmanCode) {
        reverseHuffmanCode[pair.second] = pair.first;
    }
}

void HuffmanTree::storeCodes(HuffmanNode* root, const std::string& str, std::unordered_map<char, std::string>& huffmanCode) {
    if (!root) return;
    if (root->data != '$') {
        huffmanCode[root->data] = str;
    }
    storeCodes(root->left, str + "0", huffmanCode);
    storeCodes(root->right, str + "1", huffmanCode);
}

void HuffmanTree::destroyTree(HuffmanNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

#endif // HUFFMANTREE_H
