#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TMPEDIT "/tmp/dhex.hex"
#define TMPEDITSPACE " /tmp/dhex.hex"

#define byte unsigned char
#define bool unsigned char
#define false 0
#define true !false


bool newfile = false;
bool seperateout = false;
const char* editor = NULL;
const char* inputfile = NULL;
const char* outputfile = NULL;
unsigned int hexperline = 20;

byte hex_to_char(char* inpstr){
	byte outbyte;
	char input[3] = {inpstr[0], inpstr[1], '\0'};
	sscanf(input, "%x", &outbyte);
	return outbyte;
}

bool filter_input(char inpchar){
	if(inpchar >= '0' && inpchar <= '9')
		return true;
	if(inpchar >= 'a' && inpchar <= 'f')
		return true;
	if(inpchar >= 'A' && inpchar <= 'F')
		return true;
	return false;
}

void handle_args(int argc, char** argv){
	if(!newfile){
		inputfile = argv[1];
		if(!seperateout)
			outputfile = inputfile;
	}
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

	if(!newfile){
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
	}

	// Create and run command for editing
	char command[128];
	strcpy(command, editor);
	strcat(command, TMPEDITSPACE);
	system(command);

	if(!newfile && !seperateout){
		// Prompt for if we want to save changes
		printf("Save changes made? [y/n]\n");
		char response = getchar();
		if(response != 'y' && response != 'Y')
			return 0;
	}

	// Read the edited file
	FILE* editedFp = fopen(TMPEDIT, "rb");
	byte* editedBuffer = NULL;
	long editedLength;

	if(editedFp){
		fseek(editedFp, 0, SEEK_END);
		editedLength = ftell(editedFp);
		fseek(editedFp, 0, SEEK_SET);
		editedBuffer = malloc(editedLength);
		if (editedBuffer)
			fread(editedBuffer, 1, editedLength, editedFp);
	} else {
		printf("dhex, error opening file. Filepath: %s\n", TMPEDIT);
		return -1;
	}
	fclose(editedFp);

	// Write the original file
	if(!seperateout){
		// Rewrite file
		FILE* doneFp = fopen(inputfile, "wb");
		byte readbyte;
		// Process edited buffer
		for(int n = 0; n < editedLength; n++){
			if(!filter_input(editedBuffer[n]))
				continue;
			readbyte = hex_to_char(&editedBuffer[n]);
			fprintf(doneFp, "%c", readbyte);
			n++; // Since we read two bytes
		}
		fclose(doneFp);
	} else {

	}
	return 1;
}
