/*
	this file is part of the ProjectZero GUI
	All rights reserved
*/

/*
 * NOTE TO MY DUMB SELF:
 * +-----------------> X
 * |
 * |
 * |    Axis orientation
 * |
 * |
 * |  
 * *
 * Y
 *
 */

#include "globals.h"
#include "draw.h"


#define SNAP_DIST 15

BEGIN_EVENT_TABLE(DrawWindow, wxScrolledWindow)
	EVT_PAINT	(DrawWindow::OnPaint)
	EVT_LEFT_DOWN	(DrawWindow::OnLeftDown)
	EVT_LEFT_UP		(DrawWindow::OnLeftUp)
	EVT_MOTION		(DrawWindow::OnMouseMotion)
	EVT_ERASE_BACKGROUND (DrawWindow::OnEraseBackground)
	EVT_INIT_DIALOG	(DrawWindow::OnInit)
END_EVENT_TABLE()


bool DrawWindow::OnInit(){
	wxColour *bgcol = new wxColour(184, 179, 168);
	SetBackgroundColour(*bgcol);
	Refresh();
	m_drag_x = 0;
	m_drag_y = 0;
	m_drop_x = 0;
	m_drop_y = 0;
	m_snap_x = 0; m_snap_y = 0;
	draggingline = false;
	gotsnap = false;
	gotsourcesnap = false;
	SetScrollbars(10, 10, 1000, 1000);
	EnableScrolling(true, true);
	lines = NULL;
	lines = new Line(wxPoint(0, 0), wxPoint(CANVAS_WIDTH, CANVAS_HEIGHT), (class Line *) NULL, lines);
	wxSetCursor(*wxCROSS_CURSOR);
	return true;
}

void DrawWindow::OnEraseBackground(wxEraseEvent& event){
	// do nothing - flicker preventing !!!
}

void DrawWindow::OnPaint(wxPaintEvent& event)
{
	wxMemoryDC dc;
	wxPaintDC pdc(this);
	wxBitmap * bmp;
	int height, width;

	GetSize(&width, &height);
	bmp = new wxBitmap(width, height);
	
	dc.SelectObject(*bmp);

	PrepareDC(pdc);
	dc.BeginDrawing();
	dc.SetBackground(wxBrush(wxColour(184, 179, 168), wxSOLID));
	dc.Clear();
	DrawCanvas(dc);
	DrawLines(dc);	
	DrawDragLines(dc);
	dc.EndDrawing();
	pdc.Blit(0,0,width,height,&dc, 0,0, wxCOPY, FALSE, -1, -1);
	delete(bmp);
}

void DrawWindow::DrawCanvas(wxDC& dc){
	int width, height;
	GetSize(&width, &height);
	dc.SetBrush(wxBrush(wxColour(0x66, 0x66, 0x66), wxSOLID));
	dc.SetPen(wxPen(wxColour(0x66, 0x66, 0x66), 1, wxSOLID));
	dc.DrawRectangle((width - CANVAS_WIDTH)/2 +5 , (height - CANVAS_HEIGHT) /2 +5 , CANVAS_WIDTH, CANVAS_HEIGHT);
	dc.SetBrush(wxBrush(wxColour(0xFF, 0xFF, 0xFF), wxSOLID));
	dc.SetPen(wxPen(wxColour(0xFF, 0xFF, 0xFF), 1, wxSOLID));
	dc.DrawRectangle((width - CANVAS_WIDTH)/2, (height - CANVAS_HEIGHT) /2, CANVAS_WIDTH, CANVAS_HEIGHT);
}

void DrawWindow::DrawLines(wxDC& dc){
	class Line * lp;
	int xa, xb, ya, yb;
	lp = lines;
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxSOLID));
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	while(lp != NULL){
		xa= Canvas2Dev(lp->a.x, lp->a.y).x;
		ya= Canvas2Dev(lp->a.x, lp->a.y).y;
		xb= Canvas2Dev(lp->b.x, lp->b.y).x;
		yb= Canvas2Dev(lp->b.x, lp->b.y).y;
		DrawLine(dc, xa, ya, abs(xb-xa), abs(yb-ya));
		lp = lp->next;
	}
}

