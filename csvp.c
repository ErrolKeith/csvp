// csvp.c -- converts csv files to json.

// NOTE: This does not cover all possibilities for converting CSV to JSON. The source code should be read
// to see if the program meets specific needs.

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    char *infile;
    char *outfile;
} CsvpArgs;

typedef struct
{
    char **fields;
    int fieldCount;
} CsvRow;

typedef struct
{
    CsvRow *rows;
    int rowCount;
} CsvData;

CsvpArgs parseArgs(int argc, char **argv);
CsvData readCsv(const char *filename);
CsvRow parseCsvLine(char *line);
char *replaceNewlineEscapeChars(char *str);
void makeJsonFromCsv(CsvData data, char *outfile);
void appendCsvRowsToJsonFile(FILE *file, CsvData data);
void appendRowAsObject(FILE *file, CsvRow data, char **headers);

int main(int argc, char **argv)
{
    CsvpArgs args = parseArgs(argc, argv);
    CsvData csv = readCsv(args.infile);
    makeJsonFromCsv(csv, args.outfile);
    return 0;
}

CsvpArgs parseArgs(int argc, char **argv)
{
    int option;
    char *infile = NULL;
    char *outfile = NULL;

    while ((option = getopt(argc, argv, "i:o:")) != -1)
    {
        switch (option)
        {
        case 'i':
            infile = optarg;
            break;
        case 'o':
            outfile = optarg;
            break;
        default:
            // This is intentional fallthrough for the switch statement.
            // If all required args are present, why stop execution, BUT
            // `getopt` will report an illegal option with untracked usage of options.
        }
    }

    CsvpArgs args;
    args.infile = infile;
    args.outfile = outfile;

    if (args.infile == NULL)
    {
        printf("A value for -i is required.\n");
        exit(EXIT_FAILURE);
    }

    if (args.outfile == NULL)
    {
        printf("A value for -o is required.\n");
        exit(EXIT_FAILURE);
    }

    return args;
}

CsvData readCsv(const char *filename)
{
    char line[1024];
    FILE *file = fopen(filename, "r");
    CsvData data;

    if (file == NULL)
    {
        printf("Error opening file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    data.rows = malloc(1024 * sizeof(CsvRow));
    data.rowCount = 0;

    while (fgets(line, 1024, file) != NULL)
    {
        data.rows[data.rowCount] = parseCsvLine(line);
        data.rowCount++;
    }

    fclose(file);

    return data;
}

CsvRow parseCsvLine(char *line)
{
    CsvRow row;
    row.fields = malloc(1024 * sizeof(char *));
    row.fieldCount = 0;
    char *csvSplitToken;

    csvSplitToken = strtok(line, ",");

    while (csvSplitToken != NULL)
    {
        row.fields[row.fieldCount] = strdup(csvSplitToken);
        row.fieldCount++;
        csvSplitToken = strtok(NULL, ",");
    }
    return row;
}

void makeJsonFromCsv(CsvData data, char *outfile)
{
    FILE *output = fopen(outfile, "w");
    fprintf(output, "[\n");
    appendCsvRowsToJsonFile(output, data);
    fprintf(output, "\n]\n");
    fclose(output);
}

void appendCsvRowsToJsonFile(FILE *file, CsvData data)
{
    for (int i = 1; i < data.rowCount; i++)
    {
        appendRowAsObject(file, data.rows[i], data.rows[0].fields);

        // A `,` after the last field is invalid JSON.
        if ((i + 1) != data.rowCount)
        {
            fprintf(file, ",\n");
        }
    }
}

void appendRowAsObject(FILE *file, CsvRow data, char **headers)
{
    for (int i = 0; i < data.fieldCount; i++)
    {
        char *cleanKey = replaceNewlineEscapeChars(headers[i]);
        char *cleanValue = replaceNewlineEscapeChars(data.fields[i]);

        if (i == 0)
        {
            fprintf(file, "  {\n");
        }

        fprintf(file, "\t\t\"%s\": \"%s\"", cleanKey, cleanValue);

        // A `,` after the last object in an array is invalid JSON.
        if ((i + 1) == data.fieldCount)
        {
            fprintf(file, "\n\t}");
        }
        else
        {
            fprintf(file, ",\n");
        }
    }
}

char *replaceNewlineEscapeChars(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
        }
    }

    return str;
}