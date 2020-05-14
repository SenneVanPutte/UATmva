import os
import copy
import ROOT
import math
from array import array
from analyze import analyze


# 2HDMa Ada
twoHDMa_Ada_14Vars = ['mtw1', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
twoHDMa_Ada_14Vars_missing = ['MHlnjj_deta_jVj', 'MHlnjj_deta_ljjVmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_dphi_lVmet', 'MHlnjj_MAXPTlj_D_Mlmetjj']
#MHlnjj_dphi_lVmet => (-0.005% roc -2.81 sig) least damage to try
twoHDMa_Ada_15Vars = ['mtw1', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj', 'MHlnjj_dphi_lVmet']
twoHDMa_Ada_15Vars_missing = ['MHlnjj_deta_jVj', 'MHlnjj_deta_ljjVmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MAXPTlj_D_Mlmetjj']
#MHlnjj_MINPTlj_D_PTmet => (0.058% roc -0.90 sig) least damage to try
twoHDMa_Ada_16Vars = ['mtw1', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj', 'MHlnjj_dphi_lVmet', 'MHlnjj_MINPTlj_D_PTmet']
twoHDMa_Ada_16Vars_missing = ['MHlnjj_deta_jVj', 'MHlnjj_deta_ljjVmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_MAXPTlj_D_Mlmetjj']
#MHlnjj_dphi_ljjVmet => (0.23% roc 1.06% sig sig) both better
twoHDMa_Ada_17Vars = ['mtw1', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj', 'MHlnjj_dphi_lVmet', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_dphi_ljjVmet']
twoHDMa_Ada_17Vars_missing = ['MHlnjj_deta_jVj', 'MHlnjj_deta_ljjVmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MAXPTlj_D_Mlmetjj']


def train(cfg_file = 'BDT.cfg'):
    os.system('root -l -q \'../../UATmva.C("'+cfg_file+'", "T")\'')

def validate():
    r_files = os.listdir('rootfiles')
    for r_file in r_files:
        if 'BDT' in r_file: break
    return analyze('rootfiles/'+r_file)

def run(Vars, Vars_missing, template):
    print('[run] making dirs and writing files')
    base_dir = mkDirs(Vars, Vars_missing, template)
    os.chdir(base_dir)
    #res_file = open('../results.txt', 'w')
    #res_file.close()

    dirs = ['all'] + Vars_missing
    sig_l = []
    roc_l = []
    pref_str = '{:30}\t{:20}\t{:20}\t{:20}\t{:20}\t{:20}\t{:20}\n'.format('Variable', 'min KS', 'Max Significance', 'ROC Area', 'ROC Area up %', 'Sig Area', 'Sig Area up %')
    res_file = open('results.txt', 'a')
    res_file.write(pref_str)
    res_file.close()
    for var in dirs:
        print('[run] ____BEGIN_ANALYSING_REMOVED: '+var)
        os.chdir(var)
        print('[run] start training')
        train()
        print('[run] start validating')
        pm, sig, roc, sigy = validate()
        sig_l.append(sigy)
        roc_l.append(roc)
        if 'all' == var:
            sig_all = sigy
            roc_all = roc
        print('[run] storing results')
        pref_str = '{:30}\t{:20}\t{:20}\t{:20}\t{:20}\t{:20}\t{:20}\n'.format(var, str(pm), str(sig), str(roc), str(((roc - roc_all + 0.)/(roc_all + 0.))*100.), str(sigy), str(((sigy - sig_all + 0.)/(sig_all + 0.))*100.))
        res_file = open('../results.txt', 'a')
        res_file.write(pref_str)
        res_file.close()
        os.chdir('../')
    sig_ls, var_ss = (list(t) for t in zip(*sorted(zip(sig_l, dirs))))
    roc_ls, var_rs = (list(t) for t in zip(*sorted(zip(roc_l, dirs))))
    res_file = open('results.txt', 'a')
    res_file.write('\n')
    res_file.write('Ordered importance:\n')
    res_file.write('Sig area: '+str(var_ss)+'\n')
    res_file.write('ROC area: '+str(var_rs)+'\n')
    res_file.write('\n')
    res_file.close()
    
def mkDirs(Vars, Vars_missing, template):
    template_split = template.split('_')
    nVars = len(Vars)
    nVars_dir = template_split[0] + '_' +template_split[1] +'_'+str(nVars)+'+1vars'
    os.system('mkdir '+nVars_dir)

    #template_file = '2HDMa_Grad_template.cfg'
    template_file = template

    #All variables
    os.system('mkdir '+nVars_dir+'/all')
    os.system('mkdir '+nVars_dir+'/all/rootfiles')
    
    var_str = '/F:'.join(Vars)
    var_str += '/F'
    
    template = open(template_file, 'r')
    template_lines = template.readlines()
    template.close()
    
    cfg_file = open(nVars_dir+'/all/BDT.cfg', 'w')
    for line in template_lines:
        #print('_______newline_____')
        #print(line, type(line))
        check_str = '#VARIABLE_STRING'
        if check_str in line:
            cfg_file.write(line.replace(check_str, var_str))
        else: cfg_file.write(line)
    cfg_file.close()

    
    for var in Vars_missing:
        tmp_var = Vars + [var]
        #tmp_var.remove(var)
        #print(tmp_var)
        var_str = '/F:'.join(tmp_var)
        var_str += '/F'
        var_dir = nVars_dir+'/'+var
    
        os.system('mkdir '+var_dir)
        os.system('mkdir '+var_dir+'/rootfiles')
    
        #template = open('2HDMa_Ada_template.cfg', 'r')
        template = open(template_file, 'r')
        template_lines = template.readlines()
        template.close()
    
        cfg_file = open(var_dir+'/BDT.cfg', 'w')
        for line in template_lines:
            #print('_______newline_____')
            #print(line, type(line))
            check_str = '#VARIABLE_STRING'
            if check_str in line:
                cfg_file.write(line.replace(check_str, var_str))
            else: cfg_file.write(line)
        cfg_file.close()

    return nVars_dir    

#run(twoHDMa_Ada_14Vars, twoHDMa_Ada_14Vars_missing, '2HDMa_Ada_template.cfg')
#run(twoHDMa_Ada_15Vars, twoHDMa_Ada_15Vars_missing, '2HDMa_Ada_template.cfg')
#run(twoHDMa_Ada_16Vars, twoHDMa_Ada_16Vars_missing, '2HDMa_Ada_template.cfg')
run(twoHDMa_Ada_17Vars, twoHDMa_Ada_17Vars_missing, '2HDMa_Ada_template.cfg')

