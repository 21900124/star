#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define PATH_MAX 1024

char dir_path[PATH_MAX] ; 
char file_name[80];

int main(int argc, char ** argv){
    char *a = "archive";
    char *l = "list";
    char *e = "extract";
    int mode = R_OK;

    if(argc > 2){
    if(strcmp(argv[1], a) == 0){ // 옵션이 아카이브이면
        if(argc != 4){ // 입력이 4개가 안 들어올 경우
            fputs("Input error\n",stderr); 
		    exit(1);
        }
        if(access( argv[2], mode )  != 0){
            fputs("can't access",stderr);
            exit(1);
        }
        if(access (argv[2], F_OK) == 0){ // 파일이름이 이미 있으면 이미 존재한다고 표시
            fputs("can't access",stderr);
            exit(1);
        }
    } 
    if(strcmp(argv[1], l) == 0){
        if(argc != 3 || access( argv[2], mode )  != 0){
            fputs("Input error\n",stderr);
		    exit(1);
        }
        if(access( argv[2], mode )  != 0){
            fputs("can't access",stderr);
            exit(1);
        }
    }
    if(strcmp(argv[1], e) == 0){
        if(argc != 3){
            fputs("Input error\n",stderr);
		    exit(1);
        }
        if(access( argv[2], mode )  != 0 || access(argv[3], mode) != 0){ // 파일과 디렉토리에 접근할 수 없으면 에러
            fputs("can't access",stderr);
            exit(1);
        }
        printf("%s",argv[1]);
        strcpy(dir_path, argv[3]) ; // 디렉토리 경로 복사
    }
    
    strcpy(file_name, argv[2]) ; // 파일이름 복사
    }
    
    fputs("Input error\n",stderr); // 입력이 2개도 안되는 경우 에러
	exit(1);

    return 0;
}