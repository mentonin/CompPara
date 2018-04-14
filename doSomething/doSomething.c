#include <stdio.h>

void doSomething(char* string);

int main()
{
    char* name;
    name = "<== first time through";
    doSomething(name);
    name = "<== second time through";
    doSomething(name);
    name = "<== third time through";
    doSomething(name);
    return 0;
}
void doSomething(char* string)
{
    static char separator = ' ';
    printf("%c %s \n", separator, string);
    separator = ',';
}