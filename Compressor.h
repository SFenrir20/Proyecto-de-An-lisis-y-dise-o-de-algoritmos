#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "HuffmanTree.h"
#include <sstream>
#include <bitset>

class Compressor {
public:
    static std::string compress(const std::string& data);
    static std::string decompress(const std::string& compressedData);
};

std::string Compressor::compress(const std::string& data) {
    HuffmanTree huffmanTree(data);
    std::string compressedData;
    for (char ch : data) {
        compressedData += huffmanTree.huffmanCode[ch];
    }

    // Convert bit string to byte string
    std::string byteStr;
    for (size_t i = 0; i < compressedData.size(); i += 8) {
        std::string byte = compressedData.substr(i, 8);
        while (byte.size() < 8) {
            byte += "0";
        }
        std::bitset<8> bits(byte);
        byteStr += char(bits.to_ulong());
    }

    // Serialize the Huffman tree and the compressed data
    std::ostringstream os;
    for (const auto& pair : huffmanTree.huffmanCode) {
        os << pair.first << pair.second << '\n';
    }
    os << "-----" << '\n' << byteStr;

    return os.str();
}

std::string Compressor::decompress(const std::string& compressedData) {
    std::istringstream is(compressedData);
    std::string line;
    std::unordered_map<std::string, char> reverseHuffmanCode;
    while (std::getline(is, line) && line != "-----") {
        char ch = line[0];
        std::string code = line.substr(1);
        reverseHuffmanCode[code] = ch;
    }

    std::string byteStr;
    std::getline(is, byteStr);

    std::string bitStr;
    for (char ch : byteStr) {
        std::bitset<8> bits(ch);
        bitStr += bits.to_string();
    }

    std::string decompressedData;
    std::string currentCode;
    for (char bit : bitStr) {
        currentCode += bit;
        if (reverseHuffmanCode.find(currentCode) != reverseHuffmanCode.end()) {
            decompressedData += reverseHuffmanCode[currentCode];
            currentCode = "";
        }
    }

    return decompressedData;
}

#endif // COMPRESSOR_H