void DrawWindow::DrawDragLines(wxDC& dc){
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	if(draggingline){
		dc.SetPen(wxPen(wxColour(0, 0, 0), 2, wxSOLID));
		DrawLine(dc,m_drag_x, m_drag_y, (m_drop_x-m_drag_x), (m_drop_y-m_drag_y));
	}
	dc.SetBrush(wxBrush(wxColour(0x00, 0xFF, 0x0), wxSOLID));
	dc.SetPen(wxPen(wxColour(0x00, 0xFF, 0x0), 1, wxSOLID));
	if (gotsnap){
		dc.DrawCircle(m_drop_x, m_drop_y, 3);
		dc.DrawCircle(m_drag_x, m_drag_y, 3);
    }
   	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(wxPen(wxColour(0xFF, 0x0, 0x0), 2, wxSOLID));
	if (gotsourcesnap)
		dc.DrawCircle(m_snap_x, m_snap_y, 5);
}

void DrawWindow::DrawLine(wxDC& dc, int xa, int ya, int w, int h){
    dc.DrawLine(xa, ya, xa + w, ya + h);
}

bool DrawWindow::IsInCanvas(int x, int y){
	int width, height;
	GetSize(&width, &height);
	if( ( x > (width - CANVAS_WIDTH) /2) && (x < (width + CANVAS_WIDTH) /2 ) &&
		( y > (height - CANVAS_HEIGHT) /2) && (y < (height + CANVAS_HEIGHT) /2 ))
		return true;
	else 
		return false;
}


/*
 * Calculate canvas coördinates from device coordinates
 */

wxPoint DrawWindow::Dev2Canvas(int x, int y){
	int x_c, y_c;
	int width, height;
	GetSize(&width, &height);
	x_c = x - (int) ceil(((double) (width- CANVAS_WIDTH))/2);
	y_c = y - (int) ceil(((double) (height - CANVAS_HEIGHT))/2);
	return wxPoint(x_c, y_c);	
}

wxPoint DrawWindow::Canvas2Dev(int x, int y){
	int x_w, y_w;
	int width, height;
	GetSize(&width, &height);
	x_w = x + (int) ceil(((double) (width - CANVAS_WIDTH))/2);
	y_w = y + (int) ceil(((double) (height - CANVAS_HEIGHT))/2);
	return wxPoint(x_w, y_w);	
}

/* 
 * Find a point to snap to using the given coordinates
 */

wxPoint * DrawWindow::FindSnap(wxPoint in){
	class Line * lp;
	int x, y;
	x=in.x;
	y=in.y;
	lp=lines;
	while(lp != NULL){
		// upper left corner snap
			if( (y < lp->a.y + SNAP_DIST) && (y > lp->a.y - SNAP_DIST) &&
				(x > (lp->a.x - SNAP_DIST)) && (x < (lp->a.x + SNAP_DIST)) )
					return(new wxPoint(lp->a.x, lp->a.y));
		// upper right corner snap
			if( (y < lp->a.y + SNAP_DIST) && (y > lp->a.y - SNAP_DIST) &&
				(x > (lp->b.x - SNAP_DIST)) && (x < (lp->b.x + SNAP_DIST)) )
					return(new wxPoint(lp->b.x, lp->a.y));
		// lower left corner snap
			if( (y < lp->b.y + SNAP_DIST) && (y > lp->b.y - SNAP_DIST) &&
				(x > (lp->a.x - SNAP_DIST)) && (x < (lp->a.x + SNAP_DIST)) )
					return(new wxPoint(lp->a.x, lp->b.y));
		// lower right corner snap
			if( (y < lp->b.y + SNAP_DIST) && (y > lp->b.y - SNAP_DIST) &&
				(x > (lp->b.x - SNAP_DIST)) && (x < (lp->b.x + SNAP_DIST)) )
					return(new wxPoint(lp->b.x, lp->b.y));
		// first horizontal line -- y snapping
			if( (y < lp->a.y + SNAP_DIST) && (y > lp->a.y - SNAP_DIST) &&
				(x > (lp->a.x - SNAP_DIST)) && (x < (lp->b.x + SNAP_DIST)) )
					return(new wxPoint(x, lp->a.y));
		// second horizontal line -- y snapping
			if( (y < lp->b.y + SNAP_DIST) && (y > lp->b.y - SNAP_DIST) &&
				(x > (lp->a.x - SNAP_DIST)) && (x < (lp->b.x + SNAP_DIST)) )
					return(new wxPoint(x, lp->b.y));
		// first vertical line -- x snapping
			if( (y < lp->b.y + SNAP_DIST) && (y > lp->a.y - SNAP_DIST) &&
				(x > (lp->a.x - SNAP_DIST)) && (x < (lp->a.x + SNAP_DIST)) )
					return(new wxPoint(lp->a.x, y));
		// second vertical line -- x snapping
			if( (y < lp->b.y + SNAP_DIST) && (y > lp->a.y - SNAP_DIST) &&
				(x > (lp->b.x - SNAP_DIST)) && (x < (lp->b.x + SNAP_DIST)) )
					return(new wxPoint(lp->b.x, y));
	
		lp = lp->next;
	}
	return NULL;	
}

