import os
import copy
import ROOT
import math
from array import array
from analyze import analyze

# 2HDMa Grad + MHlnjj_MAXPTlj_D_Mlmetjj
twoHDMa_Grad_12Vars = ['mtw1', 'MHlnjj_dphi_ljjVmet', 'MHlnjj_dphi_jjVl', 'MHlnjj_deta_lVmet', 'PuppiMET_pt', 'MHlnjj_pt_ljj', 'MHlnjj_PTljj_D_Mlmetjj', 'MHlnjj_MINPTlj_D_Mlmetjj', 'MHlnjj_m_jj', 'MHlnjj_m_ljj', 'MHlnjj_MAXPTlj_D_PTmet', 'MHlnjj_MTljj_D_Mlmetjj']
twoHDMa_Grad_12Vars_mis = ['MHlnjj_MTljj_D_PTmet', 'MHlnjj_deta_jVj', 'MHlnjj_MAXPTlj_D_Mlmetjj', 'MHlnjj_m_lmetjj', 'MHlnjj_dphi_jVj', 'MHlnjj_dphi_lVmet', 'MHlnjj_PTljj_D_PTmet', 'MHlnjj_MINPTlj_D_PTmet', 'MHlnjj_deta_jjVl', 'MHlnjj_deta_ljjVmet', 'MHlnjj_mt_lmetjj', 'MHlnjj_mt_jj', 'MHlnjj_mt_ljj', 'MHlnjj_pt_jj', 'MHlnjj_pt_lmetjj']

def write_python(source_dir, var):
    py_file = open('job.py', 'w')
    # imports
    py_file.write('#!/usr/bin/env python\n')
    py_file.write('import os\n')
    py_file.write('import ROOT\n')
    py_file.write('from analyze import analyze\n')
    py_file.write('\n')
  
    # train
    py_file.write('ROOT.gROOT.LoadMacro(\''+source_dir+'/UATmva.C\')\n')
    py_file.write('ROOT.UATmva("BDT.cfg", "T")\n')
    py_file.write('\n')
    
    # validate
    py_file.write('r_files = os.listdir(\'rootfiles\')\n')
    py_file.write('for r_file in r_files:\n')
    py_file.write('    if \'BDT\' in r_file: break\n')
    py_file.write('pm, sig, roc, sigy = analyze(\'rootfiles/\'+r_file)\n')
    py_file.write('\n')

    # wite results
    py_file.write('res_str = \'{:30}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\'.format(\''+var+'\', pm, sig, roc, sigy)\n')
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
    sh_file.write('cp '+source_dir+'/runDNN.py .\n')
    sh_file.write('python job.py\n') 
    sh_file.write('ls -l\n')
    sh_file.write('\n')

    # move output back
    sh_file.write('cd ../\n')
    sh_file.write('cp -r '+var+' '+os.getcwd()+'/.\n')
    #sh_file.write('cp result.txt '+os.getcwd()+'/.\n')
    #sh_file.write('cp rootfiles/* '+os.getcwd()+'/rootfiles/.\n')
    sh_file.close()
    os.system('chmod +x job.sh')

def write_cfg(source_dir, template_file, var_str):
    template = open(source_dir+'/'+template_file, 'r')
    template_lines = template.readlines()
    template.close()
    
    cfg_file = open('BDT.cfg', 'w')
    for line in template_lines:
        check_str = '#VARIABLE_STRING'
        if check_str in line:
            cfg_file.write(line.replace(check_str, var_str))
        else: cfg_file.write(line)
    cfg_file.close()
   
