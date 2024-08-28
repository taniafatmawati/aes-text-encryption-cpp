#include "aes.hpp"
#include <iostream>
#include <cstring>
#include <math.h>


using namespace std;

// AES S-Box Table
const char SBox[16][16][2] = {
    {{'6','3'}, {'7','C'}, {'7','7'}, {'7','B'}, {'F','2'}, {'6','B'}, {'6','F'}, {'C','5'}, {'3','0'}, {'0','1'}, {'6','7'}, {'2','B'}, {'F','E'}, {'D','7'}, {'A','B'}, {'7','6'}},
    {{'C','A'}, {'8','2'}, {'C','9'}, {'7','D'}, {'F','A'}, {'5','9'}, {'4','7'}, {'F','0'}, {'A','D'}, {'D','4'}, {'A','2'}, {'A','F'}, {'9','C'}, {'A','4'}, {'7','2'}, {'C','0'}},
    {{'B','7'}, {'F','D'}, {'9','3'}, {'2','6'}, {'3','6'}, {'3','F'}, {'F','7'}, {'C','C'}, {'3','4'}, {'A','5'}, {'E','5'}, {'F','1'}, {'7','1'}, {'D','8'}, {'3','1'}, {'1','5'}},
    {{'0','4'}, {'C','7'}, {'2','3'}, {'C','3'}, {'1','8'}, {'9','6'}, {'0','5'}, {'9','A'}, {'0','7'}, {'1','2'}, {'8','0'}, {'E','2'}, {'E','B'}, {'2','7'}, {'B','2'}, {'7','5'}},
    {{'0','9'}, {'8','3'}, {'2','C'}, {'1','A'}, {'1','B'}, {'6','E'}, {'5','A'}, {'A','0'}, {'5','2'}, {'3','B'}, {'D','6'}, {'B','3'}, {'2','9'}, {'E','3'}, {'2','F'}, {'8','4'}},
    {{'5','3'}, {'D','1'}, {'0','0'}, {'E','D'}, {'2','0'}, {'F','C'}, {'B','1'}, {'5','B'}, {'6','A'}, {'C','B'}, {'B','E'}, {'3','9'}, {'4','A'}, {'4','C'}, {'5','8'}, {'C','F'}},
    {{'D','0'}, {'E','F'}, {'A','A'}, {'F','B'}, {'4','3'}, {'4','D'}, {'3','3'}, {'8','5'}, {'4','5'}, {'F','9'}, {'0','2'}, {'7','F'}, {'5','0'}, {'3','C'}, {'9','F'}, {'A','8'}},
    {{'5','1'}, {'A','3'}, {'4','0'}, {'8','F'}, {'9','2'}, {'9','D'}, {'3','8'}, {'F','5'}, {'B','C'}, {'B','6'}, {'D','A'}, {'2','1'}, {'1','0'}, {'F','F'}, {'F','3'}, {'D','2'}},
    {{'C','D'}, {'0','C'}, {'1','3'}, {'E','C'}, {'5','F'}, {'9','7'}, {'4','4'}, {'1','7'}, {'C','4'}, {'A','7'}, {'7','E'}, {'3','D'}, {'6','4'}, {'5','D'}, {'1','9'}, {'7','3'}},
    {{'6','0'}, {'8','1'}, {'4','F'}, {'D','C'}, {'2','2'}, {'2','A'}, {'9','0'}, {'8','8'}, {'4','6'}, {'E','E'}, {'B','8'}, {'1','4'}, {'D','E'}, {'5','E'}, {'0','B'}, {'D','B'}},
    {{'E','0'}, {'3','2'}, {'3','A'}, {'0','A'}, {'4','9'}, {'0','6'}, {'2','4'}, {'5','C'}, {'C','2'}, {'D','3'}, {'A','C'}, {'6','2'}, {'9','1'}, {'9','5'}, {'E','4'}, {'7','9'}},
    {{'E','7'}, {'C','8'}, {'3','7'}, {'6','D'}, {'8','D'}, {'D','5'}, {'4','E'}, {'A','9'}, {'6','C'}, {'5','6'}, {'F','4'}, {'E','A'}, {'6','5'}, {'7','A'}, {'A','E'}, {'0','8'}},
    {{'B','A'}, {'7','8'}, {'2','5'}, {'2','E'}, {'1','C'}, {'A','6'}, {'B','4'}, {'C','6'}, {'E','8'}, {'D','D'}, {'7','4'}, {'1','F'}, {'4','B'}, {'B','D'}, {'8','B'}, {'8','A'}},
    {{'7','0'}, {'3','E'}, {'B','5'}, {'6','6'}, {'4','8'}, {'0','3'}, {'F','6'}, {'0','E'}, {'6','1'}, {'3','5'}, {'5','7'}, {'B','9'}, {'8','6'}, {'C','1'}, {'1','D'}, {'9','E'}},
    {{'E','1'}, {'F','8'}, {'9','8'}, {'1','1'}, {'6','9'}, {'D','9'}, {'8','E'}, {'9','4'}, {'9','B'}, {'1','E'}, {'8','7'}, {'E','9'}, {'C','E'}, {'5','5'}, {'2','8'}, {'D','F'}},
    {{'8','C'}, {'A','1'}, {'8','9'}, {'0','D'}, {'B','F'}, {'E','6'}, {'4','2'}, {'6','8'}, {'4','1'}, {'9','9'}, {'2','D'}, {'0','F'}, {'B','0'}, {'5','4'}, {'B','B'}, {'1','6'}}
};

