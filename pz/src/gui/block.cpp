/*
 * Project Zero - All Rights Reserved
 *
 */
 
#include "globals.h"
#include "block.h"
#include <wx/dialog.h>
#include <unistd.h>
#include <sys/types.h>
WX_DEFINE_LIST(MyList);

int Block::Load(xmlNodePtr myblock){
  xmlChar * prop;
  wxString filename;
  xmlNodeptr cur;
  prop = xmlGetProp(cur, "name");
  
  if(prop){
   // let's start digging to find that block
   filename.Append(wxString temp((const char*) BLOCK_DIR, wxConvUTF8));
   filename.Append(wxString temp((const char*) prop, wxConvUTF8));
   if(access(mg_str(filename), R_OK) < 0){
      // file can't be accessed
       wxDialog errdiag(this, -1, wxT("Couldn\'t load module"));
       errdiag->ShowModal;
   }
   uri = filename; 
   
   for(cur = myblock->xmlChildrenNode; cur; cur = cur->next){
    
   
   
   }
          
  } else {
    // no name provided, we don't know anything about this block
    // just drop dead and tell the user
    /* we should consider better error handling, 
     allowing the user to point where we can find this block, 
     or even where we can download it */
     wxDialog errdiag(this, -1, wxT("Couldn\'t load module"));
     errdiag->ShowModal;
    return 0;
  }
}
