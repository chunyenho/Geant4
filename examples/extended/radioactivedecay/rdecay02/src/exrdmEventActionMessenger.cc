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
// $Id: exrdmEventActionMessenger.cc 68030 2013-03-13 13:51:27Z gcosmo $
//
/// \file radioactivedecay/rdecay02/src/exrdmEventActionMessenger.cc
/// \brief Implementation of the exrdmEventActionMessenger class
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// 

#include "exrdmEventActionMessenger.hh"

#include "exrdmEventAction.hh"
#include "G4UIcmdWithAString.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

exrdmEventActionMessenger::exrdmEventActionMessenger(exrdmEventAction* EvAct)
:G4UImessenger(),
 fEventAction(EvAct),fDrawCmd(0)
{
  fDrawCmd = new G4UIcmdWithAString("/event/draw",this);
  fDrawCmd->SetGuidance("Draw the tracks in the event");
  fDrawCmd->SetGuidance("  Choice : none, charged, all (default)");
  fDrawCmd->SetParameterName("choice",true);
  fDrawCmd->SetDefaultValue("all");
  fDrawCmd->SetCandidates("none charged all");
  fDrawCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

exrdmEventActionMessenger::~exrdmEventActionMessenger()
{
  delete fDrawCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void exrdmEventActionMessenger::SetNewValue(G4UIcommand * command,
                                            G4String newValue)
{ 
  if(command == fDrawCmd)
    {fEventAction->SetDrawFlag(newValue);}
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....





