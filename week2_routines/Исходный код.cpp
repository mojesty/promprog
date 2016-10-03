#define  _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size = 0, count = 0;
char ** words = NULL;
char * buff = NULL;

int mystrlen(const char * str) {
	int len = 0;
	int c = str[0];
	while (c != '\0') {
		len++;
		c = str[len];
	}
	return len;
}

char * mystrncpy(char * destination, const char * source, int num) {
	int i = 0, fin = -1;
	for (i = 0; i < num; i++) {
		if (source[i] == '\0') fin = i;
		i>fin ? destination[i] = source[i] : destination[i] = '\0';
	}
	return destination;
}

char * mystrncat(char * destination, const char * source, int num){
	int i = 0, len = strlen(destination);
	for (i = 0; i < num; i++) {
		destination[len + i] = source[i];
		if (source[i] == '\0') break;
	}
	destination[len + i] = '\0';
	return destination;
}

void prefix(const char* str, int * prefix){
	int i = 0, k = 0;
	for (i = 1; i < strlen(str); i++) {
		k = prefix[i - 1];
		while (k>0 && str[k] != str[i]) {
			k = prefix[k - 1];
		}
		if (str[k] == str[i])	k++;
		prefix[i] = k;
	}
	return;
}

char * strstr(char * str1, char * str2){
	int index = -1, k = 0, i = 0;
	int * f = (int*)calloc(strlen(str1),sizeof(char));
	prefix(str1, f);
	for (i = 0; i < strlen(str1); i++) {
		while (k > 0 && str1[k] != str2[i]) 
			k = f[k - 1];
		if (str1[k] == str2[i]) k++;
		if (k == strlen(str1)) {
			index = i - strlen(str1) + 1;
			break;
		}
	}
	free(f);
	if (index >= 0) return &(str2[index]);
	return NULL;
}

char * strtok(char * str, const char * delimiters){
	static int pos = 0;
	static int length = strlen(str);
	int i = 0, j = 0, curr = 0, start = 0, found = 0;	//флаг start поднимается, когда мы достигли начала токена, found служит для того, чтобы начало токена отследить
	for (i = pos; i < length; i++) {
		found = i;

		for (j = 0; j < strlen(delimiters); j++) {
			if (str[i] == delimiters[j]) {
				found++;

				if (start){
					str[i] = '\0';
					curr = pos;
					pos = i+1;
					return &(str[curr]);
				}

			}
		}

		if (found <= i) {
			start = 1;
		}
		if (!start) pos++;
	}
	return NULL;
}

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