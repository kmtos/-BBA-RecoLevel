#!/bin/bash

export SCRAM_ARCH=slc6_amd64_gcc481
cd /afs/cern.ch/user/k/ktos/BBA/CMSSW_7_4_1_patch1/src
eval `scramv1 runtime -sh`
cd -
source /afs/cern.ch/cms/caf/setup.sh
cp /afs/cern.ch/user/k/ktos/BBA/CMSSW_7_4_1_patch1/src/BBA/Analyzer/BSUB/amumu_a5_NOV_17/amumuAnalyzer_a5_cfg_TrigProd_amumu_a5_NOV_17.py . 
cp /afs/cern.ch/user/k/ktos/BBA/CMSSW_7_4_1_patch1/src/BBA/Analyzer/src/BBAAnalyzer.cc /afs/cern.ch/user/k/ktos/BBA/CMSSW_7_4_1_patch1/src/BBA/Analyzer/BSUB/amumu_a5_NOV_17/BBAAnalyzer_amumu_a5_NOV_17.cc
cmsRun amumuAnalyzer_a5_cfg_TrigProd_amumu_a5_NOV_17.py
rm amumuAnalyzer_a5_cfg_TrigProd_amumu_a5_NOV_17.py 
exit 0
