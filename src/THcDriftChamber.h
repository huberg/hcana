#ifndef ROOT_THcDriftChamber
#define ROOT_THcDriftChamber

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// THcDriftChamber                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaSubDetector.h"
#include "THcDriftChamberPlane.h"
#include "TClonesArray.h"
#include "TMatrixDSym.h"

#include <map>

#define MAX_SPACE_POINTS 50
#define MAX_HITS_PER_POINT 20

//#include "TMath.h"

//class THaScCalib;
class TClonesArray;

class THcDriftChamber : public THaSubDetector {

public:
  THcDriftChamber( const char* name, const char* description, Int_t chambernum,
		   THaDetectorBase* parent = NULL );
  virtual ~THcDriftChamber();

  virtual Int_t Decode( const THaEvData& );
  virtual EStatus    Init( const TDatime& run_time );
  
  virtual void       AddPlane(THcDriftChamberPlane *plane);
  virtual Int_t      ApplyCorrections( void );
  virtual void       ProcessHits( void );
  virtual Int_t      FindSpacePoints( void ) ;
  virtual void       CorrectHitTimes( void ) ;

  virtual void   Clear( Option_t* opt="" );

  //  Int_t GetNHits() const { return fNhit; }
  
  Int_t GetNTracks() const { return fTrackProj->GetLast()+1; }
  const TClonesArray* GetTrackHits() const { return fTrackProj; }

  //  friend class THaScCalib;

  THcDriftChamber();  // for ROOT I/O
protected:

  // Calibration

  // Per-event data
  Int_t fNhits;

  Int_t fNPlanes;		// Number of planes in the chamber

  Int_t fChamberNum;

  // HMS Specific
  Int_t YPlaneInd; 		// Index of Yplane for this chamber
  Int_t YPlanePInd; 		// Index of Yplanep for this chamber
  Int_t YPlaneNum;		// Absolute plane number of Yplane
  Int_t YPlanePNum;		// Absolute plane number of Yplanep

  // Parameters 
  Int_t fMinHits; 		// Minimum hits required to do something
  Int_t fMaxHits; 		// Maximum required to do something
  Int_t fMinCombos;             // Minimum # pairs in a space point
  Int_t fRemove_Sppt_If_One_YPlane;
  Double_t fWireVelocity;
  Int_t fSmallAngleApprox;
  Double_t fStubMaxXPDiff;

  Double_t fXCenter;
  Double_t fYCenter;
  Double_t fSpacePointCriterion;
  Double_t fSpacePointCriterion2;
  Double_t* fSinBeta;
  Double_t* fCosBeta;
  Double_t* fTanBeta;
  Double_t* fSigma;
  Double_t* fPsi0;
  Double_t** fStubCoefs;

  THcDriftChamberPlane* fPlanes[20]; // List of plane objects

  TClonesArray*  fTrackProj;  // projection of track onto scintillator plane
                              // and estimated match to TOF paddle
  //  void           ClearEvent();
  void           DeleteArrays();
  virtual Int_t  ReadDatabase( const TDatime& date );
  virtual Int_t  DefineVariables( EMode mode = kDefine );

  void Setup(const char* name, const char* description);
  Int_t      FindEasySpacePoint(Int_t yplane_hitind, Int_t yplanep_hitind);
  Int_t      FindHardSpacePoints(void);
  Int_t      DestroyPoorSpacePoints(void);
  Int_t      SpacePointMultiWire(void);
  void       ChooseSingleHit(void);
  void       SelectSpacePoints(void);
  UInt_t     Count1Bits(UInt_t x);
  void       LeftRight(void);
  Double_t   FindStub(Int_t nhits, THcDCHit** hits, Int_t* plane_list,
			  UInt_t bitpat, Int_t* plusminus, Double_t* stub);

  THcDCHit* fHits[10000];	/* All hits for this chamber */
  // A simple structure until we figure out what we are doing.
  struct SpacePoint {
    Double_t x;
    Double_t y;
    Int_t nhits;
    Int_t ncombos;
    THcDCHit* hits[MAX_HITS_PER_POINT];
    Double_t stub[4];
  };
  SpacePoint fSpacePoints[MAX_SPACE_POINTS];
  Int_t fNSpacePoints;
  Int_t fEasySpacePoint;	/* This event is an easy space point */

  Double_t* stubcoef[4]; 
  std::map<int,TMatrixDSym*> fAA3Inv;

  ClassDef(THcDriftChamber,0)   // Drift Chamber class
};

////////////////////////////////////////////////////////////////////////////////

#endif
