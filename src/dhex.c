#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TMPEDIT "/tmp/dhex.hex"
#define TMPEDITSPACE " /tmp/dhex.hex"

#define byte unsigned char
#define bool unsigned char
#define false 0
#define true !false

const char* editor = NULL;
const char* inputfile = NULL;
const char* outputfile = NULL;
unsigned int hexperline = 20;

void handle_args(int argc, char** argv){
	inputfile = argv[1];
}

int main(int argc, char** argv){
	// Handle args
	if(argc < 2){
		printf("dhex, no input file. Usage: dhex <file>");
		return -1;
	} else {
		handle_args(argc, argv);
	}

	// Configure editor
	if(editor == NULL)
		editor = getenv("EDITOR");

	// Load inputfile into buffer
	FILE* inputFp = fopen(inputfile, "rb");
	byte* inputBuffer = NULL;
	long inputLength;

	if(inputFp){
		fseek(inputFp, 0, SEEK_END);
		inputLength = ftell(inputFp);
		fseek(inputFp, 0, SEEK_SET);
		inputBuffer = malloc(inputLength);
		if (inputBuffer)
			fread(inputBuffer, 1, inputLength, inputFp);
	} else {
		printf("dhex, error opening file. Filepath: %s\n", inputfile);
		return -1;
	}
	fclose(inputFp);

	// Create editing file
	FILE* editFp = fopen(TMPEDIT, "wb");
	// Write hex to it
	fprintf(editFp, "%02x ", inputBuffer[0]); // To stop indentation after first one
	for(int n = 1; n < inputLength; n++){
		fprintf(editFp, "%02x", inputBuffer[n]);
		if((n + 1) % hexperline == 0)
			fprintf(editFp, "\n");
		else if(n < inputLength - 1)
			fprintf(editFp, " ");
	}
	fclose(editFp);

	// Create and run command for editing
	char command[128];
	strcpy(command, editor);
	strcat(command, TMPEDITSPACE);
	system(command);
}
