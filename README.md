# file-encryptor
A CLI tool where you provide a filename and a "key",  and it transforms the file into unreadable gibberish (and back again).

## Features
1. A main menu (WIP)
2. Encrypt a file with a key
3. Decrypt a file with a key
4. WIP

## How the program works
- The program first asks if you want to encrypt a file.
- So logically you choose option 0 (encrypt)

### For the encryption part
- It then asks for a filename you want to encrypt, and a specific key
- It will save the file's contents and its name. Then delete it.
  
### For the decryption part
- On the first run, you will not see the decryption option, as there is no file to encrypt
- You choose option 1 (decrypt) on startup
- The program asks for the key used in the encryption
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


