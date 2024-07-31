#include <iostream>
#include <string>
#include <bitset>
#include <cstdint>

using namespace std;

uint16_t fletcher8(const string &data) {
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    
    for (char charDigit : data) {
        sum1 = (sum1 + (charDigit - '0')) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    
    return (sum2 << 8) | sum1;
}

uint32_t fletcher16(const string &data) {
    uint32_t sum1 = 0;
    uint32_t sum2 = 0;
    
    for (char charDigit : data) {
        sum1 = (sum1 + (charDigit - '0')) % 65535;
        sum2 = (sum2 + sum1) % 65535;
    }
    
    return (sum2 << 16) | sum1;
}

uint64_t fletcher32(const string &data) {
    uint64_t sum1 = 0;
    uint64_t sum2 = 0;
    
    for (char charDigit : data) {
        sum1 = (sum1 + (charDigit - '0')) % 4294967295;
        sum2 = (sum2 + sum1) % 4294967295;
    }
    
    return (sum2 << 32) | sum1;
}

string extractOriginalMessage(const string &data, int checksumBits) {
    return data.substr(0, data.size() - checksumBits);
}

uint64_t extractChecksum(const string &data, int checksumBits) {
    string checksumBin = data.substr(data.size() - checksumBits);
    return bitset<64>(checksumBin).to_ullong();
}

bool verifyChecksum(const string &data, uint64_t receivedChecksum, int blockSize) {
    uint64_t calculatedChecksum;
    if (blockSize == 8) {
        calculatedChecksum = fletcher8(data);
    } else if (blockSize == 16) {
        calculatedChecksum = fletcher16(data);
    } else if (blockSize == 32) {
        calculatedChecksum = fletcher32(data);
    } else {
        throw invalid_argument("Unsupported block size");
    }
    
    return calculatedChecksum == receivedChecksum;
}

string convertirABinario(const string& mensajeBits) {
    string asciiMessage;
    int byte = 0;
    int bitCount = 0;

    for (char bit : mensajeBits) {
        byte = (byte << 1) | (bit - '0');
        bitCount++;

        if (bitCount == 8) {
            asciiMessage += static_cast<char>(byte);
            byte = 0;
            bitCount = 0;
        }
    }

    return asciiMessage;
}

int verificarFletcherChecksum(const string& receivedMessage, int blockSize) {
    cout << "\n📡 --- Receptor ---" << endl;

    int checksumBits = (blockSize == 8) ? 16 : (blockSize == 16) ? 32 : 64;
    string originalMessage = extractOriginalMessage(receivedMessage, checksumBits);
    uint64_t receivedChecksum = extractChecksum(receivedMessage, checksumBits);
    
    bool isValid = verifyChecksum(originalMessage, receivedChecksum, blockSize);
    
    if (isValid) {
        cout << "✅ No se detectaron errores." << endl;
        
        string asciiMessage = convertirABinario(originalMessage);
        cout << "📩 Mensaje original (ASCII): " << asciiMessage << endl;
        
        return 1;
    } else {
        cout << "Se detectaron errores en el mensaje. Se descarta el mensaje ❌" << endl;
        return 0;
    }
}
