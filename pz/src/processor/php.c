#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libxml/xinclude.h>
#include <string.h>
#include "main.h"
#include "php.h"

xmlBufferPtr execute_php(char *infile, char *argfile){
    char *command; int commandlen;
    char buffer[BUFSIZE];
    FILE * ptr;
    xmlBufferPtr outbuf;
    int i;
    
    outbuf = xmlBufferCreate();

    commandlen = strlen("php -q ") + strlen(infile);
    commandlen += (strlen(argfile) + 1); 
    command = (char*)malloc(sizeof(char)*commandlen + 1);
    sprintf(command, "php -q %s ", infile);
    strcat(command, argfile);
    if((ptr = popen(command, "r")) != NULL)
    {
	while(fgets(buffer, BUFSIZE, ptr) != NULL)
		{ 
		    xmlBufferAdd(outbuf,(xmlChar *) buffer, strlen(buffer));
		 }
	pclose (ptr);
    }
    else  printf("Couldn\'t execute phpfile %s\n", infile ); 
    free(command);
    return outbuf;
}
