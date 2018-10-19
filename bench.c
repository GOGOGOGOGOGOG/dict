#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bloom.h"
#include "bench.h"

#define DICT_FILE "cities.txt"
#define WORDMAX 256

double tvgetf()
{
    struct timespec ts;
    double sec;

    clock_gettime(CLOCK_REALTIME, &ts);
    sec = ts.tv_nsec;
    sec /= 1e9;
    sec += ts.tv_sec;

    return sec;
}

int bench_test(const tst_node *root, char *out_file, const int max)
{
    char prefix[4] = "";
    char word[WORDMAX] = "";
    char **sgl;
    FILE *fp = fopen(out_file, "w");
    FILE *dict = fopen(DICT_FILE, "r");
    int idx = 0, sidx = 0;
    double t1, t2;

    if (!fp || !dict) {
        if (fp) {
            fprintf(stderr, "error: file open failed in '%s'.\n", DICT_FILE);
            fclose(fp);
        }
        if (dict) {
            fprintf(stderr, "error: file open failed in '%s'.\n", out_file);
            fclose(dict);
        }
        return 1;
    }

    sgl = (char **) malloc(sizeof(char *) * max);
    while (fscanf(dict, "%s", word) != EOF) {
        if (strlen(word) < 4)
            continue;
        strncpy(prefix, word, 3);
        t1 = tvgetf();
        tst_search_prefix(root, prefix, sgl, &sidx, max);
        t2 = tvgetf();
        fprintf(fp, "%d %f sec\n", idx, (t2 - t1) * 1000000);
        idx++;
    }

    fclose(fp);
    fclose(dict);
    return 0;
}
int bench_test_bloom(const tst_node *root, char *out_file, bloom_t filter)
{
    char word[WORDMAX] = "";
    int idx = 0;
    double t1, t2;
    FILE *fp = fopen(out_file, "w");
    FILE *dict = fopen(DICT_FILE, "r");
    tst_node *res;

    if (!fp || !dict) {
        if (fp) {
            fprintf(stderr, "error: file open failed in '%s'.\n", DICT_FILE);
            fclose(fp);
        }
        if (dict) {
            fprintf(stderr, "error: file open failed in '%s'.\n", out_file);
            fclose(dict);
        }
        return 1;
    }

    while (fscanf(dict, "%s", word) != EOF) {
        t1 = tvgetf();
        if (bloom_test(filter, word) == 1) {
            t2 = tvgetf();
            fprintf(fp, "%d %s %f ", idx++, word, (t2 - t1) * 100000);
            t1 = tvgetf();
            res = tst_search(root, word);
            t2 = tvgetf();
            if (res)
                fprintf(fp, "%f\n", (t2 - t1) * 1000000);
            else
                printf("Error.\n");
        } else
            printf("Error, it's impossible can not find string.\n");
    }

    fclose(fp);
    fclose(dict);
    return (0);
}