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
// $Id$
//
/// \file hbook/include/g4hbook_defs.hh
/// \brief Definition of the G4Hbook analysis typedefs

// Author: Ivana Hrivnacova, 15/06/2011  (ivana@ipno.in2p3.fr)

#ifndef g4hbook_defs_h
#define g4hbook_defs_h

#include <tools/hbook/h1>
#include <tools/hbook/h2>
#include <tools/hbook/wntuple>
#include "ExG4HbookAnalysisManager.hh"

namespace G4Hbook {

  // H1 types
  typedef tools::hbook::h1  G4AnaH1; // keep for backward compatibility
  typedef tools::hbook::h1  G4H1;
  typedef std::vector<tools::hbook::h1*>::iterator  G4H1Iterator;
  typedef std::vector<tools::hbook::h1*>::const_iterator  G4H1ConstIterator;

  // H2 types
  typedef tools::hbook::h2  G4AnaH2; // keep for backward compatibility    
  typedef tools::hbook::h2  G4H2;    
  typedef std::vector<tools::hbook::h2*>::iterator  G4H2Iterator;
  typedef std::vector<tools::hbook::h2*>::const_iterator  G4H2ConstIterator;
  
  // Ntuple types
  typedef tools::hbook::wntuple  G4Ntuple;
  typedef std::vector<tools::hbook::wntuple*>::iterator  G4NtupleIterator;
  typedef std::vector<tools::hbook::wntuple*>::const_iterator  G4NtupleConstIterator;
  
  // Managers
  typedef ExG4HbookAnalysisManager G4AnalysisManager; 
} 

#endif

  
