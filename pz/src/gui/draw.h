/*
 * Project Zero - All Rights Reserved
 *
 */

#ifndef DEF_DRAW_H
#define DEF_DRAW_H

#define CANVAS_WIDTH 400
#define CANVAS_HEIGHT 320
class DrawWindow : public wxScrolledWindow {
	public:
		int m_drag_x, m_drag_y, m_drop_x, m_drop_y, m_snap_x, m_snap_y;
		bool OnInit();
		void OnPaint(wxPaintEvent &);
		void OnLeftDown(wxMouseEvent& event);
		void OnLeftUp(wxMouseEvent& event);
		void OnMouseMotion(wxMouseEvent& event);
		void OnEraseBackground(wxEraseEvent& event);
    DrawWindow(wxWindow * parent, class DrawData * data);
		void Redraw();
		class Line * lines;
	DECLARE_EVENT_TABLE()
	protected:
		void DrawLine(wxDC& dc, int xa, int ya, int xb, int yb);
		void DrawCanvas(wxDC& dc);
		void DrawLines(wxDC& dc);	
		void DrawDragLines(wxDC& dc);
		bool IsInCanvas(int x, int y);
		wxPoint Dev2Canvas(int x, int y);
		wxPoint Canvas2Dev(int x, int y);
		wxPoint * DrawWindow::FindSnap(wxPoint);
		bool DrawWindow::CornerSnap(wxPoint a, wxPoint b);
		bool draggingline; bool gotsnap; bool gotsourcesnap;
};

class DrawData {
    public:
          DrawData();
          ~DrawData() {};
      		int LoadLines(xmlDocPtr doc);
      		void StoreLines(xmlDocPtr doc);
          class Line * lines;
};

class Line {
	public:
		Line(wxPoint , wxPoint , Line *, Line *);
		~Line(void);
		wxPoint a, b;
		class Line * prev;
		class Line * next;
};
#endif // DEF_DRAW_H

