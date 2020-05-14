import os
import copy
import ROOT
import math
from array import array
from analyze import analyze
#mtw1/F:MHlnjj_deta_ljjVmet/F:MHlnjj_dphi_ljjVmet/F:MHlnjj_deta_jjVl/F:MHlnjj_dphi_jjVl/F:MHlnjj_dphi_lVmet/F:MHlnjj_deta_lVmet/F:PuppiMET_pt/F:MHlnjj_pt_ljj/F:MHlnjj_m_lmetjj/F:MHlnjj_PTljj_D_PTmet/F:MHlnjj_PTljj_D_Mlmetjj/F:MHlnjj_MINPTlj_D_PTmet/F:MHlnjj_MINPTlj_D_Mlmetjj/F:MHlnjj_dphi_jVj/F:MHlnjj_deta_jVj/F:MHlnjj_m_jj/F:MHlnjj_m_ljj/F:MHlnjj_MAXPTlj_D_PTmet/F:MHlnjj_MTljj_D_PTmet/F:MHlnjj_MTljj_D_Mlmetjj/F

# 2HDMa Ada
twoHDMa_Ada_21Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_deta_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_deta_jVj
twoHDMa_Ada_20Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_deta_ljjVmet => zero effect
twoHDMa_Ada_19Vars = ['mtw1', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_MINPTlj_D_Mlmetjj => sig is better (+1,4%) without but roc bit worse (-0.16%)
twoHDMa_Ada_18Vars = ['mtw1', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_PTljj_D_Mlmetjj => both better
twoHDMa_Ada_17Vars = ['mtw1', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_dphi_ljjVmet => least damage just to look (-1.04% sig -0.23% ROC)
twoHDMa_Ada_16Vars = ['mtw1', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_MINPTlj_D_PTmet => least damage just to look (+0.91% sig -0.058% ROC)
twoHDMa_Ada_15Vars = ['mtw1', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_dphi_lVmet => both better
twoHDMa_Ada_14Vars = ['mtw1', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']


# 2HDMa Grad
twoHDMa_Grad_21Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_deta_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_deta_jVj => zero effect
twoHDMa_Grad_20Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_MTljj_D_PTmet => roc and sig actually improve, but slightly mor overtraining
twoHDMa_Grad_19Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_m_lmetjj => both better
twoHDMa_Grad_18Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_dphi_jVj => both better
twoHDMa_Grad_17Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_PTljj_D_PTmet => zero effect
twoHDMa_Grad_16Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_dphi_lVmet => both better
twoHDMa_Grad_15Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_pt_ljj => best option (0.019% roc -0.088% sig)
twoHDMa_Grad_14Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_MINPTlj_D_PTmet => best option (-0.031%roc 0.77% sig)
twoHDMa_Grad_13Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_deta_jjVl => both better
twoHDMa_Grad_12Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MTljj_D_Mlmetjj']
#MHlnjj_deta_ljjVmet
twoHDMa_Grad_11Vars = ['mtw1', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MTljj_D_Mlmetjj']


# 2HDMa Grad + MHlnjj_MAXPTlj_D_Mlmetjj
twoHDMa_Grad_22Vars_ext = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_deta_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MAXPTlj_D_Mlmetjj','MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
#15+ MHlnjj_MAXPTlj_D_Mlmetjj to check
twoHDMa_Grad_16Vars_ext = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet','MHlnjj_MAXPTlj_D_Mlmetjj','MHlnjj_MTljj_D_Mlmetjj']

def train(cfg_file = 'BDT.cfg'):
    os.system('root -l -q \'../../UATmva.C("'+cfg_file+'", "T")\'')

def validate():
    r_files = os.listdir('rootfiles')
    for r_file in r_files:
        if 'BDT' in r_file: break
    return analyze('rootfiles/'+r_file)

def run(Vars, template):
    print('[run] making dirs and writing files')
    base_dir = mkDirs(Vars, template)
    os.chdir(base_dir)
    #res_file = open('../results.txt', 'w')
    #res_file.close()

    dirs = ['all'] + Vars
    #dirs = ['all']
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
    
def mkDirs(Vars, template):
    template_split = template.split('_')
    nVars = len(Vars)
    #nVars_dir = template_split[0] + '_' +template_split[1] +'_'+str(nVars-1)+'vars'
    nVars_dir = template_split[0] + '_' +template_split[1] +'_'+str(nVars-1)+'vars_ext'
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

    
    for var in Vars:
        tmp_var = copy.deepcopy(Vars)
        tmp_var.remove(var)
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

#run(twoHDMa_Ada_21Vars, '2HDMa_Ada_template.cfg')
#run(twoHDMa_Ada_20Vars, '2HDMa_Ada_template.cfg')
#run(twoHDMa_Ada_19Vars, '2HDMa_Ada_template.cfg')
#run(twoHDMa_Ada_18Vars, '2HDMa_Ada_template.cfg')
#run(twoHDMa_Ada_17Vars, '2HDMa_Ada_template.cfg')
#run(twoHDMa_Ada_16Vars, '2HDMa_Ada_template.cfg')
#run(twoHDMa_Ada_15Vars, '2HDMa_Ada_template.cfg')
#run(twoHDMa_Ada_14Vars, '2HDMa_Ada_template.cfg')

#run(twoHDMa_Grad_21Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_20Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_19Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_18Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_17Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_16Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_15Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_14Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_13Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_12Vars, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_11Vars, '2HDMa_Grad_template.cfg')

run(twoHDMa_Grad_22Vars_ext, '2HDMa_Grad_template.cfg')
#run(twoHDMa_Grad_16Vars_ext, '2HDMa_Grad_template.cfg')


#analyze('test/rootfiles/2HDMaWjetsSRetaAda_2017_BDT_35Trees_AdaBoost_GiniIndex_20Cuts_CostComplexity_12PruneStrength_21Var.root')
#os.chdir('20vars/MHlnjj_dphi_ljjVmet')
#os.chdir('20vars/mtw1')
#os.chdir('test/test')
#train()
#pm, sig, roc = validate()
#stry = '{:10}\t{:10}\t{:10}\t{:10}'.format('very_long_var_name', str(pm), str(sig), str(roc))
#print(stry)
 
