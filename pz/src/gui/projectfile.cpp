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

int Project::LoadXmlProjectFile(wxFileName& filename) {
	xmlNodePtr cur;
	xmlChar *value;
	xmlDocPtr doc = xmlParseFile((const char*)filename.GetFullPath().mb_str());
	if(doc == NULL) {
		std::cout << "Error parsing file " << (const char *)filename.GetFullPath().mb_str() << std::endl;
		return 0;
	}
	projfile = filename;
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
	xmlFreeDoc(doc);
	return 1;
}

int Project::SaveXmlProjectFile() {
	// XXX This functions is not yet written, it will only print
	// an example projectfile, cause there's a conversion problem.
	xmlDocPtr doc;
	xmlNodePtr cur, childcur; 
	doc = xmlNewDoc((const xmlChar*) "1.0");
	cur = xmlNewNode(NULL, (const xmlChar*) "project");
	xmlDocSetRootElement(doc, cur);
	//xmlNewTextChild(cur, NULL, (const xmlChar*)"name", (const xmlChar*)projname.wc_str(wxConvUTF8));
	//xmlNewTextChild(cur, NULL, (const xmlChar*)"description", (const xmlChar*)projdescription.wc_str(wxConvUTF8));
	xmlNewTextChild(cur, NULL, (const xmlChar*)"name", (const xmlChar*)"Een fijne naam");
	xmlNewTextChild(cur, NULL, (const xmlChar*)"description", (const xmlChar*)"Een fijne omschrijving.");
	childcur =  xmlNewTextChild(cur, NULL, (const xmlChar*)"page", NULL);
	xmlNewProp(childcur, (const xmlChar*)"uri", (const xmlChar*)"./page1.xml");
	std::cout << "saving to file: " << (const char*)projfile.GetFullPath().mb_str() << std::endl;
	xmlSaveFormatFile((const char*)projfile.GetFullPath().mb_str(),doc,1);
	xmlFreeDoc(doc);
	return 1;
}


void Project::ParsePage(xmlNodePtr cur)
{
	xmlChar *prop;
	if ((prop = xmlGetProp(cur, (const xmlChar*)"uri")) != NULL) {
		wxString temp((const char*)prop, wxConvUTF8);
		wxString temp2; 
		xmlFree(prop);
		if((prop = xmlGetProp(cur, (const xmlChar*)"name")) != NULL) {
			temp2 = wxString((const char*)prop, wxConvUTF8);
			xmlFree(prop);
		}
		AddPage(temp, temp2);
	}
}

void Project::SetName(wxString& name) {
	name.Trim(FALSE);name.Trim(TRUE);name.Truncate(100);
	projname = name;
}

void Project::SetDescription(wxString& description) {
	description.Trim(FALSE);description.Trim(TRUE);description.Truncate(1024);
	projdescription = description;
}

int Project::AddPage(wxString& filename, wxString& name) {
	wxFileName fullpath(filename);
	if(fullpath.IsRelative()) { fullpath.PrependDir(projfile.GetPath()); } 
	pages.Append(new Page(fullpath, name));
}

size_t Project::GetPageCount() {
	return pages.GetCount();
}

Page *Project::GetPage(size_t position) {
	return pages.Item(position)->GetData();
}

