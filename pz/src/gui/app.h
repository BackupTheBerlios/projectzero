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
	ID_QUIT,
	ID_ABOUT,
	ID_TEST,
	ID_EXAMPLE
};

class MainFrame : public wxFrame {
public:
	MainFrame(wxFrame *frame, wxString title, const wxPoint& pos, const wxSize& size);
	~MainFrame(void);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnTest(wxCommandEvent& event);
	void OnExample(wxCommandEvent& event);
	wxSplitterWindow * splitter;
 	wxWindow * rightwin;
 	DECLARE_EVENT_TABLE()
private:
	wxSplitterWindow  *leftsplitter;
//	DrawWindow *dw;
  PageViewer *dw;
  wxStaticText *description;
	ProjectTreeCtrl *projecttree;
};
 
class MainApp : public wxApp {
public:
	MainFrame *frame;
	virtual bool OnInit(void);
};

MainFrame * mainwin;  // global variable that holds pointer to mainframe

#endif // DEF_APP_H


