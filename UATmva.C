
#include "src/UATmvaConfig.cc"
#include "src/UATmvaTree.cc"
#include "src/UATmvaClassification.cc"

void UATmva(){
  
  UATmvaConfig UATmvaConfigTest ;
  UATmvaConfigTest.ReadCfg("Config.cfg");
  UATmvaConfigTest.Print();

  UATmvaTree UATmvaTreeTest(UATmvaConfigTest);

  UATmvaClassification UATmva;
  UATmva.Do(UATmvaConfigTest,UATmvaTreeTest);
  
}
