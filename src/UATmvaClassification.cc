

#include "../includes/UATmvaClassification.h"

#include <iostream>



// ---------------------------- Init()

void UATmvaClassification::Init( UATmvaConfig& Cfg, UATmvaTree& T) {

   if ( Cfg.GetTmvaType() == "ANN" )  InitMLP(Cfg,T);

}

void UATmvaClassification::InitMLP( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 
   for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
   for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {
    
     UATmvaFactory_t      UAFactory ; 
 
     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_MLP_" << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;
     UAFactory.TmvaName = Name.str();
 
     // Open TMVA output file
     UAFactory.TmvaFile  = TFile::Open("rootfiles/" + UAFactory.TmvaName  + ".root","RECREATE" );
  
     // Create TMVA Factory
     UAFactory.TmvaFactory = new Factory(UAFactory.TmvaName , UAFactory.TmvaFile ,
      "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
  
     
     // Add VariablesCfg.GetTmvaVar())
     for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
       if (iVar->Active) UAFactory.TmvaFactory->AddVariable(iVar->VarName,iVar->VarType);
     }    
  
     // Weights
     UAFactory.TmvaFactory->SetWeightExpression(Cfg.GetTmvaWeight());
     Double_t SgWeight=1.0;
     Double_t BgWeight=1.0;
     
     // Add Tree
     for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
       if(iD->SigTrain ) {
         UAFactory.TmvaFactory->AddSignalTree     ( T.GetTree(iD->NickName) , SgWeight ) ;
       }
       if(iD->BkgdTrain) {
         UAFactory.TmvaFactory->AddBackgroundTree ( T.GetTree(iD->NickName) , BgWeight ) ;
       }
     }
  
     // Set training options
     UAFactory.TmvaFactory->PrepareTrainingAndTestTree("","");
  
     // BookMethod
     ostringstream Method;
     Method << "H:!V:NeuronType=tanh:VarTransform=N:NCycles=" << Cfg.GetANNCycles() << ":HiddenLayers=" ;
     for ( Int_t iHLayer = 1 ; iHLayer <= nHLayers ; ++iHLayer ) {
       Method << "N+" << nHNodes ; 
       if ( iHLayer != nHLayers ) Method << "," ;
     }

     UAFactory.TmvaFactory->BookMethod( TMVA::Types::kMLP, UAFactory.TmvaName , Method.str() );
  
     UAFactory.TmvaFactory->TrainAllMethods(); 
     UAFactory.TmvaFactory->TestAllMethods(); 
     UAFactory.TmvaFactory->EvaluateAllMethods();

     UAFactory.TmvaFile->Close();
     UATmvaFactory.push_back(UAFactory);
   
   } // Nodes
   } // Layers
   } // Variables

}


void UATmvaClassification::Train(){
  for (vector<UATmvaFactory_t>::iterator iUAF = UATmvaFactory.begin() ; iUAF != UATmvaFactory.end() ; ++iUAF ){
    cout << "[UATmvaClassification::Train()] Training " << iUAF->TmvaName << endl;
//    iUAF->TmvaFile->cd();
//    iUAF->TmvaFactory->TrainAllMethods(); 
    //iUAF->TmvaFactory->TestAllMethods(); 
    //iUAF->TmvaFactory->EvaluateAllMethods();
  }

}
