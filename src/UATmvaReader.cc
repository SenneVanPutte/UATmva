

#include "../includes/UATmvaReader.h"

#include <iostream>
#include <TH1D.h>
#include <TTreeFormula.h>


// ---------------------------- Init()

void UATmvaReader::Do( UATmvaConfig& Cfg, UATmvaTree& T) {

   if ( Cfg.GetTmvaType() == "CUT" ) DoCUT(Cfg,T);
   if ( Cfg.GetTmvaType() == "ANN" ) DoMLP(Cfg,T);
   if ( Cfg.GetTmvaType() == "BDT" ) DoBDT(Cfg,T);
   if ( Cfg.GetTmvaType() == "LH"  ) DoLH (Cfg,T);
   if ( Cfg.GetTmvaType() == "PDERS" ) DoPDERS (Cfg,T);
   if ( Cfg.GetTmvaType() == "PDEFoam" ) DoPDEFoam (Cfg,T);
   if ( Cfg.GetTmvaType() == "XML" ) DoXML(Cfg,T);

}

void UATmvaReader::DoMLP( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t iLumi    = 0 ; iLumi   < (signed) Cfg.GetTargetLumi()->size() ; ++iLumi  ) {
   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove()   ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 
   for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
   for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {


     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_MLP_" << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;
     if (Cfg.GetTmvaOptim()) Name << "_Optim" ;
     NAME =  Name.str();

     Read(Cfg,T,Name.str(),nVarMax,iLumi) ;
 
   } // Nodes
   } // Layers
   } // Variables
   } // TargetLumi

}


void UATmvaReader::DoBDT( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t iLumi    = 0 ; iLumi   < (signed) Cfg.GetTargetLumi()->size() ; ++iLumi  ) {
   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove()   ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 

   for( int iBDTNTrees             = 0 ; iBDTNTrees              < (signed) (Cfg.GetBDTNTrees())->size()              ; ++iBDTNTrees              ) {
   for( int iBDTBoostType          = 0 ; iBDTBoostType           < (signed) (Cfg.GetBDTBoostType())->size()           ; ++iBDTBoostType           ) {
//   for( int iBDTAdaBoostR2Loss     = 0 ; iBDTAdaBoostR2Loss      < (signed) (Cfg.GetBDTAdaBoostR2Loss())->size()      ; ++iBDTAdaBoostR2Loss      ) {
   for( int iBDTUseBaggedGrad      = 0 ; iBDTUseBaggedGrad       < (signed) (Cfg.GetBDTUseBaggedGrad())->size()       ; ++iBDTUseBaggedGrad       ) {
   for( int iBDTGradBaggingFraction= 0 ; iBDTGradBaggingFraction < (signed) (Cfg.GetBDTGradBaggingFraction())->size() ; ++iBDTGradBaggingFraction ) {
   for( int iBDTShrinkage          = 0 ; iBDTShrinkage           < (signed) (Cfg.GetBDTShrinkage())->size()           ; ++iBDTShrinkage           ) {
   for( int iBDTSeparationType     = 0 ; iBDTSeparationType      < (signed) (Cfg.GetBDTSeparationType())->size()      ; ++iBDTSeparationType      ) {
   for( int iBDTnCuts              = 0 ; iBDTnCuts               < (signed) (Cfg.GetBDTnCuts())->size()               ; ++iBDTnCuts               ) {
   for( int iBDTPruneMethod        = 0 ; iBDTPruneMethod         < (signed) (Cfg.GetBDTPruneMethod())->size()         ; ++iBDTPruneMethod         ) {
   for( int iBDTPruneStrength      = 0 ; iBDTPruneStrength       < (signed) (Cfg.GetBDTPruneStrength())->size()       ; ++iBDTPruneStrength       ) {
   //for( int iBDTNNodesMax          = 0 ; iBDTNNodesMax           < (signed) (Cfg.GetBDTNNodesMax())->size()           ; ++iBDTNNodesMax           ) {


     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_BDT_" ;
     Name << Cfg.GetBDTNTrees()->at(iBDTNTrees) << "Trees_" ;
     Name << Cfg.GetBDTBoostType()->at(iBDTBoostType) << "_" ;
     if ( Cfg.GetBDTBoostType()->at(iBDTBoostType) == "Grad" ) {
       Name << Cfg.GetBDTUseBaggedGrad()->at(iBDTUseBaggedGrad) << "Bagged_" ;
       Name << Cfg.GetBDTGradBaggingFraction()->at(iBDTGradBaggingFraction) << "BagFrac_" ;
       Name << Cfg.GetBDTShrinkage()->at(iBDTShrinkage) << "BagShrink_" ;
     }
     Name << Cfg.GetBDTSeparationType()->at(iBDTSeparationType) << "_" ;
     Name << Cfg.GetBDTnCuts()->at(iBDTnCuts) << "Cuts_" ;
     Name << Cfg.GetBDTPruneMethod()->at(iBDTPruneMethod) << "_" ;
     Name << Cfg.GetBDTPruneStrength()->at(iBDTPruneStrength) << "PruneStrength_" ;
     //Name << Cfg.GetBDTNNodesMax()->at(iBDTNNodesMax) << "NodesMax_" ;
     Name << nVarMax << "Var" ;
     if (Cfg.GetTmvaOptim()) Name << "_Optim" ;
     NAME =  Name.str();

     Read(Cfg,T,Name.str(),nVarMax,iLumi) ;

   } // BDTNTrees
   } // BDTBoostType
//   } // BDTAdaBoostR2Loss
   } // BDTUseBaggedGrad
   } // BDTGradBaggingFraction
   } // BDTShrinkage
   } // BDTSeparationType
   } // BDTnCuts
   } // BDTPruneMethod
   } // BDTPruneStrength
//   } // BDTNNodesMax

   } // Variables
   } // TargetLumi

}



void UATmvaReader::DoLH( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t iLumi    = 0 ; iLumi   < (signed) Cfg.GetTargetLumi()->size() ; ++iLumi  ) {
   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove()   ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 


     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_Likelihood_" << nVarMax << "Var" ;
     NAME =  Name.str();

     Read(Cfg,T,Name.str(),nVarMax,iLumi) ;
 
   } // Variables
   } // TargetLumi

}

void UATmvaReader::DoPDERS( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t iLumi    = 0 ; iLumi   < (signed) Cfg.GetTargetLumi()->size() ; ++iLumi  ) {
   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove()   ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;


     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_PDERS_" << nVarMax << "Var" ;
     NAME =  Name.str();

     Read(Cfg,T,Name.str(),nVarMax,iLumi) ;

   } // Variables
   } // TargetLumi

}

void UATmvaReader::DoCUT( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t iLumi    = 0 ; iLumi   < (signed) Cfg.GetTargetLumi()->size() ; ++iLumi  ) {
   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove()   ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;


     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_CUT_" << nVarMax << "Var" ;
     NAME =  Name.str();

     Read(Cfg,T,Name.str(),nVarMax,iLumi) ;

   } // Variables
   } // TargetLumi

}


void UATmvaReader::DoPDEFoam( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t iLumi    = 0 ; iLumi   < (signed) Cfg.GetTargetLumi()->size() ; ++iLumi  ) {
   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove()   ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;


     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_PDEFoam_" << nVarMax << "Var" ;
     NAME =  Name.str();

     Read(Cfg,T,Name.str(),nVarMax,iLumi) ;

   } // Variables
   } // TargetLumi

}



void UATmvaReader::DoXML( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t iLumi    = 0 ; iLumi   < (signed) Cfg.GetTargetLumi()->size() ; ++iLumi  ) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() ; 

     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() ;
     NAME =  Name.str();
   
     Read(Cfg,T,Name.str(),nVarMax,iLumi) ;

   } // TargetLumi 

}