def write_collect(start_dir):
    pwd = os.getcwd()
    os.chdir(start_dir)
    co_file = open('collect.py', 'w')
    # imports
    co_file.write('import os\n') 
    co_file.write('\n')

    # results file   
    co_file.write('path = \''+os.getcwd()+'\'\n')
    co_file.write('result_file = open(path+\'/results.txt\', \'w\')\n')
    co_file.write('open_str = \'{:30}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\n\'.format(\'Variable\', \'min KS\', \'Max Significance\', \'ROC Area\', \'ROC Area up %\', \'Sig Area\', \'Sig Area up %\')\n')
    co_file.write('result_file.write(open_str)\n')
    co_file.write('\n')

    # all dir
    co_file.write('res_file = open(path+\'/all/result.txt\', \'r\')\n')
    co_file.write('lines = res_file.readlines()\n')
    co_file.write('res_file.close()\n')
    co_file.write('for line in lines:\n')
    co_file.write('    line_sp = line.replace(\' \', \'\').split(\'\t\')\n')
    co_file.write('    if len(line_sp) == 5:\n')
    co_file.write('        var = line_sp[0]\n')
    co_file.write('        min_ks  = float(line_sp[1])\n')
    co_file.write('        max_sig = float(line_sp[2])\n')
    co_file.write('        roc_a   = float(line_sp[3])\n')
    co_file.write('        sig_a   = float(line_sp[4])\n')
    co_file.write('        break\n')
    co_file.write('ref_roc = roc_a\n')
    co_file.write('ref_sig = sig_a\n')
    co_file.write('var_str = \'{:30}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\n\'.format(var, str(min_ks), str(max_sig), str(roc_a), str((roc_a-ref_roc)*100./(ref_roc+0.)), str(sig_a), str((sig_a-ref_sig)*100./(ref_sig+0.)))\n')
    co_file.write('result_file.write(var_str)\n')
    co_file.write('\n')
 
    # var dirs  
    co_file.write('dirs = os.listdir(path)\n')
    co_file.write('dirs.sort()\n')
    co_file.write('for dir in dirs:\n')
    co_file.write('\n')
    co_file.write('    if not os.path.isdir(path+\'/\'+dir): continue\n')
    co_file.write('    if \'all\' in dir: continue\n')
    co_file.write('    res_file = open(path+\'/\'+dir+\'/result.txt\', \'r\')\n')
    co_file.write('    lines = res_file.readlines()\n')
    co_file.write('    res_file.close()\n')
    co_file.write('\n')
    co_file.write('    for line in lines:\n')
    co_file.write('        line_sp = line.replace(\' \', \'\').split(\'\t\')\n')
    co_file.write('        if len(line_sp) == 5:\n')
    co_file.write('            var = line_sp[0]\n')
    co_file.write('            min_ks  = float(line_sp[1])\n')
    co_file.write('            max_sig = float(line_sp[2])\n')
    co_file.write('            roc_a   = float(line_sp[3])\n')
    co_file.write('            sig_a   = float(line_sp[4])\n')
    co_file.write('            break\n')
    co_file.write('    var_str = \'{:30}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\t{:20}\\n\'.format(var, str(min_ks), str(max_sig), str(roc_a), str((roc_a-ref_roc)*100./(ref_roc+0.)), str(sig_a), str((sig_a-ref_sig)*100./(ref_sig+0.)))\n')
    co_file.write('    result_file.write(var_str)\n')
    co_file.write('\n')
    co_file.write('result_file.close()\n')
    co_file.write('\n')
    os.chdir(pwd)

    

def run(Vars, missing_vars, template):
    print('[run] making dirs and writing files')
    base_dir = mkDirs(Vars, missing_vars, template)
    print('[run] starting submissions')
    sub(base_dir)
    print('[run] write collection file')
    write_collect(base_dir)

def sub(start_dir):
    queue='localgrid@cream02'
    QSOPT='-l walltime=168:00:00'
    source_dir = os.getcwd()
    os.system('mkdir job/'+start_dir)
    hits = os.listdir(source_dir+'/'+start_dir)
    for hit in hits:
        path = source_dir+'/'+start_dir+'/'+hit
        if not os.path.isdir(path): continue
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
 
def mkDirs(Vars, missing_vars, template_file):
    source_dir = os.getcwd()
 
    template_split = template_file.split('_')
    nVars = len(Vars)
    #nVars_dir = template_split[0] + '_' +template_split[1] +'_'+str(nVars-1)+'vars'
    nVars_dir = template_split[0] + '_' +template_split[1] +'_'+str(nVars)+'+1vars'
    os.system('mkdir '+nVars_dir)
    os.chdir(nVars_dir)


    #All variables
    os.system('mkdir all')
    os.system('mkdir all/rootfiles')
    
    var_str = '/F:'.join(Vars)
    var_str += '/F'
    
    os.chdir('all')
    write_cfg(source_dir, template_file, var_str)
    write_bash(source_dir, 'all')
    write_python(source_dir, 'all')
    os.chdir('../')
 
    for var in missing_vars:
        tmp_var = copy.deepcopy(Vars)
        tmp_var.append(var)
        var_str = '/F:'.join(tmp_var)
        var_str += '/F'
    
        os.system('mkdir '+var)
        os.system('mkdir '+var+'/rootfiles')

        os.chdir(var)
        write_cfg(source_dir, template_file, var_str)
        write_bash(source_dir, var)
        write_python(source_dir, var)
        os.chdir('../')
    os.chdir('../')
    

    return nVars_dir    

run(twoHDMa_Grad_12Vars, twoHDMa_Grad_12Vars_mis, '2HDMa_Grad_template.cfg')
 
