import FWCore.ParameterSet.Config as cms
process = cms.Process("PAT")

#process.load("BBA/Analyzer/bbaanalyzer_cfi")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.EventContent.EventContent_cff')
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_71_V1::All', '')
process.load("Configuration.StandardSequences.MagneticField_cff")

####################
# Message Logger
####################
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
## switch to uncheduled mode
process.options.allowUnscheduled = cms.untracked.bool(True)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(300)
)

####################
# Input File List
####################
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('root://eoscms//eos/cms/store/user/ktos/RECO_Step3_BBA_a30_PAT1/RECO_Step3_BBA_a30_PAT1_NUM.root'),
    secondaryFileNames = cms.untracked.vstring()
)

############################################################
# Defining matching in DeltaR, sorting by best DeltaR
############################################################
process.bTriggerMatchHLTMu10CentralPFJet30BTagCSV = cms.EDProducer("PATTriggerMatcherDRLessByR",
        src     = cms.InputTag( 'slimmedJets' ), # matcher input collections
        matched = cms.InputTag( 'patTrigger' ),  # selections of trigger objects
        matchedCuts = cms.string( 'type( "TriggerBJet" ) && path( "HLT_Mu10_CentralPFJet30_BTagCSV0p*")' ), # input does not yet have the 'saveTags' parameter in HLT
        maxDPtRel   = cms.double( 0.5 ), # no effect here
        maxDeltaR   = cms.double( 0.3 ), ####### selection of matches
        maxDeltaEta = cms.double( 0.2 ), # no effect here
        resolveAmbiguities    = cms.bool( True ),  # definition of matcher output
        resolveByMatchQuality = cms.bool( True )  # definition of matcher output
)       

process.mTriggerMatchHLTMu10CentralPFJet30BTagCSV = cms.EDProducer("PATTriggerMatcherDRLessByR",
        src     = cms.InputTag( 'slimmedMuons' ),
        matched = cms.InputTag( 'patTrigger' ), # selections of trigger objects
        matchedCuts = cms.string( 'type( "TriggerMuon" ) && path( "HLT_Mu10_CentralPFJet30_BTagCSV0p*")' ), # input does not yet have the 'saveTags' parameter in HLT
        maxDPtRel   = cms.double( 0.5 ), # no effect here
        maxDeltaR   = cms.double( 0.3 ), #### selection of matches
        maxDeltaEta = cms.double( 0.2 ), # no effect here
        resolveAmbiguities    = cms.bool( True ),# definition of matcher output
        resolveByMatchQuality = cms.bool( True )# definition of matcher output
)       

process.bTriggerMatchHLTIsoMu24eta2p1CentralPFJet30BTagCSV = cms.EDProducer("PATTriggerMatcherDRLessByR",
        src     = cms.InputTag( 'slimmedJets' ), # matcher input collections
        matched = cms.InputTag( 'patTrigger' ),  # selections of trigger objects
        matchedCuts = cms.string( 'type( "TriggerBJet" ) && path( "HLT_IsoMu24_eta2p1_CentralPFJet30_BTagCSV*")' ), # input does not yet have the 'saveTags' parameter in HLT
        maxDPtRel   = cms.double( 0.5 ), # no effect here
        maxDeltaR   = cms.double( 0.3 ), ####### selection of matches
        maxDeltaEta = cms.double( 0.2 ), # no effect here
        resolveAmbiguities    = cms.bool( True ),  # definition of matcher output
        resolveByMatchQuality = cms.bool( True )  # definition of matcher output
)

process.mTriggerMatchHLTIsoMu24eta2p1CentralPFJet30BTagCSV = cms.EDProducer("PATTriggerMatcherDRLessByR",
        src     = cms.InputTag( 'slimmedMuons' ),
        matched = cms.InputTag( 'patTrigger' ), # selections of trigger objects
        matchedCuts = cms.string( 'type( "TriggerMuon" ) && path( "HLT_IsoMu24_eta2p1_CentralPFJet30_BTagCSV*")' ), # input does not yet have the 'saveTags' parameter in HLT
        maxDPtRel   = cms.double( 0.5 ), # no effect here
        maxDeltaR   = cms.double( 0.3 ), #### selection of matches
        maxDeltaEta = cms.double( 0.2 ), # no effect here
        resolveAmbiguities    = cms.bool( True ),# definition of matcher output
        resolveByMatchQuality = cms.bool( True )# definition of matcher output
)

# talk to output module
process.out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string("file:RECO_Step3_BBA_a30_TrigProd_NUM.root"),
	outputCommands = process.MINIAODSIMEventContent.outputCommands
)

process.out.outputCommands += [ 'drop *_*_*_*',
				'keep *_mTriggerMatchHLTIsoMu24eta2p1CentralPFJet30BTagCSV_*_*',
				'keep *_bTriggerMatchHLTIsoMu24eta2p1CentralPFJet30BTagCSV_*_*',
				'keep *_mTriggerMatchHLTMu10CentralPFJet30BTagCSV_*_*',
				'keep *_bTriggerMatchHLTMu10CentralPFJet30BTagCSV_*_*',
				'keep *_*slimmed*_*_*',
				'keep *_slimmedMuons_*_*',
				'keep *_slimmedTaus_*_*',
		        	'keep *_pfTausEI_*_*',
	        		'keep *_hpsPFTauProducer_*_*',
			        'keep *_hltTriggerSummaryAOD_*_*',
		        	'keep *_TriggerResults_*_HLT',
	        		'keep *_patTrigger*_*_*',
				'keep *_prunedGenParticles_*_*'
]


################################################################################
# Running the matching and setting the the trigger on
################################################################################
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process ) # This is optional and can be omitted.
switchOnTriggerMatching( process, triggerMatchers = [ 'mTriggerMatchHLTIsoMu24eta2p1CentralPFJet30BTagCSV',
						      'bTriggerMatchHLTIsoMu24eta2p1CentralPFJet30BTagCSV',
						      'mTriggerMatchHLTMu10CentralPFJet30BTagCSV',
						      'bTriggerMatchHLTMu10CentralPFJet30BTagCSV'
						    ])

process.outpath = cms.EndPath(process.out)
