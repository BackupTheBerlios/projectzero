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
	Page();
	~Page();
	int LoadXmlPageFile(wxString& filename);
	int SaveTemplate(void);
	int LoadTemplate(xmlChar * filename);
	void SetName(wxString& myname);
	void SetDescription(wxString& mydescription);
	wxString *GetName();
	wxString *GetDescription();
private:
	int ProcessBlocks(xmlDocPtr doc, xmlNodePtr cur);
	int ProcessTemplate(xmlDocPtr doc, xmlNodePtr cur);
	wxString name;
	wxString description;
	wxString file;
	wxString layout;
	xmlDocPtr doc;
	xmlDocPtr templates;
	wxString templatefile;
	BlockList blocks;
};
WX_DECLARE_LIST(Page, PageList);

#endif // DEF_INTERN_H

