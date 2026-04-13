#include <iostream>
#include <fstream>
#include <filesystem> // c++17
#include <limits> // for pausing program
using namespace std;
namespace fs = std::filesystem;

// function to ASCII shift a string
string shifter(string text, int key)
{
    string temp = "";
    for (auto ch : text)
    {
        temp += ch + key;
    }
    return temp;
}

unsigned char byteShifter(unsigned char uc, int key)
{
    return uc + key;
}

void pause(string message = "Press enter to continue...")
{
    cout << message << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// function to open file, apply shift and encrypt to write into new file
int encrypt(string filename, int key)
{
    // check if file exists
    if (!fs::exists(filename)) {
        cerr << "Couldn't open file." << endl;
        return 1;
    }

    // opening files that will be used in the program
    fstream inputFile(filename, ios::in | ios::binary);
    fstream outputFile("encrypted.dat", ios::out | ios::binary);
    fstream saves("saves.txt", ios::out);
    
    saves << filename << endl; // save original file name 
    saves << key << endl; // save original key
    cout << "File successfully opened" << endl;
    char c;
    while (inputFile.get(c))
    {
        unsigned char uc = byteShifter(static_cast<unsigned char>(c), key);
        outputFile.put(uc);
    }
    

    // close files
    saves.close();
    outputFile.close();
    inputFile.close();

    if (!fs::remove(filename)) {
        cerr << "Unable to complete encryption process.." << endl;
        cerr << "Please make sure the file you are trying to encrypt is closed and re-run the program" << endl;
        fs::remove("encrypted.dat");
        fs::remove("saves.txt");
        return 1;
    }

    return 0;
}


// function to decrypt 
int decrypt(int key)
{
    // open files
    fstream saves("saves.txt", ios::in);
    fstream backupFile("encrypted.dat", ios::in | ios::binary);

    // read saves file to retrieve name of original file
    string filename;
    saves >> filename;

    // retrieving old key
    int oldkey;
    saves >> oldkey;

    if (oldkey != key)
    {
        cerr << "The keys do not match." << endl;
        return 1;
    }
    

    fstream originalFile(filename, ios::out | ios::binary); // create original file

    // write to file 
    char c;
    while (backupFile.get(c)) 
    {
        unsigned char uc = byteShifter(static_cast<unsigned char>(c), -key);
        originalFile.put(uc);
    }

    // close files
    saves.close();
    backupFile.close();
    originalFile.close();

    // remove backups
    fs::remove("encrypted.dat");
    fs::remove("saves.txt");

    return 0;

}

int main()
{
    /*
    cout << "The encrypted version is:" << endl;
    string stringg = shifter(text,key);
    cout << stringg << endl;

    cout << "The decrypted version is:" << endl;
    cout << shifter(stringg, -key) << endl;
    */
    string file;
    int key;
    int choice;
    // check if encryption files exist
    if (fs::exists("encrypted.dat") && fs::exists("saves.txt"))
    {
        cout << "Do you want to encrypt(0) or decrypt(1)?" << endl;
        cin >> choice;
    }
    else
    {
        cout << "Enter 0 to encrypt" << endl;
        cin >> choice;  
    }
    switch (choice)
    {
        case 0:
            cout << "Enter filename" << endl;
            cin >> file;

            cout << "Enter the key" << endl;
            cin >> key;

            if (encrypt(file, key) == 0)
            {
                cout << "File successfully encrypted." << endl;
            }
            else
            {
                cerr << "Problems encrypting the file." << endl;
            }
            break;
        case 1:
            cout << "Enter the key to decrypt the file" << endl;
            cin >> key;

            if (decrypt(key) == 0) 
            {
                cout << "File decrypted" << endl;
            }
            else 
            {
                cerr << "Unable to decrypt file" << endl;
            }
            break;

        default:
            cerr << "Invalid choice" << endl;
            break;
    }
    pause("Program will quit... on enter");
    return 0;
}