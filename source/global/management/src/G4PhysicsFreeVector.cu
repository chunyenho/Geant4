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
// $Id: G4PhysicsFreeVector.cc 74256 2013-10-02 14:24:02Z gcosmo $
//
// 
//--------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//  G4PhysicsFreeVector.cc
//
//  History:
//    02 Dec. 1995, G.Cosmo : Structure created based on object model
//    06 June 1996, K.Amako : The 1st version of implemented
//    01 Jul. 1996, K.Amako : Cache mechanism and hidden bin from the 
//                            user introduced
//    26 Sep. 1996, K.Amako : Constructor with only 'bin size' added
//    11 Nov. 2000, H.Kurashige : use STL vector for dataVector and binVector
//    19 Jun. 2009, V.Ivanchenko : removed hidden bin 
//
//--------------------------------------------------------------------

#include "G4PhysicsFreeVector.hh"


G4PhysicsFreeVector::G4PhysicsFreeVector() 
  : G4PhysicsVector()
{
  type = T_G4PhysicsFreeVector;
}

G4PhysicsFreeVector::G4PhysicsFreeVector(size_t theNbin)
  : G4PhysicsVector()
{
  type = T_G4PhysicsFreeVector;
  numberOfNodes = theNbin;

  dataVector.reserve(numberOfNodes);
  binVector.reserve(numberOfNodes);

  for (size_t i=0; i<numberOfNodes; i++)
  {
     binVector.push_back(0.0);
     dataVector.push_back(0.0);
  }
}  

G4PhysicsFreeVector::G4PhysicsFreeVector(const G4DataVector& theBinVector, 
                                         const G4DataVector& theDataVector)
{
  type = T_G4PhysicsFreeVector;
  numberOfNodes = theBinVector.size();

  dataVector.reserve(numberOfNodes);
  binVector.reserve(numberOfNodes);

  for (size_t i=0; i<numberOfNodes; i++)
  {
     binVector.push_back(theBinVector[i]);
     dataVector.push_back(theDataVector[i]);
  }

  edgeMin = binVector[0];
  edgeMax = binVector[numberOfNodes-1];
}  

G4PhysicsFreeVector::~G4PhysicsFreeVector()
{
}

void G4PhysicsFreeVector::PutValue( size_t theBinNumber, 
				    G4double theBinValue, 
                                    G4double theDataValue )
{
  binVector[theBinNumber]  = theBinValue;
  dataVector[theBinNumber] = theDataValue;

  if( theBinNumber == numberOfNodes-1 )
  {
     edgeMax = binVector[numberOfNodes-1];
  }

  if( theBinNumber == 0 )
  {
     edgeMin = binVector[0];
  }
}
/*
size_t G4PhysicsFreeVector::FindBinLocation(G4double theEnergy) const
{
  // For G4PhysicsFreeVector, FindBinLocation is implemented using
  // the binary search algorithm.
  //
  // Because this is a virtual function, it is accessed through a
  // pointer to the G4PhysicsVector object for most usages. In this
  // case, 'inline' will not be invoked. However, there is a possibility 
  // that the user access to the G4PhysicsFreeVector object directly and 
  // not through pointers or references. In this case, the 'inline' will
  // be invoked. (See R.B.Murray, "C++ Strategies and Tactics", Chap.6.6)

  size_t lowerBound = 0;
  size_t upperBound = numberOfNodes-2;

  while (lowerBound <= upperBound)
  {
    size_t midBin = (lowerBound + upperBound)/2;
    if( theEnergy < binVector[midBin] )
       { upperBound = midBin-1; }
    else
       { lowerBound = midBin+1; }
  }

  return upperBound;
}
*/
