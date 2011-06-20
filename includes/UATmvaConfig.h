
#ifndef __UATmvaConfig_H__
#define __UATmvaConfig_H__

#include <TString.h>
#include <string>
#include <vector>
using namespace std;

class InputData_t {
  public:
  InputData_t(){;}
  virtual ~InputData_t(){;}
  //Int_t   IsOpen    ;
  TString NickName  ;
  TString FileName  ;
  Bool_t  SigTrain  ;
  Bool_t  BkgdTrain ;
  Bool_t  TrueData  ;
  Bool_t  BkgdData  ;    
};

class InputVar_t {
  public:
  InputVar_t(){;}
  virtual ~InputVar_t(){;}
  TString VarName ;
  Char_t  VarType ;  
  Bool_t  Active  ;
};

class UATmvaConfig {

  private:

  void SetInputVar(string,vector<InputVar_t>&);


  Bool_t               InitDone  ;

  // UATMVA Name 

  TString              TmvaName ;

  // Input Config: 

  Int_t                InputType  ;
  TString              TreeName   ;
  vector<InputData_t>  InputData  ;
  vector<InputVar_t>   InputVar   ;
  
  // TMVA Method:

  TString              TmvaType    ;  
  TString              TmvaWeight  ;    
  vector<InputVar_t>   TmvaVar     ;
  
  // TMVA ANN Specific
  
  Int_t                ANNCycles          ;
  Int_t                ANNVarNumRemove    ;
  Int_t                ANNHiddenLayersMin ; 
  Int_t                ANNHiddenLayersMax ; 
  Int_t                ANNHiddenNodesMin  ; 
  Int_t                ANNHiddenNodesMax  ;
  Float_t              ANNFracTrain       ;
  Float_t              ANNFracTest        ;
  Float_t              ANNFracValidate    ;

  // Cut Based Yields
  TString              CutBasedHistName   ;
  Int_t                CutBasedHistBin    ;  

  public:
 
  UATmvaConfig(){ Reset() ; InitDone = false ; }  
  virtual ~UATmvaConfig(){ Reset() ; } 
  void Reset();

  
  Bool_t IsInit() { return InitDone ; }
  void ReadCfg(TString);

  void Print(); 



  // Getters:

  TString              GetTmvaName()       { return TmvaName      ; }
  Int_t                GetInputType()      { return InputType     ; }
  TString              GetTreeName()       { return TreeName      ; }
  vector<InputData_t>* GetInputData()      { return &InputData    ; }
  vector<InputVar_t>*  GetInputVar()       { return &InputVar     ; }
 
  TString              GetTmvaType()       { return TmvaType      ; }
  TString              GetTmvaWeight()     { return TmvaWeight    ; }
  vector<InputVar_t>*  GetTmvaVar()        { return &TmvaVar      ; }

  Int_t                GetANNCycles()          { return ANNCycles          ; }
  Int_t                GetANNVarNumRemove()    { return ANNVarNumRemove    ; }
  Int_t                GetANNHiddenLayersMin() { return ANNHiddenLayersMin ; }
  Int_t                GetANNHiddenLayersMax() { return ANNHiddenLayersMax ; }
  Int_t                GetANNHiddenNodesMin()  { return ANNHiddenNodesMin  ; }
  Int_t                GetANNHiddenNodesMax()  { return ANNHiddenNodesMax  ; }
  Float_t              GetANNFracTrain()       { return ANNFracTrain       ; }
  Float_t              GetANNFracTest()        { return ANNFracTest        ; }
  Float_t              GetANNFracValidate()    { return ANNFracValidate    ; }

  TString              GetCutBasedHistName()   { return CutBasedHistName ; }
  Int_t                GetCutBasedHistBin()    { return CutBasedHistBin  ; }
  

  


};

#endif
