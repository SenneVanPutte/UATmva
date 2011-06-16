
#include "../includes/UATmvaSummary.h"

// ---------------------------- Init()

void UATmvaSummary::Do( UATmvaConfig& Cfg ) {

  // Open All files and Load Histos
  for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove() ; ++nVarRem) {
  Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;
  for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
  for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {

     UATmvaSummary_t* UASummary = new UATmvaSummary_t();

     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_MLP_" << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;
     UASummary->TmvaName = Name.str();

     UASummary->TmvaFile  = TFile::Open("rootfiles/" + UASummary->TmvaName  + ".root","READ" );

     TH1D* Cut   = (TH1D*) (UASummary->TmvaFile)->Get("OutputHistograms/Cut"); 
     TH1D* Sign  = (TH1D*) (UASummary->TmvaFile)->Get("OutputHistograms/Sign"); 
     TH1D* Limit = (TH1D*) (UASummary->TmvaFile)->Get("OutputHistograms/Limit");

//     Sign->Draw();
//     Limit->Draw("same");

     int iMethod = 9;
     cout << " " << nHLayers << " " << nHNodes << " " << nVarMax << " " << Cut  ->GetBinContent(iMethod) << " " 
                                                                        << Sign ->GetBinContent(iMethod) << " "
                                                                        << Limit->GetBinContent(iMethod) << endl;  

//     gPad->WaitPrimitive(); 

     delete UASummary;

  } // Nodes
  } // Layers
  } // Variables



}
