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
// $Id: G4H2Messenger.hh 66310 2012-12-17 11:56:35Z ihrivnac $

// The messenger class for H2 management.
// It implements commands in /analysis/h2 directory.
//
// Author: Ivana Hrivnacova, 18/06/2013  (ivana@ipno.in2p3.fr)

#ifndef G4H2Messenger_h
#define G4H2Messenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4VAnalysisManager;
class G4UIdirectory;
class G4UIcommand;

class G4H2Messenger : public G4UImessenger
{
  public:
    G4H2Messenger(G4VAnalysisManager* manager);
    virtual ~G4H2Messenger();
   
    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String value);
    
  private:
    void CreateH2Cmd();
    void SetH2Cmd();
    void SetH2TitleCmd();
    void SetH2XAxisCmd();
    void SetH2YAxisCmd();
    void SetH2ZAxisCmd();
 
    G4VAnalysisManager*  fManager; ///< Associated class
    
    G4UIdirectory*         fH2Dir;   
    G4UIcommand*           fCreateH2Cmd;
    G4UIcommand*           fSetH2Cmd;
    G4UIcommand*           fSetH2TitleCmd;   
    G4UIcommand*           fSetH2XAxisCmd;   
    G4UIcommand*           fSetH2YAxisCmd;   
    G4UIcommand*           fSetH2ZAxisCmd;   
};
  
#endif
