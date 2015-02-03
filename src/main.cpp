#ifndef unit_testing
#include <iostream>
#include <cstdio>
#include <vector>
#include <cctype>
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

    byte input[32], * output;
    cout << "Key(in hex):: ";
    cin >> input;
    byte key[16];
    for (int i = 0; i < 16; ++i) key[i] = hex_val(input[2 * i]) << 4 | hex_val(input[2 * i + 1]);
    output = rijndael::expand_key(key);

    cout << "PT(in hex):: ";
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
    byte rem = message.size() % 16;
    cout << "Message length:: " << message.size() << endl;
    while (rem > 0) {
        message.push_back(rem << 4 | rem);
        --rem;
    }
    byte * CT = new byte[message.size()];
   
    for (int i = 0; i < message.size(); ++i) {
        CT[i] = message[i];
    }

    for (int i = 0; i < message.size(); i += 16) {
        rijndael::encrypt(output, CT + i);
    }

    cout << "CT(in hex):: ";
    for (int i = 0; i < message.size(); ++i) {
        printf("%02x", CT[i]);
    }
    cout << endl;
    return 0;
}
#endif
