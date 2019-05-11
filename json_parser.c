#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define READ_ONLY_MODE "r"
#define READ_WRITE_MODE "rw"
#define WRITE_ONLY_MODE "w"

#define JSON_OBJECT_SIZE 50 

typedef struct JSON_CONTENT
{
	char *buffer;
	int size;
}JSON_CONTENT;

typedef struct JSON_OBJECT
{
	char key[JSON_OBJECT_SIZE];
	char value[JSON_OBJECT_SIZE];
}JSON_OBJECT;

void skip_whitespace(const char** cursor)
{

}

int get_file_size(char *filename, char *mode)
{
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

int json_parse(JSON_CONTENT json_buff, JSON_OBJECT *json_object) {
	int c_json = 0;
	int c_flag = 0;
	int c_obj_num = 0;
	int key_flag = 1;
	int c_num	= 0;
	
	if (json_object == NULL){
		return 0;
	}
	
	for (; c_json< json_buff.size; c_json++){
		
		if (json_buff.buffer[c_json] == '{' || json_buff.buffer[c_json] == '}'||json_buff.buffer[c_json] == '\r' || json_buff.buffer[c_json] == '\n')
			continue;
		
		if (json_buff.buffer[c_json] == ':'){
			if (key_flag){
				json_object[c_obj_num].key[c_num++] = '\0';
			}
			else{
				json_object[c_obj_num].value[c_num++] = '\0';
			}
			
			key_flag = 0;
			c_num = 0;
			continue;
		}		

		if (json_buff.buffer[c_json] == '\"' ){
			continue;
		}

		if (json_buff.buffer[c_json] == ',' ){
			if (key_flag){
				json_object[c_obj_num].key[c_num++] = '\0';
			}
			else{
				json_object[c_obj_num].value[c_num++] = '\0';
			}
			
			c_obj_num ++;
			c_num = 0;
			key_flag = 1;
			continue;
		}
		// if (c_flag > 0){
		if (1){
			if (key_flag){
				json_object[c_obj_num].key[c_num++] = json_buff.buffer[c_json];
			}
			else{
				json_object[c_obj_num].value[c_num++] = json_buff.buffer[c_json];
			}
		}
			
	}
	return 1;
}

int json_changeValue(JSON_OBJECT *json_object, char *key, char *new_value){
	
	int json_num = 0;
	if (key == NULL || new_value == NULL){
		return 0;
	}
	
	for (; json_num < 10; json_num ++){
		if (strncmp(json_object[json_num].key, key, strlen(key)) == 0){
			strcpy(json_object[json_num].value, new_value);
			break;
		}
	}

	return 1;
}

int j2str(JSON_OBJECT *json_object, JSON_CONTENT *json_buff){
	
	int json_num = 0;
	int json_buff_end = 0;
	if (json_object == NULL){
		return 0;
	}
	
	json_buff->buffer = (char*) malloc(sizeof(char) * (100 + 1) );
	json_buff->buffer[0] = '{';
	json_buff->buffer[1] = '\r';
	json_buff->buffer[2] = '\n';

	json_buff_end = 3;
	for (; json_num < 3; json_num ++){
		
		strcpy(json_buff->buffer+(json_buff_end++), "\"");
		strcpy(json_buff->buffer+json_buff_end, json_object[json_num].key);
		json_buff_end += strlen(json_object[json_num].key);
		strcpy(json_buff->buffer+(json_buff_end++), "\"");
		
		strcpy(json_buff->buffer+(json_buff_end++), ":");
		
		strcpy(json_buff->buffer+(json_buff_end++), "\"");
		strcpy(json_buff->buffer+json_buff_end, json_object[json_num].value);
		json_buff_end += strlen(json_object[json_num].value);
		strcpy(json_buff->buffer+(json_buff_end++), "\"");
		
		strcpy(json_buff->buffer+(json_buff_end++), ",");
		json_buff->buffer[json_buff_end++]= '\r';
		json_buff->buffer[json_buff_end++] = '\n';
		
	}
	json_buff->buffer[json_buff_end++] = '}';
	json_buff->buffer[json_buff_end++] = '\0';
	
	json_buff->size = json_buff_end;
	
	printf ("\n%s\n", json_buff->buffer);
	return 1;
}

void main () {
	JSON_CONTENT json_buff;
	JSON_CONTENT json_buff_temp;
	JSON_OBJECT json_object[50];
	
	json_buff = readFile("json.txt", READ_ONLY_MODE);
	
	json_parse(json_buff, json_object);
	json_changeValue(json_object, "tuoi", "30");

	j2str(json_object, &json_buff_temp);
	writeFile(json_buff_temp, "json_wr.txt", WRITE_ONLY_MODE);
	
}