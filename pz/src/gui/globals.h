#ifndef DEF_GLOBALS_H
#define DEF_GLOBALS_H

// some nasty hacks to prevent <windows.h> problems
// this is a problem in mingw 2.0 with gcc 3.2, fixed in gcc 3.2.1

#ifdef __WXMSW__
	#define wxCHECK_W32API_VERSION(maj,min) (0)
#endif
#include <iostream>
#ifdef __WXMSW__
	#include <wx/msw/winundef.h>
#endif
#include <string.h>
#ifdef __WXMSW__
	#include <wx/msw/winundef.h>
#endif
#include <math.h>
#ifdef __WXMSW__
	#include <wx/msw/winundef.h>
#endif
#include <unistd.h>
#ifdef __WXMSW__
	#include <wx/msw/winundef.h>
#endif
#include <sys/stat.h>
#ifdef __WXMSW__
	#include <wx/msw/winundef.h>
#endif
#include <fcntl.h>
#ifdef __WXMSW__
	#include <wx/msw/winundef.h>
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/list.h>
#include <wx/imaglist.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/splitter.h>
#include <wx/scrolwin.h>
#include <wx/event.h>
#include <wx/dcmemory.h>
#include <wx/treectrl.h>
#include <wx/listimpl.cpp>
#include <wx/notebook.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/parser.h>
#include <libxml/xinclude.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#endif // DEF_GLOBALS_H

