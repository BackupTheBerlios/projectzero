/*
 * Project Zero - All Rights Reserved
 *
 */
 
#ifndef DEF_BLOCK_H
#define DEF_BLOCK_H

#define BLOCK_DIR "../../phpscripts/"

class Block {
  public:
    Block::Block(){};
    int Block::Load(xmlNodePtr blocknode);
  private:
    VariableList vars;
    wxString uri;
               
};

class Argument {
  public:
    Argument(){};
    ~Argument(){};
    wxString name;
    wxString value;
};

class DataInput {
  public:
    wxString name;
    wxString description;
    int type;
};

wxDECLARE_LIST(Argument, VariableList);
wxDECLARE_LIST(Block, BlockList);
wxDEFINE_LIST(Argument, VariableList);
#endif /* DEF_BLOCK_H */