void DrawWindow::OnLeftDown(wxMouseEvent& event){
	/*if(!IsInCanvas(event.GetX(), event.GetY()))
		return;*/
	if(gotsourcesnap){
		m_drag_x = m_snap_x;
		m_drag_y = m_snap_y;
		gotsourcesnap = false;
		m_drop_x = m_drag_x;
		m_drop_y = m_drag_y;
		draggingline = true;
		Refresh();
	}
}

void DrawWindow::OnLeftUp(wxMouseEvent& event){
	int tmp;
	if(!draggingline)
		return;
	if(!gotsnap){
		draggingline = false;
		Refresh();
		return;
	}
	draggingline = false;
	gotsnap = false;
	printf("New rectangle: (%i %i) (%i %i)\n", m_drag_x, m_drag_y, m_drop_x, m_drop_y);
	if(m_drop_x < m_drag_x){
		tmp = m_drop_x;
		m_drop_x = m_drag_x;
		m_drag_x = tmp;
	}
	if(m_drop_y < m_drag_y){
		tmp = m_drop_y;
		m_drop_y = m_drag_y;
		m_drag_y = tmp;
	}
	lines = new Line(wxPoint(Dev2Canvas(m_drag_x, m_drag_y)), wxPoint(Dev2Canvas(m_drop_x, m_drop_y)), NULL, lines);
	Refresh();
}

bool DrawWindow::CornerSnap(wxPoint a, wxPoint b){
	if( FindSnap(a) && FindSnap(b))
		return true;
	else
		return false;
}

void DrawWindow::OnMouseMotion(wxMouseEvent& event){
	wxPoint * tmp;
	wxPoint tmp2;
	
	if(event.m_leftDown){
		if(!draggingline)
			return;
		if ( CornerSnap(Dev2Canvas(event.GetX(), event.GetY()), Dev2Canvas(m_drag_x, m_drag_y))  ){
			tmp = FindSnap(Dev2Canvas(event.GetX(), event.GetY()));
			tmp2 = Canvas2Dev(tmp->x, tmp->y);
			gotsnap = true;
//			printf("Snapping to: %i %i\n",tmp2.x, tmp2.y);
      if(abs(m_drag_x - m_drop_x) < abs(m_drag_y - m_drop_y)){
         	m_drop_x = m_drag_x;
			    m_drop_y = tmp2.y;
      } else {
         	m_drop_x = tmp2.x;
			    m_drop_y = m_drag_y;      
      }
			delete tmp;
		} else {
			if(!IsInCanvas(event.GetX(), event.GetY()))
				return;
			gotsnap = false;
			m_drop_x = event.GetX();
			m_drop_y = event.GetY();
		}
		Refresh();
	} else {
		tmp = FindSnap(Dev2Canvas(event.GetX(), event.GetY()));
		if(tmp != NULL){
			tmp2 = Canvas2Dev(tmp->x, tmp->y);
			gotsourcesnap = true;
			m_snap_x = tmp2.x;
			m_snap_y = tmp2.y;
			delete tmp;
			Refresh();
		} else {
			gotsourcesnap = false;
		}
	}
}

void DrawWindow::Redraw(){
 	Refresh();
}

