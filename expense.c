/**
Sums expenses of each item of a certain month
**/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "sllnode.h"

#define MAX_BUFF_SIZE 40
#define MONTH_TOTAL 12
#define EXTENSION 4
#define DAY_LEN 2
#define SPACEBAR 1

// define ANSI escape code colors for status messages
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

// define months
const char *MONTH[] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
const char *TRUNC[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
const int DAYS[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int main(int argc, char *argv[])
{
	// check command-line argument
	if (argc != 3)
	{
		fprintf(stderr, "Usage: expense [month] [year]\n");
		return 1;
	}
	
	// day, month, year variables
	char *month = argv[1];
	const char *trunc;
	int year_digits = atoi(argv[2]);
	char year[2];
	year_digits %= 2000;
	int days;

	// bookkeeping variables
	double total = 0;
	double max = 0;
	double min = INFINITY;

	// convert year back to string
	sprintf(year, "%d", year_digits);

	// convert user input to lowercase
	for (int i = 0, n = strlen(month); i < n; i++)
	{
		// reject non-alphanumeric input
		if (!isalpha(month[i]))
		{
			fprintf(stderr, "Enter ALPHABETIC letters for your month!\n");
			return 2;
		}

		if (isupper(month[i]))
		{
			month[i] = tolower(month[i]);
		}
	}

	// check correct month input
	for (int i = 0; i <= MONTH_TOTAL; i++)
	{
		if (i == MONTH_TOTAL)
		{
			fprintf(stderr, "Incorrect month input!\n");
			return 3;
		}

		if (strcmp(month, MONTH[i]) == 0)
		{
			days = DAYS[i];
			trunc = TRUNC[i];
			break;
		}
	}

	// prepare a csv file for writing
	char outfile[strlen(month) + strlen(year) + EXTENSION];
	sprintf(outfile,"%s%s.csv",month,year);

	FILE *outptr = fopen(outfile, "w");
	if (outptr == NULL)
	{
		fprintf(stderr, "Could not open CSV file for writing\n");
		return 4;
	}
	
	/** next lines of codes will iterate through all days in the month
	 ** skip file misses, write to outfile for file hits
	 **/

	// initialize file hit/miss variables
	int hit = 0;
	int miss = 0;

	// declare input file name buffer
	char infile[DAY_LEN + SPACEBAR + strlen(trunc) + SPACEBAR + strlen(year) + EXTENSION];

	// create initial ghost sllnode
	sllnode *head = create("None", 0.00);
	if (head == NULL)
	{
		fprintf(stderr, "Error initializing memory buffer\n");
		return 5;
	}

	// begin infile iteration
	for (int i = 1, n = days; i <= n; i++)
	{
		// prepare input file for reading
		sprintf(infile, "%i %s %s.txt", i, trunc, year); 
		FILE *inptr = fopen(infile, "r");

		// output status messages, file miss (NULL pointer) will show up in stdout here.
		printf("Checking existence of file %s... ", infile);
		if (inptr == NULL)
		{
			// drop writing for current filename if it does not exist
			printf(ANSI_COLOR_RED	"miss :("	ANSI_COLOR_RESET	"\n");
			miss++;
			continue;
		}
		else
		{
			printf(ANSI_COLOR_GREEN "hit! :)"	ANSI_COLOR_RESET	"\n");
			hit++;
		}

		// write name of infile as a subheader in csv outfile
		fwrite(infile, 1, sizeof(infile) - EXTENSION, outptr);
		fputc('\n',outptr);

		// file read/write variables
		char c = fgetc(inptr);
		double dollar;
		char charbuff[MAX_BUFF_SIZE];
		char intbuff[MAX_BUFF_SIZE];
		int arr_count;

		// start file reading
		while (!feof(inptr))
		{			
			arr_count = 0;
			// reset read/write char buffer
			for (int i = 0; i < MAX_BUFF_SIZE; i++)
			{
				charbuff[i] = '\0';
				intbuff[i] = '\0';
			}
			// read item string
			while (true)
			{
				fputc(c, outptr);
				charbuff[arr_count] = c;
				arr_count++;

				c = fgetc(inptr);
				if (c == ':')
				{
					fputc(',',outptr);
					break;
				}
			}
		
			// Skip blank space between item/price separation. Reset array index count
			fseek(inptr, SPACEBAR, SEEK_CUR);
			arr_count = 0;

			// Put the '$' sign to denote currency in csv outfile
			fputc('$',outptr);

			// read price string
			while (true)
			{
				c = fgetc(inptr);

				if (c == '\n')
				{
					fputc(c, outptr);
					break;
				}

				fputc(c, outptr);
				intbuff[arr_count] = c;
				arr_count++;
			}

			// store price as an actual double value
			dollar = atof(intbuff);

			// store row info of infile to sllnode
			if (!append(head, charbuff, dollar))
				head = insert(head, charbuff, dollar);

			// read next character in file
			c = fgetc(inptr);
			
		}

		// close current input file
		fclose(inptr);
	}
	
	// Output total spendings for each item to csv file
	sllnode *trav = head;
	
	while (strcmp(trav -> item, "None"))
	{
		// create a buffer to store strings of doubles
		char sprice[10];

		// append totals of each item to the file
		fwrite(trav -> item, sizeof(trav -> item), 1, outptr);
		fputc(',', outptr);
		sprintf(sprice, "%.2lf", trav -> price);
		fwrite(sprice, sizeof(sprice), 1, outptr);
		fputc('\n', outptr);

		// Update bookkeeping variables
		total += trav -> price;
		if (trav -> price > max)
			max = trav -> price;
		else if (trav -> price < min)
			min = trav -> price;

		trav = trav -> next;
	}

	// Output monthly total, max and min spending for corresponding item to csv file
	// TODO

	// close output csv file
	fclose(outptr);

	// free all sllnodes
	destroy(head);

	// print status messages
	printf("Success creating file: %s%s.csv\n",month,year);
	printf("Amount of textfile hits: %d\nAmount of textfile misses: %d\n", hit, miss);

	// success
	return 0;
}



