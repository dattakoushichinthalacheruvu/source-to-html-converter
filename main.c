#include <stdio.h>
#include "html.h"

int main()
{
    char inputFile[100];
    char outputFile[100];

    printf("Enter source file name: ");
    scanf("%s", inputFile);

    printf("Enter output HTML file name: ");
    scanf("%s", outputFile);

    convertToHTML(inputFile, outputFile);

    return 0;
}