#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#include "json.h"

int
main (int argc, char **argv)
{
	json_t *document = NULL;
	FILE *fp;
	int i;

	if(argc < 2)
	{
		printf("usage: test document1.json ...\n");
		return EXIT_SUCCESS;
	}

	
	for(i = 1; i < argc; i++)
	{
		printf("processing file %s...\n",argv[i]);
		fp = fopen(argv[i],"r");
		if(fp == NULL)
		{
			printf("file \"%s\" couldn't be open\n",argv[i]);
		}
		else
		{
			switch(json_stream_parse(fp, &document))
			{
				case JSON_OK:
					json_stream_output(stdout,document);
					json_free_value(&document);
					break;

				default:
					printf("some problem occurred\n");
					break;
			}
		}
		fclose(fp);
	}
	
	return EXIT_SUCCESS;
}
