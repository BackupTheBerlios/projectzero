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

#include <wx/stattext.h>
#include <wx/valtext.h>
#include "pageviewer.h"

#define ID_NAME 0x6001
#define ID_DESCRIPTION 0x6002

BEGIN_EVENT_TABLE(PageViewer, wxNotebook)
  EVT_TEXT(ID_NAME, PageViewer::NameChange)
  EVT_TEXT(ID_DESCRIPTION, PageViewer::DescChange)
END_EVENT_TABLE()


PageViewer::PageViewer(wxWindow * parent, Page * apage) : wxNotebook::wxNotebook(parent, -1, wxDefaultPosition, wxDefaultSize, 0, "tabCtrl"){
  mypage = apage;
  prop = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, "panel");
  layout = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, "panel");
  dd = new DrawData();
  templates = new DrawWindow(this, dd );
  InsertPage(0, prop, wxT("Page"),  false, -1);
  InsertPage(1, layout, wxT("Layout"),  false, -1);
  InsertPage(2, templates, wxT("Template"),  false, -1);
  
  /*
   * Properties tab
   */
  
  wxBoxSizer *propsizerha = new wxBoxSizer( wxHORIZONTAL );
  
  propsizerha->Add(
    new wxStaticText(prop, -1, wxT("Name:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT, "Name"),
    1,
    wxALL,
    10);
  
  propsizerha->Add(
    new wxTextCtrl(prop, ID_NAME),
    1,
    wxALL,
    10);
    
  wxBoxSizer *propsizerhb = new wxBoxSizer( wxHORIZONTAL );
  
  propsizerhb->Add(
    new wxStaticText(prop, -1, wxT("Description:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT, "Name"),
    1,
    wxALL,
    10);
  
  propsizerhb->Add(
    new wxTextCtrl(prop, ID_DESCRIPTION),
    1,
    wxALL,
    10);
  
  wxBoxSizer *propsizer = new wxBoxSizer( wxVERTICAL );
  
  propsizer->Add(
    propsizerha,
    0,
    wxALL | wxEXPAND,
    10);
  
  propsizer->Add(
    propsizerhb,
    0,
    wxALL | wxEXPAND,
    10);
    
  prop->SetSizer(propsizer);
    
  /*
   * Layout window  --->>> consider making a layout designer
   */
   
  FindWindow(ID_NAME)->SetValidator(wxTextValidator(wxFILTER_ASCII, &(mypage->name)));
  FindWindow(ID_DESCRIPTION)->SetValidator(wxTextValidator(wxFILTER_ASCII, &(mypage->descriptions)));
   
  
}

PageViewer::~PageViewer(){

}

void PageViewer::NameChange(wxEvent& event){

}

void PageViewer::DescChange(wxEvent& event){

}
