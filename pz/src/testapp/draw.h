/*
 * Project Zero - All Rights Reserved
 *
 */

#ifndef DEF_DRAW_H
#define DEF_DRAW_H

#define CANVAS_WIDTH 400
#define CANVAS_HEIGHT 320
class DrawWindow : wxScrolledWindow {
	public:
		int m_drag_x, m_drag_y, m_drop_x, m_drop_y, m_snap_x, m_snap_y;
		bool OnInit();
		void OnPaint(wxPaintEvent &);
		void DrawWindow::OnLeftDown(wxMouseEvent& event);
		void DrawWindow::OnLeftUp(wxMouseEvent& event);
		void DrawWindow::OnMouseMotion(wxMouseEvent& event);
		void DrawWindow::OnEraseBackground(wxEraseEvent& event);
		DrawWindow(wxWindow * parent);
		void Redraw();
		class Line * lines;
	DECLARE_EVENT_TABLE()
	protected:
		void DrawWindow::DrawLine(wxDC& dc, int xa, int ya, int xb, int yb);
		void DrawWindow::DrawCanvas(wxDC& dc);
		void DrawWindow::DrawLines(wxDC& dc);	
		void DrawWindow::DrawDragLines(wxDC& dc);
		bool DrawWindow::IsInCanvas(int x, int y);
		wxPoint DrawWindow::Dev2Canvas(int x, int y);
		wxPoint DrawWindow::Canvas2Dev(int x, int y);
		wxPoint * DrawWindow::FindSnap(wxPoint);
		bool DrawWindow::CornerSnap(wxPoint a, wxPoint b);
		bool draggingline; bool gotsnap; bool gotsourcesnap;
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
