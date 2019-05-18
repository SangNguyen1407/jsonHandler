#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../inc/json_inc.h"


int get_file_size(char *filename, char *mode){
	int string_size;
	FILE *handler = fopen(filename, mode);
	// Seek the last byte of the file
	fseek(handler, 0, SEEK_END);
	// Offset from the first to the last byte, or in other words, filesize
	string_size = ftell(handler);
	// go back to the start of the file
	rewind(handler);
	
	return string_size;
}

int isStringValue (char *stringValue){
	
	int isCheck = 0;
	int pos = 0;
	
	if (stringValue == NULL){
		return 0;
	}
	
	for (; pos < strlen(stringValue); pos++){
		if( (stringValue[pos]>='a' && stringValue[pos]<='z') 
			|| (stringValue[pos]>='A' && stringValue[pos]<='Z')){
			isCheck++;
		}
	}
	
	return ((isCheck == 0)? 0:1);
}


JSON_CONTENT readFile (char *filename, char *mode)
{
	JSON_CONTENT json_buff;
	int read_size;
	FILE *handler = fopen(filename, mode);

	if (handler)
	{
		// Seek the last byte of the file
		fseek(handler, 0, SEEK_END);
		// Offset from the first to the last byte, or in other words, filesize
		json_buff.size = ftell(handler);
		// go back to the start of the file
		rewind(handler);
		
		// Allocate a string that can hold it all
		json_buff.buffer = (char*) malloc(sizeof(char) * (json_buff.size + 1) );
		// Read it all in one operation
		read_size = fread(json_buff.buffer, sizeof(char), json_buff.size, handler);
		// fread doesn't set it so put a \0 in the last position
		// and buffer is now officially a string
		json_buff.buffer[json_buff.size] = '\0';
	}
	fclose(handler);
	printf ("\n%s\n", json_buff.buffer);
	return json_buff;
}

void writeFile (JSON_CONTENT json_buff, char *filename, char *mode) {
	int write_size;
	FILE *handler = fopen(filename, mode);
	
	if (handler) {
		write_size = fwrite(json_buff.buffer, sizeof(char), json_buff.size, handler);
	}
	
	fclose(handler);	
}
