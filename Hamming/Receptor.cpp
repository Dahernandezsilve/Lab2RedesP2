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

int main() {
    string input;
    cout << "Ingrese el mensaje binario con los bits de paridad generados por el emisor: 📥 ";
    cin >> input;

    vector<int> hammingCode(input.size());
    for (size_t i = 0; i < input.size(); i++) {
        hammingCode[i] = input[i] - '0';
    }

    // Detectar y corregir errores 🕵️‍♂️
    auto [correctedCode, errorPositions] = detectAndCorrectErrors(hammingCode);

    if (errorPositions.empty()) {
        cout << "No se detectaron errores. ✅" << endl;
        vector<int> originalMessage = extractOriginalMessage(correctedCode);
        cout << "Mensaje original: ";
        for (int bit : originalMessage) {
            cout << bit;
        }
        cout << endl;
    } else if (errorPositions.size() == 1) {
        cout << "Se detectaron y corrigieron errores. 🔄" << endl;
        cout << "Posición del bit a corregir: " << errorPositions[0] << endl;
        cout << "Mensaje codificado corregido: ";
        for (int bit : correctedCode) {
            cout << bit;
        }
        cout << endl;
        vector<int> originalMessage = extractOriginalMessage(correctedCode);
        cout << "Mensaje corregido: ";
        for (int bit : originalMessage) {
            cout << bit;
        }
        cout << endl;
    } else {
        cout << "Se detectaron errores múltiples. El mensaje se descarta. ❌" << endl;
    }

    return 0;
}