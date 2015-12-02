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
//
// $Id: G4Ne23GEMCoulombBarrier.hh 67983 2013-03-13 10:42:03Z gcosmo $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Dec 1999)
//
//J.M. Quesada (July 2009) Coulomb barrier now derives from new class G4GEMCoulombBarrier
// strictly according to Furihata's report
//
#ifndef G4Ne23GEMCoulombBarrier_h
#define G4Ne23GEMCoulombBarrier_h 1

//#include "G4GEMCoulombBarrierHE.hh"
#include "G4GEMCoulombBarrier.hh"
#include "globals.hh"

//JMQ 200709 now derives from new class G4GEMCoulombBarrier
//class G4Ne23GEMCoulombBarrier : public G4GEMCoulombBarrierHE
class G4Ne23GEMCoulombBarrier : public G4GEMCoulombBarrier
{
public:
//JMQ 200709
//  G4Ne23GEMCoulombBarrier() : G4GEMCoulombBarrierHE(23,10) {};
  G4Ne23GEMCoulombBarrier() : G4GEMCoulombBarrier(23,10) {};
  ~G4Ne23GEMCoulombBarrier() {};

private:
  G4Ne23GEMCoulombBarrier(const G4Ne23GEMCoulombBarrier & right);

  const G4Ne23GEMCoulombBarrier & operator=(const G4Ne23GEMCoulombBarrier & right);
  G4bool operator==(const G4Ne23GEMCoulombBarrier & right) const;
  G4bool operator!=(const G4Ne23GEMCoulombBarrier & right) const;
  

};

#endif
