/*
 * Project Zero - All Rights Reserved
 *
 */
 
#include "globals.h"
#include "block.h"
#include <wx/dialog.h>
#include <wx/listimpl.cpp>
#include <unistd.h>
#include <sys/types.h>
WX_DEFINE_LIST(VariableList);
WX_DEFINE_LIST(BlockList);

int Block::Load(xmlNodePtr myblock){
  xmlChar * prop; xmlChar * prop2;
  wxString filename;
  xmlNodePtr cur;
  xmlDocPtr blockdoc;
  prop = xmlGetProp(cur, (const xmlChar*)"name");
  
  if(prop){
   // let's start digging to find that block
   filename.Append(wxT(BLOCK_DIR));
   filename.Append(wxString((const char*)prop, wxConvUTF8));
   if(access(filename.mb_str(), R_OK) < 0){
      // file can't be accessed
       wxDialog errdiag(NULL, -1, wxT("Couldn\'t load module"));
       errdiag.ShowModal();
   }
   uri = filename; 
   blockdoc = xmlParseFile(uri.mb_str());
   
   cur = xmlDocGetRootElement(blockdoc);
   /* process block info file here */
   
   
   
   /* process block data from page file here */
   for(cur = myblock->xmlChildrenNode; cur; cur = cur->next){
    prop = xmlGetProp(cur, (const xmlChar*)"name");
    prop2 = xmlGetProp(cur, (const xmlChar*)"value");
    if(!xmlStrcmp(cur->name, (xmlChar *) "data") && prop && prop2)
      // maybe seems a bit unsafe, but wxList takes care of it
      vars.Append(new Argument(wxString((const char*)prop, wxConvUTF8),
                               wxString((const char*)prop2, wxConvUTF8)));
   }
          
  } else {
    // no name provided, we don't know anything about this block
    // just drop dead and tell the user
    /* we should consider better error handling, 
     allowing the user to point where we can find this block, 
     or even where we can download it */
     wxDialog errdiag(NULL, -1, wxT("Couldn\'t load module"));
     errdiag.ShowModal();
    return 0;
  }
}

Argument::Argument(const wxString& myname, const wxString& myvalue){
  name = myname;
  value = myvalue;
}
