#include <bits/stdc++.h>
using namespace std;

// Function to perform XOR division
string xorDivision(string data, string generator) {
    int n = generator.size();

    string temp = data.substr(0, n);

    for (int i = n; i < data.size(); i++) {
        if (temp[0] == '1') {
            // XOR with generator
            for (int j = 1; j < n; j++) {
                temp[j] = (temp[j] == generator[j]) ? '0' : '1';
            }
        }

        // Shift left and bring next bit
        temp = temp.substr(1) + data[i];
    }

    // Last step
    if (temp[0] == '1') {
        for (int j = 1; j < n; j++) {
            temp[j] = (temp[j] == generator[j]) ? '0' : '1';
        }
    }

    return temp.substr(1); // remainder (CRC)
}

int main() {
    string data = "11001";
    string generator = "101";

    int zeros = generator.size() - 1;

    // Append zeros
    string appendedData = data + string(zeros, '0');

    // Generate CRC
    string crc = xorDivision(appendedData, generator);

    // Transmitted data
    string transmitted = data + crc;

    cout << "Data: " << data << "\n";
    cout << "Generator: " << generator << "\n";
    cout << "CRC: " << crc << "\n";
    cout << "Transmitted: " << transmitted << "\n";

    // Receiver check
    string remainder = xorDivision(transmitted, generator);

    if (remainder == string(zeros, '0'))
        cout << "No Error\n";
    else
        cout << "Error Detected\n";

    return 0;
}