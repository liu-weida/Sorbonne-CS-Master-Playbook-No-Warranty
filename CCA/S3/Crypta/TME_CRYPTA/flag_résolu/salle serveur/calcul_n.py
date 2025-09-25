from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization

# Load the public key from a PEM file
with open('public_ppti.pem', 'rb') as key_file:
    public_key = serialization.load_pem_public_key(
        key_file.read(),
        backend=default_backend()
    )

# Extract the modulus (n) 
n = public_key.public_numbers().n

# Convert modulus to a hexadecimal string
n_hex = hex(n)

# Print the modulus
print(n_hex)
