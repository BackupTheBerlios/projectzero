#include <wx/wx.h>


#include <wx/event.h>
#include <wx/toolbar.h>
#include "globals.h"
#include "draw.h"
#include "render.h"
#include "testapp.h"

#include "../../img/height.xpm"

IMPLEMENT_APP(TestApp)

BEGIN_EVENT_TABLE(TestApp, wxApp)
    EVT_TOOL    (1, TestApp::ProcTool)
END_EVENT_TABLE()

bool TestApp::OnInit() {
	wxToolBar * tb;
	frame = new wxFrame( (wxFrame *) NULL, -1, wxT("Testappke"));
	wxFlexGridSizer * topsizer = new wxFlexGridSizer(2,1,0,0);
	tb = frame->CreateToolBar(wxTB_HORIZONTAL, -1, wxT("toolbarry"));

//	frame->SetSizer(topsizer, TRUE);
//	frame->SetAutoLayout(TRUE);
	
/*	topsizer->Add( 
		tb = frame->CreateToolBar(wxTB_HORIZONTAL, -1, "toolbarry"),
		1,
		wxEXPAND | wxALL,
		10, //border
		NULL );
	topsizer->Add( 
		(wxWindow *) (dw = new DrawWindow(frame)),
		1,
		wxEXPAND | wxALL,
		10, //border
		NULL );*/

		tb->AddTool( 1, wxT("test"), wxICON(height), wxT("blah"), wxITEM_NORMAL);
	tb->Realize();
	//frame->SetIcon(wxICON(testicon));
	SetTopWindow(frame);
	dw = new DrawWindow(frame);
	dw->Redraw();
	frame->Show(TRUE);
	return true;
}

void TestApp::ProcTool(wxCommandEvent& WXUNUSED(event)){
   Render * rd;
   rd= new Render();
    (void)wxMessageBox(wxT("wxWindows toolbar sample"), wxT("About wxToolBar"));
  //  (void)rd->doRender(dw->lines, "./blaat.txt");
  (void)rd->doStore(dw->lines);
    delete rd;
}
