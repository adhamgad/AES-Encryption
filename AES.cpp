#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iomanip>

typedef short byte;
typedef unsigned int word;

using namespace std;

static const short S_BOX[] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6f, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const short mult_2[256] = {
    0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
    0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
    0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
    0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,
    0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
    0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
    0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
    0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,
    0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05,
    0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25,
    0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45,
    0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65,
    0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85,
    0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5,
    0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5,
    0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5};

const short mult_3[256] = {
    0x00,0x03,0x06,0x05,0x0c,0x0f,0x0a,0x09,0x18,0x1b,0x1e,0x1d,0x14,0x17,0x12,0x11,
    0x30,0x33,0x36,0x35,0x3c,0x3f,0x3a,0x39,0x28,0x2b,0x2e,0x2d,0x24,0x27,0x22,0x21,
    0x60,0x63,0x66,0x65,0x6c,0x6f,0x6a,0x69,0x78,0x7b,0x7e,0x7d,0x74,0x77,0x72,0x71,
    0x50,0x53,0x56,0x55,0x5c,0x5f,0x5a,0x59,0x48,0x4b,0x4e,0x4d,0x44,0x47,0x42,0x41,
    0xc0,0xc3,0xc6,0xc5,0xcc,0xcf,0xca,0xc9,0xd8,0xdb,0xde,0xdd,0xd4,0xd7,0xd2,0xd1,
    0xf0,0xf3,0xf6,0xf5,0xfc,0xff,0xfa,0xf9,0xe8,0xeb,0xee,0xed,0xe4,0xe7,0xe2,0xe1,
    0xa0,0xa3,0xa6,0xa5,0xac,0xaf,0xaa,0xa9,0xb8,0xbb,0xbe,0xbd,0xb4,0xb7,0xb2,0xb1,
    0x90,0x93,0x96,0x95,0x9c,0x9f,0x9a,0x99,0x88,0x8b,0x8e,0x8d,0x84,0x87,0x82,0x81,
    0x9b,0x98,0x9d,0x9e,0x97,0x94,0x91,0x92,0x83,0x80,0x85,0x86,0x8f,0x8c,0x89,0x8a,
    0xab,0xa8,0xad,0xae,0xa7,0xa4,0xa1,0xa2,0xb3,0xb0,0xb5,0xb6,0xbf,0xbc,0xb9,0xba,
    0xfb,0xf8,0xfd,0xfe,0xf7,0xf4,0xf1,0xf2,0xe3,0xe0,0xe5,0xe6,0xef,0xec,0xe9,0xea,
    0xcb,0xc8,0xcd,0xce,0xc7,0xc4,0xc1,0xc2,0xd3,0xd0,0xd5,0xd6,0xdf,0xdc,0xd9,0xda,
    0x5b,0x58,0x5d,0x5e,0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4f,0x4c,0x49,0x4a,
    0x6b,0x68,0x6d,0x6e,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7f,0x7c,0x79,0x7a,
    0x3b,0x38,0x3d,0x3e,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2f,0x2c,0x29,0x2a,
    0x0b,0x08,0x0d,0x0e,0x07,0x04,0x01,0x02,0x13,0x10,0x15,0x16,0x1f,0x1c,0x19,0x1a};


static const short Rcon[11] = {0x00,0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};


void parseInput(byte plainText[16] , byte key[16]){
    string p , k ;
    //take inputs
    cout << "Plain Text:";
    cin >> p ;
    cout << "Key:";
    cin >> k;
    //parse input from string to array
    int current = 0 ;
    for(unsigned int i = 0 ; i < 32 ; i = i+2){
    	string temp = p.substr(i, 2);
    	stringstream ss ;
    	ss << hex << temp ;
    	ss >> plainText[current] ;
    	ss.clear();
    	temp = k.substr(i,2);
    	ss << hex << temp;
    	ss >> key[current];
        current ++;
    }
}


void rotWord(word w[4]){
	byte temp = w[0];
	w[0] = w[1];
	w[1] = w[2];
	w[2] = w[3];
	w[3] = temp;
}

void byteSubstitution(word w[4]){
	for(int i = 0 ; i < 4 ; i++){
		byte a = w[i];
		w[i] = S_BOX[a];
	}
}

word *exor(word w1[4] , word w2[4]){
	word result[4];
	for(int i = 0 ; i < 4 ; i++){
		result[i] = w1[i] ^ w2[2];
	}
	return result;
}

