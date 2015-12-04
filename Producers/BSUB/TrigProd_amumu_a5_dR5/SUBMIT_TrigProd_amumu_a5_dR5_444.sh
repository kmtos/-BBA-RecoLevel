#!/bin/bash

export SCRAM_ARCH=slc6_amd64_gcc481
cd /afs/cern.ch/user/k/ktos/BBA/CMSSW_7_4_1_patch1/src
eval `scramv1 runtime -sh`
cd -
source /afs/cern.ch/cms/caf/setup.sh
cp /afs/cern.ch/user/k/ktos/BBA/CMSSW_7_4_1_patch1/src/BBA/Producers/BSUB/TrigProd_amumu_a5_dR5/trigger_amumu_producer_cfg_TrigProd_amumu_a5_dR5_444.py . 
cmsRun trigger_amumu_producer_cfg_TrigProd_amumu_a5_dR5_444.py
cmsStage -f RECO_Step3_amumu_a5_TrigProd_444.root /store/user/ktos/TrigProd_amumu_a5_dR5
rm trigger_amumu_producer_cfg_TrigProd_amumu_a5_dR5_444.py 
exit 0
