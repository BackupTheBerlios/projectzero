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
    void PageViewer::NameChange(wxEvent& event);
    void PageViewer::DescChange(wxEvent& event);
    DECLARE_EVENT_TABLE()
  private:
    wxPanel * prop;
    wxPanel * layout;
    wxPanel * templates;
    DrawWindow * dw;
    Page * mypage;
    

};

#endif
