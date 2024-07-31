import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__))

fletcher_checksum_path = os.path.join(current_dir, '../Fletcher_Checksum')
hamming_path = os.path.join(current_dir, '../Hamming')

sys.path.append(fletcher_checksum_path)
sys.path.append(hamming_path)


import EmisorFC
import EmisorH
import random
import socket


def solicitarMensaje():
    mensaje = input("✍️  Ingrese el mensaje a enviar: ")
    algoritmo = input("👀  Ingrese el algoritmo a utilizar (e.g., Fletcher): ")
    return mensaje, algoritmo


def codificarMensaje(mensaje):
    # Convertir a binario
    mensaje_binario = ''.join(format(ord(c), '08b') for c in mensaje)

    # Rellenar con ceros si no es múltiplo de 8
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


def enviarInformacion(mensaje_binario, host, puerto):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, puerto))
        s.sendall(mensaje_binario.encode('utf-8'))


def calcularIntegridad(mensaje, algoritmo):
    if algoritmo == "Fletcher":
        messageWithChecksum, checksumBin, paddingLength, blockSize = EmisorFC.calculateFletcherChecksum(mensaje)
        print(f"🔎  Mensaje codificado con Fletcher Checksum: {messageWithChecksum}")
        return messageWithChecksum, algoritmo
    elif algoritmo == "Hamming":
        mensajeCodificado = EmisorH.paridadHamming(mensaje)
        print(f"🥓  Mensaje codificado con Hamming: {mensajeCodificado}")
        return mensajeCodificado, algoritmo
    else:
        print("🚨  Algoritmo no soportado")


if __name__ == "__main__":
    mensaje, algoritmo = solicitarMensaje()
    mensajeBinario = codificarMensaje(mensaje)
    mensajeConIntegridad, algoritmo = calcularIntegridad(mensajeBinario, algoritmo)
    mensajeConRuido = aplicarRuido(mensajeConIntegridad, 0.00)  # Probabilidad de error 1%
    mensaje = mensajeConRuido +';' + algoritmo
    enviarInformacion(mensaje, 'localhost', 12345)
    print("✅  Mensaje enviado con éxito")

