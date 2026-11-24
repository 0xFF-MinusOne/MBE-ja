/*
 *   書式文字列ラボ - 問題 A
 *   gcc -z execstack -z relro -z now -o lab4A lab4A.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BACKUP_DIR "./backups/"
#define LOG_FILE "./backups/.log"

void
log_wrapper(FILE *logf, char *msg, char *filename)
{
    char log_buf[255];
    strcpy(log_buf, msg);
    snprintf(log_buf+strlen(log_buf), 255-strlen(log_buf)-1/*NULL*/, filename);
    log_buf[strcspn(log_buf, "\n")] = '\0';
    fprintf(logf, "LOG: %s\n", log_buf);
}

int
main(int argc, char *argv[])
{
    char ch = EOF;
    char dest_buf[100];
    FILE *source, *logf;
    int target = -1;

    if (argc != 2) {
        printf("使い方: %s ファイル名\n", argv[0]);
    }

    // ログファイルを開く
    logf = fopen(LOG_FILE, "w");
    if (logf == NULL) {
        printf("エラー: %s を開けませんでした\n", LOG_FILE);
        exit(EXIT_FAILURE);
    }

    log_wrapper(logf, "Starting back up: ", argv[1]);

    // コピー元を開く
    source = fopen(argv[1], "r");
    if (source == NULL) {
        printf("エラー: %s を開けませんでした\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // コピー先を開く
    strcpy(dest_buf, BACKUP_DIR);
    strncat(dest_buf, argv[1], 100-strlen(dest_buf)-1/*NULL*/);
    target = open(dest_buf, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
    if (target < 0) {
        printf("エラー: %s%s を開けませんでした\n", BACKUP_DIR, argv[1]);
        exit(EXIT_FAILURE);
    }

    // データをコピーする
    while( ( ch = fgetc(source) ) != EOF)
        write(target, &ch, 1);

    log_wrapper(logf, "Finished back up ", argv[1]);

    // 後始末
    fclose(source);
    close(target);

    return EXIT_SUCCESS;
}
