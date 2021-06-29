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

char src_path[PATH_MAX] ;
char dst_path[PATH_MAX] ;
char dir_path[PATH_MAX] ; 
char file_name[80];

typedef struct _header{ // 헤더구조체
    char type_indicator;
    double length_pname; 
    char *path_name;
    double length_data;
} ARCH_HEADER, *PARCHIVE_H;

typedef struct _file_data{ // 파일 내용 구조체
    char f_name[256];
    int size;
    int offset;
} FILE_CNT, *PFILE_CNT;

typedef struct _ARCHIVE{ // 아카이브 구조체
    ARCH_HEADER header;
    FILE *fp;
} ARCHIVE, *P_ARCHIVE;

int FileSize(File *fp){ // 파일 크기 구하기
    double size;
    double curr_ptr = ftell(fp);

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    fseek(fp, curr_ptr, SEEK_SET);
    
    return size;
}
void Read_ArchHeader(FILE *pFile, P_ARCHIVE Parchive){ // 아카이브 파일에서 헤더를 읽는 함수
    if(fread(&Parchive->header, sizeof(ARCH_HEADER), 1, pFile) < 1){
        printf("fail to read archive header");
        fclose(pFile);
        exit(1);
    }
}

void Archive(char *filename, P_ARCHIVE archive){

    FILE *fp = fopen(filename, "rb");

    if(fp == NULL){
        printf("no file");
        exit(1);
    }
    double *buffer;
    double add_file_size;
    int size;

    size = FileSize(fp);
    buffer = malloc(size); //추가할 파일의 크기 구해서 버퍼를 만들기 

    if(fread(buffer, size, 1, fp) < 1){  // 추가할 파일 내용읽기
        printf("fail to read the file");
        exit(1);
    }
    FILE_CNT newf_cnt = malloc(sizeof(FILE_CNT)); // 새로운 파일의 내용 만들기
    memset(newf_cnt, 0, sizeof(FILE_CNT));
    strcpy(newf_cnt->f_name, filename); // 파일 내용 구조체에 새로운 파일 이름 저장
    newf_cnt -> size = size; // 파일 내용 구조체에 새로운 파일의 크기 저장

    if(fwrite(buffer, sizeof(FILE_CNT), 1, archive->fp) < 1){ //아카이브 파일에 새로운 파일의 데이터 쓰기
        printf("fail to write the data"); 
        exit(1);
    }
    printf("Success");

    free(newf_cnt);
    fclose(fp);
}

// void List(*archive_header){

// }
// void Extract(){

// }

int main(int argc, char ** argv){
    char *a = "archive";
    char *l = "list";
    char *e = "extract";
    int mode = R_OK;

    P_ARCHIVE archive = malloc(sizeof(ARCHIVE));
    memset(archive, 0, sizeof(ARCHIVE));

    if(argc > 2){
    if(strcmp(argv[1], a) == 0){ // 옵션이 아카이브이면
        if(argc != 4){ // 입력이 4개가 안 들어올 경우
            fputs("Input error\n",stderr); 
		    exit(1);
        }
        if(access( argv[2], mode )  != 0){ // 파일을 읽을 수 없으면 에러
            fputs("can't read",stderr);
            exit(1);
        }
        if(access (argv[2], F_OK) == 0){ // 파일이름이 이미 있으면 이미 존재한다고 표시
            fputs("can't access",stderr);
            exit(1);
        }

        // 에러에 걸리지 않으면 시행
        FILE *fp = fopen(argv[2], "w+b");
        if(fp == NULL) 
            exit(1);
        if(fwrite(&archive->header, sizeof(ARCH_HEADER), 1, fp) < 1){
            printf("fail to write archive heaer");
            fclose(fp);
            exit(1);
        }
        archive->fp = fp;
        Archive(argv[3], archive);
        
        fclose(fp);
        free(archive);
    } 

    if(strcmp(argv[1], l) == 0){
        if(argc != 3){ // 입력값의 개수가 다르면 에러
            fputs("Input error\n",stderr);
		    exit(1);
        }
        if(access( argv[2], mode )  != 0){ // 파일을 읽을 수 없다면 에러
            fputs("can't read",stderr);
            exit(1);
        }
    }
    if(strcmp(argv[1], e) == 0){
        if(argc != 3){
            fputs("Input error\n",stderr); // 입력할 수 없다면 에러
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
