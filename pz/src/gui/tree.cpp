/*
 * Project Zero - All Rights Reserved
 * tree.cpp
 */
 
#include "../../img/proj.xpm"
#include "../../img/page.xpm"
#include "../../img/open.xpm"
#include "globals.h"
#include "app.h"
#include "block.h"
#include "intern.h"
#include "projectfile.h"
#include "tree.h"

BEGIN_EVENT_TABLE(ProjectTreeCtrl, wxTreeCtrl)
  EVT_TREE_SEL_CHANGED(-1, ProjectTreeCtrl::SelChanged)
END_EVENT_TABLE()

ProjectTreeCtrl::ProjectTreeCtrl(wxWindow *parent) : wxTreeCtrl(parent, -1, wxDefaultPosition, wxDefaultSize
#ifndef __WXMSW__
, wxNO_BORDER|wxTR_HAS_BUTTONS|wxTR_TWIST_BUTTONS|wxTR_NO_LINES
#endif
) {
#ifndef __WXMSW__
	SetIndent(GetIndent()-5);
	SetSpacing(GetSpacing()-5);
#endif
	CreateImageList();
}

void ProjectTreeCtrl::ReFill() {
	DeleteAllItems();
	wxTreeItemId rootId = AddRoot(_("Project"));
	SetItemImage(rootId, ICON_PROJECT, wxTreeItemIcon_Normal);
	SetItemImage(rootId, ICON_PROJECT, wxTreeItemIcon_Selected);
	SetItemImage(rootId, ICON_OPEN, wxTreeItemIcon_Expanded);
	SetItemImage(rootId, ICON_OPEN, wxTreeItemIcon_SelectedExpanded);
	SetItemData(rootId, new TreeBinding(PROJECT, (void *) ws::curproj));
	for(size_t i =0;i<ws::curproj->GetPageCount();i++)
	{
		Page *page = ws::curproj->GetPage(i);
		wxTreeItemId id = AppendItem(rootId, *(page->GetName()));
		SetItemImage(id, ICON_PAGE, wxTreeItemIcon_Normal);
		SetItemImage(id, ICON_PAGE, wxTreeItemIcon_Selected);
		SetItemImage(id, ICON_PAGE, wxTreeItemIcon_Expanded);
		SetItemImage(id, ICON_PAGE, wxTreeItemIcon_SelectedExpanded);
		SetItemData(id, new TreeBinding(PAGE, (void *) page));
	}

	if(ItemHasChildren(rootId)) { Expand(rootId); }

}

void ProjectTreeCtrl::CreateImageList() {
	int size = 16;
	wxImageList *images = new wxImageList(size, size, TRUE);
	wxBusyCursor wait;
	wxIcon icons[3];
	icons[ICON_PROJECT] = wxIcon(proj_xpm);
	icons[ICON_PAGE] = wxIcon(page_xpm);
	icons[ICON_OPEN] = wxIcon(open_xpm);
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

void ProjectTreeCtrl::SelChanged(wxTreeEvent& event){
	wxTreeItemId idd;
	TreeBinding * bind;
	printf("Tree select changed\n");
	if(!(idd = (wxTreeItemId) event.GetItem()))
		return;
	if(!(bind = (TreeBinding *) this->GetItemData(idd)))
		return;
	switch( bind->type){
	case PAGE:
		ws::mainwin->ReplaceRight(new PageViewer(ws::mainwin->splitter, (Page *) bind->data));
		break;
	case PROJECT:
		wxWindow *win = new wxWindow(ws::mainwin->splitter, -1);
		win->SetBackgroundColour(wxColour(165, 165, 165));
		ws::mainwin->ReplaceRight(win);
	        break;
  }
 
}

TreeBinding::TreeBinding(int typea, void * dataa){
	data = dataa;
	type = typea;
}
