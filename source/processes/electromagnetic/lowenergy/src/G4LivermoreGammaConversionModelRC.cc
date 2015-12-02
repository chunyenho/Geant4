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
// $Id: G4LivermoreGammaConversionModelRC.cc 74822 2013-10-22 14:42:13Z gcosmo $
//
// Author: Francesco Longo & Gerardo Depaola
//         on base of G4LivermoreGammaConversionModel
//
// History:
// --------
// 12 Apr 2009   V Ivanchenko Cleanup initialisation and generation of secondaries:
//                  - apply internal high-energy limit only in constructor 
//                  - do not apply low-energy limit (default is 0)
//                  - use CLHEP electron mass for low-enegry limit
//                  - remove MeanFreePath method and table


#include "G4LivermoreGammaConversionModelRC.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4LivermoreGammaConversionModelRC::G4LivermoreGammaConversionModelRC(const G4ParticleDefinition*,
								 const G4String& nam)
  :G4VEmModel(nam),fParticleChange(0),smallEnergy(2.*MeV),isInitialised(false),
   crossSectionHandler(0),meanFreePathTable(0)
{
  lowEnergyLimit = 4.0*electron_mass_c2;
  highEnergyLimit = 100 * GeV;
  SetHighEnergyLimit(highEnergyLimit);
  	 
  verboseLevel= 0;
  // Verbosity scale:
  // 0 = nothing 
  // 1 = warning for energy non-conservation 
  // 2 = details of energy budget
  // 3 = calculation of cross sections, file openings, sampling of atoms
  // 4 = entering in methods

  if(verboseLevel > 0) {
    G4cout << "Livermore Gamma conversion is constructed " << G4endl
	   << "Energy range: "
	   << lowEnergyLimit / MeV << " MeV - "
	   << highEnergyLimit / GeV << " GeV"
	   << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4LivermoreGammaConversionModelRC::~G4LivermoreGammaConversionModelRC()
{  
  if (crossSectionHandler) delete crossSectionHandler;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void 
G4LivermoreGammaConversionModelRC::Initialise(const G4ParticleDefinition*,
					    const G4DataVector&)
{
  if (verboseLevel > 3)
    G4cout << "Calling G4LivermoreGammaConversionModelRC::Initialise()" << G4endl;

  if (crossSectionHandler)
  {
    crossSectionHandler->Clear();
    delete crossSectionHandler;
  }

  // Read data tables for all materials
  
  crossSectionHandler = new G4CrossSectionHandler();
  crossSectionHandler->Initialise(0,lowEnergyLimit,100.*GeV,400);
  G4String crossSectionFile = "pair/pp-cs-";
  crossSectionHandler->LoadData(crossSectionFile);

  //
  
  if (verboseLevel > 2) 
    G4cout << "Loaded cross section files for Livermore Gamma Conversion model RC" << G4endl;

  if (verboseLevel > 0) { 
    G4cout << "Livermore Gamma Conversion model is initialized " << G4endl
	   << "Energy range: "
	   << LowEnergyLimit() / MeV << " MeV - "
	   << HighEnergyLimit() / GeV << " GeV"
	   << G4endl;
  }

  if(isInitialised) return;
  fParticleChange = GetParticleChangeForGamma();
  isInitialised = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double 
G4LivermoreGammaConversionModelRC::ComputeCrossSectionPerAtom(const G4ParticleDefinition*,
							    G4double GammaEnergy,
							    G4double Z, G4double,
							    G4double, G4double)
{
  if (verboseLevel > 3) {
    G4cout << "Calling ComputeCrossSectionPerAtom() of G4LivermoreGammaConversionModelRC" 
	   << G4endl;
  }
  if (GammaEnergy < lowEnergyLimit || GammaEnergy > highEnergyLimit) return 0;

  G4double cs = crossSectionHandler->FindValue(G4int(Z), GammaEnergy);
  return cs;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4LivermoreGammaConversionModelRC::SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
					      const G4MaterialCutsCouple* couple,
					      const G4DynamicParticle* aDynamicGamma,
					      G4double,
					      G4double)
{

// The energies of the e+ e- secondaries are sampled using the Bethe - Heitler
// cross sections with Coulomb correction. A modified version of the random
// number techniques of Butcher & Messel is used (Nuc Phys 20(1960),15).

// Note 1 : Effects due to the breakdown of the Born approximation at low
// energy are ignored.
// Note 2 : The differential cross section implicitly takes account of
// pair creation in both nuclear and atomic electron fields. However triplet
// prodution is not generated.

  if (verboseLevel > 3)
    G4cout << "Calling SampleSecondaries() of G4LivermoreGammaConversionModelRC" << G4endl;

  G4double photonEnergy = aDynamicGamma->GetKineticEnergy();
  G4ParticleMomentum photonDirection = aDynamicGamma->GetMomentumDirection();

  G4double epsilon ;
  G4double epsilon0Local = electron_mass_c2 / photonEnergy ;
  G4double electronTotEnergy;
  G4double positronTotEnergy;

  G4double HardPhotonEnergy = 0.0;


  // Do it fast if photon energy < 2. MeV
  if (photonEnergy < smallEnergy )
    {
      epsilon = epsilon0Local + (0.5 - epsilon0Local) * G4UniformRand();
 
     if (G4int(2*G4UniformRand()))
      {
        electronTotEnergy = (1. - epsilon) * photonEnergy;
        positronTotEnergy = epsilon * photonEnergy;
      }
     else
      {
         positronTotEnergy = (1. - epsilon) * photonEnergy;
         electronTotEnergy = epsilon * photonEnergy;
       }
    }
  else
    {
      // Select randomly one element in the current material
      //const G4Element* element = crossSectionHandler->SelectRandomElement(couple,photonEnergy);
      const G4ParticleDefinition* particle =  aDynamicGamma->GetDefinition();
      const G4Element* element = SelectRandomAtom(couple,particle,photonEnergy);
      G4cout << "G4LivermoreGammaConversionModelRC::SampleSecondaries" << G4endl;

      if (element == 0)
	{
	  G4cout << "G4LivermoreGammaConversionModelRC::SampleSecondaries - element = 0" 
		 << G4endl;
	  return;
	}
      G4IonisParamElm* ionisation = element->GetIonisation();
      if (ionisation == 0)
	{
	  G4cout << "G4LivermoreGammaConversionModelRC::SampleSecondaries - ionisation = 0" 
		 << G4endl;
	  return;
	}

      // Extract Coulomb factor for this Element
      G4double fZ = 8. * (ionisation->GetlogZ3());
      if (photonEnergy > 50. * MeV) fZ += 8. * (element->GetfCoulomb());

      // Limits of the screening variable
      G4double screenFactor = 136. * epsilon0Local / (element->GetIonisation()->GetZ3()) ;
      G4double screenMax = std::exp ((42.24 - fZ)/8.368) - 0.952 ;
      G4double screenMin = std::min(4.*screenFactor,screenMax) ;

      // Limits of the energy sampling
      G4double epsilon1 = 0.5 - 0.5 * std::sqrt(1. - screenMin / screenMax) ;
      G4double epsilonMin = std::max(epsilon0Local,epsilon1);
      G4double epsilonRange = 0.5 - epsilonMin ;

      // Sample the energy rate of the created electron (or positron)
      G4double screen;
      G4double gReject ;

      G4double f10 = ScreenFunction1(screenMin) - fZ;
      G4double f20 = ScreenFunction2(screenMin) - fZ;
      G4double normF1 = std::max(f10 * epsilonRange * epsilonRange,0.);
      G4double normF2 = std::max(1.5 * f20,0.);
      G4double a=393.3750918, b=115.3070201, c=810.6428451, d=19.96497475, e=1016.874592, f=1.936685510,
               gLocal=751.2140962, h=0.099751048, i=299.9466339, j=0.002057250, k=49.81034926;
      G4double aa=-18.6371131, bb=-1729.95248, cc=9450.971186, dd=106336.0145, ee=55143.09287, ff=-117602.840,
               gg=-721455.467, hh=693957.8635, ii=156266.1085, jj=533209.9347;                            
      G4double Rechazo = 0.;
      G4double logepsMin = log(epsilonMin);
      G4double NormaRC = a + b*logepsMin + c/logepsMin + d*pow(logepsMin,2.) + e/pow(logepsMin,2.) + f*pow(logepsMin,3.) +
                            gLocal/pow(logepsMin,3.) + h*pow(logepsMin,4.) + i/pow(logepsMin,4.) + j*pow(logepsMin,5.) +
                            k/pow(logepsMin,5.);
 

      G4double HardPhotonThreshold = 0.08; 
      G4double r1, r2, r3, beta=0, gbeta, sigt = 582.068, sigh, rejet;
      // , Pi = 2.*acos(0.);
      G4double cg = (11./2.)/(exp(-11.*HardPhotonThreshold/2.)-exp(-11./2.));
      
      
      r1 = G4UniformRand();
      sigh = 1028.58*exp(-HardPhotonThreshold/0.09033) + 136.63; // sigma hard 
      if (r1 > 1.- sigh/sigt) {
        r2 = G4UniformRand();
        rejet = 0.;
        while (r2 > rejet) {
          r3 = G4UniformRand();
          beta = (-2./11.)*log(exp(-0.08*11./2.)-r3*11./(2.*cg));
          gbeta = exp(-11.*beta/2.);
          rejet = fbeta(beta)/(8000.*gbeta);
        }
        HardPhotonEnergy = beta * photonEnergy;
      }
      else{
        HardPhotonEnergy = 0.;
      }
      
      photonEnergy -= HardPhotonEnergy;
      
      do {
         do {
	   if (normF1 / (normF1 + normF2) > G4UniformRand() )
	     {
	       epsilon = 0.5 - epsilonRange * std::pow(G4UniformRand(), 0.3333) ;
	       screen = screenFactor / (epsilon * (1. - epsilon));
	       gReject = (ScreenFunction1(screen) - fZ) / f10 ;
	     }
	   else
	     {
	       epsilon = epsilonMin + epsilonRange * G4UniformRand();
	       screen = screenFactor / (epsilon * (1 - epsilon));
	       gReject = (ScreenFunction2(screen) - fZ) / f20 ;
	     }
         } while ( gReject < G4UniformRand() );
                  
         if (G4int(2*G4UniformRand())) epsilon = (1. - epsilon); // Extención de Epsilon hasta 1.
       
         G4double logepsilon = log(epsilon);
         G4double deltaP_R1 = 1. + (a + b*logepsilon + c/logepsilon + d*pow(logepsilon,2.) + e/pow(logepsilon,2.) + 
                              f*pow(logepsilon,3.) + gLocal/pow(logepsilon,3.) + h*pow(logepsilon,4.) + i/pow(logepsilon,4.) + 
                              j*pow(logepsilon,5.) + k/pow(logepsilon,5.))/100.;
         G4double deltaP_R2 = 1.+((aa + cc*logepsilon +  ee*pow(logepsilon,2.) + gg*pow(logepsilon,3.) + ii*pow(logepsilon,4.))
                             / (1. + bb*logepsilon + dd*pow(logepsilon,2.) + ff*pow(logepsilon,3.) + hh*pow(logepsilon,4.) 
                             + jj*pow(logepsilon,5.) ))/100.;
       
         if (epsilon <= 0.5)
            {
            Rechazo = deltaP_R1/NormaRC;
            }
          else
            {
            Rechazo = deltaP_R2/NormaRC;
            }       
            G4cout << Rechazo << " " << NormaRC << " " << epsilon << G4endl;
      } while (Rechazo < G4UniformRand() );
      
      electronTotEnergy = (1. - epsilon) * photonEnergy;
      positronTotEnergy = epsilon * photonEnergy;

    }   //  End of epsilon sampling
    
  // Fix charges randomly

  // Scattered electron (positron) angles. ( Z - axis along the parent photon)
  // Universal distribution suggested by L. Urban (Geant3 manual (1993) Phys211),
  // derived from Tsai distribution (Rev. Mod. Phys. 49, 421 (1977)

  G4double u;
  const G4double a1 = 0.625;
  G4double a2 = 3. * a1;
  //  G4double d = 27. ;

  //  if (9. / (9. + d) > G4UniformRand())
  if (0.25 > G4UniformRand())
    {
      u = - std::log(G4UniformRand() * G4UniformRand()) / a1 ;
    }
  else
    {
      u = - std::log(G4UniformRand() * G4UniformRand()) / a2 ;
    }

  G4double thetaEle = u*electron_mass_c2/electronTotEnergy;
  G4double thetaPos = u*electron_mass_c2/positronTotEnergy;
  G4double phi  = twopi * G4UniformRand();

  G4double dxEle= std::sin(thetaEle)*std::cos(phi),dyEle= std::sin(thetaEle)*std::sin(phi),dzEle=std::cos(thetaEle);
  G4double dxPos=-std::sin(thetaPos)*std::cos(phi),dyPos=-std::sin(thetaPos)*std::sin(phi),dzPos=std::cos(thetaPos);
  
  
  // Kinematics of the created pair:
  // the electron and positron are assumed to have a symetric angular 
  // distribution with respect to the Z axis along the parent photon
  
  G4double electronKineEnergy = std::max(0.,electronTotEnergy - electron_mass_c2) ;
  
  // SI - The range test has been removed wrt original G4LowEnergyGammaconversion class

  G4ThreeVector electronDirection (dxEle, dyEle, dzEle);
  electronDirection.rotateUz(photonDirection);
      
  G4DynamicParticle* particle1 = new G4DynamicParticle (G4Electron::Electron(),
							electronDirection,
							electronKineEnergy);

  // The e+ is always created (even with kinetic energy = 0) for further annihilation
  G4double positronKineEnergy = std::max(0.,positronTotEnergy - electron_mass_c2) ;

  // SI - The range test has been removed wrt original G4LowEnergyGammaconversion class

  G4ThreeVector positronDirection (dxPos, dyPos, dzPos);
  positronDirection.rotateUz(photonDirection);   
  
  // Create G4DynamicParticle object for the particle2 
  G4DynamicParticle* particle2 = new G4DynamicParticle(G4Positron::Positron(),
						       positronDirection, 
						       positronKineEnergy);
  // Fill output vector
//  G4cout  << "Cree el e+ " <<  epsilon << G4endl;
  fvect->push_back(particle1);
  fvect->push_back(particle2);

  if (HardPhotonEnergy > 0.)
    {
      G4double thetaHardPhoton = u*electron_mass_c2/HardPhotonEnergy;
      phi  = twopi * G4UniformRand();
      G4double dxHardP= std::sin(thetaHardPhoton)*std::cos(phi);
      G4double dyHardP= std::sin(thetaHardPhoton)*std::sin(phi);
      G4double dzHardP =std::cos(thetaHardPhoton);

      G4ThreeVector hardPhotonDirection (dxHardP, dyHardP, dzHardP);
      hardPhotonDirection.rotateUz(photonDirection);
      G4DynamicParticle* particle3 = new G4DynamicParticle (G4Gamma::Gamma(),
                                                            hardPhotonDirection,
                                                            HardPhotonEnergy);
      fvect->push_back(particle3);
    }


  // kill incident photon
  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeTrackStatus(fStopAndKill);   

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4LivermoreGammaConversionModelRC::ScreenFunction1(G4double screenVariable)
{
  // Compute the value of the screening function 3*phi1 - phi2

  G4double value;
  
  if (screenVariable > 1.)
    value = 42.24 - 8.368 * std::log(screenVariable + 0.952);
  else
    value = 42.392 - screenVariable * (7.796 - 1.961 * screenVariable);
  
  return value;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4LivermoreGammaConversionModelRC::ScreenFunction2(G4double screenVariable)
{
  // Compute the value of the screening function 1.5*phi1 - 0.5*phi2
  
  G4double value;
  
  if (screenVariable > 1.)
    value = 42.24 - 8.368 * std::log(screenVariable + 0.952);
  else
    value = 41.405 - screenVariable * (5.828 - 0.8945 * screenVariable);
  
  return value;
} 


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4LivermoreGammaConversionModelRC::fbeta(G4double x)
{
  // compute the probabililty distribution for hard photon 
  G4double Pi, gamma, eta, d, p1, p2, p3, p4, p5, p6, p7, ffbeta;
  gamma = (1.-x)*(1.-x)/x;
  eta = (1.-x)/(1.+x);
  d = Dilog(1./x)-Dilog(x);
  Pi = 2.*acos(0.);
  p1 = -1.*(25528.*pow(gamma,2) + 116044.* gamma +151556.)/105.;
  p2 = 256.* pow(gamma,3) + 1092.* pow(gamma,2) +1260.*gamma + 420.;
  p3 = (676.*pow(gamma,3) + 9877.*pow(gamma,2) + 58415.*gamma + 62160.)/105.;
  p4 = 64.*pow(gamma,3) + 305.*pow(gamma,2) + 475.*gamma + 269. - 276./gamma;
  p5 = (676.*pow(gamma,3) + 38109.*pow(gamma,2) + 211637.*gamma + 266660. - 53632./gamma)/105.;
  p6 = 32.*pow(gamma,2) + 416.*gamma + 1310. +1184./gamma;
  p7 = 128.*pow(gamma,3) + 802.*pow(gamma,2) + 1028.*gamma - 470. - 1184./gamma;
  ffbeta = (1.-x) * (p1 + p2*Pi*Pi/6. + p3*log(gamma) +
                     p4*pow(log(x),2) + (p5 + p6*log(gamma))*eta*log(x) + p7*d*eta);

  return ffbeta;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4LivermoreGammaConversionModelRC::Dilog(G4double y)
{
  G4double fdilog = 0.0;
  G4double Pi = 2.*acos(0.); // serve? 
  if (y <= 0.5) {
    fdilog = pow(Pi,2)/6. + (1.-y)*(log(1-y)-1.)+pow((1.-y),2)*((1./2.)*log(1.-y)-1./4.)
      +pow((1.-y),3)*((1./3.)*log(1.-y)-1./9.)+pow((1.-y),4)*((1./4.)*log(1.-y)-1./16.);
  }
  if (0.5 < y && y < 2.) {
    fdilog = 1.-y+pow((1.-y),2)/4.+pow((1.-y),3)/9.+pow((1.-y),4)/16.+
      pow((1.-y),5)/25.+pow((1.-y),6)/36.+pow((1.-y),7)/49.;
  }
  if (y >= 2.) {
    fdilog = -pow(log(y),2)/2. - pow(Pi,2)/6. + (log(y)+1.)/y + 
      (log(y)/2.+1./4.)/pow(y,2) + (log(y)/3.+1./9.)/pow(y,3);
  }
  return fdilog;
  
}
