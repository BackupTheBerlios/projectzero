/*
 * Project Zero - All Rights Reserved
 * tree.h
 */

#ifndef DEF_TREE_H
#define DEF_TREE_H

enum TreeIcons {
	ICON_PAGE,
	ICON_PROJECT
};

class ProjectTreeCtrl : public wxTreeCtrl {
public:
	ProjectTreeCtrl(wxWindow *parent);
	//void TestFill();
	void Fill(Project& project);
  DECLARE_EVENT_TABLE()
private:
  void SelChanged(wxTreeEvent&);
	void CreateImageList(void);
};

#define PAGE 0x0
#define PROJECT 0x1

class TreeBinding : public wxTreeItemData {
  public:
  TreeBinding::TreeBinding(int, void *);
  int type;
  void * data;
};

#endif //DEF_TREE_H


