/*
 * Project Zero - All Rights Reserved
 * intern.h
 */

#ifndef DEF_INTERN_H
#define DEF_INTERN_H

// page
class Page
{
public:
	Page(wxFileName& myfilename, wxString& myname);
	~Page();
	int LoadXmlPageFile();
	int SaveXmlPageFile();
	int UnloadXmlPageFile();
	int SaveTemplate(void);
	int LoadTemplate(xmlChar * filename);
	void SetName(wxString& myname);
	void SetDescription(wxString& mydescription);
	wxString *GetName();
	wxString *GetDescription();
	wxFileName *GetFileName();
private:
	int ProcessBlocks(xmlDocPtr doc, xmlNodePtr cur);
	int ProcessTemplate(xmlDocPtr doc, xmlNodePtr cur);
	wxString name;
	wxString description;
	wxFileName file;
	wxString layout;
	xmlDocPtr doc;
	xmlDocPtr templates;
	wxString templatefile;
	BlockList blocks;
};
WX_DECLARE_LIST(Page, PageList);

#endif // DEF_INTERN_H

