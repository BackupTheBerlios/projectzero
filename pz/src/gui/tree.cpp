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

ProjectTreeCtrl::ProjectTreeCtrl(wxWindow *parent) : wxTreeCtrl(parent, -1/*, wxDefaultPosition, wxDefaultSize, wxNO_BORDER*/) {
	SetIndent(GetIndent()-5);
	SetSpacing(GetSpacing()-5);
	CreateImageList();
}

/*void ProjectTreeCtrl::TestFill() {
	wxTreeItemId rootId = AddRoot(_("Project"));
	SetItemImage(rootId, ICON_PROJECT, wxTreeItemIcon_Normal);
	wxTreeItemId id1 = AppendItem(rootId, _("Webpage 1"));
	SetItemImage(id1, ICON_PAGE, wxTreeItemIcon_Normal);
	wxTreeItemId id2 = AppendItem(rootId, _("Webpage 2"));
	SetItemImage(id2, ICON_PAGE, wxTreeItemIcon_Normal);
	wxTreeItemId id3 = AppendItem(rootId, _("Contact"));
	SetItemImage(id3, ICON_PAGE, wxTreeItemIcon_Normal);
	wxTreeItemId id4 = AppendItem(rootId, _("Links 38"));
	SetItemImage(id4, ICON_PAGE, wxTreeItemIcon_Normal);
}*/

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
