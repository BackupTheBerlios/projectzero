/*
	This file is part of the ProjectZero GUI
	All rights reserved
*/

/* 
 * What happens here?
 *
 * A Page object can open a xml page file, parse it, and store relevant information.
 * It also keeps a xmlDoc in memory that contains the loaded page, this doesn't get modified until it's saved to disk
 *
 * A Page object should also parse templates and layouts and store xmlDoc's representing their contentµ
 * It's also responsible for handling all file interaction with the page xml-file. No other object may access those files.
 *
 */

#include "globals.h"
#include "block.h"
#include "intern.h"
WX_DEFINE_LIST(PageList);

Page::Page(wxFileName& myfilename, wxString& myname)
{
  file = myfilename;
  SetName(myname);
};

Page::~Page() {};

int Page::LoadXmlPageFile()
{
  xmlNodePtr cur;
  xmlChar * value;
  doc = xmlParseFile((const char*)file.GetFullPath().mb_str());
  if(doc == NULL)
    {
      std::cout << "Error parsing file " << (const char *)file.GetFullPath().mb_str() << std::endl;
      return 0;
    }
  cur = xmlDocGetRootElement(doc);
  if(xmlStrcmp(cur->name, (const xmlChar *)"page") != 0)
    {
      xmlFreeDoc(doc);
      return 0;
    }
  cur = cur->children;
  while(cur != NULL)
    {
      if(!xmlStrcmp(cur->name, (const xmlChar *)"name"))
        {
          value = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
          wxString temp((const char*)value, wxConvUTF8);
          SetName(temp);
          std::cout << "Added name ..." << std::endl;
          xmlFree(value);
        }
      else if(!xmlStrcmp(cur->name, (const xmlChar *)"description"))
        {
          value = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
          wxString temp((const char*)value, wxConvUTF8);
          SetDescription(temp);
          std::cout << "Added description ..." << std::endl;
          xmlFree(value);
        }
      else if(!xmlStrcmp(cur->name, (const xmlChar *)"template"))
        {
          xmlChar *prop;
          if ((prop = xmlGetProp(cur, (const xmlChar*)"uri")) != NULL)
            {
              //do something
            }
          else
            {
              std::cout << "Couldn\'t find template" << std::endl;
            }
        }
      else if(!xmlStrcmp(cur->name, (const xmlChar *)"layout"))
        {
          xmlChar *prop;
          if ((prop = xmlGetProp(cur, (const xmlChar*)"uri")) != NULL)
            {
              //do something
            }
          else
            {
              std::cout << "Couldn\'t find layout" << std::endl;
            }
        }
      else if(!xmlStrcmp(cur->name,(const xmlChar *)"blocks"))
        {
          std::cout << "Processing blocks ..." << std::endl;
          ProcessBlocks(doc, cur);
        }
      cur = cur->next;
    }
  return 1;
};

int Page::SaveXmlPageFile() {
	xmlSaveFormatFile((const char*)file.GetFullPath().mb_str(),doc,1);
}

int Page::UnloadXmlPageFile() {
	xmlFreeDoc(doc);
}

int Page::ProcessBlocks(xmlDocPtr doc, xmlNodePtr cur)
{
  /*
   * what should happen here:
   * create a "Block" object for every <plugin> element we encounter
   * same for groups
   * add them to the list of plugins, groups etc of this Page object
   * 
   * 
   */
  cur = cur->xmlChildrenNode;
  while(cur != NULL){
    if(!xmlStrcmp(cur->name, (xmlChar *) "plugin")){
      Block * t;
      t = new Block();
      t->Load(cur);
      blocks.Append(t); // t doesn't get copied, pointer just gets attached, that's why we can't use vars on stack
    }
    cur = cur->next;
  }
  return 1;
}

int Page::ProcessTemplate(xmlDocPtr doc, xmlNodePtr cur)
{
  xmlChar * prop;

  prop = xmlGetProp(cur, (xmlChar *) "uri");
  if(prop == NULL)
    return 0;
  wxString temp((const char*) prop , wxConvUTF8);
  templatefile = temp;
  templates = xmlParseFile((char *) prop);
  return 1;
}

int Page::SaveTemplate(void)
{
  if(templatefile)
    {
      xmlSaveFormatFile((const char*)templatefile.mb_str(), templates, 1);
      return true;
    }
  else
    return false; //failure
}

int Page::LoadTemplate(xmlChar * filename)
{
  xmlNodePtr cur, newnode;
  bool setit = false;
  //they want us to load another template !!!
  // first check if we've allready loaded one
  if(templatefile)
    {
      xmlFreeDoc(templates);
      delete templatefile;
    }
  wxString temp((const char*) filename, wxConvUTF8);
  templatefile = temp;
  templates = xmlParseFile((char *) filename);


  // now we have to change the <template> in the <page>
  cur = xmlDocGetRootElement(doc);
  if(xmlStrcmp(cur->name, (const xmlChar *)"page") != 0)
    {
      xmlFreeDoc(doc);
      return 0;
    }
  cur = cur->children;
  while(cur != NULL)
    {
      if(!xmlStrcmp(cur->name, (const xmlChar *)"template"))
        {
          xmlSetProp(cur, (xmlChar *) "uri", filename);
          setit = true;
          break;
        }
      cur = cur->next;
    }
  if(!setit){
    // seems there was no <template>, make a new one
    cur = xmlDocGetRootElement(doc);
    newnode = xmlNewNode(NULL, (xmlChar *) "template");
    xmlAddChild(cur, newnode);
  }
  return true;
}

void Page::SetName(wxString& myname) {
	myname.Trim(FALSE);myname.Trim(TRUE);myname.Truncate(100);
	if(!myname.IsEmpty()) {
	       	name = myname;
	} else {
		name = file.GetFullName();
	}
}
void Page::SetDescription(wxString& mydescription) {
	mydescription.Trim(FALSE);mydescription.Trim(TRUE);mydescription.Truncate(1024);
	description = mydescription;
}
wxString *Page::GetName() { return &name; }
wxString *Page::GetDescription() { return &description; }

