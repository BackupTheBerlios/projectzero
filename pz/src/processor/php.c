#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libxml/xinclude.h>
#include <string.h>
#include "main.h"
#include "php.h"

#define PHP_SCRIPTS_DIR "../../phpscripts"

xmlBufferPtr execute_php(char *infile, char *argfile){
    char *command; int commandlen;
    char buffer[BUFSIZE];
    FILE * ptr;
    xmlBufferPtr outbuf;
    int i;
    
    outbuf = xmlBufferCreate();

    commandlen = strlen("php -q -f "PHP_SCRIPTS_DIR"emulate.php ") + strlen(infile) + strlen(argfile) + 2;
    command = (char*)malloc(sizeof(char)*commandlen);
    strcpy(command, "php -q -f "PHP_SCRIPTS_DIR"/emulate.php ");
    strcat(command, infile);
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
