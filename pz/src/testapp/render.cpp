/*
	This file is part of the ProjectZero GUI
	All rights reserved
*/

#include "globals.h"
#include "draw.h"
#include "render.h"
#include <stdio.h>

#define MAXBUF 1024
#define PREV_CTABLE 0
#define PREV_CTR 1
#define PREV_CTD 2

Render::Render()
{
  // dunno

}

Render::~Render()
{

}

xmlBufferPtr Render::doRender(class Line * in)
{

  int w, h;

  out=xmlBufferCreate();
  numcel=0;

  doWrite("<page>\n<ctable>\n");
  lines = in;
  w = CANVAS_WIDTH;
  h = CANVAS_HEIGHT;
  processLine(0,0, w, h, PREV_CTABLE);
  doWrite("</ctable>\n</page>\n");
  return out;
}


void Render::processLine(int xa, int ya, int xb, int yb, int prev)
{
  int xposses[MAXBUF];
  int yposses[MAXBUF];
  class Line * lp;
  int rowcnt = 0;
  int colcnt = 0;
  int i,j;

  // find lines that go from left to right -- remember their y positions
  lp = lines;
  while(lp != NULL)
    {
       if((lp->a.y == lp->b.y) && (lp->a.y <= yb) && (lp->a.y >= ya) && (lp->a.x <= xa) && (lp->b.x >= xb))
        yposses[rowcnt++] = lp->a.y;
      lp = lp->next;

    }

  // remove duplicate rowdelimiters
  for(i=0; i<rowcnt; i++)
    {
      for(j = i+1; j<rowcnt; j++)
        {
          if(yposses[i] == yposses[j])
            {
              if(j == (rowcnt-1))
                {
                  rowcnt--;
                  j--;
                }
              else
                {
                  yposses[j] = yposses[rowcnt-1];
                  rowcnt--;
                  j--;
                }
            }
        }
    }

  
  if(rowcnt > 2)
    {  // we always have 2 rowdelimiters
      if(prev != PREV_CTABLE)
        doWrite("<ctable>\n");
      // sort rows
      for(i=0; i < rowcnt; i++)
        {
          int tmp, lowest;
          lowest = i;
          for(j = i+1; j < rowcnt; j++)
            {
              if(yposses[j] < yposses[lowest])
                lowest = j;
            }
          tmp = yposses[i];
          yposses[i] = yposses[lowest];
          yposses[lowest]= tmp;
        }
 
      // enter row loop, process each row
      for(i=0; i < (rowcnt-1); i++)
        {
          doWrite("<ctr>\n");
          // process row
          processLine(xa, yposses[i] , xb, yposses[i+1], PREV_CTR);
          doWrite("</ctr>\n");
        }
      if(prev != PREV_CTABLE)
        doWrite("</ctable>\n");
    }
  else
    {
      // now let's look for coldelimiters -- remember their x position
      lp = lines;
      while(lp != NULL)
        {
          if((lp->a.x == lp->b.x) && (lp->a.x <= xb) && (lp->a.x >= xa) && (lp->a.y <= ya) && (lp->b.y >= yb))
            xposses[colcnt++] = lp->a.x;
          lp = lp->next;
        }

      // remove duplicate coldelimiters
      for(i=0; i<colcnt; i++)
        {
          for(j = i+1; j<colcnt; j++)
            {
              if(xposses[i] == xposses[j])
                {
                  if(j == (colcnt-1))
                    {
                      colcnt--;
                      j--;
                    }
                  else
                    {

                      xposses[j] = xposses[colcnt-1];
                      colcnt--;
                      j--;
                    }
                }
            }
        }

 for(i=0; i<rowcnt; i++)
    printf("col: %i\n", yposses[i]);

      if(colcnt > 2)
        {
          if(prev != PREV_CTR)
            doWrite("<ctr>\n");
          int i, j;
          //sort cols
          for(i=0; i < colcnt; i++)
            {
              int tmp, lowest;
              lowest =i;
              for(j = i+1; j < colcnt; j++)
                {
                  if(xposses[j] < xposses[lowest])
                    lowest = j;
                }
              tmp = xposses[i];
              xposses[i] = xposses[lowest];
              xposses[lowest]= tmp;
            }
          // enter row loop, process each row
          for(i=0; i < (colcnt-1); i++)
            {
              doWrite("<ctd>\n");
              // process row
              processLine(xposses[i] , ya, xposses[i+1], yb, PREV_CTD);
              doWrite("</ctd>\n");
            }
          if(prev != PREV_CTR)
            doWrite("</ctr>\n");
        }
      else
        {
          if(prev != PREV_CTD)
            doWrite("<ctd>\n");
          doWrite("<inc />\n");
          numcel++;
          if(prev != PREV_CTD)
            doWrite("</ctd>\n");
        }

    }

}


void Render::doWrite(char * text)
{
  xmlBufferAdd(out, (const xmlChar *) text, strlen(text));
}


xmlBufferPtr Render::doStore(Line * in){
  xmlDocPtr doc;
  xmlNodePtr root, newchild;
  char prop[10];
  Line * tmp;
  
  out = xmlBufferCreate();
  doc = xmlNewDoc( (xmlChar *) "1.0");
  root = xmlNewNode(NULL, (xmlChar *) "template");
  
  xmlDocSetRootElement(doc,root);
  
  tmp = in;
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

