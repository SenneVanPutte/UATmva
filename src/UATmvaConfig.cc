
#include "../includes/UATmvaConfig.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
//#include <boost/foreach>
//#include <boost/tokenizer>
//#include <boost/algorithm/string.hpp>

using namespace std;

// ----------------------


// ----------------------


void UATmvaConfig::Reset(){

  InitDone = false ;

  InputType = -1;
  TreeName  = "";
  InputData.clear();
  InputVar.clear();

  TmvaType           = "ANN" ;
  TmvaDim            = 1 ;
  TmvaOptim          = false ;
  TmvaVarTrans.clear();
  TmvaWeight         = "" ;
  TmvaVar.clear();
  TmvaPreCut         = "" ;
  TmvaVarNumRemove   = 0   ;

  CUTOptions         = "" ;

  ANNCycles          = 500 ;
  ANNHiddenLayersMin = 1   ;
  ANNHiddenLayersMax = 2   ;
  ANNHiddenNodesMin  = 0   ;
  ANNHiddenNodesMax  = 3   ;
  ANNFracTrain       = .5  ; 
  ANNFracTest        = .5  ;
  ANNFracValidate    = 1.  ;

  BDTNTrees        .clear();
  BDTBoostType     .clear();
  BDTAdaBoostR2Loss.clear();
  BDTUseBaggedGrad .clear();
  BDTGradBaggingFraction.clear();
  BDTShrinkage     .clear();
  BDTSeparationType.clear();
  BDTnCuts         .clear();
  BDTPruneMethod   .clear();
  BDTPruneStrength .clear();
  BDTNNodesMax     .clear();

  BDTNTrees        .push_back(400)             ;
  BDTBoostType     .push_back("AdaBoost")      ;    
  BDTSeparationType.push_back("GiniIndex")     ;
  BDTAdaBoostR2Loss.push_back("Quadratic")     ;
  BDTUseBaggedGrad .push_back("False")         ;
  BDTGradBaggingFraction.push_back(0.6)        ;
  BDTShrinkage     .push_back(1.0)             ;  
  BDTnCuts         .push_back(20)              ;
  BDTPruneMethod   .push_back("CostComplexity");
  BDTPruneStrength .push_back(12) ;
  BDTNNodesMax     .push_back(100000) ;

  XMLFile           = "NULL" ;

  TmvaRespNBins     = 440 ;
  TmvaRespXMin      = -1.1 ;
  TmvaRespXMax      =  1.1 ;
  TmvaRespRebinFac  =  10 ;

  //CutBasedHistName   = "NULL" ;
  //CutBasedHistBin    = -1 ;
  CutBased         .clear();

  PlotGroup.clear() ;
  CtrlPlot.clear()  ;

  Systematic.clear() ;

}

// ----------------------

void UAError(string mess){
  cout << mess;
  exit(1);
}

vector<string> UATokenize(string input , char token){
  stringstream ssinput(input);
  string Element;
  vector<string> Elements;
  while (getline(ssinput, Element , token )) {
   Elements.push_back(Element);
  }
  return Elements;
}

// ----------------------

