import sys
import os
import random
import socket
import time
import string
import json

current_dir = os.path.dirname(os.path.abspath(__file__))

fletcher_checksum_path = os.path.join(current_dir, '../Fletcher_Checksum')
hamming_path = os.path.join(current_dir, '../Hamming')

sys.path.append(fletcher_checksum_path)
sys.path.append(hamming_path)

import EmisorFC
import EmisorH

def codificarMensaje(mensaje):
    mensaje_binario = ''.join(format(ord(c), '08b') for c in mensaje)
    while len(mensaje_binario) % 8 != 0:
        mensaje_binario += '0'
    return mensaje_binario

def aplicarRuido(data, probabilidad_error):
    noisyData = []
    for bit in data:
        if random.random() < probabilidad_error:
            noisyData.append('1' if bit == '0' else '0')
        else:
            noisyData.append(bit)
    return ''.join(noisyData)

def enviarInformacion(mensaje, host, puerto):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, puerto))
        s.sendall(mensaje.encode('utf-8'))

def calcularIntegridad(mensaje, algoritmo, blockSize=None):
    if algoritmo == "Fletcher":
        messageWithChecksum, checksumBin, paddingLength, _ = EmisorFC.calculateFletcherChecksum(mensaje, blockSize)
        return messageWithChecksum
    elif algoritmo == "Hamming":
        mensajeCodificado = EmisorH.paridadHamming(mensaje)
        return mensajeCodificado
    else:
        return None

def generarMensajeAleatorio(tamano):
    return ''.join(random.choices(string.ascii_letters + string.digits + string.punctuation, k=tamano))

def realizarPruebas(mensajes_config, host, puerto):
    resultados = []
    for mensaje, algoritmo, blockSize in mensajes_config:
        mensajeBinario = codificarMensaje(mensaje)
        mensajeConIntegridad = calcularIntegridad(mensajeBinario, algoritmo, blockSize)

        if mensajeConIntegridad is not None:
            mensajeConRuido = aplicarRuido(mensajeConIntegridad, 0.001)  
            if algoritmo == "Fletcher":
                mensaje_final = f"{mensajeConRuido};{algoritmo};{blockSize}"  
            else:
                mensaje_final = f"{mensajeConRuido};{algoritmo}" 
            
            enviarInformacion(mensaje_final, host, puerto)
            resultados.append({"mensaje": mensaje, "algoritmo": algoritmo, "ruido": mensajeConRuido})
            print("✅  Mensaje enviado con éxito:", mensaje_final)

if __name__ == "__main__":
    hamming_mensajes = [(generarMensajeAleatorio(20), "Hamming", None) for _ in range(20000)]
    fletcher_mensajes8 = [(generarMensajeAleatorio(20), "Fletcher", 8) for _ in range(20000)]
    fletcher_mensajes16 = [(generarMensajeAleatorio(20), "Fletcher", 16) for _ in range(20000)]
    fletcher_mensajes32 = [(generarMensajeAleatorio(20), "Fletcher", 32) for _ in range(20000)]

    realizarPruebas(hamming_mensajes, 'localhost', 12345)
    realizarPruebas(fletcher_mensajes8, 'localhost', 12345)
    realizarPruebas(fletcher_mensajes16, 'localhost', 12345)
    realizarPruebas(fletcher_mensajes32, 'localhost', 12345)
