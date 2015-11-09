#include "bor-util.h"

typedef struct ResponseList ResponseList;

struct ResponseList{
	char* name;
	char* response;
	ResponseList* next;
};

ResponseList* parseResponseFile(char* path) {
	ResponseList* head = NULL;
	
	FILE* fh = fopen(path, "r");
	char* line = NULL;
	size_t len;

	if(fh == NULL){ perror("Fopen Error"); exit(1); }

	ResponseList* current = NULL;
	while(getline(&line, &len, fh) != -1){
		ResponseList* next = malloc(sizeof(ResponseList));
		next->name = substr(line, ' ');
		next->response = suffix(line, ':');
		next->next = NULL;	

		if(current == NULL) { current = next; head = next;
			} else { current->next = next; current = next; }
	}

	return head;
}

char* substr(char* str, char chr) {
	char* ptr = strchr(str, chr);
	if(ptr == NULL) return NULL;

	int index = ptr - str;
	char* sub = malloc(sizeof(char) * index + 1);
	strncpy(sub, str, index);
	return sub;
}

char* suffix(char* str, char chr) {
	char* ptr = strchr(str, chr);
	if(ptr == NULL) return NULL;

	int index = ptr - str;
	int size = strlen(str) - index;
	char* suf = malloc(sizeof(char) * size + 1);
	strcpy(suf, ptr);
	return suf;
}

char* findResponse(ResponseList* responses, char* name){
	char* response = NULL;
	while(responses != NULL)
		if(strcmp(name, responses->name) == 0) { response = responses->response; break; }
	return response;
}

int main(int argc, char** argv){
	if(argc < 3) { puts("Bad arguments"); exit(1); }
	ResponseList* responses = parseResponseFile(argv[2]);
}