void UATmvaReader::Read( UATmvaConfig& Cfg, UATmvaTree& T, string Name, int nVarMax, int iLumi ) {

     TH1::SetDefaultSumw2(1);

     // Basic Reader Settings -> MVA Out Binning

     nbins  = Cfg.GetTmvaRespNBins() ; 
     minBin = Cfg.GetTmvaRespXMin() ;
     maxBin = Cfg.GetTmvaRespXMax() ;

     // 

     UAReader = new UATmvaReader_t() ; 
 
     cout << "[UATmvaReader] Reading: " << Name << endl;
     if ( Cfg.GetTmvaDim() == 1 ) {
       (UAReader->TmvaName).push_back(Name);
     } else {
       ostringstream NameND;
       NameND << Name << "_" << Cfg.GetTmvaDim() << "D" ;
       (UAReader->TmvaName).push_back(NameND.str()); // Geberic Name
       for ( int iDim = 1 ; iDim <= Cfg.GetTmvaDim() ; ++iDim ) {
         ostringstream Name1D;
         Name1D << Name << "_Dim" << iDim ;
         (UAReader->TmvaName).push_back(Name1D.str()); 
       }
     }

     // Load InputTrees branches
     Double_t treeWeight=1.;
     Int_t    IVar[nVarMax];
     Float_t  FVar[nVarMax];
     Double_t DVar[nVarMax];

     for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
        (T.GetTree(iD->NickName))->SetBranchStatus("*",1); // Enable all branch for now (helps for definint Presel cut anyway)
/*
        (T.GetTree(iD->NickName))->SetBranchStatus("*",0);
        if ( Cfg.GetTmvaWeight() != "" ) {
          (T.GetTree(iD->NickName))->SetBranchStatus(Cfg.GetTmvaWeight(),1);
          (T.GetTree(iD->NickName))->SetBranchAddress(Cfg.GetTmvaWeight(),&treeWeight);
        }
*/
        Int_t nVar = 0;
        for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
          if (++nVar <= nVarMax) {
            if ( iVar->VarType != 'E' )(T.GetTree(iD->NickName))->SetBranchStatus(iVar->VarName,1);
            if ( iVar->VarType == 'I' ) (T.GetTree(iD->NickName))->SetBranchAddress(iVar->VarName,&IVar[nVar]);
            if ( iVar->VarType == 'F' ) (T.GetTree(iD->NickName))->SetBranchAddress(iVar->VarName,&FVar[nVar]);
            if ( iVar->VarType == 'D' ) (T.GetTree(iD->NickName))->SetBranchAddress(iVar->VarName,&DVar[nVar]);
            if ( iVar->VarType == 'S' ) (T.GetTree(iD->NickName))->SetBranchAddress(iVar->VarName,&FVar[nVar]);
          } 
        } 
     }



     // Create TMVA Reader
     cout << "[UATmvaReader::Read()] Create: " << (UAReader->TmvaName).at(0) << endl;
     UAReader->TmvaReader = new Reader( "!Color:!Silent" );

     // Add VariablesCfg.GetTmvaVar())
     Int_t nVar = 0;     
     for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
       if (++nVar <= nVarMax) {
         if ( iVar->VarType == 'I' )  UAReader->TmvaReader->AddVariable(iVar->VarName,&FVar[nVar]);
         if ( iVar->VarType == 'F' )  UAReader->TmvaReader->AddVariable(iVar->VarName,&FVar[nVar]);
         if ( iVar->VarType == 'D' )  UAReader->TmvaReader->AddVariable(iVar->VarName,&FVar[nVar]);
         if ( iVar->VarType == 'E' )  UAReader->TmvaReader->AddVariable(iVar->VarName,&FVar[nVar]);
         if ( iVar->VarType == 'S' )  UAReader->TmvaReader->AddSpectator(iVar->VarName,&FVar[nVar]);
       }
     }

     // Fetch TMVA weights
     //TString TmvaWeightFile = "weights/"+UAReader->TmvaName+"_"+UAReader->TmvaName+".weights.xml" ;
     vector<TString> TmvaWeightFile ; 
     if ( Cfg.GetTmvaDim() == 1 ) {
       if ( Cfg.GetTmvaType() == "XML" ) { 
         TmvaWeightFile.push_back((Cfg.GetXMLFiles())->at(0)) ; 
       } else {
         TmvaWeightFile.push_back( "weights/UATmva_"+(UAReader->TmvaName).at(0)+".weights.xml" ) ;     
       } 
     } else {
        if ( Cfg.GetTmvaType() == "XML" ) { 
          if ( (Cfg.GetTmvaDim()) != (Cfg.GetXMLFiles())->size() ) {
            cout << "[UATmvaReader::Read()] ERROR: (Cfg.GetTmvaDim())->size() != (Cfg.GetXMLFiles())->size() !!!! " << endl;
            return ;
          }  
          for ( int iDim = 1 ; iDim <= Cfg.GetTmvaDim() ; ++iDim )
            TmvaWeightFile.push_back((Cfg.GetXMLFiles())->at(iDim-1)) ;
        } else { 
          for ( int iDim = 1 ; iDim <= Cfg.GetTmvaDim() ; ++iDim ) 
            TmvaWeightFile.push_back( "weights/UATmva_"+(UAReader->TmvaName).at(iDim)+".weights.xml" ) ; 
        }
     }

     // BookMethod
     if ( Cfg.GetTmvaDim() == 1 ) {
       cout << "METTHOD = " << (UAReader->TmvaName).at(0) << endl; 
       UAReader->TmvaReader->BookMVA( (UAReader->TmvaName).at(0) , TmvaWeightFile.at(0) ) ;
     } else {
       for ( int iDim = 1 ; iDim <= Cfg.GetTmvaDim() ; ++iDim ) 
         UAReader->TmvaReader->BookMVA( (UAReader->TmvaName).at(iDim) , TmvaWeightFile.at(iDim-1) ) ;
     }

     // Re-Open TMVA output file
     if ( Cfg.GetTmvaDim() == 1 ) {
       if ( Cfg.GetTmvaType() == "XML" ) { 
         (UAReader->TmvaFile).push_back( TFile::Open("rootfiles/" + (UAReader->TmvaName).at(0)  + ".root","RECREATE" ) );
       } else {
         (UAReader->TmvaFile).push_back( TFile::Open("rootfiles/" + (UAReader->TmvaName).at(0)  + ".root","UPDATE" )   );
       }
     } else {
       (UAReader->TmvaFile).push_back( TFile::Open("rootfiles/" + (UAReader->TmvaName).at(0)  + ".root","RECREATE" ) );
        if ( Cfg.GetTmvaType() == "XML" ) { 
          for ( int iDim = 1 ; iDim <= Cfg.GetTmvaDim() ; ++iDim ) 
            (UAReader->TmvaFile).push_back( TFile::Open("rootfiles/" + (UAReader->TmvaName).at(iDim)  + ".root","RECREATE" )   );
        } else {
          for ( int iDim = 1 ; iDim <= Cfg.GetTmvaDim() ; ++iDim ) 
            (UAReader->TmvaFile).push_back( TFile::Open("rootfiles/" + (UAReader->TmvaName).at(iDim)  + ".root","UPDATE" )   );
        } 
 
     }

     //string Directory("OutputHistograms");
     ostringstream Directory;
     Directory << "OutputHistograms_" << Cfg.GetTargetLumi()->at(iLumi).Lumi << "pbinv" ;
     for ( int iFile = 0 ; iFile < (signed) (UAReader->TmvaFile).size() ; ++iFile ) ((UAReader->TmvaFile).at(iFile))->mkdir(Directory.str().c_str()); 

     //UAReader->TmvaFile->cd(Directory.str().c_str());    


     // Fill Output for each tree
     cout << "[UATmvaReader::DoMLP()] Fill: " << (UAReader->TmvaName).at(0) << endl;

     TH1D* hMVA_data = new TH1D ("Data"     ,"Data"     ,nbins,minBin,maxBin) ;
     TH1D* hMVA_sig  = new TH1D ("Signal"   ,"Signal"   ,nbins,minBin,maxBin) ;
     TH1D* hMVA_bkgd = new TH1D ("BkgdTot"  ,"BkgdTot"  ,nbins,minBin,maxBin) ;
     TH1D* hMVA_bgTr = new TH1D ("BkgdTrain","BkgdTrain",nbins,minBin,maxBin) ;
     TH1D* hMVA_bgSp = new TH1D ("BkgdSpect","BkgdSpect",nbins,minBin,maxBin) ;

     vector<TH1D*> vhSignal;
     vector<TH1D*> vhBackground;

     cout <<(Cfg.GetInputData())->size() << endl;  
     for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
       // skip Train sample (unless we are in a poorman aproach of having Train=Test)
       if (iD->SigTrain  && Cfg.GetTestMode() != 1 ) continue;
       if (iD->BkgdTrain && Cfg.GetTestMode() != 1 ) continue;
       Double_t DaWeight=1.0; 
       Double_t SgWeight=1.0;
       Double_t BgWeight=1.0;
       Double_t Weight=1.0;
       // Set TargetLumi
       Double_t LumiScale = 1.;
       LumiScale = Cfg.GetTargetLumi()->at(iLumi).Lumi / (iD->Lumi/iD->ScaleFac) ;
       cout << iD->NickName << " : SampleLumi= " << iD->Lumi/iD->ScaleFac << " TargetLumi= " << Cfg.GetTargetLumi()->at(iLumi).Lumi << " --> LumiScale = " << LumiScale << endl;
       if ( Cfg.GetTargetLumi()->at(iLumi).useData ) {  
         if ( iD->TrueData &&  LumiScale != 1. ) cout << "[UATmvaReade::DoMLP] WARNING: Rescaling Data Luminosity" << endl; 
         DaWeight *= LumiScale;
       }
       else DaWeight = 0;
       SgWeight *= LumiScale;
       BgWeight *= LumiScale;
       cout << "------> DaWeight= " << DaWeight << " SgWeight= " << SgWeight << " BgWeight= " << BgWeight << endl;
       // File MVA Response
       TH1D* hMVA = new TH1D ((iD->NickName).c_str(),(iD->NickName).c_str(),nbins,minBin,maxBin) ;
       vector<TH1D*> hMVA_ND ;
       if ( Cfg.GetTmvaDim() != 1 ) {
         for ( int iDim = 1 ; iDim <= Cfg.GetTmvaDim() ; ++iDim ) {
           ostringstream hName;
           hName << iD->NickName << "_Dim" << iDim ;
           hMVA_ND.push_back ( new TH1D (hName.str().c_str(),hName.str().c_str(),nbins,minBin,maxBin) ) ;
         }
       }  
       vector<TH1F*> hCtrl ;
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) {
         TString HistName = iD->NickName+"_"+Cfg.GetCtrlPlot()->at(iP).VarName ;
         hCtrl.push_back( new TH1F (HistName,HistName,Cfg.GetCtrlPlot()->at(iP).nBins,Cfg.GetCtrlPlot()->at(iP).xMin,Cfg.GetCtrlPlot()->at(iP).xMax) );
       } 
       Int_t nEntries = (T.GetTree(iD->NickName))->GetEntries(); 
       // WeightFormula
       TTreeFormula* TreeWght = 0 ;
       if ( Cfg.GetTmvaWeight() != "" ) TreeWght = new TTreeFormula("TreeWght",(Cfg.GetTmvaWeight()).c_str(),T.GetTree(iD->NickName));
       // Recreate Tmva Preselection Cut
       TTreeFormula* Presel = 0 ; 
       if ( Cfg.GetTmvaPreCut() != "" ) {
         // define cut
         Presel = new TTreeFormula("Presel",(Cfg.GetTmvaPreCut()).c_str(),T.GetTree(iD->NickName));
         // Enable needed branches 
         for (Int_t bi = 0; bi<Presel->GetNcodes(); bi++) {
            T.GetTree(iD->NickName)->SetBranchStatus( Presel->GetLeaf(bi)->GetBranch()->GetName(), 1 );
         }
       }
       // Expression
       vector<TTreeFormula*> Expressions ;
       Int_t nnVar = 0;     
       for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
         ostringstream var;
         var << "var" << nnVar;
         if (++nnVar <= nVarMax) Expressions.push_back ( new TTreeFormula(var.str().c_str(), iVar->VarName , T.GetTree(iD->NickName) ) ) ;
       }
       // DataSetWeights
       for ( vector<DataSetWght_t>::iterator itDSW = (Cfg.GetDataSetWghts())->begin() ; itDSW != (Cfg.GetDataSetWghts())->end() ; ++itDSW ) itDSW->MakFormula(T.GetTree(iD->NickName));

       // Tree Loop
       cout << "[UATmvaReade::Read] Loop on tree:" << iD->NickName << endl; 
       for (Int_t jEntry = 0 ;  jEntry < nEntries ; ++jEntry) {
         (T.GetTree(iD->NickName))->GetEntry(jEntry);
         // Apply Tmva Preselection Cut
         if ( Presel && !(Presel->EvalInstance()) ) continue; 
         // ... convert everything to Float_t
         nVar = 0;     
         for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
           if (++nVar <= nVarMax) {
             if ( iVar->VarType == 'I' ) FVar[nVar] = IVar[nVar] ; 
             if ( iVar->VarType == 'D' ) FVar[nVar] = DVar[nVar] ;  
             if ( iVar->VarType == 'E' ) {
               if ( Expressions.at(nVar-1) ) FVar[nVar] = (Expressions.at(nVar))->EvalInstance() ; 
             }
             if ( iVar->VarName == "channel" && Cfg.GetTmvaType() == "XML" && Cfg.Getsmurfchannel() ) {
               int itypeNew = -1 ;
               int itype = FVar[nVar] ;
               bool Ema = false ;
               if ( Ema ) {
                 if ( FVar[nVar] == 0 ) itypeNew = 3 ;  
                 if ( FVar[nVar] == 1 ) itypeNew = 2 ;  
                 if ( FVar[nVar] == 2 ) itypeNew = 1 ;  
                 if ( FVar[nVar] == 3 ) itypeNew = 0 ;  
               } else {
                 if ( FVar[nVar] == 0 ) itypeNew = 0 ;  
                 if ( FVar[nVar] == 1 ) itypeNew = 3 ;  
                 if ( FVar[nVar] == 2 ) itypeNew = 2 ;  
                 if ( FVar[nVar] == 3 ) itypeNew = 1 ;  
               } 
               FVar[nVar] = itypeNew ;
               //cout << "Smurfing my channel #: " << itype << " --> " << itypeNew << endl;
             }   
             //cout <<  FVar[nVar] << " " ;
           }
         }
         // Evaluate DataSetWght Formula
         for ( vector<DataSetWght_t>::iterator itDSW = (Cfg.GetDataSetWghts())->begin() ; itDSW != (Cfg.GetDataSetWghts())->end() ; ++itDSW ) itDSW->EvaFormula();


         // ... Evaluate
         treeWeight  = 1.;
         if ( TreeWght ) treeWeight = TreeWght->EvalInstance() ; 
         Weight = treeWeight ; 
         if (iD->SigTest                ) Weight *= SgWeight;
         if (iD->BkgdData||iD->BkgdTest ) Weight *= BgWeight;
         if (iD->TrueData               ) Weight *= DaWeight;
         Double_t DataSetWeight = 1.0 ;
         for ( vector<DataSetWght_t>::iterator itDSW = (Cfg.GetDataSetWghts())->begin() ; itDSW != (Cfg.GetDataSetWghts())->end() ; ++itDSW ) {
            for ( vector<string>::iterator itDSN = (itDSW->DataSets).begin() ; itDSN != (itDSW->DataSets).end() ; ++itDSN ) {
              if ( (*itDSN) == iD->NickName ) DataSetWeight *= itDSW->Result() ;
            }
         }
         Weight *= DataSetWeight ;

         Double_t result = -99. ; 
         vector<Double_t> RESULTS ;
         bool kEvent = true;
         if ( Cfg.GetTmvaDim() == 1 ) { 
           result = UAReader->TmvaReader->EvaluateMVA((UAReader->TmvaName).at(0));
         } else {
           // result for each separated MVA
           for ( int iDim = 1 ; iDim <=  Cfg.GetTmvaDim() ; ++iDim ) {
             RESULTS.push_back ( UAReader->TmvaReader->EvaluateMVA((UAReader->TmvaName).at(iDim)) ) ;
           }
           // and combine
           //double r2 = 0. ;
           //for ( int iDim = 0 ; iDim < Cfg.GetTmvaDim() ; ++iDim ) r2 += pow((1.+RESULTS.at(iDim)),2)  ;
           //if (r2>=0.) result = sqrt(r2)-1.;
           //result = RESULTS.at(0) ; 
           //if ( RESULTS.at(1) < 0.5 ) kEvent = false ;

           result = 1.;
           for ( int iDim = 0 ; iDim < Cfg.GetTmvaDim() ; ++iDim ) { 
             double r;
             r = (RESULTS.at(iDim)+1.)/2. ;
             result *= r ; 
           }
           result = (2.*result-1.);

         }
         // TMVA result used in log mode 
         if ( Cfg.GetTmvaRespUseLog() ) {
           result = result + 1. + Cfg.GetTmvaRespXMin() ;
           result = log(result);
         }
