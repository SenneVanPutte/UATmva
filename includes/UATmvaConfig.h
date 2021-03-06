
#ifndef __UATmvaConfig_H__
#define __UATmvaConfig_H__

#include <TString.h>
#include <string>
#include <vector>
#include <TTree.h>
#include <TTreeFormula.h>

using namespace std;

class InputData_t {
  public:
  InputData_t(){;}
  virtual ~InputData_t(){;}
  //Int_t   IsOpen    ;
  string NickName  ;
  TString FileName  ;
  Bool_t  SigTrain  ;
  Int_t   BkgdTrain ;
  Bool_t  SigTest   ;
  Int_t   BkgdTest  ; 
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
  vector <float>  systVal  ;
  vector <string> systMember ;
};

class SyDDEstim_t {
  public:
  string SyDDEName ;
  string SyDDEType ;
  float  SyDDEmass ;
  float  SyDDEdctrl;
  float  SyDDEderr;
  vector <string> SyDDECards  ;
  vector <string> SyDDEMember ;
};


class TreeFormula_t {
  public:
  TreeFormula_t(){ bEvaluated = false ;}
  virtual ~TreeFormula_t(){;}
  string        NickName   ;
  string        Expression ;
  private:
  TTreeFormula* Formula    ;
  bool          bEvaluated ;
  Float_t       Result_    ;
  public:
  void MakFormula (TTree *);
  void EvaFormula ()       ;
  void DelFormula ()       ;
  Float_t Result()         ;
};

class DataSetWght_t : public TreeFormula_t {
  public:
//  string         NickName ;
//  Float_t        Weight   ; 
  vector<string> DataSets ;
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
  Int_t                TestMode   ; // Training & Test Separation !
  vector<InputData_t>  InputData  ;
  vector<InputVar_t>   InputVar   ;

  bool  smurfchannel ;
  
  // TMVA Method:

  TString              TmvaType    ;
  Int_t                TmvaDim     ;  
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

  // TMVA External XML (possibly multiD MVA)

  vector<string>       XMLFiles           ;

  // Reader Basic Options

  Int_t                TmvaRespNBins      ;
  Float_t              TmvaRespXMin       ;
  Float_t              TmvaRespXMax       ; 
  Int_t                TmvaRespRebinFac   ;     
  Bool_t               TmvaRespUseLog     ;
  Bool_t               TmvaRespUseSigmoid ;
 
  // Cut Based Yields
  //TString              CutBasedHistName   ;
  //Int_t                CutBasedHistBin    ;  
  vector <CutBased_t>  CutBased ; 
  TreeFormula_t        CutBaseSel ;   


  // Final Plots 
  string               SignalName;
  vector<TargetLumi_t> TargetLumi;
  vector<PlotGroup_t>  PlotGroup ;
  vector<CtrlPlot_t>   CtrlPlot  ;


  // Extra dataset weight 
  vector<DataSetWght_t>  DataSetWghts ;


  string               LimBinName;
  float                HiggsMass ;
  // Systematic errors
  vector<Systematic_t> Systematic;
  vector<SyDDEstim_t>  SyDDEstim; 
  // Statisctic errors
  vector<string>       StatMember;
  string               StatPrefix;
  string               StatMiddle;
  string               StatSuffix; 

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
  Int_t                GetTestMode()       { return TestMode      ; } 
  vector<InputData_t>* GetInputData()      { return &InputData    ; }
  vector<InputVar_t>*  GetInputVar()       { return &InputVar     ; }
 
  TString              GetTmvaType()       { return TmvaType      ; }
  Int_t                GetTmvaDim()        { return TmvaDim       ; } 
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

  vector<string>*      GetXMLFiles()           { return &XMLFiles          ; }

  Int_t                GetTmvaRespNBins()      { return TmvaRespNBins      ; }
  Float_t              GetTmvaRespXMin()       { return TmvaRespXMin       ; }
  Float_t              GetTmvaRespXMax()       { return TmvaRespXMax       ; }
  Int_t                GetTmvaRespRebinFac()   { return TmvaRespRebinFac   ; }
  Bool_t               GetTmvaRespUseLog()     { return TmvaRespUseLog     ; }
  Bool_t               GetTmvaRespUseSigmoid() { return TmvaRespUseSigmoid ; }
 
  //TString              GetCutBasedHistName()   { return CutBasedHistName ; }
  //Int_t                GetCutBasedHistBin()    { return CutBasedHistBin  ; }
  vector<CutBased_t>*  GetCutBased()           { return &CutBased ; } 
  TreeFormula_t*       GetCutBaseSel()         { return &CutBaseSel ; }

  string                 GetSignalName()       { return SignalName  ; } 
  vector<TargetLumi_t>*  GetTargetLumi()       { return &TargetLumi ; }
  vector<PlotGroup_t>*   GetPlotGroup()        { return &PlotGroup  ; }     
  vector<CtrlPlot_t>*    GetCtrlPlot()         { return &CtrlPlot  ; }     

  vector<DataSetWght_t>*  GetDataSetWghts()   { return &DataSetWghts ; }

  string                 GetLimBinName()   { return LimBinName  ; }
  float                  GetHiggsMass()    { return HiggsMass ; } 

  vector<Systematic_t>*  GetSystematic()       { return &Systematic ; }
  vector<SyDDEstim_t>*   GetSyDDEstim()        { return &SyDDEstim  ; } 

  vector<string>*        GetStatMember()    { return  &StatMember ; }
  string                 GetStatPrefix()    { return  StatPrefix ; }
  string                 GetStatMiddle()    { return  StatMiddle ; }
  string                 GetStatSuffix()    { return  StatSuffix ; }


  bool  Getsmurfchannel() { return smurfchannel ; }

};

#endif