void UATmvaConfig::ReadCfg(TString CfgName) {

  ifstream Cfg ;
  Cfg.open ( CfgName );

  if(!Cfg) {
    cout << "Cannot open input file.\n";
    return ;
  }

  char str[255];
  while( Cfg ) {

    Cfg.getline(str, 255);  
    if(!Cfg) continue;  
    istringstream iss(str);
    if (iss.str().find("#") != string::npos ) continue; 
    vector<string> Elements;
    do
    {
        string sub;
        iss >> sub;
        if(sub.size()>0) Elements.push_back(sub);
    } while (iss);
    if ( ! (Elements.size() > 0) ) continue;

    // TmvaName
    if ( Elements.at(0) == "TmvaName" ) {
      if   ( Elements.size() == 2 )  TmvaName = Elements.at(1) ;
      else UAError("[UATmvaConfig] Wrong TmvaName Input !");
    }

    // InputType
    if ( Elements.at(0) == "InputType" ) {
      if   ( Elements.size() == 2 )  InputType = atoi(Elements.at(1).c_str()) ;
      else UAError("[UATmvaConfig] Wrong InputType Input !");
    }

    // TreeName
    if ( Elements.at(0) == "TreeName" ) {
      if   ( Elements.size() == 2 ) TreeName = Elements.at(1) ;
      else UAError("[UATmvaConfig] Wrong InputType Input !");
    }


    // InputData
    if ( Elements.at(0) == "InputData" ) {
       if  ( Elements.size() == 9 ) {
          InputData_t InDat;
          InDat.NickName  = Elements.at(1);
          InDat.FileName  = Elements.at(8);
          InDat.SigTrain  = atoi(Elements.at(2).c_str()) ;
          InDat.BkgdTrain = atoi(Elements.at(3).c_str()) ;
          InDat.TrueData  = atoi(Elements.at(4).c_str()) ;
          InDat.BkgdData  = atoi(Elements.at(5).c_str()) ;
          InDat.Lumi      = atof(Elements.at(6).c_str()) ;
          InDat.ScaleFac  = atof(Elements.at(7).c_str()) ;
          InputData.push_back(InDat);
       }
       else UAError("[UATmvaConfig] Wrong InputData Input !");
    }

    // InputVar
    if ( Elements.at(0) == "InputVar" ) {
      if   ( Elements.size() == 2 ) SetInputVar(Elements.at(1),InputVar) ;
      else UAError("[UATmvaConfig] Wrong InputType Input !");
    }

    if ( Elements.at(0) == "TmvaType" ) {
      if   ( Elements.size() == 2 ) TmvaType = Elements.at(1) ;
      else UAError("[UATmvaConfig] Wrong TmvaType Input !");
    }

    if ( Elements.at(0) == "TmvaDim" ) {
      if   ( Elements.size() == 2 ) TmvaDim = atoi(Elements.at(1).c_str()) ;
      else UAError("[UATmvaConfig] Wrong TmvaDim Input !");
    }

    if ( Elements.at(0) == "TmvaOptim" ) {
      if   ( Elements.size() == 2 ) TmvaOptim = atoi(Elements.at(1).c_str()) ;
      else UAError("[UATmvaConfig] Wrong TmvaOptim Input !");
    }

    if ( Elements.at(0) == "TmvaVarTrans" ) {
      for ( int i=1 ; i < (signed) Elements.size() ; ++i ) TmvaVarTrans.push_back( Elements.at(i) ) ;
    }

    if ( Elements.at(0) == "TmvaWeight" ) {
      if   ( Elements.size() == 2 ) TmvaWeight = Elements.at(1) ;
      else UAError("[UATmvaConfig] Wrong TmvaWeight Input !");
    }

    if ( Elements.at(0) == "TmvaVar" ) {
      if   ( Elements.size() == 2 ) SetInputVar(Elements.at(1),TmvaVar) ;
      else UAError("[UATmvaConfig] Wrong TmvaVar Input !");
    }

    if ( Elements.at(0) == "TmvaPreCut" ) {
      if   ( Elements.size() == 2 ) TmvaPreCut = Elements.at(1) ;
      else UAError("[UATmvaConfig] Wrong TmvaPreCut Input !");
    }


    if ( Elements.at(0) == "TmvaVarNumRemove") {
      if   ( Elements.size() == 2 ) TmvaVarNumRemove = atoi(Elements.at(1).c_str()) ;
      else UAError("[UATmvaConfig] Wrong TmvaVarNumRemove Input !");
    }

    // ------------------ CUT Inputs -------------------------

    if ( Elements.at(0) == "CUTOptions") {
      if   ( Elements.size() == 2 ) CUTOptions = Elements.at(1) ;
    }

 
    // ------------------ MLP Inputs -------------------------
 
    if ( Elements.at(0) == "ANNCycles") {
      if   ( Elements.size() == 2 ) ANNCycles = atoi(Elements.at(1).c_str()) ;
      else UAError("[UATmvaConfig] Wrong ANNCycles Input !");
    }

    if ( Elements.at(0) == "ANNHiddenLayers") {
      if   ( Elements.size() == 3 ) {
           ANNHiddenLayersMin  = atoi(Elements.at(1).c_str()) ;
           ANNHiddenLayersMax  = atoi(Elements.at(2).c_str()) ;
      }
      else UAError("[UATmvaConfig] Wrong ANNHiddenLayers Input !");
    }

    if ( Elements.at(0) == "ANNHiddenNodes") {
      if   ( Elements.size() == 3 ) {
           ANNHiddenNodesMin  = atoi(Elements.at(1).c_str()) ;
           ANNHiddenNodesMax  = atoi(Elements.at(2).c_str()) ;
      }
      else UAError("[UATmvaConfig] Wrong ANNHiddenNodes Input !");
    }

    if ( Elements.at(0) == "ANNFracTrain") {
      if   ( Elements.size() == 2 ) ANNFracTrain = atof(Elements.at(1).c_str()) ;
      else UAError("[UATmvaConfig] Wrong ANNFracTrain Input !");
    }

    if ( Elements.at(0) == "ANNFracTest") {
      if   ( Elements.size() == 2 ) ANNFracTest = atof(Elements.at(1).c_str()) ;
      else UAError("[UATmvaConfig] Wrong ANNFracTest Input !");
    }

    if ( Elements.at(0) == "ANNFracValidate") {
      if   ( Elements.size() == 2 ) ANNFracValidate = atof(Elements.at(1).c_str()) ;
      else UAError("[UATmvaConfig] Wrong ANNFracValidate Input !");
    }

    // ------------------------------ BDT Options


   if ( Elements.at(0) == "BDTNTrees") {
     if   ( Elements.size() >= 2 ) {
       BDTNTrees.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTNTrees.push_back(atoi(Elements.at(iE).c_str()));
     }
     else UAError("[UATmvaConfig] Wrong BDTNTrees Input !"); 
   }

   if ( Elements.at(0) == "BDTBoostType") {
     if   ( Elements.size() >= 2 ) {
       BDTBoostType.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTBoostType.push_back(Elements.at(iE));
     }
     else UAError("[UATmvaConfig] Wrong BDTBoostType Input !"); 
   }

   if ( Elements.at(0) == "BDTAdaBoostR2Loss") {
     if   ( Elements.size() >= 2 ) {
       BDTAdaBoostR2Loss.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTAdaBoostR2Loss.push_back(Elements.at(iE));
     }
     else UAError("[UATmvaConfig] Wrong BDTAdaBoostR2Loss Input !");
   }

   if ( Elements.at(0) == "BDTUseBaggedGrad") {
     if   ( Elements.size() >= 2 ) {
       BDTUseBaggedGrad.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTUseBaggedGrad.push_back(Elements.at(iE));
     }
     else UAError("[UATmvaConfig] Wrong BDTUseBaggedGrad Input !");
   }

   if ( Elements.at(0) == "BDTGradBaggingFraction") {
     if   ( Elements.size() >= 2 ) {
       BDTGradBaggingFraction.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTGradBaggingFraction.push_back(atof(Elements.at(iE).c_str()));
     }
     else UAError("[UATmvaConfig] Wrong BDTGradBaggingFraction Input !");
   }

   if ( Elements.at(0) == "BDTShrinkage") {
     if   ( Elements.size() >= 2 ) {
       BDTShrinkage.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTShrinkage.push_back(atof(Elements.at(iE).c_str()));
     }
     else UAError("[UATmvaConfig] Wrong BDTShrinkage Input !");
   }


   if ( Elements.at(0) == "BDTSeparationType") {
     if   ( Elements.size() >= 2 ) {
       BDTSeparationType.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTSeparationType.push_back(Elements.at(iE));
     }
     else UAError("[UATmvaConfig] Wrong BDTSeparationType Input !"); 
   }

   if ( Elements.at(0) == "BDTnCuts") {
     if   ( Elements.size() >= 2 ) {
       BDTnCuts.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTnCuts.push_back(atoi(Elements.at(iE).c_str()));
     }
     else UAError("[UATmvaConfig] Wrong BDTnCuts Input !"); 
   }

   if ( Elements.at(0) == "BDTPruneMethod") {
     if   ( Elements.size() >= 2 ) {
       BDTPruneMethod.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTPruneMethod.push_back(Elements.at(iE));
     }
     else UAError("[UATmvaConfig] Wrong BDTPruneMethod Input !"); 
   }

   if ( Elements.at(0) == "BDTPruneStrength") {
     if   ( Elements.size() >= 2 ) {
       BDTPruneStrength.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTPruneStrength.push_back(atoi(Elements.at(iE).c_str()));
     }
     else UAError("[UATmvaConfig] Wrong BDTPruneStrength Input !"); 
   }

   if ( Elements.at(0) == "BDTNNodesMax") {
     if   ( Elements.size() >= 2 ) {
       BDTNNodesMax.clear();
       for ( int iE = 1 ; iE < (signed) Elements.size() ; ++iE ) BDTNNodesMax.push_back(atoi(Elements.at(iE).c_str()));
     }
     else UAError("[UATmvaConfig] Wrong BDTNNodesMax Input !");
   }


    // ------------------------------- External XML weights

    if ( Elements.at(0) == "XMLFile") {
      XMLFile = Elements.at(1) ; 
    }


    // ------------------------------- Reader MVA Output binning

    if ( Elements.at(0) == "TmvaRespBinning" ) {
      if   ( Elements.size() == 5 ) {
         TmvaRespNBins = atoi(Elements.at(1).c_str());
         TmvaRespXMin  = atof(Elements.at(2).c_str());
         TmvaRespXMax  = atof(Elements.at(3).c_str()); 
         TmvaRespRebinFac = atoi(Elements.at(4).c_str());
      }
      else UAError("[UATmvaConfig] Wrong TmvaResBinning Input !");
    }  


    // ------------------------------ Cut Based Yield

   if ( Elements.at(0) == "CutBased" ) {
/*
      if   ( Elements.size() == 3 ) {
        CutBasedHistName = Elements.at(1) ;
        CutBasedHistBin  = atoi(Elements.at(2).c_str()) ;
*/
      if   ( Elements.size() == 5 ) {
        CutBased_t CB;
        CB.NickName = Elements.at(1) ;
        CB.File     = Elements.at(2) ;
        CB.Hist     = Elements.at(3) ;
        CB.Bin      = atoi(Elements.at(4).c_str());
        CutBased.push_back(CB);
      }
      else UAError("[UATmvaConfig] Wrong CutBased Input !");

   }
 
   // Final Plot group ( for bkgd only )
   if ( Elements.at(0) == "PlotGroup" ) {
     if ( Elements.size() >= 3 ) {
       PlotGroup_t PlotGroupTmp;
       PlotGroupTmp.PlotGroupName  = Elements.at(1) ;
       PlotGroupTmp.PlotGroupColor = atoi(Elements.at(2).c_str()) ;
       for (int iMember = 3 ; iMember < (signed)Elements.size() ; ++iMember ) PlotGroupTmp.PlotGroupMember.push_back(Elements.at(iMember));
       PlotGroup.push_back(PlotGroupTmp);
     }
     else UAError("[UATmvaConfig] Wrong PlotGroup Input !");
   }

   // Control Plots
   if ( Elements.at(0) == "CtrlPlot" ) {
     if ( Elements.size() == 6 ) {
       // Check if variable is available
       TString VarName = Elements.at(1) ;
       Int_t iPosVar = -1;
       for ( int iV=0 ; iV < (signed) TmvaVar.size() ; ++iV ) {
         if ( VarName == TmvaVar.at(iV).VarName ) iPosVar = iV ; 
       }
       if ( iPosVar != -1 ) {
         CtrlPlot_t CtrlPlotTmp;
         CtrlPlotTmp.VarName = VarName ;
         CtrlPlotTmp.iVarPos = iPosVar ;
         CtrlPlotTmp.nBins   = atoi(Elements.at(2).c_str()) ;
         CtrlPlotTmp.xMin    = atoi(Elements.at(3).c_str()) ;
         CtrlPlotTmp.xMax    = atoi(Elements.at(4).c_str()) ;
         CtrlPlotTmp.kLogY   = atoi(Elements.at(5).c_str()) ;
         CtrlPlot.push_back(CtrlPlotTmp);
       }
       // else UAError("[UATmvaConfig] Wrong CtrlPlot VarName = "+Elements.at(1));
     }
     else UAError("[UATmvaConfig] Wrong CtrlPlot Input !");
   }

   // Plot Target Lumi
   if ( Elements.at(0) == "TargetLumi" ) {
     if ( Elements.size() == 3 ) {
       TargetLumi_t TargetLumiTmp;
       TargetLumiTmp.useData = atoi(Elements.at(1).c_str()) ;
       TargetLumiTmp.Lumi    = atof(Elements.at(2).c_str()) ;
       TargetLumi.push_back(TargetLumiTmp);
     }
     else UAError("[UATmvaConfig] Wrong TargetLumi Input !");    
   }


    // Sytematics
    if ( Elements.at(0) == "Systematic" ) {
      Systematic_t Syst ;
      Syst.systName = Elements.at(1) ;
      Syst.systType = Elements.at(2) ;
      Syst.systVal  = atof((Elements.at(3)).c_str()) ;
      vector<string> Member = UATokenize( Elements.at(4) , ':' );
      for ( int iM = 0 ; iM < (signed) Member.size() ; ++iM ) (Syst.systMember).push_back(Member.at(iM)) ;
      Systematic.push_back(Syst);
    }

  }

  // Dummy TargetLumi if needed --> 1fb-1
  if ( TargetLumi.size() == 0 ) {
    TargetLumi_t TargetLumiTmp;
    TargetLumiTmp.useData = 0     ;
    TargetLumiTmp.Lumi    = 1000. ;  
    TargetLumi.push_back(TargetLumiTmp);
  }

  InitDone = true;
 
  Cfg.close();
}

