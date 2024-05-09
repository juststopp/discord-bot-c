#include <string.h>
#include <stdlib.h>

char *str_replace(char *orig, const char *old, const char *new) {
    char *result, *ins, *tmp;
    int len_old, len_new, len_repl, count;

    if (!orig || !old)
        return NULL;

    len_old = strlen(old);
    len_new = strlen(new);

    if (len_old == 0)
        return NULL;

    count = 0;
    for (ins = (char *)orig; (tmp = strstr(ins, old)) != NULL; ++count) {
        ins = tmp + len_old;
    }

    result = (char *)malloc(strlen(orig) + (len_new - len_old) * count + 1);

    if (!result)
        return NULL;

    tmp = result;
    while (count--) {
        ins = strstr(orig, old);
        len_repl = ins - orig;
        strncpy(tmp, orig, len_repl);
        tmp += len_repl;
        strcpy(tmp, new);
        tmp += len_new;
        orig += len_repl + len_old;
    }
    strcpy(tmp, orig);
    return result;
}