const char Rcon[4][10][2] = {
    {{'0','1'}, {'0','2'}, {'0','4'}, {'0','8'}, {'1','0'}, {'2','0'}, {'4','0'}, {'8','0'}, {'1','B'}, {'3','6'}}, 
    {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
    {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
    {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}}
};

// Function to convert a string to a matrix of bytes
void stringToMatrix(const char* input, int matrix[4][4]) {
	int index = 0;

    for (int row = 0; row < 4; row++) {
   	    for (int col = 0; col < 4; col++) {
            if (index == (strlen(input)+1)){
         	    index = 0;
            } else {
         	    matrix[row][col] = static_cast<int>(input[index]);
      		    index++;
            }
        }
    }
}

// Function to convert decimal to hexadecimal
void decimalToHex(int decimalValue, char hexValue[2]) {
    int tampungan[8], temp[2][4];
    destoBiner8digit(decimalValue, tampungan);
    temp[0][0] = tampungan[0];
    temp[0][1] = tampungan[1];
    temp[0][2] = tampungan[2];
    temp[0][3] = tampungan[3];
    temp[1][0] = tampungan[4];
    temp[1][1] = tampungan[5];
    temp[1][2] = tampungan[6];
    temp[1][3] = tampungan[7];
    int temp1 = binerToDes(temp[0]);
    int temp2 = binerToDes(temp[1]);
    if (temp1 < 10) {
        hexValue[0] = static_cast<char>(temp1 + 48);
    } else {
        hexValue[0] = static_cast<char>((temp1 - 10) + 65);
    }
    if (temp2 < 10) {
        hexValue[1] = static_cast<char>(temp2 + 48);
    } else {
        hexValue[1] = static_cast<char>((temp2 - 10) + 65);
    }
}

// Function to convert matrix to hexadecimal format
void matrixToHex(const int inputMatrix[4][4], char hexMatrix[4][4][2]) {
	for (int row = 0; row < 4; row++) {
   	    for (int col = 0; col < 4; col++) {
      	    decimalToHex(inputMatrix[row][col], hexMatrix[row][col]);
        }
    }
}

// Function to convert decimal to 8-digit binary
void destoBiner8digit(int inputdesimal, int binary[8]){
    int tampungan=0,i,temp = -1;
    for (i = 7 ; i >= 0 ; i--) {
        if ((inputdesimal-tampungan) < pow(2,i)) {
            temp = temp + 1;
            binary[temp] = 0;
        } else {
            temp = temp + 1;
            binary[temp] = 1;
            tampungan = tampungan + pow(2,i);
        }
    }
}

// Function to convert decimal to binary
void desToBiner(int inputdesimal, int binary[4]){
	int tampungan=0,i,temp = -1;
    for (i = 3 ; i >= 0 ; i--)
    {
        if ((inputdesimal-tampungan) < pow(2,i))
        {
            temp = temp + 1;
            binary[temp] = 0;
        } else {
            temp = temp + 1;
            binary[temp] = 1;
            tampungan = tampungan + pow(2,i);
        }
    }
}

// Function to convert binary to decimal
int binerToDes(int inputbinary[4]){
	int desimal = 0;
    int temp = 3;
	for (int i = 0 ; i < 4; i++){
   	    if (inputbinary[i] == 1){
      	    desimal = desimal + pow(2,temp);
        }
        temp = temp - 1;
    }
    return desimal;
}

// Function to convert 8-bit binary to decimal
int biner8ToDes(int inputbinary[8]){
	int desimal = 0;
    int temp = 7;
	for (int i = 0 ; i < 8; i++){
   	    if (inputbinary[i] == 1){
      	    desimal = desimal + pow(2,temp);
        }
        temp = temp - 1;
    }
    return desimal;
}

// Function to perform RotWord
void rotWord(char word[4][2]) {
    char temp[2];
    temp[0] = word[0][0];
    temp[1] = word[0][1];
    word[0][0] = word[1][0];
    word[0][1] = word[1][1];
    word[1][0] = word[2][0];
    word[1][1] = word[2][1];
    word[2][0] = word[3][0];
    word[2][1] = word[3][1];
    word[3][0] = temp[0];
    word[3][1] = temp[1];
}

// Function to perform SubBytes
void subBytes(char word[4][2]) {
    static const char hexChars[] = "0123456789ABCDEF";
    for (int i = 0; i < 4; i++) {
        char xvalue = word[i][0];
        char yvalue = word[i][1];
        int x = -1, y = -1;

        // Converting hexadecimal character to decimal value
        for (int j = 0; j < 16; j++) {
            if (xvalue == hexChars[j]) {
                x = j;
            }
            if (yvalue == hexChars[j]) {
                y = j;
            }
        }

        if (x != -1 && y != -1) {
            // Replacing word[i] value with the value from SBox
            word[i][0] = SBox[x][y][0];
            word[i][1] = SBox[x][y][1];
        } else {
            // Error handling if the hexadecimal character is invalid
            cout << "Invalid hexadecimal character: " << xvalue << yvalue << endl;
        }
    }
}

// Function to perform XOR between two blocks
void xorBlocks(int block1[4], int block2[4], int result[4]) {
    for (int i = 0; i < 4; i++) {
        if (block1[i] == block2[i]) {
            result[i] = 0;
        } else {
            result[i] = 1;
        }
    }
}

// Function to perform XOR on the first column in each round of key generation process
void xorfisrtcolumn(int round, char subbytes[2], char rowprev[2], const char rcon[10][2], char result[2]) {
    int temp[3][2], i, j, blocktamp[3][2][4], tampungan[4], tampakhir[4];
    for (j = 0; j < 2; j++) {
        temp[0][j] = static_cast<int>(rowprev[j] - 48);
        temp[1][j] = static_cast<int>(subbytes[j] - 48);
        temp[2][j] = static_cast<int>(rcon[round - 1][j] - 48);
        for (i = 0; i < 3; i++) {
            if (temp[i][j] > 9) {
                temp[i][j] -= 7;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            desToBiner(temp[i][j], blocktamp[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        xorBlocks(blocktamp[0][i], blocktamp[1][i], tampungan);
        xorBlocks(tampungan, blocktamp[2][i], tampakhir);
        int temp1 = binerToDes(tampakhir);
        if (temp1 < 10) {
            result[i] = static_cast<char>(temp1 + 48);
        } else {
            result[i] = static_cast<char>((temp1 - 10) + 65);
        }
    }
}

// Function to perform XOR on the next column in each round of key generation process
void xornextcolumn(char col1[2], char col2[2], char result[2]) {
    int temp[2][2], i, j, blocktamp[2][2][4], tampungan[4];
    for (j = 0; j < 2; j++) {
        temp[0][j] = static_cast<int>(col1[j] - 48);
        temp[1][j] = static_cast<int>(col2[j] - 48);
        for (i = 0; i < 2; i++) {
            if (temp[i][j] > 9) {
                temp[i][j] -= 7;
            }
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            desToBiner(temp[i][j], blocktamp[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        xorBlocks(blocktamp[0][i], blocktamp[1][i], tampungan);
        int temp1 = binerToDes(tampungan);
        if (temp1 < 10) {
            result[i] = static_cast<char>(temp1 + 48);
        } else {
            result[i] = static_cast<char>((temp1 - 10) + 65);
        }
    }
}

// Function to perform AddRoundKey Transformation
void addRoundKey(char state[4][4][2], char roundKey[4][4][2], char result[4][4][2]) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            xornextcolumn(state[row][col], roundKey[row][col], result[row][col]);
        }
    }
}

// Function to perform ShiftRows Transformation
void shiftRows(char state[4][4][2]) {
    int temp[4][4][2];

    // Row 0 is not shifted
    for (int col = 0; col < 4; col++) {
        for (int i = 0; i < 2; i++) {
            temp[0][col][i] = state[0][col][i];
        }
    }

    // Row 1 is shifted 1 byte to the left
    for (int col = 0; col < 4; col++) {
        for (int i = 0; i < 2; i++) {
            temp[1][col][i] = state[1][(col + 1) % 4][i];
        }
    }

    // Row 2 is shifted 2 bytes to the left
    for (int col = 0; col < 4; col++) {
        for (int i = 0; i < 2; i++) {
            temp[2][col][i] = state[2][(col + 2) % 4][i];
        }
    }

    // Row 3 is shifted 3 bytes to the left
    for (int col = 0; col < 4; col++) {
        for (int i = 0; i < 2; i++) {
            temp[3][col][i] = state[3][(col + 3) % 4][i];
        }
    }

    // Copying the shifted result back to state
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int i = 0; i < 2; i++) {
                state[row][col][i] = temp[row][col][i];
            }
        }
    }
}

// Function to perform hexadecimal multiplication
void perkalianheksa(char mixMatrix[2], char state[2], char resultchar[2]) {
    int temp[2][2], tempakhir[2][4], tempbiner[2][2][4];
    int polynom[2][9], polynommod[9] = {1,0,0,0,1,1,0,1,1}, polynomresult[9] = {0,0,0,0,0,0,0,0,0};
    for (int j = 0; j < 2; j++){
        temp[0][j] = static_cast<int>(mixMatrix[j]-48);
        temp[1][j] = static_cast<int>(state[j]-48);
        for (int i = 0; i < 2; i++){
            if (temp[i][j] > 9) {
                temp[i][j] -= 7;
            }
        }
        desToBiner(temp[0][j], tempbiner[0][j]);
        desToBiner(temp[1][j], tempbiner[1][j]);
    }
    polynom[0][8] = 0;
    polynom[1][8] = 0;
    for (int i = 0; i < 4; i++){
        polynom[0][i] = tempbiner[0][0][i];
        polynom[0][i+4] = tempbiner[0][1][i];
        polynom[1][i] = tempbiner[1][0][i];
        polynom[1][i+4] = tempbiner[1][1][i];
    }
    int temp_i = 0;
    int temp_j = 0;
    int temp_result = 0;
    for (int i = 0; i < 8; i++){
        if (polynom[0][i] == 1){
            temp_i = 7 - i;
            for (int j = 0; j < 8; j++){
                if (polynom[1][j] == 1) {
                    temp_j = 7 - j;
                    temp_result = temp_j + temp_i;
                    polynomresult[8 - temp_result] += 1;
                }
            }
        }
    }
    if (polynomresult[0] == polynommod[0]){
        for (int i = 0; i < 9; i++){
            if (polynomresult[i] > 1) {
                polynomresult[i] = 0;
            } else {
                polynomresult[i] = polynomresult[i] - polynommod[i];
                if (polynomresult[i] < 0) {
                    polynomresult[i] = 1;
                }
            }
        }
    }
    tempakhir[0][0] = polynomresult[1];
    tempakhir[0][1] = polynomresult[2];
    tempakhir[0][2] = polynomresult[3];
    tempakhir[0][3] = polynomresult[4];
    tempakhir[1][0] = polynomresult[5];
    tempakhir[1][1] = polynomresult[6];
    tempakhir[1][2] = polynomresult[7];
    tempakhir[1][3] = polynomresult[8];
    int temp1 = binerToDes(tempakhir[0]);
    int temp2 = binerToDes(tempakhir[1]);
    if (temp1 < 10) {
        resultchar[0] = static_cast<char>(temp1 + 48);
    } else {
        resultchar[0] = static_cast<char>((temp1 - 10) + 65);
    }
    if (temp2 < 10) {
        resultchar[1] = static_cast<char>(temp2 + 48);
    } else {
        resultchar[1] = static_cast<char>((temp2 - 10) + 65);
    }
}

// Function to perform MixColumns Transformation on state
void mixColumns(char state[4][4][2]) {
    char tempresult[2];
    char tempakhir[2];
    char temp[2];

    // MixColumns matrix
    char mixMatrix[4][4][2] = {
        {{'0', '2'}, {'0', '3'}, {'0', '1'}, {'0', '1'}},
        {{'0', '1'}, {'0', '2'}, {'0', '3'}, {'0', '1'}},
        {{'0', '1'}, {'0', '1'}, {'0', '2'}, {'0', '3'}},
        {{'0', '3'}, {'0', '1'}, {'0', '1'}, {'0', '2'}}
    };

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int k = 0; k < 4; k++){
                if (k == 0){
                    perkalianheksa(mixMatrix[row][k], state[k][col], tempakhir);
                } else {
                    perkalianheksa(mixMatrix[row][k], state[k][col], tempresult);
                    for (int i = 0; i < 2; i++){
                        temp[i] = tempakhir[i];
                    }
                    xornextcolumn(temp, tempresult, tempakhir);
                }
            }
            state[row][col][0] = tempakhir[0];
            state[row][col][1] = tempakhir[1];
        }
    }
}

// Function to perform AES encryption
void aesEncrypt(char state[4][4][2], char roundKeys[11][4][4][2], char resultstate[4][4][2]) {
    // Initial Round: AddRoundKey
    addRoundKey(state, roundKeys[0], resultstate);

    // Rounds 1 through 9
    for (int round = 1; round < 10; round++) {
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                for (int k = 0; k < 2; k++){
                    state[i][j][k] = resultstate[i][j][k];
                }
            }
        }
        for (int row = 0; row < 4; row++){
            subBytes(state[row]);
        }        
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, roundKeys[round], resultstate);
    }

    // Final Round (Round 10)
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            for (int k = 0; k < 2; k++){
                state[i][j][k] = resultstate[i][j][k];
            }
        }
    }
    for (int row = 0; row < 4; row++){
        subBytes(state[row]);
    }
    shiftRows(state);
    addRoundKey(state, roundKeys[10], resultstate);
}
