import os
import copy
import ROOT
import math
from array import array
from analyze import analyze

#nTrees = [25, 50, 100, 200, 300]
#grad_trees = [50, 100, 200, 300, 400, 500, 600, 700]
#grad_trees = [150, 450, 550, 800, 900, 1000, 1200]
#grad_trees = [150, 475, 525, 650, 750, 1400, 1600]
#grad_trees = [725]
grad_trees = [50, 100, 150, 200, 300, 400, 450, 500, 550, 600, 700, 750, 800, 900, 1000, 1200, 1400, 1600]
ada_trees = [25, 37, 42, 50, 52, 55, 57, 60, 62, 75, 80, 90, 100, 150, 200, 250, 300]

twoHDMa_12Vars = ['mtw1', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
twoHDMa_15Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
twoHDMa_22Vars = ['mtw1', 'MHlnjj_deta_ljjVmet', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_deta_jjVl', 'MHlnjj_dphi_jjVl', 'MHlnjj_dphi_lVmet', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_m_lmetjj', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_deta_jVj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MAXPTlj_D_Mlmetjj','MHlnjj_MTljj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']


def write_python(source_dir, var):
    py_file = open('job.py', 'w')
    # imports
    py_file.write('#!/usr/bin/env python\n')
    py_file.write('import os\n')
    py_file.write('import ROOT\n')
    py_file.write('from analyze import analyze_ks\n')
    py_file.write('\n')
  
    # train
    py_file.write('ROOT.gROOT.LoadMacro(\''+source_dir+'/UATmva.C\')\n')
    py_file.write('ROOT.UATmva("BDT.cfg", "T")\n')
    py_file.write('\n')
    
    # validate
    py_file.write('r_files = os.listdir(\'rootfiles\')\n')
    py_file.write('for r_file in r_files:\n')
    py_file.write('    if \'BDT\' in r_file: break\n')
    py_file.write('ps, pb, pm, sig, roc, sigy = analyze_ks(\'rootfiles/\'+r_file)\n')
    py_file.write('\n')

    # wite results
    py_file.write('res_str = \'{:30}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\'.format(\''+var+'\', ps, pb, pm, sig, roc, sigy)\n')
    py_file.write('res_file = open(\'result.txt\', \'w\')\n')
    py_file.write('res_file.write(res_str)\n')
    py_file.write('res_file.close()\n')
    py_file.close()
    
def write_bash(source_dir, var): 
    sh_file = open('job.sh', 'w')
    # initial stuff    
    sh_file.write('#!/bin/bash\n')
    sh_file.write('export X509_USER_PROXY=/user/svanputt/.proxy\n')
    sh_file.write('voms-proxy-info\n')
    scram_arch = os.popen('echo $SCRAM_ARCH').read().replace('\n', '')
    sh_file.write('export SCRAM_ARCH='+scram_arch+'\n')
    sh_file.write('export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch\n')
    sh_file.write('source $VO_CMS_SW_DIR/cmsset_default.sh\n')
    sh_file.write('cd '+source_dir+'/../../\n')
    sh_file.write('eval `scramv1 ru -sh`\n')
    sh_file.write('ulimit -c 0\n')
    sh_file.write('cd $TMPDIR \n')
    sh_file.write('\n')

    # get python and run
    sh_file.write('cp -r '+os.getcwd()+' .\n')
    sh_file.write('cd '+var+'\n')
    sh_file.write('cp '+source_dir+'/analyze.py .\n')
    sh_file.write('python job.py\n') 
    sh_file.write('ls -l\n')
    sh_file.write('\n')
    os.system('chmod +x job.sh')

    # move output back
    sh_file.write('cp result.txt '+os.getcwd()+'/.\n')
    sh_file.write('cp rootfiles/* '+os.getcwd()+'/rootfiles/.\n')
    sh_file.write('cp -r weights '+os.getcwd()+'/.\n')
    sh_file.close()

def write_cfg(source_dir, template_file, var_str, trees):
    template = open(source_dir+'/'+template_file, 'r')
    template_lines = template.readlines()
    template.close()
    
    cfg_file = open('BDT.cfg', 'w')
    for line in template_lines:
        if '#VARIABLE_STRING' in line:
            cfg_file.write(line.replace('#VARIABLE_STRING', var_str))
        if '#TREES' in line:
            cfg_file.write(line.replace('#TREES', str(trees)))
        else: cfg_file.write(line)
    cfg_file.close()
   
def write_collect(start_dir):
    pwd = os.getcwd()
    os.chdir(start_dir)
    co_file = open('collect.py', 'w')
    # imports
    co_file.write('import os\n') 
    co_file.write('from analyze import tree_sort\n')
    co_file.write('from operator import itemgetter\n') 
    co_file.write('\n')

    # results file   
    co_file.write('path = \''+os.getcwd()+'\'\n')
    co_file.write('result_file = open(path+\'/results.txt\', \'w\')\n')
    co_file.write('open_str = \'{:30}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\n\'.format(\'Variable\', \'KS signal\', \'KS backgr\', \'min KS\', \'Max Significance\', \'ROC Area\', \'Sig Area\')\n')
    co_file.write('result_file.write(open_str)\n')
    co_file.write('\n')

    # dirs  
    co_file.write('dirs = os.listdir(path)\n')
    co_file.write('dirs.sort()\n')
    co_file.write('tree_l = []\n')
    co_file.write('psl_l = []\n')
    co_file.write('pbl_l = []\n')
    co_file.write('pml_l = []\n')
    co_file.write('sig_l = []\n')
    co_file.write('roc_l = []\n')
    co_file.write('for dir in dirs:\n')
    co_file.write('\n')
    co_file.write('    if not os.path.isdir(path+\'/\'+dir): continue\n')
    co_file.write('    res_file = open(path+\'/\'+dir+\'/result.txt\', \'r\')\n')
    co_file.write('    lines = res_file.readlines()\n')
    co_file.write('    res_file.close()\n')
    co_file.write('\n')
    co_file.write('    for line in lines:\n')
    co_file.write('        line_sp = line.replace(\' \', \'\').split(\'\t\')\n')
    co_file.write('        if len(line_sp) == 7:\n')
    co_file.write('            var = line_sp[0]\n')
    co_file.write('            sig_ks  = float(line_sp[1])\n')
    co_file.write('            bck_ks  = float(line_sp[2])\n')
    co_file.write('            min_ks  = float(line_sp[3])\n')
    co_file.write('            max_sig = float(line_sp[4])\n')
    co_file.write('            roc_a   = float(line_sp[5])\n')
    co_file.write('            sig_a   = float(line_sp[6])\n')
    co_file.write('            break\n')
    co_file.write('    tree_l.append(int(var.replace(\'Trees\', \'\')))\n')
    co_file.write('    psl_l.append(sig_ks)\n')
    co_file.write('    pbl_l.append(bck_ks)\n')
    co_file.write('    pml_l.append(min_ks)\n')
    co_file.write('    sig_l.append(sig_a)\n')
    co_file.write('    roc_l.append(roc_a)\n')
    co_file.write('    var_str = \'{:30}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\n\'.format(var, str(sig_ks), str(bck_ks), str(min_ks), str(max_sig), str(roc_a), str(sig_a))\n')
    co_file.write('    result_file.write(var_str)\n')
    co_file.write('\n')

    # plot str
    co_file.write('psl_l = tree_sort(tree_l, psl_l)\n')
    co_file.write('pbl_l = tree_sort(tree_l, pbl_l)\n')
    co_file.write('pml_l = tree_sort(tree_l, pml_l)\n')
    co_file.write('sig_l = tree_sort(tree_l, sig_l)\n')
    co_file.write('roc_l = tree_sort(tree_l, roc_l)\n')
    co_file.write('tree_l = tree_sort(tree_l, tree_l)\n')
    co_file.write('result_file.write(\'\\n\')\n')
    co_file.write('result_file.write(\'tree_l = [\'+\', \'.join([str(t) for t in tree_l])+\']\\n\')\n')
    co_file.write('result_file.write(\'psl_l = [\'+\', \'.join([str(t) for t in psl_l])+\']\\n\')\n')
    co_file.write('result_file.write(\'pbl_l = [\'+\', \'.join([str(t) for t in pbl_l])+\']\\n\')\n')
    co_file.write('result_file.write(\'pml_l = [\'+\', \'.join([str(t) for t in pml_l])+\']\\n\')\n')
    co_file.write('result_file.write(\'sig_l = [\'+\', \'.join([str(t) for t in sig_l])+\']\\n\')\n')
    co_file.write('result_file.write(\'roc_l = [\'+\', \'.join([str(t) for t in roc_l])+\']\\n\')\n')

    # get max KSmin idx
    co_file.write('result_file.write(\'\\n\')\n')
    co_file.write('maxKS_idx = max(enumerate(pml_l), key=itemgetter(1))[0]\n')
    co_file.write('result_file.write(\'maxKS : \'+str(pml_l[maxKS_idx])+\'\\n\')\n')
    co_file.write('result_file.write(\'sig   : \'+str(sig_l[maxKS_idx])+\'\\n\')\n')
    co_file.write('result_file.write(\'roc   : \'+str(roc_l[maxKS_idx])+\'\\n\')\n')
    co_file.write('result_file.write(\'trees : \'+str(tree_l[maxKS_idx])+\'\\n\')\n')
    co_file.write('result_file.close()\n')
    co_file.write('\n')
    os.chdir(pwd)

    

def run(Vars, nTrees, template):
    print('[run] making dirs and writing files')
    base_dir = mkDirs(Vars, nTrees, template)
    print('[run] starting submissions')
    sub(base_dir, nTrees)
    print('[run] write collection file')
    write_collect(base_dir)

def sub(start_dir, new_jobs):
    queue='localgrid@cream02'
    QSOPT='-l walltime=168:00:00'
    source_dir = os.getcwd()
    os.system('mkdir job/'+start_dir)
    hits = os.listdir(source_dir+'/'+start_dir)
    for hit in hits:
        path = source_dir+'/'+start_dir+'/'+hit
        if not os.path.isdir(path): continue
        submit = False
        for job in new_jobs:
            if str(job) in hit: 
                submit = True
                break
        if not submit: continue
        print('[sub] submitting '+hit)
        nTry=0
        jName = start_dir+'_'+hit
        outFile = 'job/'+start_dir+'/'+hit+'.out' 
        errFile = 'job/'+start_dir+'/'+hit+'.err'
        jobFile = path+'/job.sh' 
        while nTry < 3 : 
            nTry += 1
            jobid = os.system('qsub '+QSOPT+' -N '+jName+' -q '+queue+' -o '+outFile+' -e '+errFile+' '+jobFile)
            print('[sub] attempt: '+str(nTry)+'--> return : '+str(jobid))
            if jobid == 0 : nTry = 999
            #else:  os.system('rm '+jidFile)
 
def mkDirs(Vars, nTrees, template_file):
    source_dir = os.getcwd()
 
    template_split = template_file.split('_')
    nVars_dir = template_split[0] + '_' +template_split[1] +'_'+str(len(Vars))+'vars_trees'
    os.system('mkdir '+nVars_dir)
    os.system('cp analyze.py '+nVars_dir+'/analyze.py')
    os.chdir(nVars_dir)

    var_str = '/F:'.join(Vars)
    var_str += '/F'

    for trees in nTrees:
        trees_str = str(trees)+'Trees'
 
        os.system('mkdir '+trees_str)
        os.system('mkdir '+trees_str+'/rootfiles')

        os.chdir(trees_str)
        write_cfg(source_dir, template_file, var_str, trees)
        write_bash(source_dir, trees_str)
        write_python(source_dir, trees_str)
        os.chdir('../')
    os.chdir('../')
    

    return nVars_dir    

# Redo collect
#run(twoHDMa_12Vars, [], '2HDMa_Ada_trees_template.cfg')
#run(twoHDMa_15Vars, [], '2HDMa_Ada_trees_template.cfg')
#run(twoHDMa_22Vars, [], '2HDMa_Ada_trees_template.cfg')

#run(twoHDMa_12Vars, ada_trees, '2HDMa_Ada_trees_template.cfg')
#run(twoHDMa_15Vars, ada_trees, '2HDMa_Ada_trees_template.cfg')
#run(twoHDMa_22Vars, ada_trees, '2HDMa_Ada_trees_template.cfg')

#run(twoHDMa_12Vars, grad_trees, '2HDMa_Grad_trees_template.cfg')
#run(twoHDMa_15Vars, grad_trees, '2HDMa_Grad_trees_template.cfg')
#run(twoHDMa_22Vars, grad_trees, '2HDMa_Grad_trees_template.cfg')

# darkHiggs 
#run(twoHDMa_12Vars, grad_trees, 'darkHiggs_Grad_trees_template.cfg')
#run(twoHDMa_15Vars, grad_trees, 'darkHiggs_Grad_trees_template.cfg')
#run(twoHDMa_22Vars, grad_trees, 'darkHiggs_Grad_trees_template.cfg')

# High Mass
run(twoHDMa_12Vars, grad_trees, 'darkHiggsHM_Grad_trees_template.cfg')
run(twoHDMa_15Vars, grad_trees, 'darkHiggsHM_Grad_trees_template.cfg')
run(twoHDMa_22Vars, grad_trees, 'darkHiggsHM_Grad_trees_template.cfg')
