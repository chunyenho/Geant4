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
/// \file field/field03/src/F03EventActionMessenger.cc
/// \brief Implementation of the F03EventActionMessenger class
//
//
// $Id: F03EventActionMessenger.cc 76602 2013-11-13 08:33:35Z gcosmo $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "F03EventActionMessenger.hh"

#include "F03EventAction.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

F03EventActionMessenger::F03EventActionMessenger(F03EventAction* evAct)
 : G4UImessenger(),
   fEventAction(evAct),
   fSetVerboseCmd(0),
   fPrintCmd(0)
{
  fSetVerboseCmd = new G4UIcmdWithAnInteger("/event/setverbose",this);
  fSetVerboseCmd->SetGuidance("Set verbose level .");
  fSetVerboseCmd->SetParameterName("level",true);
  fSetVerboseCmd->SetDefaultValue(0);

  fPrintCmd = new G4UIcmdWithAnInteger("/event/printModulo",this);
  fPrintCmd->SetGuidance("Print events modulo n");
  fPrintCmd->SetParameterName("EventNb",false);
  fPrintCmd->SetRange("EventNb>0");
  fPrintCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

F03EventActionMessenger::~F03EventActionMessenger()
{
  delete fSetVerboseCmd;
  delete fPrintCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void F03EventActionMessenger::SetNewValue(G4UIcommand * command,
                                          G4String newValue)
{
  if(command == fSetVerboseCmd)
    {fEventAction->SetEventVerbose(fSetVerboseCmd->GetNewIntValue(newValue));}
 
  if(command == fPrintCmd)
    {fEventAction->SetPrintModulo(fPrintCmd->GetNewIntValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
