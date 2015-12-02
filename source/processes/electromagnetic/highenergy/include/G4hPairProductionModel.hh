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
// $Id: G4hPairProductionModel.hh 66241 2012-12-13 18:34:42Z gunter $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4hPairProductionModel
//
// Author:        Vladimir Ivanchenko on base of G4MuPairProductionModel
// 
// Creation date: 28.02.2008
//
// Modifications:
//
//

//
// Class Description:
//
// Implementation of e+e- pair production by hadrons

// -------------------------------------------------------------------
//

#ifndef G4hPairProductionModel_h
#define G4hPairProductionModel_h 1

#include "G4MuPairProductionModel.hh"

class G4hPairProductionModel : public G4MuPairProductionModel
{

public:

  G4hPairProductionModel(const G4ParticleDefinition* p = 0,
			 const G4String& nam = "hPairProd");

  virtual ~G4hPairProductionModel();

protected:

  virtual G4double ComputeDMicroscopicCrossSection(G4double tkin,
						   G4double Z,
						   G4double pairEnergy);
private:

  // hide assignment operator
  G4hPairProductionModel & operator=(const  G4hPairProductionModel &right);
  G4hPairProductionModel(const  G4hPairProductionModel&);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
