/*
 * Project Zero - All Right Reserved
 * app.h 
 */

#ifndef DEF_APP_H
#define DEF_APP_H

#include "tree.h"
#include "draw.h"

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
	DECLARE_EVENT_TABLE()
private:
	wxSplitterWindow *splitter, *leftsplitter;
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

#endif // DEF_APP_H


