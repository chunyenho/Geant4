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
// $Id: G4EvaporationChannel.hh 85841 2014-11-05 15:35:06Z gcosmo $
//
//
//J.M. Quesada (August2008). Based on:
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//
// 17-11-2010 V.Ivanchenko in constructor replace G4VEmissionProbability by 
//            G4EvaporationProbability and do not new and delete probability
//            object at each call; use G4Pow

#ifndef G4EvaporationChannel_h
#define G4EvaporationChannel_h 1

#include "G4VEvaporationChannel.hh"
#include "G4EvaporationProbability.hh"
#include "G4VCoulombBarrier.hh"

class G4EvaporationLevelDensityParameter;
class G4PairingCorrection;

class G4EvaporationChannel : public G4VEvaporationChannel
{
public:

  G4EvaporationChannel(G4int theA, G4int theZ, const G4String & aName,
		       G4EvaporationProbability * aEmissionStrategy,
	               G4VCoulombBarrier * aCoulombBarrier);

  virtual ~G4EvaporationChannel();

  void Initialise();
  
  inline void SetEmissionStrategy(G4EvaporationProbability * aEmissionStrategy)
  {theEvaporationProbabilityPtr = aEmissionStrategy;}

  inline void SetCoulombBarrierStrategy(G4VCoulombBarrier * aCoulombBarrier)
  {theCoulombBarrierPtr = aCoulombBarrier;} 
    
  virtual G4double GetEmissionProbability(G4Fragment* fragment); 

  virtual G4Fragment* EmittedFragment(G4Fragment* theNucleus);

  virtual G4FragmentVector * BreakUp(const G4Fragment & theNucleus);
  
private: 
  
 // Samples fragment kinetic energy.
  G4double  SampleKineticEnergy(const G4Fragment & aFragment);

  // This has to be removed and put in Random Generator
  G4ThreeVector IsotropicVector(G4double Magnitude  = 1.0);

  G4EvaporationChannel(const G4EvaporationChannel & right);
  const G4EvaporationChannel & operator=(const G4EvaporationChannel & right);
  G4bool operator==(const G4EvaporationChannel & right) const;
  G4bool operator!=(const G4EvaporationChannel & right) const;

private:

  // This data member define the channel. 
  // They are intializated at object creation (constructor) time.

  // Atomic Number of ejectile
  G4int theA;

  // Charge of ejectile
  G4int theZ;

  G4double EvaporatedMass;
  G4double ResidualMass;

  // For evaporation probability calcualation
  G4EvaporationProbability * theEvaporationProbabilityPtr;

  // For Level Density calculation
  G4VLevelDensityParameter * theLevelDensityPtr;

  // For Coulomb Barrier calculation
  G4VCoulombBarrier * theCoulombBarrierPtr;
  G4double CoulombBarrier;

  G4PairingCorrection* pairingCorrection;
   
  //---------------------------------------------------

  // These values depend on the nucleus that is being evaporated.
  // They are calculated through the Initialize method which takes as parameters 
  // the atomic number, charge and excitation energy of nucleus.

  // Residual Mass Number
  G4int ResidualA;

  // Residual Charge
  G4int ResidualZ;
	
  // Emission Probability
  G4double EmissionProbability;

  // Maximal Kinetic Energy that can be carried by fragment
  G4double MaximalKineticEnergy;

};


#endif
