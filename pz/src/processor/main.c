/*
 *	Syntax: layout.xml template.xsl file01.php file02.php ....
 */

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libxml/xinclude.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "main.h"
#include "php.h"

#define STATIC_INCLUDE 0x00001

#define PROC_CHILD	0x0
#define PROC_THIS	0x1



int process_group(xmlDocSet * outdoc);
xmlDocPtr apply_stylesheet(xmlDocPtr, char *);
xmlDocSet * parse_document(char *);
int process_php_tag(xmlDocSet * outfile);
int process_xml(xmlDocSet * outdoc);
xmlNodePtr find_group(xmlDocPtr in, xmlNodePtr searchcur);

int main(int argc, char** argv){
    int i=0;
    char filename[256];
    xmlDocPtr doc;
    xmlDocSet * docset, * cur;
    
    if(argc < 3){
	printf("Syntax: xmlcompile <layout.xml> <template.xsl> <outputfile.html>\n");
	exit(1); // errorcode 1
    }
    docset = parse_document(argv[1]);

    cur = docset;
    while(cur != NULL){
	sprintf(filename, "%s%i", argv[3], i++);
//	cur->doc = apply_stylesheet(cur->doc, argv[2]);
        xmlSaveFormatFile(filename, cur->doc, 1);
	cur = cur->next;
    }
}



xmlDocSet * parse_document(char *file){
    xmlDocPtr indoc;
    xmlNodePtr cur, cpcur, outcur;    
    xmlInitParser();
    xmlIndentTreeOutput = 1;
    xmlDocSet * outdocset;
        
    outdocset = (xmlDocSet *) malloc(sizeof(xmlDocSet));
    outdocset->next = NULL;

    
    indoc = xmlParseFile(file);
    if(indoc == NULL){
	printf("Error while opening %s\n", indoc);
	exit(1);
    }
    xmlXIncludeProcess(indoc); // allow xInclude's
    cur = xmlDocGetRootElement(indoc);
    
    if(xmlStrcmp(cur->name, (xmlChar *) "page")){
	printf("Error: root element not <page>\n");
	exit(1);
    }    
    
    outdocset->doc = indoc;
    outdocset->cur = cur;
    outdocset->proctype = PROC_CHILD;
    outdocset->param = NULL;
    process_xml(outdocset);
    return outdocset;
}    
 
int process_xml(xmlDocSet * outdoc){
    xmlNodePtr cpcur;
    xmlDocSet * curdocset;

    curdocset = outdoc;
    while(curdocset != NULL){
	int brflag = 0;
	// we only pass here once for every doc
	if(curdocset->proctype == PROC_CHILD)
            curdocset->cur = curdocset->cur->xmlChildrenNode;
        while(curdocset->cur != NULL) {
            if(!xmlStrcmp(curdocset->cur->name, (xmlChar *) "php")){
                process_php_tag(curdocset);
		curdocset->proctype = PROC_THIS;
            } else if(!xmlStrcmp(curdocset->cur->name, (xmlChar *) "group")) {
	        curdocset->processed = 1;
		process_group(curdocset);
	    } else if(!xmlStrcmp(curdocset->cur->name, (xmlChar *) "param")) {
		printf("iek\n");
		variable * t;
		t = (variable *) malloc(sizeof(variable));
		if(t == NULL){
		    printf("Not enough memory...\n");
		    exit(1);
		}
		t->next = curdocset->param;
		curdocset->param = t;
		curdocset->param->name = (char *) xmlGetProp(curdocset->cur, (xmlChar *) "name");
		curdocset->param->data = (char *) xmlGetProp(curdocset->cur, (xmlChar *) "value");
	    
	    }
	    cpcur = curdocset->cur;	    
	    curdocset->cur = curdocset->cur->xmlChildrenNode;
	    curdocset->proctype = PROC_THIS;
	    
	    if(curdocset->processed){
		curdocset->processed = 0;
		curdocset = outdoc;
		break; //re-enter while(curdocset != NULL)
	    }
        }
	curdocset->cur = cpcur;
	while(curdocset->cur->next == NULL){
	    curdocset->cur = curdocset->cur->parent;
	    if(curdocset->cur == NULL){
		curdocset = curdocset->next;
		brflag=1;
		break;
	    }
	}
	if(brflag) 
	    break;
	curdocset->cur = curdocset->cur->next;
    }
}

