def fletcher8(data: str) -> int:
    sum1 = 0
    sum2 = 0
    
    for char in data:
        if char not in '01':
            raise ValueError("Data must be a binary string with '0' and '1' characters only.")
        sum1 = (sum1 + int(char)) % 255
        sum2 = (sum2 + sum1) % 255
    
    return (sum2 << 8) | sum1

def fletcher16(data: str) -> int:
    sum1 = 0
    sum2 = 0
    
    for char in data:
        if char not in '01':
            raise ValueError("Data must be a binary string with '0' and '1' characters only.")
        sum1 = (sum1 + int(char)) % 65535
        sum2 = (sum2 + sum1) % 65535
    
    return (sum2 << 16) | sum1

def fletcher32(data: str) -> int:
    sum1 = 0
    sum2 = 0
    
    for char in data:
        if char not in '01':
            raise ValueError("Data must be a binary string with '0' and '1' characters only.")
        sum1 = (sum1 + int(char)) % 4294967295
        sum2 = (sum2 + sum1) % 4294967295
    
    return (sum2 << 32) | sum1

def pad_message(data: str, block_size: int) -> str:
    padding_length = (block_size - (len(data) % block_size)) % block_size
    if padding_length != 0:
        data = '0' * padding_length + data
    return data, padding_length

def add_checksum_to_message(data: str, block_size: int) -> str:
    padded_data, padding_length = pad_message(data, block_size)
    
    if block_size == 8:
        checksum = fletcher8(padded_data)
        checksum_bits = 16  # 8 bits para cada suma
    elif block_size == 16:
        checksum = fletcher16(padded_data)
        checksum_bits = 32
    elif block_size == 32:
        checksum = fletcher32(padded_data)
        checksum_bits = 64
    else:
        raise ValueError("Unsupported block size")

    checksum_hex = f"{checksum:0{checksum_bits//4}X}"
    checksum_bin = f"{int(checksum_hex, 16):0{checksum_bits}b}"
    message_with_checksum = padded_data + checksum_bin
    return message_with_checksum, checksum_bin, padding_length

def main():
    print("📡 --- Emisor ---")
    message = input("🔠 Ingrese el mensaje binario: ")
    block_size = int(input("📏 Ingrese el tamaño del bloque (8, 16, o 32): "))
    
    message_with_checksum, checksum_bin, padding_length = add_checksum_to_message(message, block_size)
    print(f"📜 Mensaje con checksum: {message_with_checksum}")
    print(f"🔍 Checksum en binario: {checksum_bin}")

if __name__ == "__main__":
    main()
