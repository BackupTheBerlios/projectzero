/*
 * Project Zero - All Rights Reserved
 *
 */
 
#ifndef DEF_BLOCK_H
#define DEF_BLOCK_H

#define BLOCK_DIR "../../phpscripts/"

class Argument {
  public:
    Argument(const wxString& myname, const wxString& myvalue);
    ~Argument(){};
    wxString name;
    wxString value;
};
WX_DECLARE_LIST(Argument, VariableList);

class DataInput {
  public:
    wxString name;
    wxString description;
    int type;
};

class Block {
  public:
    Block::Block(){};
    int Block::Load(xmlNodePtr blocknode);
  private:
    VariableList vars;
    wxString uri;
               
};
WX_DECLARE_LIST(Block, BlockList);

#endif /* DEF_BLOCK_H */
