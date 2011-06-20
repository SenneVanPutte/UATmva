#include "src/UATmvaConfig.cc"
#include "src/UATmvaTree.cc"
#include "src/UATmvaClassification.cc"
#include "src/figureOfMerit.cc"
#include "src/UATmvaTools.cc"
#include "src/UATmvaReader.cc"
#include "src/UATmvaSummary.cc"


void UATmva(TString Cfg = "Config.cfg" ){

  UATmvaConfig Config;
  Config.ReadCfg(Cfg);
  Config.Print();

//    UATmvaTree Tree(Config);

//    UATmvaClassification UATmva;
//    UATmva.Do(Config,Tree);

//    UATmvaReader Reader;
//    Reader.SetNbin(100); 
//    Reader.Do(Config,Tree); 
  
    UATmvaSummary Summary;
    Summary.Init(Config);
    Summary.Plots();

}
