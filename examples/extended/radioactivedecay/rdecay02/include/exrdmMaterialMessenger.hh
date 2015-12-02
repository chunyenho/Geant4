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
// $Id: exrdmMaterialMessenger.hh 68007 2013-03-13 11:28:03Z gcosmo $
//
/// \file radioactivedecay/rdecay02/include/exrdmMaterialMessenger.hh
/// \brief Definition of the exrdmMaterialMessenger class
//
#ifndef exrdmMaterialMessenger_h
#define exrdmMaterialMessenger_h 1
////////////////////////////////////////////////////////////////////////////////
//
#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

class exrdmMaterial;
////////////////////////////////////////////////////////////////////////////////
//
class exrdmMaterialMessenger: public G4UImessenger
{
public:
  exrdmMaterialMessenger(exrdmMaterial* );
  virtual ~exrdmMaterialMessenger();

  virtual void SetNewValue (G4UIcommand*, G4String);

private:

  exrdmMaterial                *fMaterialsManager;

  G4UIdirectory             *fMaterialDir;
  G4UIcmdWithoutParameter   *fListCmd;
  G4UIcmdWithAnInteger      *fDeleteIntCmd;
  G4UIcmdWithAString        *fDeleteNameCmd;
  G4UIcommand               *fAddCmd;
};
////////////////////////////////////////////////////////////////////////////////
#endif
