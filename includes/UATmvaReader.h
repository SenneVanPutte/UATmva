#ifndef __UATmvaReader_H__
#define __UATmvaReader_H__

#include "../includes/UATmvaConfig.h"
#include "../includes/UATmvaTree.h"


#include "TMVA/Reader.h"
using namespace TMVA;

#include <TString.h>
#include <string>
#include <vector>
using namespace std;


class UATmvaReader_t {
  public:
  UATmvaReader_t(){ TmvaFile = NULL ; TmvaReader = NULL; }
  ~UATmvaReader_t(){ TmvaFile->Close(); delete TmvaFile ; delete TmvaReader ; }
   
  TString             TmvaName ;
  TFile*              TmvaFile ;
  Reader*             TmvaReader ; 
};


class UATmvaReader {

  private:

  
  Bool_t                   InitDone  ;
  string                   NAME ;
  UATmvaReader_t*          UAReader ;

  Int_t nbins ;
  Float_t minBin, maxBin;


//  void Train(); 
//  void Plot();
 
  public:

  UATmvaReader(){ InitDone = false ; nbins = 44 ; minBin=-1.1 ; maxBin=1.1 ;}
  virtual ~UATmvaReader(){;}

  Bool_t IsInit() { return InitDone ; }

  void Do    ( UATmvaConfig&, UATmvaTree&);

  private:

  void DoCUT ( UATmvaConfig&, UATmvaTree&);
  void DoMLP ( UATmvaConfig&, UATmvaTree&);
  void DoBDT ( UATmvaConfig&, UATmvaTree&);
  void DoLH  ( UATmvaConfig&, UATmvaTree&);
  void DoPDERS ( UATmvaConfig&, UATmvaTree&);
  void DoPDEFoam ( UATmvaConfig&, UATmvaTree&);
  void DoXML ( UATmvaConfig&, UATmvaTree&);
  void Read  ( UATmvaConfig&, UATmvaTree&, string, int, int );


  //void SetNbin   ( Int_t   N ) { nbins  = N ;}
  //void SetMinbin ( Float_t V ) { minBin = V ;}
  //void SetMaxbin ( Float_t V ) { maxBin = V ;}


};


#endif

