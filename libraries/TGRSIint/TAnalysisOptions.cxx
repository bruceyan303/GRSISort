#include "TAnalysisOptions.h"

#include <algorithm>
#include <cctype>
#include <iostream>

#include "TEnv.h"
#include "TKey.h"

#include "Globals.h"
#include "DynamicLibrary.h"
#include "TGRSIUtilities.h"
#include "GRootCommands.h"

TAnalysisOptions::TAnalysisOptions()
{
   Clear();
}

void TAnalysisOptions::Clear(Option_t*)
{
   /// Clears all of the variables in the TAnalysisOptions
   fBuildWindow            = 2000;
	fBuildEventsByTimeStamp = false;
   fAddbackWindow          = 300;
   fSuppressionWindow      = 300.;
   fSuppressionEnergy      = 0.;
   fStaticWindow           = false;
   fWaveformFitting        = false;
   fIsCorrectingCrossTalk  = true;
}

void TAnalysisOptions::Print(Option_t*) const
{
   /// Print the current status of TAnalysisOptions, includes all names, lists and flags
   std::cout<<BLUE<<"fBuildWindow: "<<DCYAN<<fBuildWindow<<std::endl
		      <<BLUE<<"fBuildEventsByTimeStamp: "<<DCYAN<<fBuildEventsByTimeStamp<<std::endl
            <<BLUE<<"fAddbackWindow: "<<DCYAN<<fAddbackWindow<<std::endl
            <<BLUE<<"fSuppressionWindow: "<<DCYAN<<fSuppressionWindow<<std::endl
            <<BLUE<<"fSuppressionEnergy: "<<DCYAN<<fSuppressionEnergy<<std::endl
            <<BLUE<<"fStaticWindow: "<<DCYAN<<fStaticWindow<<std::endl
            <<BLUE<<"fWaveformFitting: "<<DCYAN<<fWaveformFitting<<std::endl
            <<BLUE<<"fIsCorrectingCrossTalk: "<<DCYAN<<fIsCorrectingCrossTalk<<std::endl
            <<RESET_COLOR<<std::endl;
}

bool TAnalysisOptions::WriteToFile(TFile* file)
{
   /// Writes options information to the root file
   // Maintain old gDirectory info
   bool        success = true;
   TDirectory* oldDir  = gDirectory;

	// if no file was provided, try to use the current file
   if(file == nullptr) {
      file = gDirectory->GetFile();
   }
	// check if we got a file
   if(file == nullptr) {
		std::cout<<"Error, no file provided and no file open (gDirectory = "<<gDirectory->GetName()<<")!"<<std::endl;
		return !success;
	}

   file->cd();
   std::string oldoption = std::string(file->GetOption());
   if(oldoption == "READ") {
      file->ReOpen("UPDATE");
   }

	// check again that we have a directory to write to
   if(gDirectory == nullptr) {
		std::cout<<"No file opened to write to."<<std::endl;
      return !success;
   }
	// write analysis options
	std::cout<<"Writing TAnalysisOptions to "<<gDirectory->GetFile()->GetName()<<std::endl;
	Write("AnalysisOptions",TObject::kOverwrite);

	// check if we need to change back to read mode
   if(oldoption == "READ") {
		std::cout<<"  Returning "<<gDirectory->GetFile()->GetName()<<" to \""<<oldoption.c_str()<<"\" mode."<<std::endl;
      file->ReOpen("READ");
   }

	// go back to original gDirectory
   oldDir->cd();

   return success;
}

void TAnalysisOptions::ReadFromFile(const std::string& file)
{
   TDirectory* oldDir = gDirectory;
   auto        f      = new TFile(file.c_str());
   if(f != nullptr && f->IsOpen()) {
      TList* list = f->GetListOfKeys();
      TIter  iter(list);
      while(TKey* key = static_cast<TKey*>(iter.Next())) {
         if((key == nullptr) || (strcmp(key->GetClassName(), "TAnalysisOptions") != 0)) {
            continue;
         }

         *this = *static_cast<TAnalysisOptions*>(key->ReadObj());
         f->Close();
         oldDir->cd();
         return;
      }
		std::cout<<R"(Failed to find analysis options in file ")"<<CYAN<<f->GetName()<<RESET_COLOR<<R"(":)"<<std::endl;
		f->Close();
   } else {
      std::cout<<R"(Failed to open file ")"<<file<<R"(")"<<std::endl;
   }
   oldDir->cd();
}

void TAnalysisOptions::SetCorrectCrossTalk(const bool flag, Option_t* opt)
{
   fIsCorrectingCrossTalk = flag;
   TString opt1           = opt;
   opt1.ToUpper();
   if(opt1.Contains("Q")) {
      return;
   }

   printf("Please call TGriffin::ResetFlags() on current event to avoid bugs\n");
}
