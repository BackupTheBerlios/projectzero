/*
 * Project Zero - All Right Reserved
 * app.cpp 
 */

#include "../../img/mondrian.xpm"

#include "globals.h"
#include "intern.h"
#include "projectfile.h"
#include "draw.h"
#include "tree.h"
#include <wx/notebook.h>
#include "pageviewer.h"
#include "app.h"


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_QUIT, MainFrame::OnQuit)
	EVT_MENU(ID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(ID_TEST, MainFrame::OnTest)
	EVT_MENU(ID_EXAMPLE, MainFrame::OnExample)
END_EVENT_TABLE()

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit() {
	// to remember that this should be calles once at program startup.
	//xmlInitParser();
	frame = new MainFrame((wxFrame*)NULL, _("Project Zero"), wxPoint(1,1), wxSize(750, 600));
	frame->Show(TRUE);
	SetTopWindow(frame);
	return true;
}

MainFrame::MainFrame(wxFrame *frame, wxString title,  const wxPoint& pos, const wxSize& size) : wxFrame(frame, -1, title,  pos, size)
{
	SetIcon(wxICON(mondrian));
	/*wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND)*/
	SetBackgroundColour(wxColour(165, 165, 165));

	wxMenu *file_menu = new wxMenu;
	file_menu->Append(ID_TEST, _("&Run Test ..."));
	file_menu->Append(ID_EXAMPLE, _("Open &Example"));
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

	splitter = new wxSplitterWindow(this, -1, wxDefaultPosition, wxDefaultSize, /*wxSP_LIVE_UPDATE |*/ wxCLIP_CHILDREN | wxSP_3DSASH);

	leftsplitter = new wxSplitterWindow(splitter, -1, wxDefaultPosition, wxDefaultSize, /*wxSP_LIVE_UPDATE |*/ wxCLIP_CHILDREN | wxSP_3DSASH);
	projecttree = new ProjectTreeCtrl(leftsplitter);
	description = new wxStaticText(leftsplitter, -1,_("Put some text here.\nBla Bla Bla."));
	leftsplitter->SplitHorizontally(projecttree, description, 400);
	leftsplitter->SetMinimumPaneSize(50);
/*	dw = new DrawWindow(splitter);
	dw->Redraw();*/
	dw = new PageViewer(splitter, new Page());
	dw->Refresh();

	splitter->SplitVertically(leftsplitter, dw, 200);
	splitter->SetMinimumPaneSize(50);
}



MainFrame::~MainFrame(void) {
	//empty	
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(TRUE);
}


void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	(void)wxMessageBox(_("Project Zero, 2003."), _("About"));
}

void MainFrame::OnTest(wxCommandEvent& WXUNUSED(event)) {
        Project proj;
	wxFileDialog dialog (this, _("Open Project File"), _(""), _(""),
	wxT("Xml Project Files (*.xml)|*.xml"));
	dialog.SetDirectory(wxGetHomeDir());
	if(dialog.ShowModal() == wxID_OK)
	{
		wxString filename = dialog.GetPath();
		wxString dir = dialog.GetDirectory();
		proj.LoadXmlProjectFile(filename, dir);
		projecttree->Fill(proj);
	}
}

void MainFrame::OnExample(wxCommandEvent& WXUNUSED(event)) {
        Project proj;
	wxString filename = wxT("../../examples/proj.xml");
	wxString dir = wxT("../../examples/");
	proj.LoadXmlProjectFile(filename, dir);
	projecttree->Fill(proj);
}

