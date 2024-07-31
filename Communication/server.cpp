#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <fstream>
#include <vector>
#include "../Fletcher_Checksum/Receptor.cpp"  
#include "../Hamming/Receptor.cpp"           

using namespace std;

const int PORT = 12345;

// Estructura para almacenar los resultados
struct Result {
    string mensaje;
    string algoritmo;
    string estado;
};

// Función para verificar la integridad del mensaje
void verificarIntegridad(const string& mensaje, vector<Result>& resultados) {
    size_t pos1 = mensaje.find(';');
    size_t pos2 = mensaje.find(';', pos1 + 1);

    string mensajeRecibido = mensaje.substr(0, pos1);
    string algoritmoSeleccionado = mensaje.substr(pos1 + 1, pos2 - (pos1 + 1));
    
    int blockSize = -1;
    if (pos2 != string::npos) {
        string blockSizeStr = mensaje.substr(pos2 + 1);
        blockSize = stoi(blockSizeStr);
    }

    cout << "📥 Mensaje recibido: " << mensajeRecibido << endl;
    cout << "🔍 Algoritmo seleccionado: " << algoritmoSeleccionado << endl;

    Result resultado = {mensajeRecibido, algoritmoSeleccionado, ""};

    if (algoritmoSeleccionado == "Fletcher") {
        if (blockSize == -1) {
            resultado.estado = "🚨 Error: Se requiere un blockSize para el algoritmo Fletcher.";
            resultados.push_back(resultado);
            return;
        }

        // Concatenar el nombre del algoritmo con el blockSize
        string algoritmoConBloque = "Fletcher" + to_string(blockSize);
        if (verificarFletcherChecksum(mensajeRecibido, blockSize) == 1) {
            resultado.estado = "✅ Mensaje recibido sin errores";
        } else {
            resultado.estado = "🚨 Error detectado en el mensaje (Fletcher).";
        }
        resultado.algoritmo = algoritmoConBloque;  // Almacena el algoritmo con el bloque
    } else if (algoritmoSeleccionado == "Hamming") {
        // Llamar a corregirHamming y manejar el resultado
        auto [mensajeCorregido, estado] = corregirHamming(mensajeRecibido);
        
        // Almacena el algoritmo con el bloque
        string algoritmoConBloque = "Hamming"; // El algoritmo Hamming no tiene bloque
        
        resultado.algoritmo = algoritmoConBloque;  // Almacena el algoritmo sin bloque
        resultado.estado = estado; // Asignar el estado retornado por corregirHamming
        
        // Si se corrigió el mensaje, se incluye en el resultado
        if (estado == "corregido") {
            resultado.mensaje = mensajeCorregido; // Almacena el mensaje corregido
        } else if (estado == "sin errores") {
            resultado.mensaje = mensajeRecibido; // Almacena el mensaje original
        } // Si el estado es "descartado", el mensaje se queda vacío.
    } else {
        resultado.estado = "🚨 Algoritmo no soportado.";
    }

    resultados.push_back(resultado);
}

void guardarResultados(const vector<Result>& resultados) {
    ofstream file("Resultados.csv", ios::app); // Abre en modo append
    for (const auto& result : resultados) {
        file << result.mensaje << "," << result.algoritmo << "," << result.estado << "\n";
    }
    file.close();
}

void manejarCliente(int cliente_socket) {
    char buffer[1024] = {0};
    vector<Result> resultados;  // Almacena los resultados de la sesión actual

    read(cliente_socket, buffer, sizeof(buffer));
    string mensaje(buffer);
    
    verificarIntegridad(mensaje, resultados);
    guardarResultados(resultados);  // Guarda los resultados

    memset(buffer, 0, sizeof(buffer));
    close(cliente_socket);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Error en la creación del socket" << endl;
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        cerr << "Error en setsockopt" << endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        cerr << "Error en el bind" << endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        cerr << "Error en listen" << endl;
        return -1;
    }

    cout << "🕵️‍♂️ Esperando conexiones en el puerto " << PORT << "..." << endl;

    while (true) {
        int cliente_socket;
        if ((cliente_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            cerr << "Error en accept" << endl;
            return -1;
        }

        thread hilo(manejarCliente, cliente_socket);
        hilo.detach();
    }

    close(server_fd);
    
    return 0;
}
