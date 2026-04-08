# file-encryptor
A CLI tool where you provide a filename and a "key",  and it transforms the file into unreadable gibberish (and back again).

## Features
1. A main menu (WIP)
2. Encrypt a file with a key
3. Decrypt a file with a key
4. WIP

## How the program works
- The program first asks if you want to encrypt or decrypt a file, i'll change it later on as initially you do not have any encrypted file to decrypt
- So logically you choose option 0 (encrypt)
### For the encryption part
- It then asks for a filename you want to encrypt, and a specific key
- It will save the file's contents and its name. Then delete it.
### For the decryption part
- You choose option 1 (decrypt) on startup
- The program asks for the key used in the encryption
- If you enter a key that was not used for the encryption, the contents of the file will be **different** from what they were before, as there is no key comparison as of right now
- The backup files are deleted and the original file is restored.
- **As of right now you cannot encrypt a file while there is another one decrypted, works one at a time, will soon change it.**

## How to run it
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


