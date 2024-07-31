#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "../Fletcher_Checksum/Receptor.cpp"  
#include "../Hamming/Receptor.cpp"           

const int PORT = 12345;

void verificarIntegridad(const std::string& mensaje, const std::string& algoritmo) {
    if (algoritmo == "Fletcher") {
        // Implementar la verificación de integridad usando Fletcher
        if (verificarFletcherChecksum(mensaje)==1) { // Asumiendo que esta función está implementada
            std::cout << "✅ Mensaje recibido sin errores: " << mensaje << std::endl;
        } else {
            std::cout << "🚨 Error detectado en el mensaje (Fletcher)." << std::endl;
        }
    } else if (algoritmo == "Hamming") {
        // Implementar la verificación de integridad usando Hamming
        std::string mensajeCorregido = corregirHamming(mensaje); // Asumiendo que esta función está implementada
        if (!mensajeCorregido.empty()) {
            std::cout << "✅ Mensaje recibido y corregido: " << mensajeCorregido << std::endl;
        } else {
            std::cout << "🚨 Error no corregible detectado en el mensaje (Hamming)." << std::endl;
        }
    } else {
        std::cout << "🚨 Algoritmo no soportado." << std::endl;
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Error en la creación del socket" << std::endl;
        return -1;
    }

    // Configurar opciones del socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Error en setsockopt" << std::endl;
        return -1;
    }

    // Configurar dirección del servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Enlazar el socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error en el bind" << std::endl;
        return -1;
    }

    // Escuchar conexiones
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error en listen" << std::endl;
        return -1;
    }

    std::cout << "🕵️‍♂️ Esperando conexiones en el puerto " << PORT << "..." << std::endl;

    // Aceptar una conexión
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Error en accept" << std::endl;
        return -1;
    }

    // Leer datos del socket
    read(new_socket, buffer, sizeof(buffer));
    std::string mensaje(buffer);
    
    // Suponiendo que el mensaje tiene la forma "mensaje;algoritmo"
    std::string mensajeRecibido = mensaje.substr(0, mensaje.find(';'));
    std::string algoritmo = mensaje.substr(mensaje.find(';') + 1);

    std::cout << "📥 Mensaje recibido: " << mensajeRecibido << std::endl;
    std::cout << "🔍 Algoritmo seleccionado: " << algoritmo << std::endl;

    // Verificar integridad del mensaje
    verificarIntegridad(mensajeRecibido, algoritmo);

    // Cerrar sockets
    close(new_socket);
    close(server_fd);
    
    return 0;
}
