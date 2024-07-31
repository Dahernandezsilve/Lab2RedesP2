#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

// Función para calcular el número de bits de paridad necesarios
int calculateParityBits(int m) {
    int r = 0;
    while (pow(2, r) < (m + r + 1)) {
        r++;
    }
    return r;
}

// Función para detectar y corregir errores en el código Hamming
pair<vector<int>, vector<int>> detectAndCorrectErrors(vector<int> hammingCode) {
    int n = hammingCode.size();
    int r = log2(n + 1);
    vector<int> errorPositions;

    int errorPos = 0;
    for (int i = 0; i < r; i++) {
        int parityBitPos = pow(2, i);
        int parity = 0;
        for (int j = 1; j <= n; j++) {
            if (j & parityBitPos) { 
                parity ^= hammingCode[j - 1];
            }
        }
        if (parity != 0) {
            errorPos += parityBitPos;
        }
    }

    if (errorPos != 0) {
        errorPositions.push_back(errorPos);
        hammingCode[errorPos - 1] ^= 1;  // Corrige el error 🛠️
    }

    return make_pair(hammingCode, errorPositions);
}

// Función para extraer el mensaje original (sin los bits de paridad)
vector<int> extractOriginalMessage(const vector<int>& hammingCode) {
    int n = hammingCode.size();
    int r = log2(n + 1);
    vector<int> originalMessage;

    int j = 0;
    for (int i = 1; i <= n; i++) {
        if (pow(2, j) != i) {
            originalMessage.push_back(hammingCode[i - 1]);
        } else {
            j++;
        }
    }

    return originalMessage;
}

string corregirHamming(const string& input) {
    cout << "Mensaje recibido. ✅" << endl;

    vector<int> hammingCode(input.size());
    for (size_t i = 0; i < input.size(); i++) {
        hammingCode[i] = input[i] - '0';
    }

    // Detectar y corregir errores 🕵️‍♂️
    auto [correctedCode, errorPositions] = detectAndCorrectErrors(hammingCode);

    vector<int> originalMessage;
    if (errorPositions.empty()) {
        cout << "No se detectaron errores. ✅" << endl;
        originalMessage = extractOriginalMessage(correctedCode);
    } else if (errorPositions.size() == 1) {
        cout << "Se detectaron y corrigieron errores. 🔄" << endl;
        cout << "Posición del bit a corregir: " << errorPositions[0] << endl;
        cout << "Mensaje codificado corregido: ";
        for (int bit : correctedCode) {
            cout << bit;
        }
        cout << endl;
        originalMessage = extractOriginalMessage(correctedCode);
    } else {
        cout << "Se detectaron errores múltiples. El mensaje se descarta. ❌" << endl;
        return ""; // Mensaje descartado
    }

    string asciiMessage;
    int byte = 0;
    int bitCount = 0;

    // Convertir originalMessage de bits a ASCII
    for (int bit : originalMessage) {
        byte = (byte << 1) | bit; // Desplazar byte a la izquierda y añadir el nuevo bit
        bitCount++;

        // Si hemos recogido 8 bits, convertir a carácter ASCII
        if (bitCount == 8) {
            asciiMessage += static_cast<char>(byte); // Convertir a char y añadir al mensaje
            byte = 0; // Reiniciar byte
            bitCount = 0; // Reiniciar contador de bits
        }
    }

    // En caso de que haya bits sobrantes que no formen un byte completo
    if (bitCount > 0) {
        // Si hay menos de 8 bits, no se puede formar un byte completo
        cout << "Bits sobrantes que no forman un byte completo: ";
        for (int i = 0; i < bitCount; i++) {
            cout << originalMessage[originalMessage.size() - bitCount + i];
        }
        cout << endl;
    }

    cout << "Mensaje ASCII corregido: " << asciiMessage << endl;

    return asciiMessage; // Retorna el mensaje corregido
}
