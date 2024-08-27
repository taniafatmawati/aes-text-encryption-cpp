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

    // Memasukkan plaintext
    cout<<"Masukkan plaintext (maksimum 16 karakter): ";cin.getline(plaintext,20);

    if (strlen(plaintext) > 16) {
   	    cout<<"Plaintext terlalu panjang. Maksimum 16 karakter."<<endl;
    } else {
   	    // Memasukkan cipherkey
   	    cout<<"Masukkan cipherkey (maksimum 16 karakter): ";cin.getline(cipherkey,20);

   	    if (strlen(cipherkey) > 16) {
   		    cout<<"Cipherkey terlalu panjang. Maksimum 16 karakter."<<endl;
   	    } else {
      	    // Memastikan plaintext dan cipherkey memiliki panjang yang sama
   		    if (strlen(plaintext) != strlen(cipherkey)) {
   			    cout<<"Panjang plaintext dan cipherkey harus sama."<<endl;
   		    } else {
         	    // Mengubah plaintext ke format desimal
   			    int despt[4][4];
                char resultEncryption[4][4][2];
   			    stringToMatrix(plaintext, despt);
   			    cout<<"\n Matrix of Byte Plaintext:"<<endl;
   			    for (int row = 0; row < 4; row++) {
                    cout<<" ";
   				    for (int col = 0; col < 4; col++) {
      				    cout<<despt[row][col]<<"\t";
      			    }
      			    cout<<endl;
   			    }

   			    // Mengubah desimal plaintext format heksadesimal 16-bit
   			    char state[4][4][2];
   			    matrixToHex(despt, state);
   			    cout<<"\n State (Plaintext):"<<endl;
   			    for (int row = 0; row < 4; row++) {
                    cout<<" ";
   				    for (int col = 0; col < 4; col++) {
                        for (int i = 0; i < 2; i++){
      				        cout<<state[row][col][i];
                        }
                        cout<<"\t";
      			    }
      			    cout<<endl;
   			    }

                // Mengubah cipherkey ke format desimal
   			    int desck[4][4];
   			    stringToMatrix(cipherkey, desck);
   			    cout<<"\n Matrix of Byte Cipherkey:"<<endl;
   			    for (int row = 0; row < 4; row++) {
                    cout<<" ";
   				    for (int col = 0; col < 4; col++) {
      				    cout<<desck[row][col]<<"\t";
      			    }
      			    cout<<endl;
   			    }

   			    // Mengubah desimal cipherkey format heksadesimal 16-bit
   			    char key[4][4][2];
   			    matrixToHex(desck, key);
   			    cout<<"\n Cipherkey (hexadecimal):"<<endl;
   			    for (int row = 0; row < 4; row++) {
                    cout<<" ";
   				    for (int col = 0; col < 4; col++) {
                        for (int i = 0; i < 2; i++){
      				        cout<<key[row][col][i];
                        }
                        cout<<"\t";
      			    }
      			    cout<<endl;
   			    }

                cout<<"\n\n\n\n";
                cout<<" ======================================== "<<endl;
                cout<<" ---------- Pembangkitan Kunci ---------- "<<endl;
                cout<<" ======================================== "<<endl;

                // Kunci ronde 0 adalah cipherkey
                cout<<"\n ---------- \n ROUND 0 : \n ---------- "<<endl;
    			for (int row = 0; row < 4; row++) {
        			for (int col = 0; col < 4; col++) {
               	        for (int i = 0; i < 2; i++){
                            roundKeys[0][row][col][i] = key[row][col][i];
                        }
        			}
    			}
                tampilmatrix (roundKeys[0]);

    			// Membuat round keys
    			for (int round = 1; round <= 10; round++) {
                    cout<<"\n ---------- \n ROUND "<<round<<" : \n ---------- "<<endl;
        			char temp[4][2]; // Penyimpanan kolom sementara

        			// RotWord (Shift Byte ke atas)
        			for (int i = 0; i < 4; i++) {
            		    temp[i][0] = roundKeys[round - 1][i][3][0];
            		    temp[i][1] = roundKeys[round - 1][i][3][1];
        			}
        			rotWord(temp);

        			// SubBytes
        			subBytes(temp);

                    // Kolom 1 ronde ke-i
                    for (int i = 0; i < 4; i++){
                        xorfisrtcolumn (round, temp[i], roundKeys[round - 1][i][0], Rcon[i], roundKeys[round][i][0]);
                    }

        			// Kolom 2-4 ronde ke-i
        			for (int col = 1; col < 4; col++) {
            		    for (int row = 0; row < 4; row++) {
                            xornextcolumn (roundKeys[round][row][col - 1], roundKeys[round - 1][row][col], roundKeys[round][row][col]);
                        }
        			}

                    // Mencetak matriks pada setiap ronde
                    tampilmatrix (roundKeys[round]);
    			}

                // AES
                cout<<"\n\n\n";
                cout<<" ======================================== "<<endl;
                cout<<" ------------ Proses Enkripsi ----------- "<<endl;
                cout<<" ======================================== "<<endl;
                aesEncrypt(state, roundKeys, resultEncryption);
                cout<<"\n\n\n";
                cout<<" ======================================== "<<endl;
                cout<<" |  Hasil Enkripsi :   ";
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
                            desToBiner(tampunganakhir[i],blocktampungan[i]);
                        }
                        for (int i = 0; i < 4; i++){
                            tampbiner[i] = blocktampungan[0][i];
                            tampbiner[i + 4] = blocktampungan[1][i];
                        }
                        char ascii = static_cast<char>(biner8ToDes(tampbiner));
                        cout<<ascii;
                    }
                }
                cout<<"\n ======================================== "<<endl;
                cout<<endl<<endl<<endl;
            }
        }
    }
	getch();
}
