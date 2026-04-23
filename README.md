# file-encryptor
A lightweight CLI tool that transforms files into encrypted data using a user-provided key. It supports multiple files and organizes them within a secure local vault.

## Features
1. **Multi-File Support**: Encrypt several files independently, each with its own (or the same) key.
2. **Vault Organization**: All encrypted data is automatically stored in a dedicated `/vault` directory.
3. **Metadata Integrity**: Stores the original filename and a hash of the key inside the encrypted file to ensure safe and verified restoration.
4. **XOR Scrambling**: Uses a byte-wise XOR operation with a repeating key for efficient data transformation.
5. **Secure Lifecycle**: Automatically deletes the original file upon successful encryption and deletes the vault entry upon successful decryption.

## How the program works
The program uses a simple XOR-based encryption algorithm. For added security, it does not store your key in plain text; instead, it stores a cryptographic hash to verify your identity during decryption.

### 1. Encryption
- Provide the path to a file (e.g., `notes.txt`).
- Provide a secret key.
- The program creates `vault/notes.txt.enc`.
- The first line of the `.enc` file contains the hashed key for verification.
- The second line contains the encrypted original path.
- The remaining bytes are the XOR-scrambled contents of the file.
- **The original file is removed once the process is verified.**
  
### 2. Decryption
- Select a file from the list of available items in the `vault`.
- Enter the original key.
- If the key hash matches, the program restores the file to its original location.
- **The vault file is removed once the restoration is complete.**

## How to run it

### Requirements
- A C++ compiler supporting **C++17** (required for `std::filesystem`).

### With the release
1. Download the release
2. Unzip the file and cd into the directory
```bash
./main
```
### Building it yourself
1. Clone the repo and cd into the directory
```bash
git clone https://github.com/theazrs/file-encryptor.git
```
```bash
cd file-encryptor
```
2. Build it
#### For Linux/MacOS
```bash
g++ main.cpp -std=c++17 -o main
```
#### For Windows
- Just make sure you use c++17 to build it, otherwise it will not succeed.
  
## Suggestions
Any suggestion or help can be submitted as well as pull requests.

## License 
[GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.en.html)
