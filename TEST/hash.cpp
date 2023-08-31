#include <iostream>
#include <iomanip>
#include <sstream>
#include <cryptopp/sha.h>

using CryptoPP::SHA256;


int main() {
    std::string data = "Hello, world!";
    std::vector<byte> digest(SHA256::DIGESTSIZE);
    
    SHA256 hash;
    hash.Update((const byte*)data.data(), data.size());
    hash.Final(digest.data());
    std::stringstream ss;
    for (auto b : digest) ss << std::hex << std::setw(2) << std::setfill('0') << (int)b;

    std::string hex_hash = ss.str();



    return 0;
}