void keyExpansion(byte key[16] , word w[44][4]){

	int counter = 0 ;
	for(int i = 0 ; i< 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			w[i][j] = key[counter];
			counter ++;
		}
	}
	//cout <<"Word 3 "<< hex << w[3][0] << endl ;
	for(int i = 4 ; i < 44 ; i++){

		word temp[4];
		for(int k = 0 ;k<4 ; k++ ){
			temp[k] = w[i-1][k];
		}

		if(i % 4 == 0){
			rotWord(temp);
			byteSubstitution(temp);

			temp[0] = temp[0] ^ Rcon[i/4];
		}

		for(int j = 0 ; j<4 ; j++){
			w[i][j] = w[i-4][j] ^ temp[j];

		}
	}

}

void firstRoundKey(byte plainText[16],byte key[16]){
	for(int i = 0 ; i < 16 ; i ++){
		plainText[i] = plainText[i] ^ key[i] ;
	}
}

void subBytes(byte text[16]){
	for(int i = 0 ; i < 16 ; i++){
		byte a = text[i];
		text[i] = S_BOX[a];
	}
}

void shiftRows(byte text[16]){
	int counter = 0;
	for(int i = 0 ; i < 4 ; i ++ ){
		if(i != 0 ){
			counter ++;
		}

		if(counter == 1){
			byte temp = text[i];
			text[i] = text[i+4];
			text[i+4] = text[i+8];
			text[i+8] = text[i+12];
			text[i+12] = temp;
		}
		if(counter == 2){
			byte temp1 = text[i];
			byte temp2 = text[i+4];
			text[i] = text[i+8];
			text[i+4] = text[i+12];
			text[i+8] = temp1;
			text[i+12] = temp2;
		}
		if(counter == 3){
			byte temp1 = text[i];
			byte temp2 = text[i+4];
			byte temp3 = text[i+8];

			text[i] = text[i+12];
			text[i+4] = temp1;
			text[i+8] = temp2;
			text[i+12] = temp3;

		}
	}
}

void copyArr(byte arr1[16] , byte arr2[16]){
	for(int i = 0 ; i < 16 ; i++){
		arr1[i] = arr2[i];
	}
}

void mixColumns(byte text[16]){
	byte mul[16] = {2,3,1,1,1,2,3,1,1,1,2,3,3,1,1,2};
	byte newText[16];
	byte output[16];


	int count = 0;
	for(int i = 0 ; i < 16 ; i = i+4){
		copyArr(newText,text);
		for(int j = 0 ; j < 16 ; j = j+4){
			for(int k = i ; k < i+4 ; k++){
				if(mul[k] == 2)
					newText[j+(k%4)] = mult_2[text[j+(k%4)]];
				if(mul[k] == 3)
					newText[j+(k%4)] = mult_3[text[j+(k%4)]];
			}
		}

		for(int k = 0 ; k < 16 ; k = k+4){
			output[count + k] = newText[k] ^ newText[k+1] ^ newText[k+2] ^ newText[k+3];
		}

		count ++;
	}
	copyArr(text,output);

}

void roundKey(byte text[16] ,byte rKey[16]){
	for(int i = 0 ; i < 16 ; i ++){
		text[i] = text[i] ^ rKey[i] ;
	}
}

int main()
{

    byte plainText[16];
    byte key[16];
    word w[44][4];
    parseInput(plainText,key);
    keyExpansion(key,w);
    firstRoundKey(plainText,key);

	for(int i = 0 ; i < 16 ; i++){
	    	cout << hex << setw(2) << setfill('0') << plainText[i];
	    }
	    cout << endl;

    for(int currRound = 1 ; currRound <= 10 ; currRound ++){

    	    subBytes(plainText);

    	    for(int i = 0 ; i < 16 ; i++){
    	    	cout << hex << setw(2) << setfill('0') << plainText[i];
    	    }

    	    cout << endl;

    	    shiftRows(plainText);


    	    for(int i = 0 ; i < 16 ; i++){
    	    	cout << hex << setw(2) << setfill('0') << plainText[i];
    	    }
    	    cout << endl;

    	    if(currRound==10){
    	    	//do nothing
    	    }else{
        	    mixColumns(plainText);

        	    for(int i = 0 ; i < 16 ; i++){
        	    	cout << hex << setw(2) << setfill('0') << plainText[i];
        	    }
        	    cout << endl;
    	    }



    	    byte temp[16];
    	    int count = 0;
    	    int t = currRound * 4;
    	    for(int i = t ; i < (t+4) ; i++){
    	    	for(int j = 0 ; j < 4 ; j++){
    	    		temp[count] = w[i][j];
    	    		count++;
    	    	}
    	    }

    	    roundKey(plainText,temp);

    	    for(int i = 0 ; i < 16 ; i++){
    	       	cout << hex << setw(2) << setfill('0') << plainText[i];
    	     }
    	    cout << endl;
    }


    return 0;
}
