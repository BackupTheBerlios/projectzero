/*
 * Project Zero - All Rights Reserved
 * projectfile.h
 */

#ifndef DEF_PROJECTFILE_H
#define DEF_PROJECTFILE_H
class Project
{
public:
	Project();
	~Project();
	// TODO: add enum for errors
	int LoadXmlProjectFile(wxFileName& filename);
	int SaveXmlProjectFile();
	void SetName(wxString& name);
	void SetDescription(wxString& filename);
	int AddPage(wxString& filename, wxString& name);
	size_t GetPageCount();
	Page *GetPage(size_t position);
private:
	void ParsePage(xmlNodePtr cur);
	PageList pages;
	wxString projname;
	wxString projdescription;
	wxFileName projfile;
};
#endif // DEF_PROJECTFILE_H