int process_group(xmlDocSet * outdoc){
    xmlDocSet * outdocset;
    xmlDocSet * tmp;
    xmlDocSet * outcur;
    xmlDocSet * prevout;
    xmlNodePtr searchcur;
    xmlDocPtr backupdoc;
    xmlNodePtr cur, copy;
    int first = 1;

    printf("Processing group\n");
    /* we always get one doc in an xmlDocSet and we make it a linked lists containing several docs */
    /* after one group is processed, process_xml makes sure everything gets finished in all the new docs in the set that get attached */

    // create a backup of the document that we get passed, we're going to need the info in it for all the new docs created
    backupdoc = xmlCopyDoc(outdoc->doc, 1);

    // outdoc->cur points to the <group>
    cur = find_group(backupdoc, xmlDocGetRootElement(backupdoc));
    cur = cur->xmlChildrenNode;
    while(cur != NULL){

        if(!xmlStrcmp(cur->name, (xmlChar *) "groupelem")){
	    variable * t;
	    variable * newt;
            if(first){
                outcur = outdoc;
            } else {
                outcur = (xmlDocSet *) malloc(sizeof(xmlDocSet));
                outcur->doc = xmlCopyDoc(backupdoc, 1);
            }
            // now find the <group>, this shouldn't be too hard, the first <group> we encounter is the one we need
            searchcur = find_group(outcur->doc,  xmlDocGetRootElement(outcur->doc));
	    if(searchcur == NULL){
		printf("Arrrghz, this shouldn\'t happen\n");
		exit(1);
	    }
            copy = xmlCopyNode(cur, 1);
            xmlReplaceNode(searchcur, copy);
	    outcur->cur = copy;
	    outcur->proctype = PROC_THIS;
	    outcur->next = NULL;
	    
	    if(outcur != outdoc){
		    outcur->param = NULL;
		    t = outdoc->param;
		    while(t != NULL){
			newt = (variable *) malloc(sizeof(variable));
			newt->name = (char *) malloc(sizeof(char) * (strlen(t->name) +1));
			newt->data = (char *) malloc(sizeof(char) * (strlen(t->data) +1));
			strcpy(newt->name, t->name);
			strcpy(newt->data, t->data);
			newt->next = outcur->param;
			outcur->param = newt;
			t = t->next;
		    }
	    }
	    
	    if(!first)
		    prevout->next = outcur;
	    prevout = outcur;
	    first = 0;
        }
        cur = cur->next;
    }
    xmlFreeDoc(backupdoc);


}


xmlNodePtr find_group(xmlDocPtr in, xmlNodePtr searchcur){
    xmlNodePtr ret;
    searchcur = searchcur->xmlChildrenNode;
    while(searchcur != NULL){
        if(!xmlStrcmp(searchcur->name, (const xmlChar *) "group"))
            return(searchcur);
        if((ret = find_group(in, searchcur)) != NULL)
            return ret;
        searchcur = searchcur->next;
    }
    return NULL;
}


int process_php_tag(xmlDocSet * outfile){
    xmlChar *arg;
    
    arg = xmlGetProp(outfile->cur, (const xmlChar *) "static");
    if(!(!xmlStrcmp(arg, (xmlChar *) "yes"))){
	printf("Error: only static <php> supported\n");
	return 0;
//	exit(1);
    } else {
	char phpname[2048];
	char argfile[2048];
	int fd;
	xmlBuffer argbuf;
	xmlBufferPtr buffer;
	xmlDocPtr doc;
	xmlNodePtr rootnode, copyroot, tc;
	variable * t;
	
	xmlFree(arg);
	arg = xmlGetProp(outfile->cur, (xmlChar *) "name");
	sprintf(phpname,"./phpscripts/%s.php",arg);
	
	t = outfile->param;
	while(t != NULL){
	printf("iek2\n");
	    tc = xmlNewNode(NULL, (xmlChar *) "param");
	    xmlNewProp(tc, (xmlChar *) "name", (xmlChar *) t->name);
	    xmlNewProp(tc, (xmlChar *) "value", (xmlChar *) t->data);
	    xmlAddChild(outfile->cur, tc);
	    t = t->next;
	}

	
	xmlNodeDump(&argbuf, outfile->doc, outfile->cur, 1, 1);
	fd = open("/tmp/xmlargbuf", O_WRONLY | O_CREAT);
	write(fd, (char *) xmlBufferContent(&argbuf), strlen( (const char*) xmlBufferContent(&argbuf)));
	
	buffer = execute_php(phpname, "/tmp/xmlargbuf");    
	doc = xmlParseMemory( (char *) xmlBufferContent(buffer), xmlBufferLength(buffer));
	if(doc == NULL){
	    printf("Error parsing php file %s\n", phpname);
	    exit(1);
	}
	rootnode = xmlDocGetRootElement(doc);
	copyroot = xmlCopyNode(rootnode,1); // 1 = recursion = on
	xmlReplaceNode(outfile->cur, copyroot);
	xmlFreeNode(outfile->cur);
	outfile->cur = copyroot;
	xmlFreeDoc(doc);
	xmlBufferFree(buffer);
    }
    return 0;
}

xmlDocPtr apply_stylesheet(xmlDocPtr doc, char *stylesheet){
    xsltStylesheetPtr cur = NULL;
    xmlDocPtr res;
    
    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;
    
    cur = xsltParseStylesheetFile((const xmlChar *) stylesheet);
    if((res = xsltApplyStylesheet(cur, doc, NULL)) == NULL){
	printf("Error while applying stylesheet %s", stylesheet);
	exit(1);
    }
    
    xmlFreeDoc(doc);
    xsltFreeStylesheet(cur);

    xmlCleanupParser();
    return res;
}

