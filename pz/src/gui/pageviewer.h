/*
 * Project Zero - All Rights Reserved
 *
 */

#ifndef DEF_PAGEVIEWER_H
#define DEF_PAGEVIEWER_H


class PageViewer : public wxNotebook {
public:
	PageViewer(wxWindow * parent, Page * apage);
	~PageViewer();
	void PageViewer::NameChange(wxCommandEvent& event);
	void PageViewer::DescChange(wxCommandEvent& event);
private:
	DrawData * dd;
	wxPanel * prop;
	wxPanel * layout;
	wxPanel * templates;
	DrawWindow * dw;
	Page * mypage;
DECLARE_EVENT_TABLE()
};

#endif
