/*
 * Project Zero - All Rights Reserved
 * projectfile.cpp
 */

#include "globals.h"
#include "block.h"
#include "intern.h"
#include "projectfile.h"

Project::Project() {}
Project::~Project() {
	pages.DeleteContents(TRUE);
	pages.Clear();
}

int Project::LoadXmlProjectFile(wxString& filename, wxString& path) {
	xmlNodePtr cur;
	xmlChar * value;
	doc = xmlParseFile((const char*)filename.mb_str());
	if(doc == NULL) {
		std::cout << "Error parsing file " << (const char *)filename.mb_str() << std::endl;
		return 0;
	}
	projpath = path;
	cur = xmlDocGetRootElement(doc);
	if(xmlStrcmp(cur->name, (const xmlChar *)"project") != 0) { 
		xmlFreeDoc(doc);
		return 0;
	}
	cur = cur->children;
	while(cur != NULL) {
		if(!xmlStrcmp(cur->name, (const xmlChar *)"name")) {
			value = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			wxString temp((const char*)value, wxConvUTF8);
			SetName(temp);
			std::cout << "Added name ..." << std::endl;
			xmlFree(value);
		}
		else if(!xmlStrcmp(cur->name, (const xmlChar *)"description")) {
			value = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			wxString temp((const char*)value, wxConvUTF8);
			SetDescription(temp);
			std::cout << "Added description ..." << std::endl;
			xmlFree(value);
		}
		else if(!xmlStrcmp(cur->name,(const xmlChar *)"page")) {
			std::cout << "Parsing page ..." << std::endl;
			ParsePage(cur);
		}
	cur = cur->next;
	}
	return 1;
}

int Project::SaveXmlProjectFile(wxString& filename) {
    xmlNodePtr cur, childcur; 
    xmlFreeDoc(doc);
    doc = xmlNewDoc((xmlChar *) "1.0");
    cur = xmlNewNode(NULL, (xmlChar *) "project");
    xmlDocSetRootElement(doc, cur);
    return 0;
}


void Project::ParsePage(xmlNodePtr cur)
{
	xmlChar *prop;
	if ((prop = xmlGetProp(cur, (const xmlChar*)"uri")) != NULL) {
		wxString temp((const char*)prop, wxConvUTF8);
		xmlFree(prop);
		AddPage(temp);
	}
}

void Project::SetName(wxString& name) {
	projname = name;
}

void Project::SetDescription(wxString& description) {
	projdescription = description;
}

int Project::AddPage(wxString& filename) {
	Page *page = new Page();
	wxString fullpath = projpath + filename;
	// XXX too buggy at the moment
	/*if(*/page->LoadXmlPageFile(fullpath);/*)*/
	/*{*/ pages.Append(page);/* }*/
	//else { delete page; }
}

size_t Project::GetPageCount() {
	return pages.GetCount();
}

Page *Project::GetPage(size_t position) {
	return pages.Item(position)->GetData();
}

