/*
 * Project Zero - All Rights Reserved
 *
 */
 
#include "globals.h"
#include "block.h"
#include "intern.h"
#include "projectfile.h"
#include "draw.h"
#include "tree.h"
#include "app.h"

#include <wx/stattext.h>
#include <wx/valtext.h>
#include "pageviewer.h"

#define ID_NAME 0x6001
#define ID_DESCRIPTION 0x6002

BEGIN_EVENT_TABLE(PageViewer, wxNotebook)
  EVT_TEXT(ID_NAME, PageViewer::NameChange)
  EVT_TEXT(ID_DESCRIPTION, PageViewer::DescChange)
END_EVENT_TABLE()


PageViewer::PageViewer(wxWindow * parent, Page * apage) : wxNotebook::wxNotebook(parent, -1, wxDefaultPosition, wxDefaultSize, 0, wxT("tabCtrl")){
  mypage = apage;
  prop = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("panel"));
  layout = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("panel"));
  dd = new DrawData();
  templates = new DrawWindow(this, dd );
  InsertPage(0, prop, wxT("Page"),  false, -1);
  InsertPage(1, layout, wxT("Layout"),  false, -1);
  InsertPage(2, templates, wxT("Template"),  false, -1);
  
  // Properties tab
  
  wxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
  wxStaticBox *genbox = new wxStaticBox(prop, -1, _("&General"));
  wxSizer *vertboxsizer = new wxStaticBoxSizer(genbox, wxVERTICAL);
  wxSizer *rowsizer = new wxBoxSizer(wxHORIZONTAL);
  rowsizer->Add(new wxStaticText(prop, -1, _("Name:")), 0, wxRIGHT, 5);
  wxTextCtrl *text = new wxTextCtrl(prop, ID_NAME, *(mypage->GetName()));
  text->SetMaxLength(100);
  rowsizer->Add(text, 1, wxLEFT, 5);
  vertboxsizer->Add(rowsizer, 0, wxALL | wxGROW, 5);
  rowsizer = new wxBoxSizer(wxHORIZONTAL);
  rowsizer->Add(new wxStaticText(prop, -1, _("Description:")), 0, wxRIGHT, 5);
  text = new wxTextCtrl(prop, ID_DESCRIPTION, *(mypage->GetDescription()), wxDefaultPosition, wxSize(0,50), wxTE_MULTILINE);
  text->SetMaxLength(1024);
  rowsizer->Add(text, 1, wxLEFT, 5);
  vertboxsizer->Add(rowsizer, 0, wxALL | wxGROW, 5);
  topsizer->Add(vertboxsizer, 0, wxGROW | wxALL, 10);
  prop->SetSizer(topsizer);
  FindWindow(ID_NAME)->SetValidator(wxTextValidator(wxFILTER_ASCII, mypage->GetName()));
  FindWindow(ID_DESCRIPTION)->SetValidator(wxTextValidator(wxFILTER_ASCII, mypage->GetDescription()));
    
  //Layout window  --->>> consider making a layout designer
   
}

PageViewer::~PageViewer() {}

void PageViewer::NameChange(wxCommandEvent& event){
	wxString newname = event.GetString();
	mypage->SetName(newname);
	ws::mainwin->ChangeCurrentPageName(newname);
}

void PageViewer::DescChange(wxCommandEvent& event){
	wxString newdescr = event.GetString();
	mypage->SetDescription(newdescr);
}

