#ifndef __UATmvaClassification_H__
#define __UATmvaClassification_H__

#include "../includes/UATmvaConfig.h"
#include "../includes/UATmvaTree.h"


#include "TMVA/Factory.h"
using namespace TMVA;

#include <TString.h>
#include <string>
#include <vector>
using namespace std;

class UATmvaFactory_t {
  public:
  UATmvaFactory_t(){ TmvaFile = NULL ; TmvaFactory = NULL; }
  ~UATmvaFactory_t(){ TmvaFile->Close(); delete TmvaFile ; delete TmvaFactory ; }
   
  TString              TmvaName ;
  TFile*               TmvaFile ;
  Factory*             TmvaFactory ; 
};


class UATmvaClassification {

  private:

  string NAME;
  Bool_t                    InitDone  ;
  UATmvaFactory_t*          UAFactory ;
  void Train(UATmvaConfig&, UATmvaTree& ,string ,string ,int); 
  void Plot(bool);
 
  public:

  UATmvaClassification(){ InitDone = false ;}
  virtual ~UATmvaClassification(){;}

  Bool_t IsInit() { return InitDone ; }

  void Do      ( UATmvaConfig&, UATmvaTree&);
  void DoCUT   ( UATmvaConfig&, UATmvaTree&);
  void DoMLP   ( UATmvaConfig&, UATmvaTree&);
  void DoBDT   ( UATmvaConfig&, UATmvaTree&);
  void DoLH    ( UATmvaConfig&, UATmvaTree&);
  void DoPDERS ( UATmvaConfig&, UATmvaTree&);
  void DoPDEFoam ( UATmvaConfig&, UATmvaTree&);


};


#endif

