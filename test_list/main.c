#include <stdio.h>
#include <stdlib.h>
#include "list.h"
 
typedef struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
} birthday_t;
 
typedef struct birthday *birthday_ptr;
 
int main(void) {
    LIST_HEAD(birthday_list);
    
    birthday_t *b;
    int i;
    for(i=0; i<5; i++) {
        b = malloc(sizeof(*b));
        b->day = i;
        b->month = i;
        b->year = i;
        list_add_tail(&b->list, &birthday_list);
    }
 
    birthday_ptr ptr;
    list_for_each_entry(ptr, &birthday_list, list) {
        printf("day: %d, month: %d, year: %d\n", ptr->day, ptr->month, ptr->year);
    }
 
    birthday_ptr next;
    list_for_each_entry_safe(ptr, next, &birthday_list, list) {
        printf("removing day: %d, month: %d, year: %d\n", ptr->day, ptr->month, ptr->year);
        list_del(&ptr->list);
        free(ptr);
    }
 
    return 0;
}
