import FWCore.ParameterSet.Config as cms

process = cms.Process("BBA")

#
# This begins the part of the demo along with the top two lines of text
#
# initialize MessageLogger and output report
#process.load("BBA/Analyzer/bbaanalyzer_cfi")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('BBA')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(-1)
)

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

readFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
    fileNames = readFiles,
    skipEvents = cms.untracked.uint32(0)
    )
readFiles.extend([
### <---Means Files corrupted or the like
# <----Means file doesn't exist
    'file:/afs/cern.ch/user/k/ktos/BoostedDiTau/CMSSW_7_4_1_patch1/src/BoostedTauAnalysis/CleanJets/OUTPUT/CleanJets_edmOuput.root'

    ])

process.ggh = cms.EDAnalyzer("GGHAnalyzer",
   outFileName = cms.string('/afs/cern.ch/user/k/ktos/BBA/CMSSW_7_4_1_patch1/src/BBA/Analyzer/BSUB/TEST/TEST.root'),
   genParticleTag = cms.InputTag("genParticles", "", "SIM"),
   akJetTag = cms.InputTag("ak4PFJets"),
   muonsTag = cms.InputTag("muons"),
   muonMapTag = cms.InputTag("CleanJets", "muonValMap"),
   jetValMapTag = cms.InputTag("CleanJets", "jetValMap", "CLEANJETS"),
   tauRECOTag = cms.InputTag("hpsPFTauProducer", "", "RECO"),
   tauCJTag = cms.InputTag("hpsPFTauProducer", "", "CLEANJETS"),
   pizerosTag = cms.InputTag("hpsPFTauProducer", "pizeros" ),
   looseIsoTag = cms.InputTag("hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits", "", "CLEANJETS"),
   medIsoTag = cms.InputTag("hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits", "", "CLEANJETS"),
   tightIsoTag = cms.InputTag("hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits", "", "CLEANJETS"),
   decayModeFindingTag = cms.InputTag("hpsPFTauDiscriminationByDecayModeFindingOldDMs", "", "CLEANJETS")
)

process.p2 = cms.Path(process.ggh)