//         cout << result << " " << Weight << endl;

         if ( Cfg.GetTmvaDim() != 1 ) {
           for ( int iDim = 0 ; iDim < Cfg.GetTmvaDim() ; ++iDim ) (hMVA_ND.at(iDim))->Fill( RESULTS.at(iDim) , Weight ) ;
         }
         if (kEvent) {
           hMVA -> Fill( result , Weight);
           if (iD->TrueData                ) hMVA_data -> Fill( result , Weight);
           if (iD->SigTest                 ) hMVA_sig  -> Fill( result , Weight);
           if (iD->BkgdData||iD->BkgdTest  ) hMVA_bkgd -> Fill( result , Weight);
           if (iD->BkgdData&&!iD->BkgdTest ) hMVA_bgSp -> Fill( result , Weight);
           if (iD->BkgdTest                ) hMVA_bgTr -> Fill( result , Weight);
           // Var Control Plots 
           for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) hCtrl.at(iP)->Fill( FVar[((Cfg.GetCtrlPlot())->at(iP)).iVarPos+1] , Weight);
         }
       }
       cout << "[UATmvaReade::Read] Loop End for tree:" << iD->NickName << endl; 
       ((UAReader->TmvaFile).at(0))->cd(Directory.str().c_str());
       hMVA->Write();
       if (iD->SigTest                ) vhSignal.push_back     ( (TH1D*) hMVA->Clone() ); 
       if (iD->BkgdData||iD->BkgdTest ) vhBackground.push_back ( (TH1D*) hMVA->Clone() );

       if ( Cfg.GetTmvaDim() != 1 ) 
         for ( int iDim = 0 ; iDim < Cfg.GetTmvaDim() ; ++iDim ) (hMVA_ND.at(iDim))->Write();
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { hCtrl.at(iP)->Write() ; } 
       if ( Cfg.GetTmvaDim() != 1 ) {
         for ( int iDim = 0 ; iDim < Cfg.GetTmvaDim() ; ++iDim ) {
           ((UAReader->TmvaFile).at(iDim+1))->cd(Directory.str().c_str());
           hMVA->Write();
           (hMVA_ND.at(iDim))->Write(); 
           for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { hCtrl.at(iP)->Write() ; }  
         }
       }
       for ( int iP = 0 ; iP < (signed) hMVA_ND.size()  ; ++iP ) { delete  hMVA_ND.at(iP) ; }
       hMVA_ND.clear();
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { delete hCtrl.at(iP) ; } 
       hCtrl.clear() ; 
       delete hMVA; 
       // Delete DataSetWght Formula
       for ( vector<DataSetWght_t>::iterator itDSW = (Cfg.GetDataSetWghts())->begin() ; itDSW != (Cfg.GetDataSetWghts())->end() ; ++itDSW ) itDSW->DelFormula();
     }     

     ((UAReader->TmvaFile).at(0))->cd(Directory.str().c_str());
     hMVA_data ->Write();
     hMVA_sig  ->Write();
     hMVA_bkgd ->Write(); 
     hMVA_bgSp ->Write(); 
     hMVA_bgTr ->Write(); 


     // --------------- Cut Based Reference --------------------------

     Double_t CutBased_Data                 = 0.;
     Double_t CutBased_Signal               = 0.;
     Double_t CutBased_Bkgd                 = 0.;
     Double_t CutBased_Limit                = 100. ;
     Double_t CutBased_Limit_Data           = 100. ;
     Double_t CutBased_SoverSqrtSPlusB      = 0. ;
     Double_t CutBased_SoverSqrtBPlusDeltaB = 0. ;


     //if ( Cfg.GetCutBasedHistName() != "NULL" ) {
     if ( (Cfg.GetCutBased())->size() > 0 ) {
       // Retrive yields
       for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {

         Double_t DaWeight=1.0; 
         Double_t SgWeight=1.0;
         Double_t BgWeight=1.0;
         // Set TargetLumi
         Double_t LumiScale = 1.;
         LumiScale = Cfg.GetTargetLumi()->at(iLumi).Lumi / (iD->Lumi/iD->ScaleFac) ;
         cout << iD->NickName << " : SampleLumi= " << iD->Lumi/iD->ScaleFac << " TargetLumi= " << Cfg.GetTargetLumi()->at(iLumi).Lumi << " --> LumiScale = " << LumiScale << endl;
         if ( Cfg.GetTargetLumi()->at(iLumi).useData ) {  
           if ( iD->TrueData &&  LumiScale != 1. ) cout << "[UATmvaReade::DoMLP] WARNING: Rescaling Data Luminosity" << endl; 
           DaWeight *= LumiScale;
         }
         else DaWeight = 0;
         SgWeight *= LumiScale;
         BgWeight *= LumiScale;
         cout << "------> DaWeight= " << DaWeight << " SgWeight= " << SgWeight << " BgWeight= " << BgWeight << endl;

/*
         TFile* File = new TFile(iD->FileName,"READ" );
         if (iD->TrueData                ) CutBased_Data   += DaWeight * ( (TH1*) File->Get(Cfg.GetCutBasedHistName()) ) -> GetBinContent(Cfg.GetCutBasedHistBin()) ;
         if (iD->SigTrain                ) CutBased_Signal += SgWeight * ( (TH1*) File->Get(Cfg.GetCutBasedHistName()) ) -> GetBinContent(Cfg.GetCutBasedHistBin()) ;
         if (iD->BkgdData||iD->BkgdTrain ) CutBased_Bkgd   += BgWeight * ( (TH1*) File->Get(Cfg.GetCutBasedHistName()) ) -> GetBinContent(Cfg.GetCutBasedHistBin()) ;
         File->Close();
         delete File;
*/
         for ( vector<CutBased_t>::iterator itCB = (Cfg.GetCutBased())->begin() ; itCB != (Cfg.GetCutBased())->end() ; ++ itCB ) {
           if ( itCB->NickName == iD->NickName ) {
             cout << "[UATmvaReader::CutBased] File = " << (itCB->File).c_str() << endl;
             TFile* File = new TFile((itCB->File).c_str(),"READ");
             cout << "[UATmvaReader::CutBased] Hist = " << (itCB->Hist).c_str() << " " << ( (TH1F*) File->Get((itCB->Hist).c_str()) ) -> GetBinContent(itCB->Bin) << endl;
             // Assuming the normalisation is done externally for cut based analysis:
             //if (iD->TrueData                ) CutBased_Data   += DaWeight * ( (TH1F*) File->Get((itCB->Hist).c_str()) ) -> GetBinContent(itCB->Bin) ;
             //if (iD->SigTrain                ) CutBased_Signal += SgWeight * ( (TH1F*) File->Get((itCB->Hist).c_str()) ) -> GetBinContent(itCB->Bin) ;
             //if (iD->BkgdData||iD->BkgdTrain ) CutBased_Bkgd   += BgWeight * ( (TH1F*) File->Get((itCB->Hist).c_str()) ) -> GetBinContent(itCB->Bin) ;
             if (iD->TrueData                ) CutBased_Data   += ( (TH1F*) File->Get((itCB->Hist).c_str()) ) -> GetBinContent(itCB->Bin) ;
             if (iD->SigTest                 ) CutBased_Signal += ( (TH1F*) File->Get((itCB->Hist).c_str()) ) -> GetBinContent(itCB->Bin) ;
             if (iD->BkgdData||iD->BkgdTest  ) CutBased_Bkgd   += ( (TH1F*) File->Get((itCB->Hist).c_str()) ) -> GetBinContent(itCB->Bin) ;
             File->Close();
             delete File; 
           }
         }
       }
       // Compute Significance and Limit
       CutBased_SoverSqrtSPlusB       = CutBased_Signal/sqrt(CutBased_Signal+CutBased_Bkgd);
       CutBased_SoverSqrtBPlusDeltaB  = CutBased_Signal/sqrt(CutBased_Bkgd+pow(0.35*CutBased_Bkgd,2)) ;
       init();
       CutBased_Limit = limitBayesian(CutBased_Bkgd,.35,CutBased_Signal,.2);
       delete wRoo;
       init();
       CutBased_Limit_Data = limitBayesian(CutBased_Data,.35,CutBased_Signal,.2);
       delete wRoo;

     }

     TH1D* CutBased = new TH1D("CutBased","CutBased",7,0.5,7.5);
     CutBased->SetBinContent(1,CutBased_Data );
     CutBased->SetBinContent(2,CutBased_Signal );
     CutBased->SetBinContent(3,CutBased_Bkgd );
     CutBased->SetBinContent(4,CutBased_SoverSqrtSPlusB  );
     CutBased->SetBinContent(5,CutBased_SoverSqrtBPlusDeltaB );
     CutBased->SetBinContent(6,CutBased_Limit );
     CutBased->SetBinContent(7,CutBased_Limit_Data );
     ((UAReader->TmvaFile).at(0))->cd(Directory.str().c_str());
     CutBased->Write();


     // Build Significance
     cout << "[UATmvaReader::DoMLP()] Evaluate: " << (UAReader->TmvaName).at(0) << endl;

     TH1D* bgTr_SoverB = GetSoverB("bgTr_SoverB",hMVA_sig ,hMVA_bgTr); bgTr_SoverB->Write();
     TH1D* bgSp_SoverB = GetSoverB("bgSp_SoverB",hMVA_sig ,hMVA_bgSp); bgSp_SoverB->Write();
     TH1D* bkgd_SoverB = GetSoverB("bkgd_SoverB",hMVA_sig ,hMVA_bkgd); bkgd_SoverB->Write();
 
     TH1D* bgTr_SoverSqrtSPlusB = GetSoverSqrtSPlusB("bgTr_SoverSqrtSPlusB",hMVA_sig ,hMVA_bgTr); bgTr_SoverSqrtSPlusB->Write();
     TH1D* bgSp_SoverSqrtSPlusB = GetSoverSqrtSPlusB("bgSp_SoverSqrtSPlusB",hMVA_sig ,hMVA_bgSp); bgSp_SoverSqrtSPlusB->Write();
     TH1D* bkgd_SoverSqrtSPlusB = GetSoverSqrtSPlusB("bkgd_SoverSqrtSPlusB",hMVA_sig ,hMVA_bkgd); bkgd_SoverSqrtSPlusB->Write();
     
     TH1D* bgTr_SoverSqrtBPlusDeltaB = GetSoverSqrtBPlusDeltaB("bgTr_SoverSqrtBPlusDeltaB",hMVA_sig ,hMVA_bgTr); bgTr_SoverSqrtBPlusDeltaB->Write();
     TH1D* bgSp_SoverSqrtBPlusDeltaB = GetSoverSqrtBPlusDeltaB("bgSp_SoverSqrtBPlusDeltaB",hMVA_sig ,hMVA_bgSp); bgSp_SoverSqrtBPlusDeltaB->Write();
     TH1D* bkgd_SoverSqrtBPlusDeltaB = GetSoverSqrtBPlusDeltaB("bkgd_SoverSqrtBPlusDeltaB",hMVA_sig ,hMVA_bkgd); bkgd_SoverSqrtBPlusDeltaB->Write();

     // Find Optimal Cut for Significance
     Double_t Cut_bgTr_SoverSqrtSPlusB = OptimalCutHigh(bgTr_SoverSqrtSPlusB);
     Double_t Cut_bgSp_SoverSqrtSPlusB = OptimalCutHigh(bgSp_SoverSqrtSPlusB);
     Double_t Cut_bkgd_SoverSqrtSPlusB = OptimalCutHigh(bkgd_SoverSqrtSPlusB);

     Double_t Cut_bgTr_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgTr_SoverSqrtBPlusDeltaB);
     Double_t Cut_bgSp_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgSp_SoverSqrtBPlusDeltaB);
     Double_t Cut_bkgd_SoverSqrtBPlusDeltaB = OptimalCutHigh(bkgd_SoverSqrtBPlusDeltaB);

     Double_t Sign_bgTr_SoverSqrtSPlusB = OptimalCutHigh(bgTr_SoverSqrtSPlusB,1);
     Double_t Sign_bgSp_SoverSqrtSPlusB = OptimalCutHigh(bgSp_SoverSqrtSPlusB,1);
     Double_t Sign_bkgd_SoverSqrtSPlusB = OptimalCutHigh(bkgd_SoverSqrtSPlusB,1);

     Double_t Sign_bgTr_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgTr_SoverSqrtBPlusDeltaB,1);
     Double_t Sign_bgSp_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgSp_SoverSqrtBPlusDeltaB,1);
     Double_t Sign_bkgd_SoverSqrtBPlusDeltaB = OptimalCutHigh(bkgd_SoverSqrtBPlusDeltaB,1);

     Double_t Bin_bgTr_SoverSqrtSPlusB = OptimalCutHigh(bgTr_SoverSqrtSPlusB,2);
     Double_t Bin_bgSp_SoverSqrtSPlusB = OptimalCutHigh(bgSp_SoverSqrtSPlusB,2);
     Double_t Bin_bkgd_SoverSqrtSPlusB = OptimalCutHigh(bkgd_SoverSqrtSPlusB,2);

     Double_t Bin_bgTr_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgTr_SoverSqrtBPlusDeltaB,2);
     Double_t Bin_bgSp_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgSp_SoverSqrtBPlusDeltaB,2);
     Double_t Bin_bkgd_SoverSqrtBPlusDeltaB = OptimalCutHigh(bkgd_SoverSqrtBPlusDeltaB,2);

     // Get Limits
     int LimitMethod =  0 ;
     TH1D* bgTr_BayesLimit = NULL ;
     TH1D* bgSp_BayesLimit = NULL ;
     TH1D* bkgd_BayesLimit = NULL ;
     if      ( LimitMethod == 0 ) {
       bgTr_BayesLimit = (TH1D*) hMVA_sig->Clone("bgTr_BayesLimit");
       bgSp_BayesLimit = (TH1D*) hMVA_sig->Clone("bgSp_BayesLimit");
       bkgd_BayesLimit = (TH1D*) hMVA_sig->Clone("bkgd_BayesLimit");
       bgTr_BayesLimit->Reset();
       bgSp_BayesLimit->Reset();
       bkgd_BayesLimit->Reset();
       bgTr_BayesLimit->SetTitle("bgTr_BayesLimit");
       bgSp_BayesLimit->SetTitle("bgSp_BayesLimit");
       bkgd_BayesLimit->SetTitle("bkgd_BayesLimit");
     }
     else if ( LimitMethod == 1 ) {
       bgTr_BayesLimit = GetExclusionLimit("bgTr_BayesLimit",hMVA_sig ,hMVA_bgTr);
       bgSp_BayesLimit = GetExclusionLimit("bgSp_BayesLimit",hMVA_sig ,hMVA_bgSp);
       bkgd_BayesLimit = GetExclusionLimit("bkgd_BayesLimit",hMVA_sig ,hMVA_bkgd);   
     }
     else if ( LimitMethod == 2 ) {
       bgTr_BayesLimit = (TH1D*) hMVA_sig->Clone("bgTr_BayesLimit");
       bgSp_BayesLimit = (TH1D*) hMVA_sig->Clone("bgSp_BayesLimit");
       bgTr_BayesLimit->Reset();
       bgSp_BayesLimit->Reset();
       bgTr_BayesLimit->SetTitle("bgTr_BayesLimit");
       bgSp_BayesLimit->SetTitle("bgSp_BayesLimit");
       bkgd_BayesLimit = GetExclusionLimitiACLs("bkgd_BayesLimit",hMVA_data,vhSignal,vhBackground,Cfg); bgSp_BayesLimit->Write(); 
     } else {
       bgTr_BayesLimit = GetExclusionLimit("bgTr_BayesLimit",hMVA_sig ,hMVA_bgTr); 
       bgSp_BayesLimit = GetExclusionLimitiACLs("bgSp_BayesLimit",hMVA_data,vhSignal,vhBackground,Cfg); 
       bkgd_BayesLimit = GetExclusionLimit("bkgd_BayesLimit",hMVA_sig ,hMVA_bkgd); 
     }

     bgTr_BayesLimit->Write();
     bgSp_BayesLimit->Write();
     bkgd_BayesLimit->Write();

     // Find Optimal Cut for Limit

     Double_t Cut_bgTr_BayesLimit = OptimalCutLow(bgTr_BayesLimit);
     Double_t Cut_bgSp_BayesLimit = OptimalCutLow(bgSp_BayesLimit);
     Double_t Cut_bkgd_BayesLimit = OptimalCutLow(bkgd_BayesLimit);

     Double_t Sign_bgTr_BayesLimit = OptimalCutLow(bgTr_BayesLimit,1);
     Double_t Sign_bgSp_BayesLimit = OptimalCutLow(bgSp_BayesLimit,1);
     Double_t Sign_bkgd_BayesLimit = OptimalCutLow(bkgd_BayesLimit,1);

     Double_t Bin_bgTr_BayesLimit = OptimalCutLow(bgTr_BayesLimit,2);
     Double_t Bin_bgSp_BayesLimit = OptimalCutLow(bgSp_BayesLimit,2);
     Double_t Bin_bkgd_BayesLimit = OptimalCutLow(bkgd_BayesLimit,2);

     // Find Cut matching cut base analysis

     Double_t Cut_CutBased  = FindCutBased(hMVA_sig,  CutBased_Signal );
     Double_t Sign_CutBased = FindCutBased(hMVA_sig,  CutBased_Signal,1 );
     Double_t Bin_CutBased  = FindCutBased(hMVA_sig,  CutBased_Signal,2 );

     // Keep Cuts

     TH1D* Cut = new TH1D("Cut","Cut",10,0.5,10.5);
     Cut->SetBinContent(1,Cut_bgTr_SoverSqrtSPlusB);
     Cut->SetBinContent(2,Cut_bgSp_SoverSqrtSPlusB);
     Cut->SetBinContent(3,Cut_bkgd_SoverSqrtSPlusB);
     Cut->SetBinContent(4,Cut_bgTr_SoverSqrtBPlusDeltaB);
     Cut->SetBinContent(5,Cut_bgSp_SoverSqrtBPlusDeltaB);
     Cut->SetBinContent(6,Cut_bkgd_SoverSqrtBPlusDeltaB);
     Cut->SetBinContent(7,Cut_bgTr_BayesLimit);
     Cut->SetBinContent(8,Cut_bgSp_BayesLimit);
     Cut->SetBinContent(9,Cut_bkgd_BayesLimit);
     Cut->SetBinContent(10,Cut_CutBased);
     Cut->Write();

     TH1D* Bin = new TH1D("Bin","Bin",10,0.5,10.5);
     Bin->SetBinContent(1,Bin_bgTr_SoverSqrtSPlusB);
     Bin->SetBinContent(2,Bin_bgSp_SoverSqrtSPlusB);
     Bin->SetBinContent(3,Bin_bkgd_SoverSqrtSPlusB);
     Bin->SetBinContent(4,Bin_bgTr_SoverSqrtBPlusDeltaB);
     Bin->SetBinContent(5,Bin_bgSp_SoverSqrtBPlusDeltaB);
     Bin->SetBinContent(6,Bin_bkgd_SoverSqrtBPlusDeltaB);
     Bin->SetBinContent(7,Bin_bgTr_BayesLimit);
     Bin->SetBinContent(8,Bin_bgSp_BayesLimit);
     Bin->SetBinContent(9,Bin_bkgd_BayesLimit);
     Bin->SetBinContent(10,Bin_CutBased);
     Bin->Write();

     // Keep Cut Discri values
     TH1D* Sign = new TH1D("Sign","Sign",9,0.5,9.5);
     Sign->SetBinContent(1,Sign_bgTr_SoverSqrtSPlusB);
     Sign->SetBinContent(2,Sign_bgSp_SoverSqrtSPlusB);
     Sign->SetBinContent(3,Sign_bkgd_SoverSqrtSPlusB);
     Sign->SetBinContent(4,Sign_bgTr_SoverSqrtBPlusDeltaB);
     Sign->SetBinContent(5,Sign_bgSp_SoverSqrtBPlusDeltaB);
     Sign->SetBinContent(6,Sign_bkgd_SoverSqrtBPlusDeltaB);
     Sign->SetBinContent(7,Sign_bgTr_BayesLimit);
     Sign->SetBinContent(8,Sign_bgSp_BayesLimit);
     Sign->SetBinContent(9,Sign_bkgd_BayesLimit);
     Sign->SetBinContent(10,Sign_CutBased);
     Sign->Write();

     // Keep Cut Limit values
     Double_t S = 0.;
     Double_t B = 0.;
     Double_t D = 0.;

     TH1D* Limit = new TH1D("Limit","Limit",10,0.5,10.5);

     for (Int_t iBin = 1 ; iBin < 9 ; ) {
      // Training Only
      S = hMVA_sig->Integral((int) Bin->GetBinContent(iBin),hMVA_sig->GetNbinsX());
      B = hMVA_bgTr->Integral((int) Bin->GetBinContent(iBin),hMVA_bgTr->GetNbinsX());
      init();
      Limit->SetBinContent(iBin,limitBayesian(B,.35,S,.2));
      delete wRoo;
      // Spectator Only
      S = hMVA_sig->Integral((int) Bin->GetBinContent(iBin+1),hMVA_sig->GetNbinsX());
      B = hMVA_bgSp->Integral((int) Bin->GetBinContent(iBin+1),hMVA_bgSp->GetNbinsX());
      init();
      Limit->SetBinContent(iBin+1,limitBayesian(B,.35,S,.2));
      delete wRoo;
      // All
      S = hMVA_sig->Integral((int) Bin->GetBinContent(iBin+2),hMVA_sig->GetNbinsX());
      B = hMVA_bkgd->Integral((int) Bin->GetBinContent(iBin+2),hMVA_bkgd->GetNbinsX());
      init();
      Limit->SetBinContent(iBin+2,limitBayesian(B,.35,S,.2));
      delete wRoo;
      iBin+=3;
     }
     S = hMVA_sig->Integral((int) Bin->GetBinContent(10),hMVA_sig->GetNbinsX());
     B = hMVA_bkgd->Integral((int) Bin->GetBinContent(10),hMVA_bkgd->GetNbinsX());
     init();
     Limit->SetBinContent(10,limitBayesian(B,.35,S,.2));
     delete wRoo;

     Limit->Write();

     // Data Limit
     TH1D* DataLimit = new TH1D("DataLimit","DataLimit",10,0.5,10.5);

     for (Int_t iBin = 1 ; iBin < 9 ; ) {
       // Training Only
       D = hMVA_data->Integral((int) Bin->GetBinContent(iBin),hMVA_data->GetNbinsX());
       S = hMVA_sig->Integral((int) Bin->GetBinContent(iBin),hMVA_sig->GetNbinsX());
       B = hMVA_bgTr->Integral((int) Bin->GetBinContent(iBin),hMVA_bgTr->GetNbinsX());
       //S = D-B;
       //if(S<0) S=0.;
       init();
//       cout << "[DataLimit] = " << limitBayesian(B,.35,S,.2) << endl;
       //DataLimit->SetBinContent(iBin,limitBayesian(B,.35,S,.2));
       DataLimit->SetBinContent(iBin,limitBayesian(D,.35,S,.2));
       delete wRoo;
       // Spectator Only
       D = hMVA_data->Integral((int) Bin->GetBinContent(iBin+1),hMVA_data->GetNbinsX());
       S = hMVA_sig->Integral((int) Bin->GetBinContent(iBin+1),hMVA_sig->GetNbinsX());
       B = hMVA_bgSp->Integral((int) Bin->GetBinContent(iBin+1),hMVA_bgSp->GetNbinsX());
       //S = D-B;
       //if(S<0) S=0.;
       init();
//       cout << "[DataLimit] = " << limitBayesian(B,.35,S,.2) << endl;
       //DataLimit->SetBinContent(iBin+1,limitBayesian(B,.35,S,.2));
       DataLimit->SetBinContent(iBin+1,limitBayesian(D,.35,S,.2));
       delete wRoo;
       // All
       D = hMVA_data->Integral((int) Bin->GetBinContent(iBin+2),hMVA_data->GetNbinsX());
       S = hMVA_sig->Integral((int) Bin->GetBinContent(iBin+2),hMVA_sig->GetNbinsX());
       B = hMVA_bkgd->Integral((int) Bin->GetBinContent(iBin+2),hMVA_bkgd->GetNbinsX());
       //S = D-B;
       //if(S<0) S=0.;
       init();
//       cout << "[DataLimit] = " << limitBayesian(B,.35,S,.2) << endl;
       //DataLimit->SetBinContent(iBin+2,limitBayesian(B,.35,S,.2));
       DataLimit->SetBinContent(iBin+2,limitBayesian(D,.35,S,.2));
       delete wRoo; 
       iBin+=3;
     }
     D = hMVA_data->Integral((int) Bin->GetBinContent(10),hMVA_data->GetNbinsX());
     S = hMVA_sig->Integral((int) Bin->GetBinContent(10),hMVA_sig->GetNbinsX());
     B = hMVA_bkgd->Integral((int) Bin->GetBinContent(10),hMVA_bkgd->GetNbinsX());
     //S = D-B;
     //if(S<0) S=0.;
     init();
