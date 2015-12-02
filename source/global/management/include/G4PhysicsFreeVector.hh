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
// $Id: G4PhysicsFreeVector.hh 74256 2013-10-02 14:24:02Z gcosmo $
//
// 
//--------------------------------------------------------------------
//      GEANT 4 class header file
//
//  G4PhysicsFreeVector.hh
//
//  Class description:
//
//    A physics vector which has values of energy-loss, cross-section, 
//    and other physics values of a particle in matter in a given 
//    range of the energy, momentum, etc. The scale of energy/momentum
//    bins is in free, ie. it is NOT need to be linear or log. Only 
//    restrication is that bin values alway have to increase from
//    a lower bin to a higher bin. This is necessary for the binary
//    search to work correctly.

//  History:
//    02 Dec. 1995, G.Cosmo : Structure created based on object model
//    06 Jun. 1996, K.Amako : Implemented the 1st version
//    01 Jul. 1996, K.Amako : Cache mechanism and hidden bin from the 
//                            user introduced
//    26 Sep. 1996, K.Amako : Constructor with only 'bin size' added
//    11 Nov. 2000, H.Kurashige : Use STL vector for dataVector and binVector
//    02 Oct. 2013  V.Ivanchenko : Remove FindBinLocation method
//
//--------------------------------------------------------------------

#ifndef G4PhysicsFreeVector_h
#define G4PhysicsFreeVector_h 1

#include "globals.hh"
#include "G4PhysicsVector.hh"
#include "G4DataVector.hh"

class G4PhysicsFreeVector : public G4PhysicsVector  
{
  public: // with description

    G4PhysicsFreeVector();
    explicit G4PhysicsFreeVector(size_t theNbin);
    G4PhysicsFreeVector(const G4DataVector& binVector, 
                        const G4DataVector& dataVector);
         // Constructors:
         // 'binVector' has the low edge value of each scale bin. 
         // 'dataVector' has the cross-section/energy-loss/etc at 
         // the energy/momenturm of the corresponding a bin of 
         // 'binVector'. 'binVector' and 'dataVector' need to have 
         // the same vector length.
  
    virtual ~G4PhysicsFreeVector();
         // Destructor

    void PutValue( size_t binNumber, G4double binValue, 
                                     G4double dataValue );   
         // To use this method to fill a PhysicsFreeVector, you have
         // to Construct a PhysicsFreeVector of the size you need
         // using G4PhysicsFreeVector(size_t theNbin). Also take
         // note that you have to fill all bin values and data 
         // values before you the PhysicsFreeVector.

};


#endif
