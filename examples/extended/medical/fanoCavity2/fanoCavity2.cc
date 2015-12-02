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
/// \file medical/fanoCavity2/fanoCavity2.cc
/// \brief Main program of the medical/fanoCavity2 example
//
// $Id: fanoCavity2.cc 72961 2013-08-14 14:35:56Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "SteppingVerbose.hh"

#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
 #include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
int main(int argc,char** argv) {
 
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
 
  //my Verbose output class
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);

  //Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  //set mandatory initialization classes
  DetectorConstruction* det;
  PhysicsList* phys;
  PrimaryGeneratorAction* kin;

  runManager->SetUserInitialization(det  = new DetectorConstruction);
  runManager->SetUserInitialization(phys = new PhysicsList());
  runManager->SetUserAction(kin = new PrimaryGeneratorAction(det));

  //set user action classes
  RunAction* run        = new RunAction(det,kin);
  EventAction* event    = new EventAction(run);
  TrackingAction* track = new TrackingAction(run);
  SteppingAction* step  = new SteppingAction(det,run,track);

  runManager->SetUserAction(run);
  runManager->SetUserAction(event);
  runManager->SetUserAction(track);      
  runManager->SetUserAction(step);

  //get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

#ifdef G4VIS_USE
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();
#endif

  if (argc!=1)   // batch mode
    {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UImanager->ApplyCommand(command+fileName);
    }
 
  else           // interactive mode :define visualization and UI terminal
    {
#ifdef G4UI_USE
     G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
     UImanager->ApplyCommand("/control/execute vis.mac");
#endif
     ui->SessionStart();
     delete ui;
#endif

#ifdef G4VIS_USE
     delete visManager;
#endif
    }

  //job termination
  //
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
