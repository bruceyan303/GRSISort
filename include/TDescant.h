#ifndef TDESCANT_H
#define TDESCANT_H

#include "Globals.h"

#include <vector>
#include <cstdio>

#include "TDescantHit.h"
#if !defined (__CINT__) && !defined (__CLING__)
#include "TDescantData.h"
#else
class TDescantData;
#endif
#include "TVector3.h" 

#include "TGRSIDetector.h" 


class TDescant : public TGRSIDetector {

  public:
     TDescant();
     TDescant(const TDescant&);
     virtual ~TDescant();

  public: 
     TGRSIDetectorHit* GetHit(const Int_t& idx =0);
     TDescantHit* GetDescantHit(const Int_t& idx = 0);

     Short_t GetMultiplicity() const	       {	return descant_hits.size();}	//!

     static TVector3 GetPosition(int DetNbr, double dist=222);	//!

     void BuildHits(TDetectorData *data =0,Option_t *opt = "");           //!
     void FillData(TFragment*,TChannel*,MNEMONIC*);                           //!

     TDescant& operator=(const TDescant&);  // 

   private: 
     TDescantData *descantdata;                                               //!  Used to build DESCANT Hits
     std::vector <TDescantHit> descant_hits;                                  //   The set of crystal hits

     static bool fSetWave;		                                                //  Flag for Waveforms ON/OFF

     bool hit_flag;                                                               //   Is there a Descant hit?

   public:
     static bool SetWave()      { return fSetWave;  }	                        //!
     bool Hit()                {return hit_flag;}                                //!  
     void SetHit(bool flag = true) { hit_flag = flag; }
   private:
     static TVector3 gPosition[2][71];                                     //!
   public:         
     void Copy(TObject&) const;                //!
     void Clear(Option_t *opt = "");		//!
     void Print(Option_t *opt = "") const;		//!

     void PushBackHit(TGRSIDetectorHit* deshit);

     ClassDef(TDescant,1)  // Descant Physics structure


};

#endif


