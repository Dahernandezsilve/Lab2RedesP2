#include <iostream>
#include <string>
#include <bitset>
#include <cstdint>

uint16_t fletcher8(const std::string &data) {
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    
    for (char charDigit : data) {
        sum1 = (sum1 + (charDigit - '0')) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    
    return (sum2 << 8) | sum1;
}

uint32_t fletcher16(const std::string &data) {
    uint32_t sum1 = 0;
    uint32_t sum2 = 0;
    
    for (char charDigit : data) {
        sum1 = (sum1 + (charDigit - '0')) % 65535;
        sum2 = (sum2 + sum1) % 65535;
    }
    
    return (sum2 << 16) | sum1;
}

uint64_t fletcher32(const std::string &data) {
    uint64_t sum1 = 0;
    uint64_t sum2 = 0;
    
    for (char charDigit : data) {
        sum1 = (sum1 + (charDigit - '0')) % 4294967295;
        sum2 = (sum2 + sum1) % 4294967295;
    }
    
    return (sum2 << 32) | sum1;
}

std::string extractOriginalMessage(const std::string &data, int checksumBits) {
    return data.substr(0, data.size() - checksumBits);
}

uint64_t extractChecksum(const std::string &data, int checksumBits) {
    std::string checksumBin = data.substr(data.size() - checksumBits);
    return std::bitset<64>(checksumBin).to_ullong();
}

bool verifyChecksum(const std::string &data, uint64_t receivedChecksum, int blockSize) {
    uint64_t calculatedChecksum;
    if (blockSize == 8) {
        calculatedChecksum = fletcher8(data);
    } else if (blockSize == 16) {
        calculatedChecksum = fletcher16(data);
    } else if (blockSize == 32) {
        calculatedChecksum = fletcher32(data);
    } else {
        throw std::invalid_argument("Unsupported block size");
    }
    
    return calculatedChecksum == receivedChecksum;
}

int main() {
    std::cout << "\n📡 --- Receptor ---" << std::endl;
    std::string receivedMessage;
    std::cout << "🔠 Ingrese el mensaje binario con el checksum: ";
    std::cin >> receivedMessage;
    int blockSize;
    std::cout << "📏 Ingrese el tamaño del bloque (8, 16, o 32): ";
    std::cin >> blockSize;
    
    int checksumBits = blockSize == 8 ? 16 : blockSize == 16 ? 32 : 64;
    std::string originalMessage = extractOriginalMessage(receivedMessage, checksumBits);
    uint64_t receivedChecksum = extractChecksum(receivedMessage, checksumBits);
    
    bool isValid = verifyChecksum(originalMessage, receivedChecksum, blockSize);
    
    if (isValid) {
        std::cout << "✅ No se detectaron errores." << std::endl;
        std::cout << "📩 Mensaje original: " << originalMessage << std::endl;
    } else {
        std::cout << "Se detectaron errores en el mensaje. Se descarta el mensaje ❌" << std::endl;
    }
    
    return 0;
}
