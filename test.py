import os
#from analyze import analyze
#from UATmva.analyze import analyze 

#analyze('test/rootfiles/2HDMaWjetsSRetaAda_2017_BDT_35Trees_AdaBoost_GiniIndex_20Cuts_CostComplexity_12PruneStrength_21Var.root')
scram_arch = os.popen('echo $SCRAM_ARCH').read().replace('\n', '')
print(scram_arch)
print(os.getcwd())
