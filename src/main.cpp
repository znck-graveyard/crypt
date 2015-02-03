#ifndef unit_testing
#include <iostream>
#include <cstdio>
#include <vector>
#include <cctype>
#include "aes.h"
#include "rijndael.h"

using namespace std;
using namespace znck;

byte hex_val(byte x) {
    x = tolower(x);
    if ('0' <= x && x <= '9') return x - '0';
    else if ('a' <= x && x <= 'f') return x - 'a' + 10;
    else return 0xff;
}

int main() {

    byte input[32];
    cout << "Key(in hex):: ";
    cin >> input;
    byte key[16];
    for (int i = 0; i < 16; ++i) key[i] = hex_val(input[2 * i]) << 4 | hex_val(input[2 * i + 1]);
    
    cout << "PT(in hex) :: ";
    byte ln, hn;
    vector<byte> message;
    getchar();
    while(cin.good()) {
        hn = getchar();
        ln = getchar();
        if (0xff == hex_val(hn) || 0xff == hex_val(ln)) break;
        message.push_back(hex_val(hn) << 4 | hex_val(ln));
        if (cin.eof()) break;
    }

    cout << "Message length:: " << message.size() << "byte" << endl;
    
    byte * CT = new byte[message.size()];
   
    for (int i = 0; i < message.size(); ++i) {
        CT[i] = message[i];
    }

    size_t length = message.size();
    CT = AES::encrypt(input, CT, length, AES_MODE_ECB);

    cout << "Message length:: " << length << "byte" << endl;
    cout << "CT(in hex):: " ;
    for (int i = 0; i < length; ++i) {
        printf("%c", CT[i]);
    }
    cout << endl;

    for (int i = 0; i < message.size(); ++i) {
        CT[i] = message[i];
    }


    return 0;
}
#endif
