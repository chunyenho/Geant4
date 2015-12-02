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
/// \file field/field01/include/F01PhysicsList.hh
/// \brief Definition of the F01PhysicsList class
//
//
// $Id: F01PhysicsList.hh 77881 2013-11-29 08:37:53Z gcosmo $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef F01PhysicsList_h
#define F01PhysicsList_h 1

#include "G4VUserPhysicsList.hh"

class F01DetectorConstruction;
class F01PhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class F01PhysicsList: public G4VUserPhysicsList
{
  public:
    F01PhysicsList( F01DetectorConstruction*);
    virtual ~F01PhysicsList();

  protected:
    // Construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();
 
    void SetCuts();

  protected:
    // these methods construct particles
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBarions();

  protected:

    // these methods construct physics processes and register them

    void ConstructGeneral();
    void ConstructEM();

  public:

    void SetGammaCut(G4double);
    void SetElectronCut(G4double);

    void SetMaxStep(G4double);

  public:

    G4double fMaxChargedStep;

  private:

    G4double fCutForGamma;
    G4double fCutForElectron;

    F01DetectorConstruction* fDet;
    F01PhysicsListMessenger* fPhysicsListMessenger;
};

#endif
