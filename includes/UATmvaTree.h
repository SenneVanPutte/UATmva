

#ifndef __UATmvaTree_H__
#define __UATmvaTree_H__

#include "../includes/UATmvaConfig.h"

#include <TTree.h>
#include <TString.h>

#include <vector>
using namespace std;

class InputTree_t {
  public:
  TString      NickName ;
  TTree*       Tree     ;
};

class UATmvaTree {

  private:

  Bool_t                InitDone  ;
  vector<InputTree_t>   InputTree ;    

  public:

  UATmvaTree(){ InitDone = false ;}
  UATmvaTree(UATmvaConfig&);
  virtual ~UATmvaTree(){Close();}

  Bool_t IsInit() { return InitDone ; }

  void Open  (UATmvaConfig&);
  void Close ();
  void Write ();
  void Print();

  // Getters:
  vector<InputTree_t>*   GetInputTree()   { return &InputTree ; } 
  TTree*                 GetTree(TString) ;

};

#endif
