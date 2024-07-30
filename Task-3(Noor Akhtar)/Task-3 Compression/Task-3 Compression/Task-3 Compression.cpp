#include  <iostream>

#include  <fstream>

#include  <string>
using namespace std;

void compress(const string& inputFile, const string& outputFile) { 

    ifstream inFile(inputFile, ios::binary);

    ofstream outFile(outputFile, ios::binary);
    if (!inFile.is_open() || !outFile.is_open()) {
        cerr << "Error opening files!" << endl;
        return;
    }

    char currentChar;
    char previousChar = '\0';
    int count = 0;

    while (inFile.get(currentChar)) {
        if (currentChar == previousChar && count < 255) {
            ++count;
        }
        else {
            if (count > 0) {
                outFile.put(previousChar);
                outFile.put(static_cast<char>(count));
            }
            previousChar = currentChar;
            count = 1;
        }
    }

    if (count > 0) {
        outFile.put(previousChar);
        outFile.put(static_cast<char>(count));
    }

    inFile.close();
    outFile.close();
}

void decompress(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);
    if (!inFile.is_open() || !outFile.is_open()) {
       cerr << "Error opening files!" << endl;
        return;
    }

    char currentChar;
    char countChar;

    while (inFile.get(currentChar)) {
        inFile.get(countChar);
        int count = static_cast<unsigned char>(countChar);
        for (int i = 0; i < count; ++i) {
            outFile.put(currentChar);
        }
    }

    inFile.close();
    outFile.close();
}

int main() {
    string inputFilePath, outputFilePath;
    char choice;

    cout << "Enter input file path: ";
    getline(cin, inputFilePath);

    cout << "Enter output file path: ";
    getline(cin, outputFilePath);

    cout << "'c' to compress and 'd' for decompress the file? ";
    cin >> choice;

    try {
        if (choice == 'c' || choice == 'C') {
            compress(inputFilePath, outputFilePath);
            cout << "File compressed successfully!" << endl;
        }
        else if (choice == 'd' || choice == 'D') {
            decompress(inputFilePath, outputFilePath);
            cout << "File decompressed successfully!" << endl;
        }
        else {
            cerr << "Invalid choice!" << endl;
        }
    }
    catch (const std::exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    }

    return 0;
}