// ----------------------

void UATmvaConfig::SetInputVar(string sVarList, vector<InputVar_t>& vVarList){
   
   vector<string> Var = UATokenize( sVarList, ':' );
  
   for (vector<string>::iterator iVar = Var.begin() ; iVar != Var.end() ; ++iVar){
     vector<string> VarSplit = UATokenize( *iVar , '/' );
     if (VarSplit.size() == 1) {VarSplit.push_back("F");}
     InputVar_t InVar;
     InVar.VarName = VarSplit.at(0);
     InVar.VarType = VarSplit.at(1)[0];
     InVar.Active  = true; 
     vVarList.push_back(InVar);
   } 
   
}

// ----------------------

void UATmvaConfig::Print(){

  cout << "----------------------------------------------------------" << endl;
  cout << "----------------[UATmvaConfig::Print()] ------------------" << endl;
  cout << "----------------------------------------------------------" << endl;
  cout << "TmvaName   = " << TmvaName  << endl;
  cout << "----------------------------------------------------------" << endl;
  cout << "InputType  = " << InputType << endl;
  cout << "TreeName   = " << TreeName  << endl;
  cout << "----------------------------------------------------------" << endl;
  for ( vector<InputData_t>::iterator iD = InputData.begin() ; iD != InputData.end() ; ++iD ) {
    cout << "InputData  = " << iD->NickName << endl;
    cout << " --> FileName: " << iD->FileName << endl;
    cout << " --> SigTrain: "<<iD->SigTrain<<" BkgdTrain: "<< iD->SigTrain ;
    cout <<     " TrueData: "<<iD->TrueData<<" BkgdData : "<< iD->BkgdData << endl;
  }
  cout << "----------------------------------------------------------" << endl;
//  cout << "InputVar    = " << InputVarRaw << endl;
  for (vector<InputVar_t>::iterator iVar = InputVar.begin() ; iVar != InputVar.end() ; ++iVar ) {
    cout << "InputVar : " << iVar->VarName << " " << iVar->VarType << " " << iVar->Active << endl;
  }
  cout << "----------------------------------------------------------" << endl;
  cout << "TmvaType    = " << TmvaType << endl;
  if ( TmvaVarTrans.size() > 0 ) {
    cout << "TmvaVarTrans : " ;
    for ( int i = 0 ; i < (signed) TmvaVarTrans.size() ; ++i ) cout << " " << TmvaVarTrans.at(i) ;
    cout << endl;
  }
  cout << "TmvaWeight  = " << TmvaWeight << endl;
  for (vector<InputVar_t> ::iterator iVar = TmvaVar.begin() ; iVar != TmvaVar.end() ; ++iVar ) {
     cout << "TmvaVar : " << iVar->VarName << " " << iVar->VarType << " " << iVar->Active << endl;
  }
  cout << "TmvaVarNumRemove= " << TmvaVarNumRemove << endl;

  if ( TmvaType == "ANN" ) {
    cout << "--------------------- TMVA: ANN ---------------------------" << endl;
    cout << "ANNCycles       = " << ANNCycles << endl;
    cout << "ANNHiddenLayers = " << ANNHiddenLayersMin << " " << ANNHiddenLayersMax << endl;
    cout << "ANNHiddenNodes  = " << ANNHiddenNodesMin  << " " << ANNHiddenNodesMax  << endl;
    cout << "ANNFracTrain    = " << ANNFracTrain << endl;
    cout << "ANNFracTest     = " << ANNFracTest << endl;
    cout << "ANNFracValidate = " << ANNFracValidate << endl;
  }
 

  if ( TmvaType == "BDT" ) {
     cout << "--------------------- TMVA: BDT ---------------------------" << endl;
     cout << "BDTNTrees        = " ; for (int iE = 0 ; iE<(signed)BDTNTrees.size()         ; ++iE ) cout << BDTNTrees.at(iE)         << " " ; cout << endl;
     cout << "BDTBoostType     = " ; for (int iE = 0 ; iE<(signed)BDTBoostType.size()      ; ++iE ) cout << BDTBoostType.at(iE)      << " " ; cout << endl;
     cout << "BDTAdaBoostR2Loss= " ; for (int iE = 0 ; iE<(signed)BDTAdaBoostR2Loss.size() ; ++iE ) cout << BDTAdaBoostR2Loss.at(iE) << " " ; cout << endl;
     cout << "BDTUseBaggedGrad = " ; for (int iE = 0 ; iE<(signed)BDTUseBaggedGrad.size()  ; ++iE ) cout << BDTUseBaggedGrad.at(iE)  << " " ; cout << endl;
     cout << "BDTGradBaggingFraction = " ; for (int iE = 0 ; iE<(signed)BDTGradBaggingFraction.size()      ; ++iE ) cout << BDTGradBaggingFraction.at(iE)      << " " ; cout << endl;
     cout << "BDTShrinkage     = " ; for (int iE = 0 ; iE<(signed)BDTShrinkage.size()      ; ++iE ) cout << BDTShrinkage.at(iE)      << " " ; cout << endl;
     cout << "BDTSeparationType= " ; for (int iE = 0 ; iE<(signed)BDTSeparationType.size() ; ++iE ) cout << BDTSeparationType.at(iE) << " " ; cout << endl;
     cout << "BDTnCuts         = " ; for (int iE = 0 ; iE<(signed)BDTnCuts.size()          ; ++iE ) cout << BDTnCuts.at(iE)          << " " ; cout << endl;
     cout << "BDTPruneMethod   = " ; for (int iE = 0 ; iE<(signed)BDTPruneMethod.size()    ; ++iE ) cout << BDTPruneMethod.at(iE)    << " " ; cout << endl;
     cout << "BDTPruneStrength = " ; for (int iE = 0 ; iE<(signed)BDTPruneStrength.size()  ; ++iE ) cout << BDTPruneStrength.at(iE)  << " " ; cout << endl;
     cout << "BDTNNodesMax     = " ; for (int iE = 0 ; iE<(signed)BDTNNodesMax.size()      ; ++iE ) cout << BDTNNodesMax.at(iE)      << " " ; cout << endl;
  }

  if ( TmvaType == "XML" ) {
     cout << "--------------------- TMVA: XML ---------------------------" << endl;
     cout << "XMLFile  = " << XMLFile << endl;
  }

/*
  if ( CutBasedHistName != "NULL" ) {
    cout << "--------------------- Cut Based Limit ---------------------" << endl;
    cout << "CutBasedHistName = " << CutBasedHistName<< endl;
    cout << "CutBasedHistBin  = " << CutBasedHistBin<< endl;
  }
*/

  if ( PlotGroup.size() > 0 ) {
    cout << "--------------------- Plot Group ( for bkgd only ) --------" << endl;
    for (int iG=0 ; iG < (signed) PlotGroup.size() ; ++iG ) {
      cout << "PlotGroup :" << PlotGroup.at(iG).PlotGroupName << " " << PlotGroup.at(iG).PlotGroupColor << " = "; 
      for (int iGM=0 ; iGM < (signed) PlotGroup.at(iG).PlotGroupMember.size() ; ++iGM ) cout << PlotGroup.at(iG).PlotGroupMember.at(iGM) ;
      cout << endl;
    } 
  }

  if ( CtrlPlot.size() > 0 ) {
    cout << "--------------------- Control Plots -----------------------" << endl;
    for (int iG=0 ; iG < (signed) CtrlPlot.size() ; ++iG ) {
      cout << "CtrlPlot: " << CtrlPlot.at(iG).VarName 
                           << " (" << CtrlPlot.at(iG).nBins
                           << "," << CtrlPlot.at(iG).nBins
                           << "," << CtrlPlot.at(iG).nBins 
                           << ") " << CtrlPlot.at(iG).kLogY 
                           << endl;
    }
  }

  if (TargetLumi.size() > 0 ) {
    cout << "--------------------- Plot Target Lumi --------------------" << endl;
    for (int iL=0 ; iL < (signed) TargetLumi.size() ; ++iL ) {
      cout << "TargetLumi :  useData = " << TargetLumi.at(iL).useData << " Lumi = " << TargetLumi.at(iL).Lumi << endl;
    }
  }

}
