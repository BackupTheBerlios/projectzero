/*
 * Project Zero - All Rights Reserved
 * tree.cpp
 */
 
#include "../../img/page.xpm"
#include "../../img/proj.xpm"

#include "globals.h"
#include "intern.h"
#include "projectfile.h"
#include "tree.h"

ProjectTreeCtrl::ProjectTreeCtrl(wxWindow *parent) : wxTreeCtrl(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER) {
	SetIndent(GetIndent()-5);
	SetSpacing(GetSpacing()-5);
	CreateImageList();
}

void ProjectTreeCtrl::Fill(Project& project) {
	DeleteAllItems();
	wxTreeItemId rootId = AddRoot(_("Project"));
	SetItemImage(rootId, ICON_PROJECT, wxTreeItemIcon_Normal);
	for(size_t i =0;i<project.GetPageCount();i++)
	{
		Page *page = project.GetPage(i);
		wxString name = page->GetName();
		wxTreeItemId id = AppendItem(rootId, name);
		SetItemImage(id, ICON_PAGE, wxTreeItemIcon_Normal);
	}
	// XXX + disappears because of this - why?
	if(ItemHasChildren(rootId)) { Expand(rootId); }
}

void ProjectTreeCtrl::CreateImageList() {
	int size = 16;
	wxImageList *images = new wxImageList(size, size, TRUE);
	wxBusyCursor wait;
	wxIcon icons[2];
	icons[ICON_PROJECT] = wxIcon(proj_xpm);
	icons[ICON_PAGE] = wxIcon(page_xpm);
	int sizeOrig = icons[0].GetWidth();
	for ( size_t i = 0; i < WXSIZEOF(icons); i++ ) {
		if ( size == sizeOrig ) {
			images->Add(icons[i]);
		}
		else {
			images->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
		}
	}
	AssignImageList(images);
}	

