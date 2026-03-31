#include <iostream>
#include <fstream>
using namespace std;

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

// function to open file, apply shift and encrypt to write into new file
void encrypt(string filename, int key)
{
    fstream inputFile(filename, ios::in | ios::binary);
    fstream outputFile("encrypted.dat", ios::out | ios::binary);
    if (!inputFile.is_open())
    {
        cerr << "Error: Unable to open file!\n";
    }
    else
    {
        cout << "File successfully opened" << endl;
        char c;
        while (inputFile.get(c))
        {
            unsigned char uc = byteShifter(static_cast<unsigned char>(c), key);
            outputFile.put(uc);
        }
    }
    outputFile.close();
    inputFile.close();
}

int main()
{
    string text;
    int key;
    cout << "Enter filename" << endl;
    cin >> text;

    cout << "Enter the key" << endl;
    cin >> key;

    /*
    cout << "The encrypted version is:" << endl;
    string stringg = shifter(text,key);
    cout << stringg << endl;

    cout << "The decrypted version is:" << endl;
    cout << shifter(stringg, -key) << endl;
    */
    encrypt(text, key);


    return 0;
}