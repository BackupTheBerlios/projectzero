/*
 * Project Zero - All Right Reserved
 * app.cpp 
 */

#include "../../img/mondrian.xpm"

#include "globals.h"
#include "block.h"
#include "intern.h"
#include "projectfile.h"
#include "draw.h"
#include "tree.h"
#include <wx/notebook.h>
#include "pageviewer.h"
#include "app.h"

namespace ws {
  MainFrame * mainwin;  // global variable that holds pointer to mainframe
  Project *curproj;
}; 

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_QUIT, MainFrame::OnQuit)
	EVT_MENU(ID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(ID_OPENPROJECT, MainFrame::OnOpenProject)
	EVT_MENU(ID_CLOSEPROJECT, MainFrame::OnCloseProject)
	EVT_MENU(ID_EXAMPLE, MainFrame::OnExample)
END_EVENT_TABLE()

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit() {
	// Init Xml Parser, because we have a multithreaded program
	xmlInitParser();
	ws::mainwin = new MainFrame((wxFrame*)NULL, _("Project Zero"), wxPoint(1,1), wxSize(750, 600));
	ws::mainwin->Show(TRUE);
	SetTopWindow(frame);
	return true;
}

MainFrame::MainFrame(wxFrame *frame, wxString title,  const wxPoint& pos, const wxSize& size) : wxFrame(frame, -1, title,  pos, size)
{
	SetIcon(wxICON(mondrian));
	/*wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND)*/
	SetBackgroundColour(wxColour(165, 165, 165));

	ws::mainwin = this;
	wxMenu *file_menu = new wxMenu;
	file_menu->Append(ID_OPENPROJECT, _("&Open Project ..."));
	file_menu->Append(ID_EXAMPLE, _("Open &Test Project"));
	file_menu->Append(ID_CLOSEPROJECT, _("&Close Project"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_QUIT, _("&Quit"));
	wxMenu *help_menu = new wxMenu;
	help_menu->Append(ID_ABOUT, _("&Help"));
	help_menu->AppendSeparator();
	help_menu->Append(ID_ABOUT, _("&About ..."));
	wxMenuBar *menu_bar = new wxMenuBar;
	menu_bar->Append(file_menu, _("&File"));
	menu_bar->Append(help_menu, _("&Help"));
	SetMenuBar(menu_bar);
}

void MainFrame::CreateSplitters() {
	splitter = new wxSplitterWindow(this, -1, wxDefaultPosition, wxDefaultSize, /*wxSP_LIVE_UPDATE |*/ wxCLIP_CHILDREN | wxSP_3DSASH);

	leftsplitter = new wxSplitterWindow(splitter, -1, wxDefaultPosition, wxDefaultSize, /*wxSP_LIVE_UPDATE |*/ wxCLIP_CHILDREN | wxSP_3DSASH);
	projecttree = new ProjectTreeCtrl(leftsplitter);
	description = new wxStaticText(leftsplitter, -1,_("ProjectZero / Webstruct v0.00\n2003"));
	leftsplitter->SplitHorizontally(projecttree, description, 400);
	leftsplitter->SetMinimumPaneSize(50);
	rightwin = new wxWindow(splitter, -1);
	rightwin->SetBackgroundColour(wxColour(165, 165, 165));
	splitter->SplitVertically(leftsplitter, rightwin, 200);
	splitter->SetMinimumPaneSize(50);

}

MainFrame::~MainFrame(void) {
	if(ws::curproj!=NULL) delete ws::curproj;
}

void MainFrame::ReplaceRight(wxWindow * newwin){
  splitter->ReplaceWindow(rightwin, newwin);
  delete rightwin;
  rightwin = newwin;
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(TRUE);
}


void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	(void)wxMessageBox(_("Project Zero, 2003."), _("About"), wxOK, ws::mainwin);
}

void MainFrame::LoadProject(wxString& filename, wxString& path) {
	UnLoadProject();
	ws::curproj = new Project();
	if(ws::curproj->LoadXmlProjectFile(filename, path)) {
		if(splitter==NULL) CreateSplitters();
		projecttree->ReFill();
		this->Refresh();
	} else {
		UnLoadProject();
	}
}

void MainFrame::UnLoadProject() {
	// TODO: disable all functionality, expect for loading projects, etc .
	// this solution will work, but isn't really how it should be, i think*
	if(!(splitter==NULL)) { delete splitter; splitter = NULL; }
	if(!(ws::curproj==NULL)) { delete ws::curproj; ws::curproj = NULL; }
}

// XXX this is not the way it is supposed to be; we will probably need to rethink
// how to change the controls after data in a page or project object has changed
// maybe we need to give them an id, or use wxTreeItemData, ...
void MainFrame::ChangeCurrentPageName(wxString& newname) {
	projecttree->SetItemText(projecttree->GetSelection(),newname);
}

void MainFrame::OnCloseProject(wxCommandEvent& WXUNUSED(event)) {
	if(wxMessageBox( _("Do you really want to close the currently opened project?"), _("Warning"), wxYES_NO, ws::mainwin) == wxYES) {
		UnLoadProject();
	}
}

void MainFrame::OnOpenProject(wxCommandEvent& WXUNUSED(event)) {
	if(ws::curproj!=NULL) {
		if(wxMessageBox( _("Do you really want to close the currently opened project?"), _("Warning"), wxYES_NO, ws::mainwin) == wxYES) {
			return;
		}
	}
	wxFileDialog dialog (this, _("Open Project File"), _(""), _(""),
	wxT("Xml Project Files (*.xml)|*.xml"));
	dialog.SetDirectory(wxGetHomeDir());
	if(dialog.ShowModal() == wxID_OK) {
		UnLoadProject();
		wxString filename = dialog.GetPath();
		wxString dir = dialog.GetDirectory();
		LoadProject(filename, dir);
	}
}

void MainFrame::OnExample(wxCommandEvent& WXUNUSED(event)) {
	UnLoadProject();
	wxString filename = wxT("../../examples/proj.xml");
	wxString dir = wxT("../../examples/");
	LoadProject(filename, dir);
}

