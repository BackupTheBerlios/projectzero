/*
 * Project Zero - All Rights Reserved
 * tree.h
 */

#ifndef DEF_TREE_H
#define DEF_TREE_H

enum {
	ICON_PAGE,
	ICON_PROJECT,
	ICON_OPEN
};

class ProjectTreeCtrl : public wxTreeCtrl {
public:
	ProjectTreeCtrl(wxWindow *parent);
	void ReFill();
private:
	void SelChanged(wxTreeEvent&);
	void CreateImageList(void);
DECLARE_EVENT_TABLE()
};

enum {
	PAGE,
	PROJECT
};

class TreeBinding : public wxTreeItemData {
	public:
	TreeBinding::TreeBinding(int, void *);
	int type;
	void * data;
};

#endif //DEF_TREE_H


