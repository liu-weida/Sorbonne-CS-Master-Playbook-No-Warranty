import subprocess

# Define the path to your private key file
private_key_file = 'private_key.pem'

# Ask the user for the challenge
challenge = input("Enter the challenge: ")

try:
    # Run the openssl dgst command and capture the output
    command = ['openssl', 'dgst', '-sha256', '-hex', '-sign', private_key_file]
    openssl_process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    output, error = openssl_process.communicate(input=challenge)

    # Check if the command was successful
    if openssl_process.returncode == 0:
        # Extract and print only the signature part
        signature = output.strip().split('= ')[1]
        # print("Signature:")
        print(signature)
    else:
        print("Error executing openssl:")
        print(error)
except Exception as e:
    print("An error occurred:", e)

