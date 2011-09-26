
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
  string NickName  ;
  TString FileName  ;
  Bool_t  SigTrain  ;
  Bool_t  BkgdTrain ;
  Bool_t  TrueData  ;
  Bool_t  BkgdData  ;    
  Float_t Lumi      ;
  Float_t ScaleFac  ;
};

class PlotGroup_t {
  public:
  string           PlotGroupName   ;
  Int_t            PlotGroupColor  ;
  vector<TString>  PlotGroupMember ;
};

class InputVar_t {
  public:
  InputVar_t(){;}
  virtual ~InputVar_t(){;}
  TString VarName ;
  Char_t  VarType ;  
  Bool_t  Active  ;
};

class CtrlPlot_t {
  public:
  TString VarName ;
  Int_t   iVarPos ; // Var position in TmvaInput
  Int_t   nBins   ;
  Float_t xMin    ;
  Float_t xMax    ;
  Bool_t  kLogY   ;
}; 

class TargetLumi_t {
  public:
  Float_t Lumi    ;
  Bool_t  useData ; 
};

class CutBased_t {
  public:
  string NickName ;
  string File     ;
  string Hist     ;
  Int_t  Bin      ;
};

class Systematic_t {
  public:
  string systName ;
  string systType ;
  float  systVal  ;
  vector <string> systMember ;
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
  Bool_t               TmvaOptim   ;
  vector<string>       TmvaVarTrans; 
  string               TmvaWeight  ;    
  vector<InputVar_t>   TmvaVar     ;
  string               TmvaPreCut  ;  
  Int_t                TmvaVarNumRemove    ;

  // TMVA Cut Specific

  string               CUTOptions  ;
  
  // TMVA ANN Specific
  
  Int_t                ANNCycles          ;
  Int_t                ANNHiddenLayersMin ; 
  Int_t                ANNHiddenLayersMax ; 
  Int_t                ANNHiddenNodesMin  ; 
  Int_t                ANNHiddenNodesMax  ;
  Float_t              ANNFracTrain       ;
  Float_t              ANNFracTest        ;
  Float_t              ANNFracValidate    ;

  // TMVA BDT Specific

  vector<Int_t>        BDTNTrees          ;
  vector<TString>      BDTBoostType       ;
  vector<TString>      BDTAdaBoostR2Loss  ;
  vector<TString>      BDTUseBaggedGrad      ;
  vector<Float_t>      BDTGradBaggingFraction;
  vector<Float_t>      BDTShrinkage       ;
  vector<TString>      BDTSeparationType  ;
  vector<Int_t>        BDTnCuts           ;
  vector<TString>      BDTPruneMethod     ;
  vector<Int_t>        BDTPruneStrength   ;
  vector<Int_t>        BDTNNodesMax       ;

  // TMVA External XML

  string               XMLFile            ;

  // Reader Basic Options

  Int_t                TmvaRespNBins      ;
  Float_t              TmvaRespXMin       ;
  Float_t              TmvaRespXMax       ; 
  Int_t                TmvaRespRebinFac   ;     

  // Cut Based Yields
  //TString              CutBasedHistName   ;
  //Int_t                CutBasedHistBin    ;  
  vector <CutBased_t>  CutBased ; 

  // Final Plots 
  vector<TargetLumi_t> TargetLumi;
  vector<PlotGroup_t>  PlotGroup ;
  vector<CtrlPlot_t>   CtrlPlot  ;

  // Systematics
  vector<Systematic_t> Systematic;

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
  Bool_t               GetTmvaOptim()      { return TmvaOptim     ; } 
  vector<string>*      GetTmvaVarTrans()   { return &TmvaVarTrans ; }
  string               GetTmvaWeight()     { return TmvaWeight    ; }
  vector<InputVar_t>*  GetTmvaVar()        { return &TmvaVar      ; }
  string               GetTmvaPreCut()     { return TmvaPreCut    ; }
  Int_t                GetTmvaVarNumRemove()   { return TmvaVarNumRemove   ; }

  string               GetCUTOptions()         {return  CUTOptions ; }

  Int_t                GetANNCycles()          { return ANNCycles          ; }
  Int_t                GetANNVarNumRemove()    { return TmvaVarNumRemove   ; }
  Int_t                GetANNHiddenLayersMin() { return ANNHiddenLayersMin ; }
  Int_t                GetANNHiddenLayersMax() { return ANNHiddenLayersMax ; }
  Int_t                GetANNHiddenNodesMin()  { return ANNHiddenNodesMin  ; }
  Int_t                GetANNHiddenNodesMax()  { return ANNHiddenNodesMax  ; }
  Float_t              GetANNFracTrain()       { return ANNFracTrain       ; }
  Float_t              GetANNFracTest()        { return ANNFracTest        ; }
  Float_t              GetANNFracValidate()    { return ANNFracValidate    ; }

  vector<Int_t>*       GetBDTNTrees()          { return &BDTNTrees         ; }
  vector<TString>*     GetBDTBoostType()       { return &BDTBoostType      ; }
  vector<TString>*     GetBDTAdaBoostR2Loss()  { return &BDTAdaBoostR2Loss ; } 
  vector<TString>*     GetBDTUseBaggedGrad()   { return &BDTUseBaggedGrad  ; } 
  vector<Float_t>*     GetBDTGradBaggingFraction() { return &BDTGradBaggingFraction ; }
  vector<Float_t>*     GetBDTShrinkage()       { return &BDTShrinkage      ; } 
  vector<TString>*     GetBDTSeparationType()  { return &BDTSeparationType ; }
  vector<Int_t>*       GetBDTnCuts()           { return &BDTnCuts          ; }
  vector<TString>*     GetBDTPruneMethod()     { return &BDTPruneMethod    ; }
  vector<Int_t>*       GetBDTPruneStrength()   { return &BDTPruneStrength  ; } 
  vector<Int_t>*       GetBDTNNodesMax()       { return &BDTNNodesMax      ; } 

  string               GetXMLFile()            { return XMLFile            ; }

  Int_t                GetTmvaRespNBins()      { return TmvaRespNBins      ; }
  Float_t              GetTmvaRespXMin()       { return TmvaRespXMin       ; }
  Float_t              GetTmvaRespXMax()       { return TmvaRespXMax       ; }
  Int_t                GetTmvaRespRebinFac()   { return TmvaRespRebinFac   ; }
 
  //TString              GetCutBasedHistName()   { return CutBasedHistName ; }
  //Int_t                GetCutBasedHistBin()    { return CutBasedHistBin  ; }
  vector<CutBased_t>*  GetCutBased()           { return &CutBased ; } 

  vector<TargetLumi_t>*  GetTargetLumi()       { return &TargetLumi ; }
  vector<PlotGroup_t>*   GetPlotGroup()        { return &PlotGroup  ; }     
  vector<CtrlPlot_t>*    GetCtrlPlot()         { return &CtrlPlot  ; }     

  vector<Systematic_t>*  GetSystematic()       { return &Systematic ; }

};

#endif
