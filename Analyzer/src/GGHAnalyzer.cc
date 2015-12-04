// -*- C++ -*-
//
// Package:    GGHAnalyzer
// Class:      GGHAnalyzer
// 
/**\class GGHAnalyzer GGHAnalyzer.cc Analyzer/src/GGHAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Kyle Martin Tos
//         Created:  Thu Feb 26 09:51:23 CET 2015
// $Id$
//
//


// system include files
#include <memory>
#include <string>
#include <sstream>
#include <typeinfo>

// user include files
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetCollection.h"
#include "DataFormats/JetReco/interface/JetFloatAssociation.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "HLTrigger/HLTcore/interface/HLTConfigData.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTanalyzers/interface/HLTInfo.h"

#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "BBA/VariousFunctions/interface/VariousFunctions.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TLegend.h"


using namespace std;
using namespace edm;
using namespace reco;
using namespace trigger;


//
// class declaration
//

class GGHAnalyzer : public edm::EDAnalyzer {
   public:
      typedef reco::JetFloatAssociation::Container JetBCEnergyRatioCollection;
      explicit GGHAnalyzer(const edm::ParameterSet&);
      ~GGHAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      //delete memory
      void reset(const bool);
    
      // ----------member data ---------------------------
      //pointer to output file object
      TFile* out_;

      //name of output root file
      std::string outFileName_;
      edm::EDGetTokenT<vector<reco::GenParticle> > genParticleTag_;
      edm::EDGetTokenT<vector<reco::PFJet> > akJetTag_;
      edm::EDGetTokenT<vector<reco::Muon> > muonsTag_;
      edm::EDGetTokenT<edm::ValueMap<edm::RefVector<vector<reco::Muon>,reco::Muon,edm::refhelper::FindUsingAdvance<vector<reco::Muon>,reco::Muon> > > > muonMapTag_;
      edm::EDGetTokenT<edm::ValueMap<reco::PFJetRef> >  jetValMapTag_;
      edm::EDGetTokenT<vector<reco::PFTau> > tauRECOTag_;
      edm::EDGetTokenT<vector<reco::PFTau> > tauCJTag_;
      edm::EDGetTokenT<vector<reco::RecoTauPiZero> > pizerosTag_;      
      edm::EDGetTokenT<reco::PFTauDiscriminator>  looseIsoTag_;
      edm::EDGetTokenT<reco::PFTauDiscriminator>  medIsoTag_;
      edm::EDGetTokenT<reco::PFTauDiscriminator> tightIsoTag_;
      edm::EDGetTokenT<reco::PFTauDiscriminator> decayModeFindingTag_;

      //Histograms
      TH1F* NEvents_;   
      TH1F* NMuRemoved_;
      TH1F* TauMuTauHadMass_;
      TH1F* GenTauMomPDGID_;
      TH2F* NTausRECOvsCLEANJETS_;

      TH1F* MatchedLooseIsoOldPt_;
      TH1F* MatchedMedIsoOldPt_;
      TH1F* MatchedTightIsoOldPt_;
      TH1F* MatchedDMFindOldPt_;
      TH1F* MatchedOldPt_;
      TH1F* FinalEffLooseIsoOldPt_;
      TH1F* FinalEffMedIsoOldPt_;
      TH1F* FinalEffTightIsoOldPt_;
      TH1F* FinalEffDMFindOldPt_;

      TH1F* MatchedLooseIsoNewPt_;
      TH1F* MatchedMedIsoNewPt_;
      TH1F* MatchedTightIsoNewPt_;
      TH1F* MatchedDMFindNewPt_;
      TH1F* MatchedNewPt_;
      TH1F* FinalEffLooseIsoNewPt_;
      TH1F* FinalEffMedIsoNewPt_;
      TH1F* FinalEffTightIsoNewPt_;
      TH1F* FinalEffDMFindNewPt_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GGHAnalyzer::GGHAnalyzer(const edm::ParameterSet& iConfig):
  outFileName_(iConfig.getParameter<std::string>("outFileName")),
  genParticleTag_(consumes<vector<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("genParticleTag"))),
  akJetTag_(consumes<vector<reco::PFJet> >(iConfig.getParameter<edm::InputTag>("akJetTag"))),
  muonsTag_(consumes<vector<reco::Muon> >(iConfig.getParameter<edm::InputTag>("muonsTag"))),
  muonMapTag_(consumes<edm::ValueMap<edm::RefVector<vector<reco::Muon>,reco::Muon,edm::refhelper::FindUsingAdvance<vector<reco::Muon>,reco::Muon> > > >(iConfig.getParameter<edm::InputTag>("muonMapTag"))),
  jetValMapTag_(consumes<edm::ValueMap<reco::PFJetRef> >(iConfig.getParameter<edm::InputTag>("jetValMapTag"))),
  tauRECOTag_(consumes<vector<reco::PFTau> >(iConfig.getParameter<edm::InputTag>("tauRECOTag"))),
  tauCJTag_(consumes<vector<reco::PFTau> >(iConfig.getParameter<edm::InputTag>("tauCJTag"))),
  pizerosTag_(consumes<vector<reco::RecoTauPiZero> >(iConfig.getParameter<edm::InputTag>("pizerosTag"))),
  looseIsoTag_(consumes<reco::PFTauDiscriminator>(iConfig.getParameter<edm::InputTag>("looseIsoTag"))),
  medIsoTag_(consumes<reco::PFTauDiscriminator>(iConfig.getParameter<edm::InputTag>("medIsoTag"))),
  tightIsoTag_(consumes<reco::PFTauDiscriminator>(iConfig.getParameter<edm::InputTag>("tightIsoTag"))),
  decayModeFindingTag_(consumes<reco::PFTauDiscriminator>(iConfig.getParameter<edm::InputTag>("decayModeFindingTag")))
{
  reset(false);    
}//GGHAnalyzer



GGHAnalyzer::~GGHAnalyzer()
{
  reset(true);
}


//
// member functions
//

// ------------ method called for each event  ------------
void GGHAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::cout << "\n<------------THIS IS A NEW EVENT------------>" << std::endl;
  NEvents_->Fill(0);

  //Get gen particle collection
  edm::Handle<reco::GenParticleCollection> pGenParts;
  iEvent.getByToken(genParticleTag_, pGenParts);

  //Get ak4Jets particle collection
  edm::Handle<std::vector<reco::PFJet> > pAkJets;
  iEvent.getByToken(akJetTag_, pAkJets);

  //Get RECO Muons particle collection
  edm::Handle<std::vector<reco::Muon> > pMuons;
  iEvent.getByToken(muonsTag_, pMuons);

  //get jet-muon map
  edm::Handle<edm::ValueMap<reco::MuonRefVector> > pMuonMap;
  iEvent.getByToken(muonMapTag_, pMuonMap);

  //get old-new jet map
  edm::Handle<edm::ValueMap<reco::PFJetRef> > pJetValMap;
  iEvent.getByToken(jetValMapTag_, pJetValMap);

  //Get RECO Taus particle collection
  edm::Handle<std::vector<reco::PFTau> > pTausRECO;
  iEvent.getByToken(tauRECOTag_, pTausRECO);

  //Get CleanJets Tau particle collection
  edm::Handle<std::vector<reco::PFTau> > pTausCJ;
  iEvent.getByToken(tauCJTag_, pTausCJ);

  //Get pi-Zero particle collection
  edm::Handle<std::vector<reco::RecoTauPiZero> > pPiZero;
  iEvent.getByToken(pizerosTag_, pPiZero);

  //Get Loose Iso Collection
  Handle<PFTauDiscriminator> pLooseIsoDisc; 
  iEvent.getByToken(looseIsoTag_, pLooseIsoDisc); 

  //Get Medium Iso Collection
  Handle<PFTauDiscriminator> pMedIsoDisc; 
  iEvent.getByToken(medIsoTag_, pMedIsoDisc);

  //Get Tight Iso Collection
  Handle<PFTauDiscriminator> pTightIsoDisc; 
  iEvent.getByToken(tightIsoTag_, pTightIsoDisc);

  //Get Decay Mode Finding Collection
  Handle<PFTauDiscriminator> pDMFinding; 
  iEvent.getByToken(decayModeFindingTag_, pDMFinding);

  NTausRECOvsCLEANJETS_->Fill(pTausRECO->size(), pTausCJ->size() );
  reco::GenParticleRef tau1Ref, tau2Ref;
  int checkGenMuHad = false;
  for (reco::GenParticleCollection::const_iterator iGenParticle = pGenParts->begin(); iGenParticle != pGenParts->end(); ++iGenParticle)
  {
    if(iGenParticle->pdgId() == 36 && iGenParticle->numberOfDaughters() == 2 && fabs(iGenParticle->daughter(0)->pdgId() ) == 15 )
    {
      tau1Ref = iGenParticle->daughterRef(0);
      tau2Ref = iGenParticle->daughterRef(1);
      if (VariousFunctions::tauDecayMode(tau1Ref) == 7  && VariousFunctions::tauDecayMode(tau2Ref) < 5 )
      {
        NEvents_->Fill(2);
        checkGenMuHad = 2;
      }//if tau2 Ref
      else if (VariousFunctions::tauDecayMode(tau2Ref) == 7  && VariousFunctions::tauDecayMode(tau1Ref) < 5 ) 
      {
        NEvents_->Fill(2);
        checkGenMuHad = 1;
      }//if
    }//ir found a1 and it decayed to taus
  }//for iGen Particle
 
/////////////////////// 
// Analyze
/////////////////////// 
  double dR_GenMatchNEW1 = 1000, dR_GenMatchNEW2 = 1000, dR_GenMatchNEWTEMP1 = 1000, dR_GenMatchNEWTEMP2 = 1000, Pt_GenMatchNEW1 = -1, Pt_GenMatchNEW2 = -1;
  unsigned int DMNEW1 = 0, DMNEW2 = 0, MedIsoNEW1 = 0, MedIsoNEW2 = 0, LooseIsoNEW1 = 0, LooseIsoNEW2 = 0, TightIsoNEW1 = 0, TightIsoNEW2 = 0;
  for (std::vector<reco::PFTau>::const_iterator iTauCJ = pTausCJ->begin(); iTauCJ != pTausCJ->end(); ++iTauCJ)
  {
    const reco::PFJetRef& tauJetRef = (*iTauCJ).jetRef();
    const reco::PFJetRef& tauOldJetRef = (*pJetValMap)[tauJetRef];
    const reco::MuonRefVector& removedMuons = (*pMuonMap)[tauJetRef];

    //find the highest pT associated muon
    std::vector<reco::MuonRef> removedMuonRefs;
    for (reco::MuonRefVector::const_iterator iMuon = removedMuons.begin(); iMuon != removedMuons.end(); ++iMuon) {removedMuonRefs.push_back(*iMuon);}//for iMuon
    for (unsigned int iter = 0; iter < removedMuonRefs.size(); iter++)
    {
      for (unsigned int jter = iter + 1; jter < removedMuonRefs.size(); jter++)
      {
        if (removedMuonRefs[jter]->pt() > removedMuonRefs[iter]->pt())
	{
 	  reco::MuonRef TEMPRef = removedMuonRefs[iter];
	  removedMuonRefs[iter] = removedMuonRefs[jter];  
	  removedMuonRefs[jter] = TEMPRef;
        }//if jter > iter
      }//for jter
    }//for iter

    //Calculate Tau_mu tau_H dR
    for (unsigned int iter = 0; iter < removedMuonRefs.size(); iter++)
    {
      double dPhi = reco::deltaPhi(removedMuonRefs[iter]->phi(), iTauCJ->phi() );
      double dR_tauMu = sqrt( (removedMuonRefs[iter]->eta() - iTauCJ->eta() ) * (removedMuonRefs[iter]->eta() - iTauCJ->eta() )  +  dPhi * dPhi );
      std::cout << "\t\t\tMuRef->pt()= " << removedMuonRefs[iter]->pt() << "  dR_tauMu= " << dR_tauMu << std::endl;
      if (dR_tauMu < .5)
      {
        NEvents_->Fill(1);
	break;
      }//if
    }//for itre

    //Gen Match the CleanJets taus
    double dPhi1 = reco::deltaPhi(tau1Ref->phi(), iTauCJ->phi() ), dPhi2 = reco::deltaPhi(tau2Ref->phi(), iTauCJ->phi() );
    dR_GenMatchNEWTEMP1 = sqrt( (tau1Ref->eta() - iTauCJ->eta() ) * (tau1Ref->eta() - iTauCJ->eta() )  +  dPhi1 * dPhi1);
    dR_GenMatchNEWTEMP2 = sqrt( (tau2Ref->eta() - iTauCJ->eta() ) * (tau2Ref->eta() - iTauCJ->eta() )  +  dPhi2 * dPhi2); 
    reco::PFTauRef PFTauRef(pTausCJ, iTauCJ - pTausCJ->begin()); 
    //first 2 checks are basic requirements, the 2 in the "or" are making sure it doesn't matche better with the second tau
    if (dR_GenMatchNEWTEMP1 < .4 && dR_GenMatchNEWTEMP1 < dR_GenMatchNEW1 && (dR_GenMatchNEWTEMP1 < dR_GenMatchNEWTEMP2 || dR_GenMatchNEWTEMP2 > dR_GenMatchNEW2) )
    {
      dR_GenMatchNEW1 = dR_GenMatchNEWTEMP1;
      Pt_GenMatchNEW1 = iTauCJ->pt();
      DMNEW1 = (*pDMFinding)[PFTauRef];
      MedIsoNEW1 = (*pMedIsoDisc)[PFTauRef];
      LooseIsoNEW1 = (*pLooseIsoDisc)[PFTauRef];
      TightIsoNEW1 = (*pTightIsoDisc)[PFTauRef];
    }//if iTauCJ matches tau1Ref 
    else if (dR_GenMatchNEWTEMP2 < .4 && dR_GenMatchNEWTEMP2 < dR_GenMatchNEW2 && (dR_GenMatchNEWTEMP2 < dR_GenMatchNEWTEMP1 || dR_GenMatchNEWTEMP1 > dR_GenMatchNEW1) )
    {
      dR_GenMatchNEW2 = dR_GenMatchNEWTEMP2;
      Pt_GenMatchNEW2 = iTauCJ->pt();
      DMNEW2 = (*pDMFinding)[PFTauRef];
      MedIsoNEW2 = (*pMedIsoDisc)[PFTauRef];
      LooseIsoNEW2 = (*pLooseIsoDisc)[PFTauRef];      
      TightIsoNEW2 = (*pTightIsoDisc)[PFTauRef];
    }//else
    
    if (dR_GenMatchNEW2 != 1000 && checkGenMuHad == 2)
    {
      MatchedNewPt_->Fill(Pt_GenMatchNEW2 );
      if (DMNEW2 == 1)
	MatchedDMFindNewPt_->Fill(Pt_GenMatchNEW2 );
      if (TightIsoNEW2 == 1)
        MatchedTightIsoNewPt_->Fill(Pt_GenMatchNEW2 );
      if (MedIsoNEW2 == 1)
	MatchedMedIsoNewPt_->Fill(Pt_GenMatchNEW2 );
      if (LooseIsoNEW2 == 1)
	MatchedLooseIsoNewPt_->Fill(Pt_GenMatchNEW2 );
    }//if GEN tau2Ref is the had in mu+had and it is matched to CleanJets Jet
    else if (dR_GenMatchNEW1 != 1000 && checkGenMuHad == 1)
    {
      MatchedNewPt_->Fill(Pt_GenMatchNEW1 );
      if (DMNEW1 == 1)
        MatchedDMFindNewPt_->Fill(Pt_GenMatchNEW1 );
      if (TightIsoNEW1 == 1)
        MatchedTightIsoNewPt_->Fill(Pt_GenMatchNEW2 );
      if (MedIsoNEW1 == 1)
        MatchedMedIsoNewPt_->Fill(Pt_GenMatchNEW1 );
      if (LooseIsoNEW1 == 1)
        MatchedLooseIsoNewPt_->Fill(Pt_GenMatchNEW1 );
    }//if GEN tau1Ref is the had in mu+had and it is matched to CleanJets Jet

    std::cout << "\t\tdR_GenMatchNEW1= " << dR_GenMatchNEW1 << "  dR_GenMatchNEWTEMP1= " << dR_GenMatchNEWTEMP1 << "  PtDiff1= " << abs(Pt_GenMatchNEW1 - iTauCJ->pt() ) << std::endl;
    std::cout << "\t\tdR_GenMatchNEW2= " << dR_GenMatchNEW2 << "  dR_GenMatchNEWTEMP2= " << dR_GenMatchNEWTEMP2 << "  PtDiff2= " << abs(Pt_GenMatchNEW2 - iTauCJ->pt() ) << std::endl;

  }//for auto iTauCJ

//////////////////////////
// Matching to RECO Taus
//////////////////////////
  double dR_GenMatchOLD1 = 1000, dR_GenMatchOLD2 = 1000, dR_GenMatchOLDTEMP1 = 1000, dR_GenMatchOLDTEMP2 = 1000, Pt_GenMatchOLD1 = -1, Pt_GenMatchOLD2 = -1;
  unsigned int DMOLD1 = 0, DMOLD2 = 0, MedIsoOLD1 = 0, MedIsoOLD2 = 0, LooseIsoOLD1 = 0, LooseIsoOLD2 = 0, TightIsoOLD1 = 0, TightIsoOLD2 = 0;
  for (std::vector<reco::PFTau>::const_iterator iTauRECO = pTausRECO->begin(); iTauRECO != pTausRECO->end(); ++iTauRECO)
  {
    //Gen Match the CleanJets taus
    double dPhi1 = reco::deltaPhi(tau1Ref->phi(), iTauRECO->phi() ), dPhi2 = reco::deltaPhi(tau2Ref->phi(), iTauRECO->phi() );
    dR_GenMatchOLDTEMP1 = sqrt( (tau1Ref->eta() - iTauRECO->eta() ) * (tau1Ref->eta() - iTauRECO->eta() )  +  dPhi1 * dPhi1);
    dR_GenMatchOLDTEMP2 = sqrt( (tau2Ref->eta() - iTauRECO->eta() ) * (tau2Ref->eta() - iTauRECO->eta() )  +  dPhi2 * dPhi2);

    reco::PFTauRef PFTauRef(pTausRECO, iTauRECO - pTausRECO->begin());
    //first 2 checks are basic requirements, the 2 in the "or" are making sure it doesn't matche better with the second tau
    if (dR_GenMatchOLDTEMP1 < .4 && dR_GenMatchOLDTEMP1 < dR_GenMatchOLD1 && (dR_GenMatchOLDTEMP1 < dR_GenMatchOLDTEMP2 || dR_GenMatchOLDTEMP2 > dR_GenMatchOLD2) )
    {
      dR_GenMatchOLD1 = dR_GenMatchOLDTEMP1;
      Pt_GenMatchOLD1 = iTauRECO->pt();
      DMOLD1 = (*pDMFinding)[PFTauRef];
      MedIsoOLD1 = (*pMedIsoDisc)[PFTauRef];
      LooseIsoOLD1 = (*pLooseIsoDisc)[PFTauRef];
      TightIsoOLD1 = (*pTightIsoDisc)[PFTauRef];
    }//if iTauRECO matches tau1Ref 
    else if (dR_GenMatchOLDTEMP2 < .4 && dR_GenMatchOLDTEMP2 < dR_GenMatchOLD2 && (dR_GenMatchOLDTEMP2 < dR_GenMatchOLDTEMP1 || dR_GenMatchOLDTEMP1 > dR_GenMatchOLD1) )
    {
      dR_GenMatchOLD2 = dR_GenMatchOLDTEMP2;
      Pt_GenMatchOLD2 = iTauRECO->pt();
      DMOLD2 = (*pDMFinding)[PFTauRef];
      MedIsoOLD2 = (*pMedIsoDisc)[PFTauRef];
      LooseIsoOLD2 = (*pLooseIsoDisc)[PFTauRef]; 
      TightIsoOLD2 = (*pTightIsoDisc)[PFTauRef];
    }//else

    if (dR_GenMatchOLD2 != 1000 && checkGenMuHad == 2)
    {
      MatchedNewPt_->Fill(Pt_GenMatchOLD2 );
      if (DMOLD2 == 1)
        MatchedDMFindOldPt_->Fill(Pt_GenMatchOLD2 );
      if (TightIsoOLD2 == 1)
        MatchedTightIsoOldPt_->Fill(Pt_GenMatchOLD2 );
      if (MedIsoOLD2 == 1)
        MatchedMedIsoOldPt_->Fill(Pt_GenMatchOLD2 );
      if (LooseIsoOLD2 == 1)
        MatchedLooseIsoOldPt_->Fill(Pt_GenMatchOLD2 );
    }//if GEN tau2Ref is the had in mu+had and it is matched to CleanJets Jet
    else if (dR_GenMatchOLD1 != 1000 && checkGenMuHad == 1)
    {
      MatchedNewPt_->Fill(Pt_GenMatchOLD1 );
      if (DMOLD1 == 1)
        MatchedDMFindOldPt_->Fill(Pt_GenMatchOLD1 );
      if (TightIsoOLD1 == 1)
        MatchedTightIsoOldPt_->Fill(Pt_GenMatchOLD2 );
      if (MedIsoOLD1 == 1)
        MatchedMedIsoOldPt_->Fill(Pt_GenMatchOLD1 );
      if (LooseIsoOLD1 == 1)
        MatchedLooseIsoOldPt_->Fill(Pt_GenMatchOLD1 );
    }//if GEN tau1Ref is the had in mu+had and it is matched to CleanJets Jet

    std::cout << "\t\tdR_GenMatchOLD1= " << dR_GenMatchOLD1 << "  dR_GenMatchOLDTEMP1= " << dR_GenMatchOLDTEMP1 << "  PtDiff1= " << abs(Pt_GenMatchOLD1 - iTauRECO->pt() ) << std::endl;
    std::cout << "\t\tdR_GenMatchOLD2= " << dR_GenMatchOLD2 << "  dR_GenMatchOLDTEMP2= " << dR_GenMatchOLDTEMP2 << "  PtDiff2= " << abs(Pt_GenMatchOLD2 - iTauRECO->pt() ) << std::endl;
  }//iTauRECO
 
}//End GGHAnalyzer::analyze


// ------------ method called once each job just before starting event loop  ------------
void GGHAnalyzer::beginJob()
{
  std::cout << "Begin Job" << std::endl;

  //Open output file
  out_ = new TFile(outFileName_.c_str(), "RECREATE");

  //Book histograms
  NEvents_     = new TH1F("NEvents"    , "", 9, -.5, 8.5);
      NEvents_->GetXaxis()->SetBinLabel(1, "TotalEvents"); 
      NEvents_->GetXaxis()->SetBinLabel(2, "#tau_{#mu} + #tau_{had} Match");
      NEvents_->GetXaxis()->SetBinLabel(3, "Gen #tau_{#mu} + #tau_{had}");
      NEvents_->GetXaxis()->SetBinLabel(4, "");
      NEvents_->GetXaxis()->SetBinLabel(5, "");
      NEvents_->GetXaxis()->SetBinLabel(6, "");
      NEvents_->GetXaxis()->SetBinLabel(7, "");
  NMuRemoved_            = new TH1F("NMuRemoved"    , "", 9, -.5, 8.5);
  TauMuTauHadMass_       = new TH1F("TauMuTauHadMass"    , "", 100, 0, 100);
  GenTauMomPDGID_        = new TH1F("GenTauMomPDGID"    , "", 1000, 0, 1000);
  NTausRECOvsCLEANJETS_  = new TH2F("NTausRECOvsCLEANJETS" , "", 11, -.5, 10.5, 11, -.5, 10.5);

  MatchedLooseIsoOldPt_  = new TH1F("MatchedLooseIsoOldPt"    , "", 50, 0, 50.0);
  MatchedMedIsoOldPt_    = new TH1F("MatchedMedIsoOldPt", "", 50, 0, 50);
  MatchedTightIsoOldPt_    = new TH1F("MatchedTightIsoOldPt", "", 50, 0, 50);
  MatchedDMFindOldPt_    = new TH1F("MatchedDMFindOldPt"    , "", 50, 0, 50);
  MatchedOldPt_          = new TH1F("MatchedOldPt"    , "", 50, 0, 50);
  FinalEffLooseIsoOldPt_ = new TH1F("FinalEffLooseIsoOldPt"    , "", 50, 0, 50.0);
      FinalEffLooseIsoOldPt_->GetYaxis()->SetTitle("#epsilon");
  FinalEffMedIsoOldPt_   = new TH1F("FinalEffMedIsoOldPt", "", 50, 0, 50);
      FinalEffMedIsoOldPt_->GetYaxis()->SetTitle("#epsilon");
  FinalEffTightIsoOldPt_   = new TH1F("FinalEffTightIsoOldPt", "", 50, 0, 50);
      FinalEffTightIsoOldPt_->GetYaxis()->SetTitle("#epsilon");
  FinalEffDMFindOldPt_   = new TH1F("FinalEffDMFindOldPt"    , "", 50, 0, 50);
      FinalEffDMFindOldPt_->GetYaxis()->SetTitle("#epsilon");

  MatchedLooseIsoNewPt_  = new TH1F("MatchedLooseIsoNewPt"    , "", 50, 0, 50.0);
  MatchedMedIsoNewPt_    = new TH1F("MatchedMedIsoNewPt", "", 50, 0, 50);
  MatchedTightIsoNewPt_    = new TH1F("MatchedTightIsoNewPt", "", 50, 0, 50);
  MatchedDMFindNewPt_    = new TH1F("MatchedDMFindNewPt"    , "", 50, 0, 50);
  MatchedNewPt_          = new TH1F("MatchedNewPt"    , "", 50, 0, 50);
  FinalEffLooseIsoNewPt_ = new TH1F("FinalEffLooseIsoNewPt"    , "", 50, 0, 50.0);
      FinalEffLooseIsoNewPt_->GetYaxis()->SetTitle("#epsilon");
  FinalEffMedIsoNewPt_   = new TH1F("FinalEffMedIsoNewPt", "", 50, 0, 50);
      FinalEffMedIsoNewPt_->GetYaxis()->SetTitle("#epsilon");
  FinalEffTightIsoNewPt_   = new TH1F("FinalEffTightIsoNewPt", "", 50, 0, 50);
      FinalEffTightIsoNewPt_->GetYaxis()->SetTitle("#epsilon");
  FinalEffDMFindNewPt_   = new TH1F("FinalEffDMFindNewPt"    , "", 50, 0, 50);
      FinalEffDMFindNewPt_->GetYaxis()->SetTitle("#epsilon");
}

// ------------ method called once each job just after ending the event loop  ------------
void GGHAnalyzer::endJob()
{
  //Make the Canvases
  TCanvas NEventsCanvas("NEvents","",600,600);
  TCanvas NMuRemovedCanvas("NMuRemoved","",600,600);
  TCanvas TauMuTauHadMassCanvas("TauMuTauHadMass","",600,600);
  TCanvas GenTauMomPDGIDCanvas("GenTauMomPDGID","",600,600);
  TCanvas NTausRECOvsCLEANJETSCanvas("NTausRECOvsCLEANJETS","",600,600);

  TCanvas MatchedLooseIsoOldPtCanvas("MatchedLooseIsoOldPt","",600,600);
  TCanvas MatchedMedIsoOldPtCanvas("MatchedMedIsoOldPt","",600,600);
  TCanvas MatchedTightIsoOldPtCanvas("MatchedTightIsoOldPt","",600,600);
  TCanvas MatchedDMFindOldPtCanvas("MatchedDMFindOldPt","",600,600);
  TCanvas MatchedOldPtCanvas("MatchedOldPt","",600,600);
  TCanvas FinalEffLooseIsoOldPtCanvas("FinalEffLooseIsoOldPt","",600,600);
  TCanvas FinalEffMedIsoOldPtCanvas("FinalEffMedIsoOldPt","",600,600);
  TCanvas FinalEffTightIsoOldPtCanvas("FinalEffTightIsoOldPt","",600,600);
  TCanvas FinalEffDMFindOldPtCanvas("FinalEffDMFindOldPt","",600,600);

  TCanvas MatchedLooseIsoNewPtCanvas("MatchedLooseIsoNewPt","",600,600);
  TCanvas MatchedMedIsoNewPtCanvas("MatchedMedIsoNewPt","",600,600);
  TCanvas MatchedTightIsoNewPtCanvas("MatchedTightIsoNewPt","",600,600);
  TCanvas MatchedDMFindNewPtCanvas("MatchedDMFindNewPt","",600,600);
  TCanvas MatchedNewPtCanvas("MatchedNewPt","",600,600);
  TCanvas FinalEffLooseIsoNewPtCanvas("FinalEffLooseIsoNewPt","",600,600);
  TCanvas FinalEffMedIsoNewPtCanvas("FinalEffMedIsoNewPt","",600,600);
  TCanvas FinalEffTightIsoNewPtCanvas("FinalEffTightIsoNewPt","",600,600);
  TCanvas FinalEffDMFindNewPtCanvas("FinalEffDMFindNewPt","",600,600);


std::cout << "<----------------Declared Canvases-------------->" << std::endl;

  //Format the 1D plots and Draw (canvas, hist, grid, log y, log z, color, size, style, xAxisTitle, xTitleSize, xLabelSize, xTitleOffSet, yAxisTitle, yTitleSize, yLabelSize, yTitleOffset)
  VariousFunctions::formatAndDrawCanvasAndHist1D(NEventsCanvas, NEvents_, 1, 0, 0, kBlack, 7, 20, "Number of Taus", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(NMuRemovedCanvas, NMuRemoved_, 1, 0, 0, kBlack, 7, 20, "N #mu Removed", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(TauMuTauHadMassCanvas, TauMuTauHadMass_, 1, 0, 0, kBlack, 7, 20, "mass(#tau_{mu} + #tau_{H})", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(GenTauMomPDGIDCanvas, GenTauMomPDGID_, 1, 0, 0, kBlack, 7, 20, "Tau Mom PDGID", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist2D(NTausRECOvsCLEANJETSCanvas, NTausRECOvsCLEANJETS_, 1, 0, 0, kBlack, 7, 20, "nRECO #tau's", .04, .04, 1.1, "nCleanJets #tau's", .04, .04, 1.6, "", .04, .04, 1.0);

  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedLooseIsoOldPtCanvas, MatchedLooseIsoOldPt_, 1, 0, 0, kBlack, 7, 20, "Pt(RECO Matched + Loose Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedMedIsoOldPtCanvas, MatchedMedIsoOldPt_, 1, 0, 0, kBlack, 7, 20, "Pt(RECO Matched + Med Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedTightIsoOldPtCanvas, MatchedTightIsoOldPt_, 1, 0, 0, kBlack, 7, 20, "Pt(RECO Matched + Tight Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedDMFindOldPtCanvas, MatchedDMFindOldPt_, 1, 0, 0, kBlack, 7, 20, "Pt(RECO Matched + DecayModeFinding)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedOldPtCanvas, MatchedOldPt_, 1, 0, 0, kBlack, 7, 20, "Pt(Matched)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(FinalEffLooseIsoOldPtCanvas, FinalEffLooseIsoOldPt_, 1, 0, 0, kBlack, 7, 20, "Pt(RECO Matched + Loose Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(FinalEffMedIsoOldPtCanvas, FinalEffMedIsoOldPt_, 1, 0, 0, kBlack, 7, 20, "Pt(RECO Matched + Med Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(FinalEffTightIsoOldPtCanvas, FinalEffTightIsoOldPt_, 1, 0, 0, kBlack, 7, 20, "Pt(RECO Matched + Tight Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(FinalEffDMFindOldPtCanvas, FinalEffDMFindOldPt_, 1, 0, 0, kBlack, 7, 20, "Pt(RECO Matched + DecayModeFinding)", .04, .04, 1.1,  "", .04, .04, 1.0, false);

  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedLooseIsoNewPtCanvas, MatchedLooseIsoNewPt_, 1, 0, 0, kBlack, 7, 20, "Pt(CleanJets Matched + Loose Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedMedIsoNewPtCanvas, MatchedMedIsoNewPt_, 1, 0, 0, kBlack, 7, 20, "Pt(CleanJets Matched + Med Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedTightIsoNewPtCanvas, MatchedTightIsoNewPt_, 1, 0, 0, kBlack, 7, 20, "Pt(CleanJets Matched + Tight Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedDMFindNewPtCanvas, MatchedDMFindNewPt_, 1, 0, 0, kBlack, 7, 20, "Pt(CleanJets Matched + DecayModeFinding)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(MatchedNewPtCanvas, MatchedNewPt_, 1, 0, 0, kBlack, 7, 20, "Pt(Matched)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(FinalEffLooseIsoNewPtCanvas, FinalEffLooseIsoNewPt_, 1, 0, 0, kBlack, 7, 20, "Pt(CleanJets Matched + Loose Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(FinalEffMedIsoNewPtCanvas, FinalEffMedIsoNewPt_, 1, 0, 0, kBlack, 7, 20, "Pt(CleanJets Matched + Med Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(FinalEffTightIsoNewPtCanvas, FinalEffTightIsoNewPt_, 1, 0, 0, kBlack, 7, 20, "Pt(CleanJets Matched + Tight Iso)", .04, .04, 1.1,  "", .04, .04, 1.0, false);
  VariousFunctions::formatAndDrawCanvasAndHist1D(FinalEffDMFindNewPtCanvas, FinalEffDMFindNewPt_, 1, 0, 0, kBlack, 7, 20, "Pt(CleanJets Matched + DecayModeFinding)", .04, .04, 1.1,  "", .04, .04, 1.0, false);


std::cout << "<----------------Formatted Canvases and Histos-------------->" << std::endl;

  //Write output file
  out_->cd();

  NEventsCanvas.Write();
  NMuRemovedCanvas.Write();
  TauMuTauHadMassCanvas.Write();
  GenTauMomPDGIDCanvas.Write();
  NTausRECOvsCLEANJETSCanvas.Write();

  MatchedLooseIsoOldPtCanvas.Write();
  MatchedMedIsoOldPtCanvas.Write();
  MatchedTightIsoOldPtCanvas.Write();
  MatchedDMFindOldPtCanvas.Write();
  MatchedOldPtCanvas.Write();
  FinalEffLooseIsoOldPtCanvas.Write();
  FinalEffMedIsoOldPtCanvas.Write();
  FinalEffTightIsoOldPtCanvas.Write();
  FinalEffDMFindOldPtCanvas.Write();

  MatchedLooseIsoNewPtCanvas.Write();
  MatchedMedIsoNewPtCanvas.Write();
  MatchedTightIsoNewPtCanvas.Write();
  MatchedDMFindNewPtCanvas.Write();
  MatchedNewPtCanvas.Write();
  FinalEffLooseIsoNewPtCanvas.Write();
  FinalEffMedIsoNewPtCanvas.Write();
  FinalEffTightIsoNewPtCanvas.Write();
  FinalEffDMFindNewPtCanvas.Write();

  out_->Write();
  out_->Close();
std::cout << "DONE" << std::endl;
}//EndJob

// ------------ method called when starting to processes a run  ------------
void GGHAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&) {}

// ------------ method called when ending the processing of a run  ------------
void GGHAnalyzer::endRun(edm::Run const&, edm::EventSetup const&) {}

// ------------ method called when starting to processes a luminosity block  ------------
void GGHAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {}

// ------------ method called when ending the processing of a luminosity block  ------------
void GGHAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {}

//Delete Memory
void GGHAnalyzer::reset(const bool doDelete)
{
  if ((doDelete) && (NEvents_ != NULL)) delete NEvents_;
  NEvents_ = NULL;
  if ((doDelete) && (NMuRemoved_ != NULL)) delete NMuRemoved_;
  NMuRemoved_ = NULL;
  if ((doDelete) && (TauMuTauHadMass_ != NULL)) delete TauMuTauHadMass_;
  TauMuTauHadMass_ = NULL;
  if ((doDelete) && (GenTauMomPDGID_ != NULL)) delete GenTauMomPDGID_;
  GenTauMomPDGID_ = NULL;
  if ((doDelete) && (NTausRECOvsCLEANJETS_ != NULL)) delete NTausRECOvsCLEANJETS_;
  NTausRECOvsCLEANJETS_ = NULL;

  if ((doDelete) && (MatchedLooseIsoOldPt_ != NULL)) delete MatchedLooseIsoOldPt_;
  MatchedLooseIsoOldPt_ = NULL;
  if ((doDelete) && (MatchedMedIsoOldPt_ != NULL)) delete MatchedMedIsoOldPt_;
  MatchedMedIsoOldPt_ = NULL;
  if ((doDelete) && (MatchedTightIsoOldPt_ != NULL)) delete MatchedTightIsoOldPt_;
  MatchedTightIsoOldPt_ = NULL;
  if ((doDelete) && (MatchedDMFindOldPt_ != NULL)) delete MatchedDMFindOldPt_;
  MatchedDMFindOldPt_ = NULL;
  if ((doDelete) && (MatchedOldPt_ != NULL)) delete MatchedOldPt_;
  MatchedOldPt_ = NULL;
  if ((doDelete) && (FinalEffLooseIsoOldPt_ != NULL)) delete FinalEffLooseIsoOldPt_;
  FinalEffLooseIsoOldPt_ = NULL;
  if ((doDelete) && (FinalEffMedIsoOldPt_ != NULL)) delete FinalEffMedIsoOldPt_;
  FinalEffMedIsoOldPt_ = NULL;
  if ((doDelete) && (FinalEffTightIsoOldPt_ != NULL)) delete FinalEffTightIsoOldPt_;
  FinalEffTightIsoOldPt_ = NULL;
  if ((doDelete) && (FinalEffDMFindOldPt_ != NULL)) delete FinalEffDMFindOldPt_;
  FinalEffDMFindOldPt_ = NULL;

  if ((doDelete) && (MatchedLooseIsoNewPt_ != NULL)) delete MatchedLooseIsoNewPt_;
  MatchedLooseIsoNewPt_ = NULL;
  if ((doDelete) && (MatchedMedIsoNewPt_ != NULL)) delete MatchedMedIsoNewPt_;
  MatchedMedIsoNewPt_ = NULL;
  if ((doDelete) && (MatchedTightIsoNewPt_ != NULL)) delete MatchedTightIsoNewPt_;
  MatchedTightIsoNewPt_ = NULL;
  if ((doDelete) && (MatchedDMFindNewPt_ != NULL)) delete MatchedDMFindNewPt_;
  MatchedDMFindNewPt_ = NULL;
  if ((doDelete) && (MatchedNewPt_ != NULL)) delete MatchedNewPt_;
  MatchedNewPt_ = NULL;
  if ((doDelete) && (FinalEffLooseIsoNewPt_ != NULL)) delete FinalEffLooseIsoNewPt_;
  FinalEffLooseIsoNewPt_ = NULL;
  if ((doDelete) && (FinalEffMedIsoNewPt_ != NULL)) delete FinalEffMedIsoNewPt_;
  FinalEffMedIsoNewPt_ = NULL;
  if ((doDelete) && (FinalEffTightIsoNewPt_ != NULL)) delete FinalEffTightIsoNewPt_;
  FinalEffTightIsoNewPt_ = NULL;
  if ((doDelete) && (FinalEffDMFindNewPt_ != NULL)) delete FinalEffDMFindNewPt_;
  FinalEffDMFindNewPt_ = NULL;

}//void GGHAnalyzer

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void GGHAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GGHAnalyzer);
