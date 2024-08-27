#include <iostream>
#include <cstring>
#include "aes.hpp"
#include <conio.h>
#include <iomanip>
#include <math.h>

using namespace std;

int main(){
    char plaintext[20];
    char cipherkey[20];
    char roundKeys[11][4][4][2];

    // Input plaintext
    cout << "Enter plaintext (maximum 16 characters): "; 
    cin.getline(plaintext, 20);

    if (strlen(plaintext) > 16) {
        cout << "Plaintext is too long. Maximum 16 characters." << endl;
    } else {
        // Input cipherkey
        cout << "Enter cipherkey (maximum 16 characters): "; 
        cin.getline(cipherkey, 20);

        if (strlen(cipherkey) > 16) {
            cout << "Cipherkey is too long. Maximum 16 characters." << endl;
        } else {
            // Ensure plaintext and cipherkey have the same length
            if (strlen(plaintext) != strlen(cipherkey)) {
                cout << "Plaintext and cipherkey must have the same length." << endl;
            } else {
                // Convert plaintext to decimal format
                int despt[4][4];
                char resultEncryption[4][4][2];
                stringToMatrix(plaintext, despt);

                // Convert decimal plaintext to hexadecimal 16-bit format
                char state[4][4][2];
                matrixToHex(despt, state);

                // Convert cipherkey to decimal format
                int desck[4][4];
                stringToMatrix(cipherkey, desck);

                // Convert decimal cipherkey to hexadecimal 16-bit format
                char key[4][4][2];
                matrixToHex(desck, key);

                // Round key 0 is the cipherkey
                for (int row = 0; row < 4; row++) {
                    for (int col = 0; col < 4; col++) {
                        for (int i = 0; i < 2; i++){
                            roundKeys[0][row][col][i] = key[row][col][i];
                        }
                    }
                }

                // Generate round keys
                for (int round = 1; round <= 10; round++) {
                    char temp[4][2]; // Temporary column storage

                    // RotWord (Shift Byte upwards)
                    for (int i = 0; i < 4; i++) {
                        temp[i][0] = roundKeys[round - 1][i][3][0];
                        temp[i][1] = roundKeys[round - 1][i][3][1];
                    }
                    rotWord(temp);

                    // SubBytes
                    subBytes(temp);

                    // First column of round i
                    for (int i = 0; i < 4; i++){
                        xorfisrtcolumn(round, temp[i], roundKeys[round - 1][i][0], Rcon[i], roundKeys[round][i][0]);
                    }

                    // Columns 2-4 of round i
                    for (int col = 1; col < 4; col++) {
                        for (int row = 0; row < 4; row++) {
                            xornextcolumn(roundKeys[round][row][col - 1], roundKeys[round - 1][row][col], roundKeys[round][row][col]);
                        }
                    }
                }

                // AES
                aesEncrypt(state, roundKeys, resultEncryption);
                cout << "\n\n\n";
                cout << " ======================================== " << endl;
                cout << " |  Encryption Result :   ";
                int tampunganakhir[2], blocktampungan[2][4], tampbiner[8];
                for (int row = 0; row < 4; row++){
                    for (int col = 0; col < 4; col++){
                        for (int j = 0; j < 2; j++){
                            tampunganakhir[j] = static_cast<int>(resultEncryption[row][col][j]-48);
                            if (tampunganakhir[j] > 9) {
                                tampunganakhir[j] -= 7;
                            }
                        }
                        for (int i = 0; i < 2; i++){
                            desToBiner(tampunganakhir[i], blocktampungan[i]);
                        }
                        for (int i = 0; i < 4; i++){
                            tampbiner[i] = blocktampungan[0][i];
                            tampbiner[i + 4] = blocktampungan[1][i];
                        }
                        char ascii = static_cast<char>(biner8ToDes(tampbiner));
                        cout << ascii;
                    }
                }
                cout << "\n ======================================== " << endl;
                cout << endl << endl << endl;
            }
        }
    }
    return 0;
}
