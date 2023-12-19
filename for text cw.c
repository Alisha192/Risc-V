#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void print(char** txt, int n){
	for(int i =0; i<n; i++){
		printf("%s\n", txt[i]);
		printf("***************************\n");
	}
} 
char* ltrim(char* s)
{
    while (isspace(*s)) s++;
    return s;
}

char* rtrim(char* s)
{
    char* back = s + strlen(s);
    while (isspace(*--back));
    *(back + 1) = '\0';
    return s;
}

char* trim(char* s)
{
    return rtrim(ltrim(s));
}
char** maketxt(int* N2) {
    int size = 5;
    int sizein = 5;
    char** text =(char**) malloc(size * sizeof(char*));
    int N = 0;
    int Nin = 0;
    int k = 1;
    char ch;
    while (k == 1) {
        text[N] = (char*)malloc(sizein * sizeof(char));
        if(text[N] == NULL){
        	printf("Нехватка памяти\n");
        	k = -1;
        	break;
        }
        text[N][Nin] = getchar();
        if (text[N][Nin] == '\n')
            Nin--;
        
        while (text[N][Nin] != '.') {
            Nin++;
            ch = getchar();
            if (ch == '\n')
                ch = ' ';
            text[N][Nin] = ch;

            if (sizein == Nin + 1) {
                sizein *= 2;
                text[N] =(char*) realloc(text[N], sizein * sizeof(char));
                if(text[N] == NULL){
                	printf("Нехватка памяти\n");
                	k = -1;
                	break;
                }
            }
            text[N][Nin + 1] = '\0';
            if (!strcmp(text[N], "end")) {
                k = 0;
                break;
            }
        }
        text[N] = trim(text[N]);
        N++;
        if (size == N + 1) {
            size *= 2;
            text = (char**)realloc(text, size * sizeof(char*));
            if(text == NULL){
            	printf("Нехватка памяти\n");
            	k = -1;
            	break;
            }
        }
        Nin = 0;
    }

    for (int i = 0; i < N-1; i++)
    {
        if (strlen(text[i]) <= 2 && (text[i][0] == '\n' || text[i][0] == '.')) {
            text[i] = text[i + 1];
            N--;
        }
        else if (text[i][strlen(text[i]) - 1] == '.')
        {
            text[i][strlen(text[i]) - 1] = '\0';
         if (text[i][0] == '\n')
            text[i] = text[i]+1;
        }
    }

    *N2 = N-1;
    return text;
}

void alphabet(char** txt, int n) {
    char alp [] = "aeiouy";
    int k = 0;

    while (k < n) {

        for (int i = 0; i < strlen(txt[k]); i++)
        {
            for (int j = 0; j < strlen(alp); j++)
                if (tolower(txt[k][i]) == alp[j])
                {
                    txt[k][i] = txt[k][i] + 1;
                    break;
                }

        }
        k++;
    }
    print(txt, n); 
}
void To_or_not_to(char** txt, int n) {
    char ch[3];
    char* token;
    int size=0;
    char s[2] = " ";
    for (size_t i = 0; i < n; i++)
    {
        //const int size = strlen(txt[i]);
        char A[strlen(txt[i])];
        strcpy(A, txt[i]);
        //token = strtok(A, s);
        char* str = strstr(A, " or not to ");
        if (strcmp(strncpy(ch, A, 2), "To") == 0 && str) 
        {
            str[0] = '\0';
            if(strlen(A+3)>strlen(A+3+strlen(A+3)+11)){
            	printf("%s\n", A+3);
            }
            if(strlen(A+3)<strlen(A+3+strlen(A+3)+11)){
            	printf("%s\n", A+3+strlen(A+3)+11);
            }
        }
       
    }

}
int delete_sentence(char** txt, int* n) {
    int k = 0;
    while (k < *n) {
        
        for (int i = k+1; i < *n; i++) {
            if (strcmp(txt[k], txt[i]) == 0) {
                for (int j = i; j < (*n)-1; j++) {
                    txt[j] = txt[j + 1];
                }
                (*n)--;
                i--;
            }
        }

        k++;
    }
    return k;

}


int delete_sent4(char** str,int n){
	int k = 0;
	char* token;
 	char s[10] = " .,?;:";
 	while (k < n)
 	{
  		char A[strlen(str[k])];
  		strcpy(A, str[k]);
  		token = strtok(A, s);
  		if (strlen(token) == 4)
  		{
   			for (int j = k; j < n - 1; j++)
   			{
    				str[j] = str[j + 1];
   			}

   			k--;
   			n--;
  		}

  	k++;
 	}

	print(str, k);
return k;
}

int cmpfunc(const void* a, const void* b){
	char* a1 = *(char**)a;
	char* b1 = *(char**)b;
	return(a1[strlen(a1)-1] - b1[strlen(b1)-1]);
}	
int main(){
	char** txt;
	int n = 0;
	printf("Введите текст\n");
	printf("Для окончания ввода введите команду 'end' \n");
	int k;
	txt = maketxt(&n);
	while(n == 0){
		printf("Введен пустой текст\n");
		printf("Вы хотите выйти из программы?\n");
		printf("Введите 1 если НЕТ\n");
		printf("Введите 2 если ДА\n");
		scanf("%d", &k);
		if(k == 1){
			printf("Введите текст\n");
			txt = maketxt(&n);
		}
		if(k == 2){
			printf("До новых встреч!\n");
			return 0;
		}	
	}
	n = delete_sentence(txt, &n);
	int value = 0;
	printf("Введите пожалуйста номер желаемой функции\n");
	printf("1)Меняет все гласные буквы в тексте на следующую букву алфавита\n");
	printf("2)Находит в тексте все предложения вида To ... ot not to ...  и выводит на экран длину большей подстроки для каждого предложения такого типа\n");
	printf("3)Находит и удаляет все предложения длина первого слова которых равна 4\n");
	printf("4)Сортирует предложения по увелечению кода последнего символа, не являющегося разделителем\n");
	printf("Для выхода из программы введите 5\n");
	while(value != 5){
		scanf("%d", &value);
		if(value < 1 || value > 5){
			printf("Введите корректный номер функции\n");
			
		}
		switch(value){
			case 1:
			{
				alphabet(txt, n);
				break;
			}
			case 2:
			{
				To_or_not_to(txt, n);
				break;
			}
			case 3:
			{
				n = delete_sent4(txt, n);
				break;
			}
			case 4:
			{
				qsort(txt, n, sizeof(char*), cmpfunc);
				print(txt, n);
				break;
			}
		}
	}
	printf("До новых встреч!\n");
	free(txt);
	return 0;
}
