# Lab2RedesP2
Laboratorio 2 - Segunda parte Esquemas de detección y corrección de errores 🚨

📚 **Descripción**

Esta es la continuación del laboratorio 2 en la que se implementa un sistema de comunicación cliente-servidor que permite enviar mensajes con verificación de integridad utilizando los algoritmos de checksum de Fletcher y Hamming programados anteriormente. El objetivo es asegurar la integridad de los mensajes transmitidos a través de una red, detectando y corrigiendo errores (ruido) en los datos.

🔧 **Ejecución**

### 🚨 Requisitos 

- Python 3.x
- Un compilador de C++
- Biblioteca estándar de sockets (disponible en la mayoría de los sistemas operativos)

### Pasos para Ejecutar (Desde Ubuntu) 🐧

1. **Navegar a la carpeta Communication**:  
   - Compila y ejecuta el servidor C++.
     ```sh
     cd Communication/
     ```

1. 📥 **Servidor**:  
   - Compila y ejecuta el servidor C++.
     ```sh
     g++ -o server server.cpp
     ./server
     ```

2. 📤 **Cliente**: 
   - Ejecuta el cliente Python.
     ```sh
     python3 client.py
     ```

### 🔩 Configuración 

- El servidor escucha en el puerto `12345`.
- El cliente envía mensajes a `localhost` en el puerto `12345`.

### ☝️ Funcionalidades 

- **Codificación de Mensajes**: Transforma el mensaje en su representación binaria.
- **Aplicación de Ruido**: Introduce errores aleatorios en el mensaje para simular una transmisión real.
- **Cálculo de Integridad**: Utiliza los algoritmos de Fletcher y Hamming para asegurar la integridad del mensaje.
- **Envío de Información**: Transmite el mensaje con la información de integridad al servidor.
- **Verificación en el Servidor**: El servidor verifica la integridad del mensaje recibido y guarda los resultados en un archivo CSV.
- **Estadística de Resultados**: Junto al CSV con los resultados se encuentra un `.ipynb` el cual contiene código de python que genera algunas gráficas estadísticas sobre los resultados obtenidos por cada algoritmo de detección y correción errores así como estadística sobre los mensajes enviados con o sin errores.

👨‍💻 **Autores**

- Diego Alexander Hernández Silvestre, Carné: 21270 🛻
- Linda Inés Jiménez Vides, Carné: 21169 🏎️
