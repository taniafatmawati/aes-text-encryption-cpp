#ifndef AES_H
#define AES_H

// Tabel S-Box AES
extern const char SBox[16][16][2];
extern const char Rcon[4][10][2];

// Function declarations
void stringToMatrix(const char* input, int matrix[4][4]);
void decimalToHex(int decimalValue, char hexValue[2]);
void matrixToHex(int inputMatrix[4][4], char hexMatrix[4][4][2]);
void destoBiner8digit(int inputdesimal, int binary[8]);
void desToBiner(int inputdesimal, int binary[4]);
int binerToDes(int inputbinary[4]);
int biner8ToDes(int inputbinary[8]);
void rotWord(char word[4][2]);
void subBytes(char word[4][2]);
void xorBlocks(int block1[4], int block2[4], int result[4]);
void xorfisrtcolumn(int round, char subbytes[2], char rowprev[2], const char rcon[10][2], char result[2]);
void tampilmatrix(char matrix[4][4][2]);
void xornextcolumn(char col1[2], char col2[2], char result[2]);
void addRoundKey(char state[4][4][2], char roundKey[4][4][2], char result[4][4][2]);
void shiftRows(char state[4][4][2]);
void perkalianheksa(char mixMatrix[2], char state[2], char resultchar[2]);
void mixColumns(char state[4][4][2]);
void aesEncrypt(char state[4][4][2], char roundKeys[11][4][4][2], char resultstate[4][4][2]);

#endif // AES_H
