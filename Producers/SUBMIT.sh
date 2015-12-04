#!/bin/bash

export SCRAM_ARCH=slc6_amd64_gcc481
cd /afs/cern.ch/user/k/ktos/BBA/CMSSW_7_4_1_patch1/src
eval `scramv1 runtime -sh`
cd -
source /afs/cern.ch/cms/caf/setup.sh
cp /afs/cern.ch/user/k/ktos/BBA/CMSSW_7_4_1_patch1/src/BBA/Producers/BSUB/DIRNAME/PRODUCER.py . 
cmsRun PRODUCER.py
cmsStage -f RECO_Step3_amumu_a5_TrigProd_NUM.root /store/user/ktos/DIRNAME
rm PRODUCER.py 
exit 0
