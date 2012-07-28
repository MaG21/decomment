/*
 * Autor: Manuel "MaG" A. Güílamo, Newbie's Shell
 * URL: http://www.newbieshell.blogspot.com
 * 
 * released under the terms of MIT
 *
 * Basado en las especificaciones de la clase COS 217:
 * "Introduction to Programming Systems"
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef FALSE
#define FALSE 0
#else
#undef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#else
#undef TRUE
#define TRUE 1
#endif

#define MAX_BUF         1024

#define INIT_COMMENT    "/*"
#define END_COMMENT     "*/"
#define SHORT_COMMENT   "//"

#define SCAPE_CHAR      '\\'

#define DOUBLE_QUOTE    '\"'
#define SIMPLE_QUOTE    '\''

#define SIMPLE   1
#define DOUBLE   2

/* Info respecto del procesamiento */
struct file_proc {
	size_t  lineno;        /* linea actual */
	size_t  init_comline;  /* linea de partida del comentario actual */

	/* variables de estado */
	uint8_t comment;
	uint8_t quotation;
	uint8_t quote_type; /* SIMPLE o DOUBLE */
	uint8_t comment_type; /* SIMPLE o DOUBLE */
};

int
main(int argc, char *argv[])
{
	FILE               *file;
	char               *p;
	char               *ptr;
	size_t             len;
	char               buf[MAX_BUF+1];
	struct file_proc   proc;

	if(argc==1) {
		file = stdin;
	} else if(argc==2) {
		file = fopen(argv[1], "r");
		if(!file) {
			perror(argv[1]);
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stderr, "Uso: %s <fichero>\n", argv[0]);
		_Exit(EXIT_FAILURE);
	}

	(void)memset(&proc, 0 , sizeof(proc));
	proc.lineno = 1;

	while(1) {
		if(feof(file))
			break;

		len = fread(&buf[0], sizeof(char), MAX_BUF, file);

		buf[len]='\0';

		for(p=&buf[0]; *p!='\0'; p++) {
			if(*p=='\n') {
				proc.lineno++;

			/* para  el caso en que '\n' no este en este bloque */
			} else if(proc.comment && proc.comment_type==SIMPLE) {
				ptr = strchr(p, '\n');
				if(!ptr)
					break; /* lineas mayores que MAX_BUFF?, raro */
				p = ptr-1;
				proc.comment = FALSE;
				continue;
			} else if(proc.quotation && *p==SCAPE_CHAR ) {
				printf("%c%c", *p, *(p+1));
				p++;
				continue;

			}else if(!proc.quotation && !proc.comment && *p==DOUBLE_QUOTE ){
				proc.quotation = TRUE;
				proc.quote_type = DOUBLE;

			}else if(!proc.quotation && !proc.comment && *p==SIMPLE_QUOTE ){
				proc.quotation = TRUE;
				proc.quote_type = SIMPLE;

			} else if(proc.quotation && proc.quote_type==SIMPLE &&
			                                            *p==SIMPLE_QUOTE ) {
				proc.quotation = FALSE;

			} else if(proc.quotation  && proc.quote_type==DOUBLE &&
			                                            *p==DOUBLE_QUOTE ) {
				proc.quotation = FALSE;

			} else if( !proc.comment && !proc.quotation &&
				        !strncmp(p, INIT_COMMENT, strlen(INIT_COMMENT)) ) {
				proc.comment = TRUE;
				proc.comment_type = DOUBLE;
				proc.init_comline = proc.lineno;
				printf(" ");
				p++;
				continue;

			} else if( proc.comment && proc.comment_type==DOUBLE && 
			               !strncmp(p, END_COMMENT, strlen(END_COMMENT)) ) {
				proc.comment = FALSE;
				p++;
				continue;

			} else if( !proc.comment && !proc.quotation &&
				      !strncmp(p, SHORT_COMMENT, strlen(SHORT_COMMENT)) ) {
				ptr = strchr(p, '\n');
				if(!ptr) {
					proc.comment_type = SIMPLE;
					break;
				} 
				p = ptr-1;
				continue;
			}

			if(proc.comment && *p!='\n')
				continue;

			printf("%c", *p);
		}
	}

	fclose(file);

	if(proc.comment) {
		fprintf(stderr, "Error: linea %ld: comentario no cerrado\n",
		                                                  proc.init_comline);
		return EXIT_FAILURE;
	}

   return EXIT_SUCCESS;
}
