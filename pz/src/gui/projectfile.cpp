/*
 * Project Zero - All Rights Reserved
 * projectfile.cpp
 */

#include "globals.h"
#include "intern.h"
#include "projectfile.h"

Project::Project() {}
Project::~Project() {
	pages.DeleteContents(TRUE);
	pages.Clear();
	styles.DeleteContents(TRUE);
	styles.Clear();
	toppages.Clear();
}
int Project::LoadXmlProjectFile(wxString& filename) {

	xmlNodePtr cur;
	xmlChar * value;
	doc = xmlParseFile((const char*)filename.c_str());
	if(doc == NULL) {
		std::cout << "Error parsing file " << (const char *)filename.c_str() << std::endl;
		return 0;
	}
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
			ParseTopPages(doc, cur);
		}
	cur = cur->next;
	}
	return 1;
}

int Project::SaveXmlProjectFile(wxString& filename) {
    return 0;
}


void Project::ParseTopPages(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *prop;
	if ((prop = xmlGetProp(cur, (const xmlChar*)"id")) != NULL) {
		unsigned long myid;
		wxString temp((const char*)prop, wxConvUTF8);
		if(temp.ToULong(&myid)) { AddTopPage(myid); }
		std::cout << "Added toppage ..." << std::endl;
		xmlFree(prop);
		prop = xmlGetProp(cur, (const xmlChar*)"uri");
		wxString tempp((const char*) prop, wxConvUTF8);
		AddPage(tempp);
	}
	cur = cur->next;
}

void Project::SetName(wxString& name) {
	// strip (trim) this string?
	projname = name.Truncate(100);
}

void Project::SetDescription(wxString& description) {
	// strip (trim) this string?
	projdescription = description.Truncate(1000);
}

int Project::AddPage(wxString& filename) {
	Page *page = new Page();
	if(page->LoadXmlPageFile(filename))
	{
		pages.Append(page);
	}
	else
	{
		delete page;
	}
}

void Project::AddTopPage(unsigned long id) {
	// check if exists?
	toppages.Add(id);
}

size_t Project::GetPageCount() {
	return pages.GetCount();
}

Page *Project::GetPage(size_t position) {
	return pages.Item(position)->GetData();
}

