/*
 * AES_main.c
 *
 * Created on: 02/09/2021
 *     Author: Anyka
 *      	   Phoebe Luo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------------------------------------
--------------------------------------- Function Declaration -------------------------------------
--------------------------------------------------------------------------------------------------*/
int main();
void A5_1(int text[64], int key[64]);
void FeedbackPoly(int X[19], int Y[22], int Z[23]);
void ShiftLeftX(int X[19]);
void ShiftLeftY(int Y[22]);
void ShiftLeftZ(int Z[23]);
int GetKey(int X[19], int Y[22], int Z[23]);
void GetArray(int text[64], int choice);


/*------------------------------------------------------------------------------------------------
--------------------------------------- Function Definition --------------------------------------
--------------------------------------------------------------------------------------------------*/

int main(){
	// initialize arrays
	int text[64] = {0}; //initialize array to hold text
	int key[64] = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
					1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
					1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 }; //initialize array to hold key

	A5_1(text, key);
	return 0;
}

void A5_1(int text[64], int key[64]){
	// get user input to identify operation
	int choice = 0;
	int count;
	printf("Choose designated function:\n1. Encryption\n2. Decryption\n");
	fflush(stdout);
	scanf("%d", &choice);

	GetArray(text, choice); // get user input for text

	// print input text
	if(choice == 1){
		printf("Plaintext: \n");
	}else if(choice == 2){
		printf("Ciphertext: \n");
	}
	for(count = 0; count < 64; count++){
		printf("%d ", text[count]);
	}
	printf("\n\n");

	// 1. key
	int X[19] = {0}; // register X with 19 digits
	int Y[22] = {0}; // register Y with 22 digits
	int Z[23] = {0}; // register Z with 23 digits

	// initialize registers with key
	for(count = 0; count < 19; count++){
		X[count] = key[count]; // fill in X
	}
	for(count = 0; count < 22; count++){
		Y[count] = key[count + 19]; // fill in Y
	}
	for(count = 0; count < 23; count++){
		Z[count] = key[count + 41]; // fill in Z
	}

	// start round operation to get streamkey
	int round;
	int newkey[64] = {0}; // newkey array for holding the key bit of each round
	for(round = 0; round < 64; round++){
		FeedbackPoly(X, Y, Z);
		newkey[round] = GetKey(X, Y, Z);
	}

	printf("Final Key:\n");
	for(count = 0; count < 64; count++){
		printf("%d ", newkey[count]);
	}
	printf("\n\n");

	// 1. text
	for(count = 0; count < 64; count++){
		text[count] = text[count] ^ newkey[count];
	}

	// print output text
	if(choice == 1){
		printf("Ciphertext: \n");
	}else if(choice == 2){
		printf("Plaintext: \n");
	}
	for(count = 0; count < 64; count++){
		printf("%d ", text[count]);
	}
	printf("\n\n");
}

void FeedbackPoly(int X[19], int Y[22], int Z[23]){
	// 1.  X8, Y10, Z10 to select left shift register
	int count0 = 0; // keep track of the number of 0's
	if(X[8] == 0){
		count0++;
	}
	if(Y[10] == 0){
		count0++;
	}
	if(Z[10] == 0){
		count0++;
	}

	// 2. Calculate the new X[0], Y[0], or Z[0]
	int newX, newY, newZ;
	if(count0 >= 2){ // 0's more than 1's
		if(X[8] == 0){
			newX = X[13] ^ X[16] ^ X[17] ^ X[18];
			ShiftLeftX(X);
			X[0] = newX;
		}
		if(Y[10] == 0){
			newY = Y[20] ^ Y[21];
			ShiftLeftY(Y);
			Y[0] = newY;
		}
		if(Z[10] == 0){
			newZ = Z[7] ^ Z[20] ^ Z[21] ^ Z[22];
			ShiftLeftZ(Z);
			Z[0] = newZ;
		}
	}else{ // 1's more than 0's
		if(X[8] == 1){
			newX = X[13] ^ X[16] ^ X[17] ^ X[18];
			ShiftLeftX(X);
			X[0] = newX;
		}
		if(Y[10] == 1){
			newY = Y[20] ^ Y[21];
			ShiftLeftY(Y);
			Y[0] = newY;
		}
		if(Z[10] == 1){
			newZ = Z[7] ^ Z[20] ^ Z[21] ^ Z[22];
			ShiftLeftZ(Z);
			Z[0] = newZ;
		}
	}

/*	int count;
	printf("X:\n");
	for(count = 0; count < 19; count++){
		printf("%d ", X[count]);
	}
	printf("\n\n");
	printf("Y:\n");
	for(count = 0; count < 22; count++){
		printf("%d ", Y[count]);
	}
	printf("\n\n");
	printf("Z:\n");
	for(count = 0; count < 23; count++){
		printf("%d ", Z[count]);
	}
	printf("\n\n");*/
}

void ShiftLeftX(int X[19]){
	int count;
	for(count = 18; count > 0; count--){
		X[count] = X[count - 1];
	}
}

void ShiftLeftY(int Y[22]){
	int count;
	for(count = 21; count > 0; count--){
		Y[count] = Y[count - 1];
	}
}

void ShiftLeftZ(int Z[23]){
	int count;
	for(count = 22; count > 0; count--){
		Z[count] = Z[count - 1];
	}
}

int GetKey(int X[19], int Y[22], int Z[23]){
	int key;
	key = X[18] ^ Y[21] ^ Z[22];
	return key;
}

void GetArray(int text[64], int choice){
	// get user input for plaintext OR ciphertext
	unsigned int temptext[16] = {0};
	if (choice == 1){
		printf("Input plaintext:\n");
	}else if (choice == 2){
		printf("Input ciphertext:\n");
	}

	int count;
	for(count = 0; count < 16; count++){
		fflush(stdout);
		scanf("%x", text+count);
	}
}