DrawWindow::DrawWindow(wxWindow * parent) : wxScrolledWindow::wxScrolledWindow(parent, -1, wxDefaultPosition, wxDefaultSize,/* wxHSCROLL | wxVSCROLL*/0 , wxString::Format(wxT("Draw"))){
	wxColour *bgcol = new wxColour(184, 179, 168);
	SetBackgroundColour(*bgcol);
	Refresh();
	m_drag_x = 0;
	m_drag_y = 0;
	m_drop_x = 0;
	m_drop_y = 0;
	m_snap_x = 0; m_snap_y = 0;
	draggingline = false;
	gotsnap = false;
	gotsourcesnap = false;
	SetScrollbars(10, 10, 1000, 1000);
	//EnableScrolling(true, true);
	lines = new Line(wxPoint(0, 0), wxPoint(CANVAS_WIDTH, 0), (class Line *) NULL, NULL);
	lines = new Line(wxPoint(0, 0), wxPoint(0, CANVAS_HEIGHT), (class Line *) NULL, lines);
  lines = new Line(wxPoint(0, CANVAS_HEIGHT), wxPoint(CANVAS_WIDTH, CANVAS_HEIGHT), (class Line *) NULL, lines);
  lines = new Line(wxPoint(CANVAS_WIDTH, 0), wxPoint(CANVAS_WIDTH, CANVAS_HEIGHT), (class Line *) NULL, lines);
	//wxSetCursor(*wxCROSS_CURSOR);
}

int DrawWindow::LoadLines(xmlDocPtr doc){
  xmlNodePtr root, cur;
  xmlChar * prop;
  int xa, xb, ya, yb;
  Line * tmp;
  
  //remove old lines
  while(lines != NULL)
    delete lines;
  
  root = xmlDocGetRootElement(doc);
  if(xmlStrcmp(root->name, (xmlChar *) "template")){
    printf("Error parsing template\n");
    return 0;
  }
  
  cur = root->xmlChildrenNode;
  while(cur != NULL){
    if(!xmlStrcmp(cur->name, (xmlChar *) "line")){
      prop = xmlGetProp(cur, (xmlChar *) "xa");
      xa = atoi((char *) prop);
      xmlFree(prop);
      prop = xmlGetProp(cur, (xmlChar *) "ya");
      ya = atoi((char *) prop);
      xmlFree(prop);
      prop = xmlGetProp(cur, (xmlChar *) "xb");
      xb = atoi((char *) prop);
      xmlFree(prop);
      prop = xmlGetProp(cur, (xmlChar *) "yb");
      yb = atoi((char *) prop);
      xmlFree(prop);                
    }
    cur = cur->next;
  }

}

void DrawWindow::StoreLines(xmlDocPtr doc){
  xmlNodePtr root, newchild;
  char prop[10];
  Line * tmp;
  
  
  root = xmlNewNode(NULL, (xmlChar *) "template");
  
  xmlDocSetRootElement(doc,root);
  
  tmp = lines;
  while(tmp != NULL){
    newchild = xmlNewNode(NULL, (xmlChar *) "line");
    sprintf( prop, "%i", tmp->a.x);
    xmlNewProp(newchild, (xmlChar *) "xa", (xmlChar *) prop);
    sprintf( prop, "%i", tmp->a.y);
    xmlNewProp(newchild, (xmlChar *) "ya", (xmlChar *) prop);
    sprintf( prop, "%i", tmp->b.x);
    xmlNewProp(newchild, (xmlChar *) "xb", (xmlChar *) prop);
    sprintf(prop, "%i", tmp->b.y);
    xmlNewProp(newchild, (xmlChar *) "yb", (xmlChar *) prop);
    xmlAddChild(root, newchild); 
    tmp = tmp->next;
  }  
  xmlDocDump(fopen("./foobar.txt", "w"), doc);


}


Line::Line(wxPoint ain , wxPoint bin, class Line * pre, class Line * nex){
	a.x = ain.x;
	a.y = ain.y;
	b.x = bin.x;
	b.y = bin.y;
	prev = pre;
	next = nex;
	if (next != NULL)
		next->prev = this;
	if (prev != NULL)
		prev->next = this;
}

Line::~Line(void){
	if(prev != NULL){
		prev->next = next;
	}
	if(next != NULL){
		next->prev = prev;
	}
}

