/*
 * Project Zero - All Right Reserved
 * app.h 
 */

#ifndef DEF_APP_H
#define DEF_APP_H

#include "block.h"
#include "intern.h"
#include "projectfile.h"
#include "draw.h"
#include "tree.h"
#include "pageviewer.h"


enum
{
// menu id's
	ID_QUIT,
	ID_ABOUT,
	ID_OPENPROJECT,
	ID_CLOSEPROJECT,
	ID_SAVEPROJECT,
	ID_EXAMPLE
};

class MainFrame : public wxFrame {
public:
	MainFrame(wxFrame *frame, wxString title, const wxPoint& pos, const wxSize& size);
	~MainFrame(void);
	void ReplaceRight(wxWindow * newwin);
	void ChangeCurrentPageName(wxString& newname);
	wxSplitterWindow *splitter;
private:
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpenProject(wxCommandEvent& event);
	void OnCloseProject(wxCommandEvent& event);
	void OnSaveProject(wxCommandEvent& event);
	void OnExample(wxCommandEvent& event);

	void CreateSplitters();
	void LoadProject(wxFileName& filename);
	void UnLoadProject();

	wxSplitterWindow  *leftsplitter;
	wxStaticText *description;
	ProjectTreeCtrl *projecttree;
 	wxWindow *rightwin;

DECLARE_EVENT_TABLE()
};
 
class MainApp : public wxApp {
public:
	MainFrame *frame;
	virtual bool OnInit(void);
};

namespace ws {
	extern MainFrame * mainwin; 
	extern Project * curproj;
};

#endif // DEF_APP_H


