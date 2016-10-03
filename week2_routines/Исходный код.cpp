#define  _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mystring.h"
#pragma once

int size = 0, count = 0;
char ** words = NULL;
char * buff = NULL;


int filesize(FILE * fp){
	int res = 0;
	fseek(fp, 0L, SEEK_END);
	res = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return res;
}


int mysort(const void * a, const void * b){
	//return _stricmp((char*)a, (char*)b);
	return _stricmp(*(char* const*)a, *(char* const*)b);
}

int mysort2(const void * a, const void *b) {
	return 0;
}

int separate(char * buff, char ** words) {
	FILE * fp;
	fp = fopen("romeo.txt", "r");
	if (!fp) return -1;
	int i = 0, cond = 0;
	size = filesize(fp);

	char c = '\0';
	buff = (char*)calloc(size, sizeof(char));
	for (i = 0; i < size; i++) {
		buff[i] = (char)fgetc(fp);
		//printf("%c", buff[i]);
	}
	buff[i] = '\0';			//точто ли это здесь надо писать??
	fclose(fp);

	char * curr;
	count = 1;

	//посчитаем число слов, чтобы создать 2мерный массив для всех слов
	//for (i = 0; i < size; i++) {
	//	if (delimiter(buff[i])) count++;
	//}
	//но оказалось, что так делать нельзя, попытка номер 2
	for (i = 0; i < size; i++) {
		if (!(((buff[i] >= 'a' && buff[i] <= 'z') || (buff[i] >= 'A' && buff[i] <= 'Z')) || (buff[i] == '-' || buff[i] == '\''))) {
			if (cond == 0) {
				cond = 1;
				count++;
			}
		}
		else {
			if (cond == 1) cond = 0;
		}
	}

	words = (char**)calloc(count , sizeof(char*));
	curr = strtok(buff, " .,?!:;()");
	i = 0;
	while (curr){
		words[i] = (char*)calloc(strlen(curr) , sizeof(char));
		words[i] = curr;
		i++;
		curr = strtok(buff, " .,?!:;()");
	}

	//free(buff);	//buff портится в процессе нарезания строки!
	qsort(words, count, sizeof(char *), mysort);

	fp = fopen("romeo_sorted.txt", "w");
	for (i = 0; i < count; i++) {
		fputs(words[i], fp);
		fputs("\n", fp);
		//free(words[i]);
	}
	fclose(fp);
	free(words);
	return 0;
}


int main() {
	////char *s1 = (char*)malloc(16 * sizeof(char));
	////char *s2 = (char*)malloc(16 * sizeof(char));
	//char s1[32] = "Thisi,,...sa.,..,beu,tiful";
	//char s2[16] = "cbabcababc";
	//int l = strlen(s2);
	////s1[0] = s2[0];
	////strncpy(s1, s2, l-2);
	////strncat(s1, s2, 6);
	////char * s3 = strstr(s2, "abab");
	//char * s4 = strtok(s1, ",.");
	//printf("%s\n", s4);
	//s4 = strtok(s1, ",.");
	//printf("%s\n", s4);
	//s4 = strtok(s1, ",.");
	//printf("%s\n", s4);
	////printf("%d", strlen(s1));

	separate(buff, words);
	//output("onegin_sorted.txt", words);
	//FILE * fp = fopen("romeo2.txt", "r");
	//char c = (char)fgetc(fp);
	//fclose(fp);
	//printf("%d %c", strcmp("Аф", "Бол"),c);
	return 0;
}