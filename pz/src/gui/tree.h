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
private:
	void CreateImageList(void);
};
#endif //DEF_TREE_H
