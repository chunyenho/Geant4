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
// $Id: G4MolecularDissociationChannel.cc 84858 2014-10-21 16:08:22Z gcosmo $
//
// ----------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      History: first implementation by Alfonso Mantero 4 Mar 2009
//
// **********************************************************************

#include "G4MolecularDissociationChannel.hh"
#include "G4Molecule.hh"
#include "G4MoleculeHandleManager.hh"
#include "G4MoleculeTable.hh"

using namespace std;

// ######################################################################
// ###                      MolecularDecayChannel                     ###
// ######################################################################

struct CompMoleculePointer
{
  bool operator()(const G4Molecule* mol1, const G4Molecule* mol2) const
  {
    return (*mol1) < (*mol2);
  }
};

G4MolecularDissociationChannel::G4MolecularDissociationChannel(G4String aName) :
    fName(aName)
{
  //pointer
  fProductsVector = 0;
  //double
  fDecayTime = 0;
  fProbability = 0;
  fReleasedEnergy = 0;
  fRMSMotherMoleculeDisplacement = 0;
  fDisplacementType = 0; // meaning no displacement cf G4VMolecularDisplacer
}

G4MolecularDissociationChannel::G4MolecularDissociationChannel()
{
  // pointer
  fProductsVector = 0;
  // double
  fReleasedEnergy = 0;
  fDecayTime = 0;
  fProbability = 0;
  fRMSMotherMoleculeDisplacement = 0;
  fDisplacementType = 0; // meaning no displacement cf G4VMolecularDisplacer
}

G4MolecularDissociationChannel::~G4MolecularDissociationChannel()
{
  if (fProductsVector)
  {
    fProductsVector->clear();
    delete fProductsVector;
  }
}

G4MolecularDissociationChannel::G4MolecularDissociationChannel(const G4MolecularDissociationChannel& right)
{
  *this = right;
}

G4MolecularDissociationChannel& G4MolecularDissociationChannel::operator=(const G4MolecularDissociationChannel& right)
{
  if (&right == this) return *this;

  // string
  fName = right.fName;
  //displacement type
  fDisplacementType = right.fDisplacementType;
  // pointer
  if (right.fProductsVector)
  {
//        fProductsVector = new vector<G4MoleculeHandle>(*(right.fProductsVector));
    fProductsVector = new vector<const G4Molecule*>(*(right.fProductsVector));
  }
  else fProductsVector = 0;

  // double
  fReleasedEnergy = right.fReleasedEnergy;
  fDecayTime = right.fDecayTime;
  fProbability = right.fProbability;
  // vector
  fRMSMotherMoleculeDisplacement = right.fRMSMotherMoleculeDisplacement;
  fRMSProductsDisplacementVector = right.fRMSProductsDisplacementVector;
  return *this;

}

void G4MolecularDissociationChannel::AddProduct(const G4Molecule* molecule,
                                                G4double displacement)
{
//    if(!fProductsVector) fProductsVector = new vector<G4MoleculeHandle> ;
  if (!fProductsVector) fProductsVector = new vector<const G4Molecule*>;

//    G4MoleculeHandle molHandle(G4MoleculeHandleManager::Instance()->GetMoleculeHandle(molecule));
//    fProductsVector->push_back(molHandle);

  fProductsVector->push_back(molecule);
  fRMSProductsDisplacementVector.push_back(displacement);
}

void G4MolecularDissociationChannel::AddProduct(const G4String& molecule,
                                                G4double displacement)
{
//    if(!fProductsVector) fProductsVector = new vector<G4MoleculeHandle> ;
  if (!fProductsVector) fProductsVector = new vector<const G4Molecule*>;

  fProductsVector->push_back(
      G4MoleculeTable::Instance()->GetMoleculeModel(molecule));
  fRMSProductsDisplacementVector.push_back(displacement);
}

G4int G4MolecularDissociationChannel::GetNbProducts() const
{
  if (fProductsVector) return fProductsVector->size();
  return 0;
}

const G4Molecule* G4MolecularDissociationChannel::GetProduct(int index) const
{
  if (fProductsVector)
//        return ((*fProductsVector)[index]).get();
  return ((*fProductsVector)[index]);

  return 0;
}

G4double G4MolecularDissociationChannel::GetRMSRadialDisplacementOfProduct(const G4Molecule* product)
{
  if (!fProductsVector) return -1.;

  G4int sz = fProductsVector->size();
  G4double value = DBL_MAX;
  for (G4int i = 0; i < sz; i++)
  {
//        if(*product != *((*fProductsVector)[i]).get())
    if (*product != *((*fProductsVector)[i]))
    {
      value = fRMSProductsDisplacementVector[i];
    }
  }
  return value;
}