//       cout << "[DataLimit] = " << limitBayesian(B,.35,S,.2) << endl;
     //DataLimit->SetBinContent(10,limitBayesian(B,.35,S,.2));
     DataLimit->SetBinContent(10,limitBayesian(D,.35,S,.2));
     delete wRoo;

     DataLimit->Write();


     // And the histo in the signal region ...

     TH1D* hMVA_data_CB = new TH1D ("Data_CB"     ,"Data_CB"     ,nbins,minBin,maxBin) ;
     TH1D* hMVA_sig_CB  = new TH1D ("Signal_CB"   ,"Signal_CB"   ,nbins,minBin,maxBin) ;
     TH1D* hMVA_data_NoCB = new TH1D ("Data_NoCB"     ,"Data_NoCB"     ,nbins,minBin,maxBin) ;
     TH1D* hMVA_sig_NoCB  = new TH1D ("Signal_NoCB"   ,"Signal_NoCB"   ,nbins,minBin,maxBin) ;


     for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) { 
       // skip Train sample (unless we are in a poorman aproach of having Train=Test)
       if (iD->SigTrain  && Cfg.GetTestMode() != 1 ) continue;
       if (iD->BkgdTrain && Cfg.GetTestMode() != 1 ) continue;
       Double_t DaWeight=1.0;
       Double_t SgWeight=1.0;
       Double_t BgWeight=1.0;
       Double_t Weight=1.0;
       // Set TargetLumi
       Double_t LumiScale = 1.;
       LumiScale = Cfg.GetTargetLumi()->at(iLumi).Lumi / (iD->Lumi/iD->ScaleFac) ;
       cout << iD->NickName << " : SampleLumi= " << iD->Lumi/iD->ScaleFac << " TargetLumi= " << Cfg.GetTargetLumi()->at(iLumi).Lumi << " --> LumiScale = " << LumiScale << endl;
       if ( Cfg.GetTargetLumi()->at(iLumi).useData ) {
         if ( iD->TrueData &&  LumiScale != 1. ) cout << "[UATmvaReade::DoMLP] WARNING: Rescaling Data Luminosity" << endl;
         DaWeight *= LumiScale;
       }
       else DaWeight = 0;
       SgWeight *= LumiScale;
       BgWeight *= LumiScale;
       cout << "------> DaWeight= " << DaWeight << " SgWeight= " << SgWeight << " BgWeight= " << BgWeight << endl;
       // WeightFormula
       TTreeFormula* TreeWght = 0 ;
       if ( Cfg.GetTmvaWeight() != "" ) TreeWght = new TTreeFormula("TreeWght",(Cfg.GetTmvaWeight()).c_str(),T.GetTree(iD->NickName));
       // Recreate Tmva Preselection Cut
       TTreeFormula* Presel = 0 ;
       if ( Cfg.GetTmvaPreCut() != "" ) {
         // define cut
         Presel = new TTreeFormula("Presel",(Cfg.GetTmvaPreCut()).c_str(),T.GetTree(iD->NickName));
         // Enable needed branches 
         for (Int_t bi = 0; bi<Presel->GetNcodes(); bi++) {
            T.GetTree(iD->NickName)->SetBranchStatus( Presel->GetLeaf(bi)->GetBranch()->GetName(), 1 );
         }
       }
       // Expression
       vector<TTreeFormula*> Expressions ;
       Int_t nnVar = 0;
       for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
         ostringstream var;
         var << "var" << nnVar;
         if (++nnVar <= nVarMax) Expressions.push_back ( new TTreeFormula(var.str().c_str(), iVar->VarName , T.GetTree(iD->NickName) ) ) ;
       }
       // Cut Base selection
       (Cfg.GetCutBaseSel())->MakFormula(T.GetTree(iD->NickName));
       // DataSetWeights
       for ( vector<DataSetWght_t>::iterator itDSW = (Cfg.GetDataSetWghts())->begin() ; itDSW != (Cfg.GetDataSetWghts())->end() ; ++itDSW ) itDSW->MakFormula(T.GetTree(iD->NickName));
       // Init histograms
       vector<TH1F*> hCtrlSig ;
       vector<TH1F*> hCtrlNoSig ;
       vector<TH1F*> hCtrlCB ;
       vector<TH1F*> hCtrlNoCB ;
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) {
         TString HistName = iD->NickName+"_SigSel_"+Cfg.GetCtrlPlot()->at(iP).VarName ;
         hCtrlSig.push_back( new TH1F (HistName,HistName,Cfg.GetCtrlPlot()->at(iP).nBins,Cfg.GetCtrlPlot()->at(iP).xMin,Cfg.GetCtrlPlot()->at(iP).xMax) );
       }
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) {
         TString HistName = iD->NickName+"_NoSigSel_"+Cfg.GetCtrlPlot()->at(iP).VarName ;
         hCtrlNoSig.push_back( new TH1F (HistName,HistName,Cfg.GetCtrlPlot()->at(iP).nBins,Cfg.GetCtrlPlot()->at(iP).xMin,Cfg.GetCtrlPlot()->at(iP).xMax) );
       }
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) {
         TString HistName = iD->NickName+"_CBSel_"+Cfg.GetCtrlPlot()->at(iP).VarName ;
         hCtrlCB.push_back( new TH1F (HistName,HistName,Cfg.GetCtrlPlot()->at(iP).nBins,Cfg.GetCtrlPlot()->at(iP).xMin,Cfg.GetCtrlPlot()->at(iP).xMax) );
       }
       TString MHistName = iD->NickName+"_CBSel" ;
       TH1D* hMVA_CB = new TH1D (MHistName,MHistName,nbins,minBin,maxBin) ;
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) {
         TString HistName = iD->NickName+"_NoCBSel_"+Cfg.GetCtrlPlot()->at(iP).VarName ;
         hCtrlNoCB.push_back( new TH1F (HistName,HistName,Cfg.GetCtrlPlot()->at(iP).nBins,Cfg.GetCtrlPlot()->at(iP).xMin,Cfg.GetCtrlPlot()->at(iP).xMax) );
       }
       MHistName = iD->NickName+"_NoCBSel" ;
       TH1D* hMVA_NoCB = new TH1D (MHistName,MHistName,nbins,minBin,maxBin) ;
       // Tree Loop
       Int_t nEntries = (T.GetTree(iD->NickName))->GetEntries();
       for (Int_t jEntry = 0 ;  jEntry < nEntries ; ++jEntry) {
         (T.GetTree(iD->NickName))->GetEntry(jEntry);
         // Apply Tmva Preselection Cut
         if ( Presel && !(Presel->EvalInstance()) ) continue;
         // ... convert everything to Float_t
         nVar = 0;
         for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
           if (++nVar <= nVarMax) {
             if ( iVar->VarType == 'I' ) FVar[nVar] = IVar[nVar] ;
             if ( iVar->VarType == 'D' ) FVar[nVar] = DVar[nVar] ;
             if ( iVar->VarType == 'E' ) {
               if ( Expressions.at(nVar-1) ) FVar[nVar] = (Expressions.at(nVar))->EvalInstance() ;
             }
             if ( iVar->VarName == "channel" && Cfg.GetTmvaType() == "XML" && Cfg.Getsmurfchannel() ) {
               int itypeNew = -1 ;
               int itype = FVar[nVar] ;
               bool Ema = false ;
               if ( Ema ) {
                 if ( FVar[nVar] == 0 ) itypeNew = 3 ;
                 if ( FVar[nVar] == 1 ) itypeNew = 2 ;
                 if ( FVar[nVar] == 2 ) itypeNew = 1 ;
                 if ( FVar[nVar] == 3 ) itypeNew = 0 ;
               } else {
                 if ( FVar[nVar] == 0 ) itypeNew = 0 ;
                 if ( FVar[nVar] == 1 ) itypeNew = 3 ;
                 if ( FVar[nVar] == 2 ) itypeNew = 2 ;
                 if ( FVar[nVar] == 3 ) itypeNew = 1 ;
               }
               FVar[nVar] = itypeNew ;
               //cout << "Smurfing my channel #: " << itype << " --> " << itypeNew << endl;
             }
             //cout <<  FVar[nVar] << " " ;
           }
         }
         // Evaluate DataSetWght Formula
         for ( vector<DataSetWght_t>::iterator itDSW = (Cfg.GetDataSetWghts())->begin() ; itDSW != (Cfg.GetDataSetWghts())->end() ; ++itDSW ) itDSW->EvaFormula();


         // ... Evaluate
         treeWeight  = 1.;
         if ( TreeWght ) treeWeight = TreeWght->EvalInstance() ;
         Weight = treeWeight ;
         if (iD->SigTest                ) Weight *= SgWeight;
         if (iD->BkgdData||iD->BkgdTest ) Weight *= BgWeight;
         if (iD->TrueData               ) Weight *= DaWeight;
         Double_t DataSetWeight = 1.0 ;
         for ( vector<DataSetWght_t>::iterator itDSW = (Cfg.GetDataSetWghts())->begin() ; itDSW != (Cfg.GetDataSetWghts())->end() ; ++itDSW ) {
            for ( vector<string>::iterator itDSN = (itDSW->DataSets).begin() ; itDSN != (itDSW->DataSets).end() ; ++itDSN ) {
              if ( (*itDSN) == iD->NickName ) DataSetWeight *= itDSW->Result() ;
            }
         }
         Weight *= DataSetWeight ;

         Double_t result = -99. ;
         vector<Double_t> RESULTS ;
         bool kEvent = true;
         if ( Cfg.GetTmvaDim() == 1 ) {
           result = UAReader->TmvaReader->EvaluateMVA((UAReader->TmvaName).at(0));
         } else {
           // result for each separated MVA
           for ( int iDim = 1 ; iDim <=  Cfg.GetTmvaDim() ; ++iDim ) {
             RESULTS.push_back ( UAReader->TmvaReader->EvaluateMVA((UAReader->TmvaName).at(iDim)) ) ;
           }
           // and combine
           //double r2 = 0. ;
           //for ( int iDim = 0 ; iDim < Cfg.GetTmvaDim() ; ++iDim ) r2 += pow((1.+RESULTS.at(iDim)),2)  ;
           //if (r2>=0.) result = sqrt(r2)-1.;
           //result = RESULTS.at(0) ; 
           //if ( RESULTS.at(1) < 0.5 ) kEvent = false ;

           result = 1.;
           for ( int iDim = 0 ; iDim < Cfg.GetTmvaDim() ; ++iDim ) {
             double r;
             r = (RESULTS.at(iDim)+1.)/2. ;
             result *= r ;
           }
           result = (2.*result-1.);

         }
         // TMVA result used in log mode 
         if ( Cfg.GetTmvaRespUseLog() ) {
           result = result + 1. + Cfg.GetTmvaRespXMin() ;
           result = log(result);
         }

         // Select MVA Signal region
         Float_t SigCut = Cut->GetBinContent(3) ; // Use SoverSqrtBPlusDeltaB [FIXME]
         if ( result > SigCut ) { 
           for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) hCtrlSig.at(iP)->Fill( FVar[((Cfg.GetCtrlPlot())->at(iP)).iVarPos+1] , Weight);
         } else {
           for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) hCtrlNoSig.at(iP)->Fill( FVar[((Cfg.GetCtrlPlot())->at(iP)).iVarPos+1] , Weight); 
         }

         // Select Cut Base Signal region
         (Cfg.GetCutBaseSel())->EvaFormula();
         if ( (Cfg.GetCutBaseSel())->Result() ) {
           for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) hCtrlCB.at(iP)->Fill( FVar[((Cfg.GetCtrlPlot())->at(iP)).iVarPos+1] , Weight);
           hMVA_CB->Fill(result,Weight);
           if (iD->TrueData                ) hMVA_data_CB -> Fill( result , Weight);
           if (iD->SigTest                 ) hMVA_sig_CB  -> Fill( result , Weight);
         } else {
           for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) hCtrlNoCB.at(iP)->Fill( FVar[((Cfg.GetCtrlPlot())->at(iP)).iVarPos+1] , Weight); 
           hMVA_NoCB->Fill(result,Weight);
           if (iD->TrueData                ) hMVA_data_NoCB -> Fill( result , Weight);
           if (iD->SigTest                 ) hMVA_sig_NoCB  -> Fill( result , Weight);
         }

         

       } // tree loop end
       // Write to File
       ((UAReader->TmvaFile).at(0))->cd(Directory.str().c_str());
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { hCtrlSig.at(iP)->Write() ; } 
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { hCtrlNoSig.at(iP)->Write() ; } 
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { hCtrlCB.at(iP)->Write() ; } 
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { hCtrlNoCB.at(iP)->Write() ; } 
       // clean
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { delete hCtrlSig.at(iP) ; }
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { delete hCtrlNoSig.at(iP) ; }
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { delete hCtrlCB.at(iP) ; }
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { delete hCtrlNoCB.at(iP) ; }
       hCtrlSig.clear() ;
       hCtrlNoSig.clear() ;
       hCtrlCB.clear() ;
       hCtrlNoCB.clear() ;

       hMVA_CB->Write() ;
       hMVA_NoCB->Write() ;
       delete hMVA_CB;
       delete hMVA_NoCB;

       for ( vector<DataSetWght_t>::iterator itDSW = (Cfg.GetDataSetWghts())->begin() ; itDSW != (Cfg.GetDataSetWghts())->end() ; ++itDSW ) itDSW->DelFormula();
       (Cfg.GetCutBaseSel())->DelFormula();


     } // Dataset loop end

     ((UAReader->TmvaFile).at(0))->cd(Directory.str().c_str());
     hMVA_data_CB ->Write();
     hMVA_sig_CB  ->Write();
     hMVA_data_NoCB ->Write();
     hMVA_sig_NoCB  ->Write();

     delete hMVA_data_CB;
     delete hMVA_sig_CB;
     delete hMVA_data_NoCB;
     delete hMVA_sig_NoCB;


     // clean histo's 
     delete hMVA_data;
     delete hMVA_sig;
     delete hMVA_bkgd;
     delete hMVA_bgSp;
     delete hMVA_bgTr;

     delete bgTr_SoverB ;
     delete bgSp_SoverB ;
     delete bkgd_SoverB ;

     delete bgTr_SoverSqrtSPlusB ;
     delete bgSp_SoverSqrtSPlusB ;
     delete bkgd_SoverSqrtSPlusB ;

     delete bgTr_SoverSqrtBPlusDeltaB ;
     delete bgSp_SoverSqrtBPlusDeltaB ;
     delete bkgd_SoverSqrtBPlusDeltaB ;

     delete bgTr_BayesLimit ;
     delete bgSp_BayesLimit ;
     delete bkgd_BayesLimit ;

     delete Cut ;
     delete Bin ;
     delete Sign ;
     delete Limit ;
     delete DataLimit ;

     // Clean UAReader
     delete UAReader ; 
   

}

