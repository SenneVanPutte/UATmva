

#include "../includes/UATmvaClassification.h"

#include <iostream>



// ---------------------------- Init()

void UATmvaClassification::Do( UATmvaConfig& Cfg, UATmvaTree& T) {

   if ( Cfg.GetTmvaType() == "ANN" )  DoMLP(Cfg,T);

}

void UATmvaClassification::DoMLP( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 
   for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
   for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {
    
     UAFactory = new UATmvaFactory_t() ; 
 
     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_MLP_" << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;
     UAFactory->TmvaName = Name.str();
     NAME =  Name.str();

 
     // Open TMVA output file
     cout << "[UATmvaClassification::DoMLP()] Create: " << UAFactory->TmvaName << endl;
     UAFactory->TmvaFile  = TFile::Open("rootfiles/" + UAFactory->TmvaName  + ".root","RECREATE" );
  
     // Create TMVA Factory
     UAFactory->TmvaFactory = new Factory(UAFactory->TmvaName , UAFactory->TmvaFile ,
      "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
  
     
     // Add VariablesCfg.GetTmvaVar())
     Int_t nVar = 0;     
     for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
       if (++nVar <= nVarMax) UAFactory->TmvaFactory->AddVariable(iVar->VarName,iVar->VarType);
     }    
  
     // Weights
     UAFactory->TmvaFactory->SetWeightExpression(Cfg.GetTmvaWeight());
     Double_t SgWeight=1.0;
     Double_t BgWeight=1.0;
     
     // Add Tree
     for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
       if(iD->SigTrain ) {
         UAFactory->TmvaFactory->AddSignalTree     ( T.GetTree(iD->NickName) , SgWeight ) ;
       }
       if(iD->BkgdTrain) {
         UAFactory->TmvaFactory->AddBackgroundTree ( T.GetTree(iD->NickName) , BgWeight ) ;
       }
     }
  
     // Set training options
     UAFactory->TmvaFactory->PrepareTrainingAndTestTree("","");
  
     // BookMethod
     ostringstream Method;
     Method << "H:!V:NeuronType=tanh:VarTransform=N:NCycles=" << Cfg.GetANNCycles() << ":HiddenLayers=" ;
     for ( Int_t iHLayer = 1 ; iHLayer <= nHLayers ; ++iHLayer ) {
       Method << "N+" << nHNodes ; 
       if ( iHLayer != nHLayers ) Method << "," ;
     }

     UAFactory->TmvaFactory->BookMethod( TMVA::Types::kMLP, UAFactory->TmvaName , Method.str() );

     // Train , Test, Validate
     Train();

     // Plots
 
     // Clean UAFactory
     delete UAFactory ; 

     // Making some basic plots
     Plot();
   
   } // Nodes
   } // Layers
   } // Variables

}


void UATmvaClassification::Train( ){
    cout << "[UATmvaClassification::Train()] Training " << UAFactory->TmvaName << endl;
    UAFactory->TmvaFactory->TrainAllMethods(); 
    UAFactory->TmvaFactory->TestAllMethods();
    UAFactory->TmvaFactory->EvaluateAllMethods();
}

void UATmvaClassification::Plot( ){
    cout << "[UATmvaClassification::Plot()] Plotting " << UAFactory->TmvaName << endl;
    string command;

    // Correlations
    command = "root -b -q $ROOTSYS/tmva/test/correlations.C\\(\\\"rootfiles/"+NAME+".root\\\"\\)";
    system(command.c_str());
    command = "mv plots/CorrelationMatrixS.eps plots/CorrelationMatrixS_"+NAME+".eps";
    system(command.c_str());
    command = "mv plots/CorrelationMatrixS.gif plots/CorrelationMatrixS_"+NAME+".gif";
    system(command.c_str());
    command = "mv plots/CorrelationMatrixB.eps plots/CorrelationMatrixB_"+NAME+".eps";
    system(command.c_str());
    command = "mv plots/CorrelationMatrixB.gif plots/CorrelationMatrixB_"+NAME+".gif";
    system(command.c_str());

    // overtraining (test and training)
    command = "root -b -q $ROOTSYS/tmva/test/mvas.C\\(\\\"rootfiles/"+NAME+".root\\\",3\\)";
    system(command.c_str());

    // epoch plot (test and training)
    command = "root -b -q $ROOTSYS/tmva/test/annconvergencetest.C\\(\\\"rootfiles/"+NAME+".root\\\"\\)";
    system(command.c_str());
    command = "mv plots/annconvergencetest.eps plots/annconvergencetest_"+NAME+".eps";
    system(command.c_str());
    command = "mv plots/annconvergencetest.gif plots/annconvergencetest_"+NAME+".gif";
    system(command.c_str());
 
    // MVA efficiency
    //command = "root -b -q $ROOTSYS/tmva/test/mvaeffs.C\\(\\\"rootfiles/"+NAME+".root\\\"\\)";
    //system(command.c_str());
}
