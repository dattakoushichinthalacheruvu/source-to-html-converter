#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "html.h"

int isKeyword(char *word)
{
    char *keywords[] =
    {
        "auto", "break", "case", "char",
        "const", "continue", "default",
        "do", "double", "else", "enum",
        "extern", "float", "for", "goto",
        "if", "int", "long", "register",
        "return", "short", "signed",
        "sizeof", "static", "struct",
        "switch", "typedef", "union",
        "unsigned", "void", "volatile",
        "while"
    };

    int count;
    int i;

    count = sizeof(keywords) / sizeof(keywords[0]);

    for (i = 0; i < count; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void writeHTMLHeader(FILE *fp)
{
    fprintf(fp, "<!DOCTYPE html>\n");
    fprintf(fp, "<html>\n");
    fprintf(fp, "<head>\n");
    fprintf(fp, "<title>Source Code</title>\n");

    fprintf(fp, "<style>\n");
    fprintf(fp, "body { background-color: black; color: white; ");
    fprintf(fp, "font-family: monospace; font-size: 16px; }\n");

    fprintf(fp, ".keyword { color: blue; font-weight: bold; }\n");
    fprintf(fp, ".string { color: orange; }\n");
    fprintf(fp, ".comment { color: green; }\n");
    fprintf(fp, ".number { color: red; }\n");

    fprintf(fp, "</style>\n");
    fprintf(fp, "</head>\n");
    fprintf(fp, "<body>\n");
    fprintf(fp, "<pre>\n");
}

void writeHTMLFooter(FILE *fp)
{
    fprintf(fp, "</pre>\n");
    fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");
}

void writeEscapedCharacter(FILE *fp, char ch)
{
    if (ch == '<')
    {
        fprintf(fp, "&lt;");
    }
    else if (ch == '>')
    {
        fprintf(fp, "&gt;");
    }
    else if (ch == '&')
    {
        fprintf(fp, "&amp;");
    }
    else
    {
        fputc(ch, fp);
    }
}

void processSource(FILE *input, FILE *output)
{
    char ch;
    char word[100];
    int i;

    while ((ch = fgetc(input)) != EOF)
    {
        if (isalpha(ch) || ch == '_')
        {
            i = 0;

            while (isalnum(ch) || ch == '_')
            {
                if (i < 99)
                {
                    word[i] = ch;
                    i++;
                }

                ch = fgetc(input);
            }

            word[i] = '\0';

            if (isKeyword(word))
            {
                fprintf(output, "<span class=\"keyword\">");
                fprintf(output, "%s", word);
                fprintf(output, "</span>");
            }
            else
            {
                fprintf(output, "%s", word);
            }

            ungetc(ch, input);
        }

        else if (isdigit(ch))
        {
            i = 0;

            fprintf(output, "<span class=\"number\">");

            while (isdigit(ch))
            {
                writeEscapedCharacter(output, ch);
                i++;
                ch = fgetc(input);
            }

            fprintf(output, "</span>");

            ungetc(ch, input);
        }

        else if (ch == '"')
        {
            fprintf(output, "<span class=\"string\">");
            writeEscapedCharacter(output, ch);

            ch = fgetc(input);

            while (ch != '"' && ch != EOF)
            {
                writeEscapedCharacter(output, ch);
                ch = fgetc(input);
            }

            if (ch == '"')
            {
                writeEscapedCharacter(output, ch);
            }

            fprintf(output, "</span>");
        }

        else if (ch == '/' )
        {
            char next;

            next = fgetc(input);

            if (next == '/')
            {
                fprintf(output, "<span class=\"comment\">");
                fprintf(output, "//");

                while ((ch = fgetc(input)) != '\n' && ch != EOF)
                {
                    writeEscapedCharacter(output, ch);
                }

                fprintf(output, "</span>");

                if (ch == '\n')
                {
                    fputc('\n', output);
                }
            }

            else if (next == '*')
            {
                fprintf(output, "<span class=\"comment\">");
                fprintf(output, "/*");

                char previous = 0;

                while ((ch = fgetc(input)) != EOF)
                {
                    writeEscapedCharacter(output, ch);

                    if (previous == '*' && ch == '/')
                    {
                        break;
                    }

                    previous = ch;
                }

                fprintf(output, "</span>");
            }

            else
            {
                fputc('/', output);
                ungetc(next, input);
            }
        }

        else if (ch == '<' || ch == '>' || ch == '&')
        {
            writeEscapedCharacter(output, ch);
        }

        else
        {
            fputc(ch, output);
        }
    }
}

void convertToHTML(char *inputFile, char *outputFile)
{
    FILE *input;
    FILE *output;

    input = fopen(inputFile, "r");

    if (input == NULL)
    {
        printf("Unable to open input file.\n");
        return;
    }

    output = fopen(outputFile, "w");

    if (output == NULL)
    {
        printf("Unable to create output file.\n");
        fclose(input);
        return;
    }

    writeHTMLHeader(output);

    processSource(input, output);

    writeHTMLFooter(output);

    fclose(input);
    fclose(output);

    printf("\nSource file converted successfully.\n");
    printf("HTML file created: %s\n", outputFile);
}