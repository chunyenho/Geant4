//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4CsvNtupleManager.hh 70604 2013-06-03 11:27:06Z ihrivnac $

// Manager class for CSV ntuples.
//
// Author: Ivana Hrivnacova, 18/06/2013  (ivana@ipno.in2p3.fr)

#ifndef G4CsvNtupleManager_h
#define G4CsvNtupleManager_h 1


#include "G4VNtupleManager.hh"
#include "globals.hh"

#include "tools/wcsv_ntuple"

#include <vector>

class G4CsvFileManager;
struct G4CsvNtupleDescription;

class G4CsvNtupleManager : public G4VNtupleManager
{
  friend class G4CsvAnalysisManager;

  protected:
    G4CsvNtupleManager(const G4AnalysisManagerState& state);
    ~G4CsvNtupleManager();
 
    // Functions specific to the output type
    //
    
    // Methods to manipulate ntuples  
    void CreateNtuplesFromBooking();
    G4bool IsEmpty() const;
    G4bool Reset();

    // Set methods
    void SetFileManager(G4CsvFileManager* fileManager);

    // Access methods
    tools::wcsv::ntuple* GetNtuple() const;
    tools::wcsv::ntuple* GetNtuple(G4int ntupleId) const;

    // Iterators
    std::vector<tools::wcsv::ntuple*>::iterator BeginNtuple();
    std::vector<tools::wcsv::ntuple*>::iterator EndNtuple();
    std::vector<tools::wcsv::ntuple*>::const_iterator BeginConstNtuple() const;
    std::vector<tools::wcsv::ntuple*>::const_iterator EndConstNtuple() const;
    
    // Access to ntuple vector (needed for Write())
    const std::vector<G4CsvNtupleDescription*>& GetNtupleDescriptionVector() const;

    // Functions independent from the output type 
    //

    // Methods to create ntuples
    virtual G4int CreateNtuple(const G4String& name, const G4String& title);
    // Create columns in the last created ntuple
    virtual G4int CreateNtupleIColumn(
                    const G4String& name, std::vector<int>* vector);
    virtual G4int CreateNtupleFColumn(
                    const G4String& name, std::vector<float>* vector);
    virtual G4int CreateNtupleDColumn(
                    const G4String& name, std::vector<double>* vector);
    virtual G4int CreateNtupleSColumn(const G4String& name);
    virtual void  FinishNtuple();   
    // Create columns in the ntuple with given id
    virtual G4int CreateNtupleIColumn(G4int ntupleId, 
                    const G4String& name, std::vector<int>* vector);
    virtual G4int CreateNtupleFColumn(G4int ntupleId, 
                    const G4String& name, std::vector<float>* vector);
    virtual G4int CreateNtupleDColumn(G4int ntupleId, 
                    const G4String& name, std::vector<double>* vector);
    virtual G4int CreateNtupleSColumn(G4int ntupleId, const G4String& name);
    virtual void  FinishNtuple(G4int ntupleId);   

    // Methods to fill ntuples
    // Methods for ntuple with id = FirstNtupleId                     
    virtual G4bool FillNtupleIColumn(G4int columnId, G4int value);
    virtual G4bool FillNtupleFColumn(G4int columnId, G4float value);
    virtual G4bool FillNtupleDColumn(G4int columnId, G4double value);
    virtual G4bool FillNtupleSColumn(G4int columnId, const G4String& value);
    virtual G4bool AddNtupleRow();
    // Methods for ntuple with id > FirstNtupleId (when more ntuples exist)                      
    virtual G4bool FillNtupleIColumn(G4int ntupleId, G4int columnId, G4int value);
    virtual G4bool FillNtupleFColumn(G4int ntupleId, G4int columnId, G4float value);
    virtual G4bool FillNtupleDColumn(G4int ntupleId, G4int columnId, G4double value);
    virtual G4bool FillNtupleSColumn(G4int ntupleId, G4int columnId, 
                                     const G4String& value);
    virtual G4bool AddNtupleRow(G4int ntupleId);

    // Access methods
    virtual G4int GetNofNtuples() const;

    // Csv format specific option
    void SetIsCommentedHeader(G4bool isCommentedHeader);
    void SetIsHippoHeader(G4bool isHippoHeader);
    
  private:
    // methods
    //
    tools::wcsv::ntuple::column<int>*    
      GetNtupleIColumn(G4int ntupleId, G4int columnId) const;
    tools::wcsv::ntuple::column<float>*  
      GetNtupleFColumn(G4int ntupleId, G4int columnId) const;
    tools::wcsv::ntuple::column<double>* 
      GetNtupleDColumn(G4int ntupleId, G4int columnId) const;
    tools::wcsv::ntuple::column<std::string>* 
      GetNtupleSColumn(G4int ntupleId, G4int columnId) const;

    virtual G4CsvNtupleDescription*  GetNtupleInFunction(G4int id, 
                                        G4String function,
                                        G4bool warn = true,
                                        G4bool onlyIfActive = true) const;

    G4bool WriteHeader(tools::wcsv::ntuple* ntuple) const;

    // data members
    //
    G4CsvFileManager*  fFileManager;
    std::vector<G4CsvNtupleDescription*> fNtupleDescriptionVector;
    std::vector<tools::wcsv::ntuple*> fNtupleVector;
    G4bool  fIsCommentedHeader;
    G4bool  fIsHippoHeader;
};

// inline functions

inline void G4CsvNtupleManager::SetFileManager(G4CsvFileManager* fileManager)
{ fFileManager = fileManager; }

inline std::vector<tools::wcsv::ntuple*>::iterator 
G4CsvNtupleManager::BeginNtuple()
{ return fNtupleVector.begin(); }

inline std::vector<tools::wcsv::ntuple*>::iterator 
G4CsvNtupleManager::EndNtuple()
{ return fNtupleVector.end(); }

inline std::vector<tools::wcsv::ntuple*>::const_iterator 
G4CsvNtupleManager::BeginConstNtuple() const
{ return fNtupleVector.begin(); }

inline  std::vector<tools::wcsv::ntuple*>::const_iterator 
G4CsvNtupleManager::EndConstNtuple() const
{ return fNtupleVector.end(); }

inline G4int G4CsvNtupleManager::GetNofNtuples() const
{ return fNtupleDescriptionVector.size(); }

inline const std::vector<G4CsvNtupleDescription*>& 
G4CsvNtupleManager::GetNtupleDescriptionVector() const
{ return fNtupleDescriptionVector; }

inline void G4CsvNtupleManager::SetIsCommentedHeader(G4bool isCommentedHeader)
{ fIsCommentedHeader = isCommentedHeader; }

inline void G4CsvNtupleManager::SetIsHippoHeader(G4bool isHippoHeader)
{ fIsHippoHeader = isHippoHeader; }

#endif

