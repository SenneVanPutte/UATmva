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
  TString              TmvaName ;
  TFile*               TmvaFile ;
  Factory*             TmvaFactory ; 
  vector<TString>      TmvaConfig  ; 
};


class UATmvaClassification {

  private:

  Bool_t                    InitDone    ;
  vector<UATmvaFactory_t>   UATmvaFactory ;

  //TFile*               TmvaOutput  ;
  //Factory*             TmvaFactory ; 
 
  public:

  UATmvaClassification(){ InitDone = false ;}
  virtual ~UATmvaClassification(){;}

  Bool_t IsInit() { return InitDone ; }

  void Init    ( UATmvaConfig&, UATmvaTree&);
  void InitMLP ( UATmvaConfig&, UATmvaTree&);

  void Train(); 

};


#endif

