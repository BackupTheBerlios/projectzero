/*
 * Project Zero - All Rights Reserved
 * intern.h
 */

#ifndef DEF_INTERN_H
#define DEF_INTERN_H
// style
class StyleSheet
{
public:
	StyleSheet(){};
	~StyleSheet(){};
private:
	wxString file;
	unsigned long id;
	// add real content here
	// ...
};
WX_DECLARE_LIST(StyleSheet, StyleSheetList);

// elements
class PageElement
{
public:
	PageElement(){};
	virtual ~PageElement(){};
protected:
	wxString name;
	unsigned long id;
	// ...
};
WX_DECLARE_LIST(PageElement, PageElementList);

class SubPagesElement : public PageElement
{
public:
	SubPagesElement(){};
	~SubPagesElement(){};
private:
	// list of pageid's
	// ...
};

class PhpElement : public PageElement
{
public:
	PhpElement(){};
	~PhpElement(){};
private:
	wxString type;
	//PhpElementOptions options;
	// ...
};

// page
class Page
{
public:
	Page();
  ~Page();
	int LoadXmlPageFile(wxString& filename);
	void SetName(wxString& myname);
	wxString GetName();
	int SaveTemplate(void);
	int LoadTemplate(xmlChar * filename);
	wxString name;
	wxString descriptions;
private:
  int ProcessBlocks(xmlDocPtr doc, xmlNodePtr cur);
  int ProcessTemplate(xmlDocPtr doc, xmlNodePtr cur);

	wxString file;
	unsigned long id;

	unsigned long styleid;
	wxString layout;
	xmlDocPtr doc;
	xmlDocPtr templates;
	wxString templatefile;
	BlockList blocks;
	// page layout (tables)

	// ...

};

WX_DECLARE_LIST(Page, PageList);



#endif // DEF_INTERN_H

