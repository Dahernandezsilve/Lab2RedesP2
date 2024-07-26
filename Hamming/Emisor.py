def paridad_hamming(mensaje):

    m = len(mensaje)
    r = 1
    while 2**r < m + r + 1:
        r += 1

    mensaje_extendido = ['0'] * (m + r)
    j = 0
    for i in range(1, len(mensaje_extendido) + 1):
        if (i & (i - 1)) == 0:  
            continue
        mensaje_extendido[i - 1] = mensaje[j]
        j += 1

    for i in range(r):
        paridad = 0
        for j in range(1, len(mensaje_extendido) + 1):
            if j & (1 << i):
                paridad ^= int(mensaje_extendido[j - 1])
        mensaje_extendido[(1 << i) - 1] = str(paridad)

    return ''.join(mensaje_extendido)

mensaje = input("Ingrese el mensaje a codificar: ")
mensaje_codificado = paridad_hamming(mensaje)
print(f"Mensaje codificado con Hamming: {mensaje_codificado}")
