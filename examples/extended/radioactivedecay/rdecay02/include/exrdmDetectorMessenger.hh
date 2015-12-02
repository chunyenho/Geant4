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
// $Id: exrdmDetectorMessenger.hh 68007 2013-03-13 11:28:03Z gcosmo $
//
/// \file radioactivedecay/rdecay02/include/exrdmDetectorMessenger.hh
/// \brief Definition of the exrdmDetectorMessenger class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef exrdmDetectorMessenger_h
#define exrdmDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class exrdmDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class exrdmDetectorMessenger: public G4UImessenger
{
  public:
    exrdmDetectorMessenger(exrdmDetectorConstruction*);
    virtual         ~exrdmDetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    exrdmDetectorConstruction* fMyDetector;
    
    G4UIdirectory*             fExrdmDir;
    G4UIdirectory*             fDetDir;
    G4UIcmdWithAString*        fTargMatCmd;
    G4UIcmdWithAString*        fDetectMatCmd;
    G4UIcmdWithADoubleAndUnit* fTargRadiusCmd;
    G4UIcmdWithADoubleAndUnit* fDetectThicknessCmd;
    G4UIcmdWithADoubleAndUnit* fTargLengthCmd;
    G4UIcmdWithADoubleAndUnit* fDetectLengthCmd;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

