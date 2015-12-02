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
// $Id: G4GoudsmitSaundersonMscModel.cc 88979 2015-03-17 10:10:21Z gcosmo $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
// File name:     G4GoudsmitSaundersonMscModel
//
// Author:        Omrane Kadri
//
// Creation date: 20.02.2009
//
// Modifications:
// 04.03.2009 V.Ivanchenko cleanup and format according to Geant4 EM style
//
// 15.04.2009 O.Kadri: cleanup: discard no scattering and single scattering theta 
//                     sampling from SampleCosineTheta() which means the splitting 
//                     step into two sub-steps occur only for msc regime
//
// 12.06.2009 O.Kadri: linear log-log extrapolation of lambda0 & lambda1 between 1 GeV - 100 TeV
//                     adding a theta min limit due to screening effect of the atomic nucleus
// 26.08.2009 O.Kadri: Cubic Spline interpolation was replaced with polynomial method
//                     within CalculateIntegrals method
// 05.10.2009 O.Kadri: tuning small angle theta distributions
//                     assuming the case of lambdan<1 as single scattering regime
//                     tuning theta sampling for theta below the screening angle
// 08.02.2010 O.Kadri: bugfix in compound xsection calculation and small angle computation
//                     adding a rejection condition to hard collision angular sampling
//                     ComputeTruePathLengthLimit was taken from G4WentzelVIModel
// 26.03.2010 O.Kadri: direct xsection calculation not inverse of the inverse
//                     angular sampling without large angle rejection method
//                     longitudinal displacement is computed exactly from <z>
// 12.05.2010 O.Kadri: exchange between target and projectile has as a condition the particle type (e-/e-)
//                     some cleanup to minimize time consuming (adding lamdan12 & Qn12, changing the error to 1.0e-12 for scrA)
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
//REFERENCES:
//Ref.1:E. Benedito et al.,"Mixed simulation ... cross-sections", NIMB 174 (2001) pp 91-110;
//Ref.2:I. Kawrakow et al.,"On the condensed ... transport",NIMB 142 (1998) pp 253-280;
//Ref.3:I. Kawrakow et al.,"On the representation ... calculations",NIMB 134 (1998) pp 325-336;
//Ref.4:Bielajew et al.,".....", NIMB 173 (2001) 332-343;
//Ref.5:F. Salvat et al.,"ELSEPA--Dirac partial ...molecules", Comp.Phys.Comm.165 (2005) pp 157-190;
//Ref.6:G4UrbanMscModel G4 9.2; 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4GoudsmitSaundersonTable.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4ParticleChangeForMSC.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4DynamicParticle.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4LossTableManager.hh"
#include "G4Track.hh"
#include "G4PhysicsTable.hh"
#include "Randomize.hh"
#include "G4Log.hh"
#include "G4Exp.hh"
#include <fstream>

using namespace std;

G4double G4GoudsmitSaundersonMscModel::ener[] = {-1.};
G4double G4GoudsmitSaundersonMscModel::TCSE[103][106] ;
G4double G4GoudsmitSaundersonMscModel::FTCSE[103][106] ;
G4double G4GoudsmitSaundersonMscModel::TCSP[103][106] ;
G4double G4GoudsmitSaundersonMscModel::FTCSP[103][106] ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4GoudsmitSaundersonMscModel::G4GoudsmitSaundersonMscModel(const G4String& nam)
  : G4VMscModel(nam),lowKEnergy(0.1*keV),highKEnergy(100.*TeV)
{ 
  currentKinEnergy=currentRange=skindepth=par1=par2=par3
    =zPathLength=truePathLength
    =tausmall=taulim=tlimit=charge=lambdalimit=tPathLength=lambda0=lambda1
    =lambda11=mass=0.0;
  currentMaterialIndex = -1;

  fr=0.02,rangeinit=0.,masslimite=0.6*MeV,
  particle=0;tausmall=1.e-16;taulim=1.e-6;tlimit=1.e10*mm;
  tlimitmin=10.e-6*mm;geombig=1.e50*mm;geommin=1.e-3*mm,tgeom=geombig;
  tlimitminfix=1.e-6*mm;stepmin=tlimitminfix;lambdalimit=1.*mm;smallstep=1.e10;
  theManager=G4LossTableManager::Instance();
  inside=false;insideskin=false;
  samplez=false;
  firstStep = true; 

  currentCouple = 0;
  fParticleChange = 0;

  GSTable = new G4GoudsmitSaundersonTable();

  if(ener[0] < 0.0){ 
    G4cout << "### G4GoudsmitSaundersonMscModel loading ELSEPA data" << G4endl;
    LoadELSEPAXSections();
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4GoudsmitSaundersonMscModel::~G4GoudsmitSaundersonMscModel()
{
  delete GSTable;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4GoudsmitSaundersonMscModel::Initialise(const G4ParticleDefinition* p,
					      const G4DataVector&)
{ 
  skindepth=skin*stepmin;
  SetParticle(p);
  fParticleChange = GetParticleChangeForMSC(p);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double 
G4GoudsmitSaundersonMscModel::ComputeCrossSectionPerAtom(const G4ParticleDefinition* p,
                       G4double kineticEnergy,G4double Z, G4double, G4double, G4double)
{  
  G4double kinEnergy = kineticEnergy;
  if(kinEnergy<lowKEnergy) kinEnergy=lowKEnergy;
  if(kinEnergy>highKEnergy)kinEnergy=highKEnergy;

  G4double cs(0.0), cs0(0.0);
  CalculateIntegrals(p,Z,kinEnergy,cs0,cs);
  
  return cs;
}  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector& 
G4GoudsmitSaundersonMscModel::SampleScattering(const G4ThreeVector& oldDirection, 
					       G4double)
{
  fDisplacement.set(0.0,0.0,0.0);
  G4double kineticEnergy = currentKinEnergy;
  //dynParticle->GetKineticEnergy();
  if((kineticEnergy <= 0.0) || (tPathLength <= tlimitminfix)||
     (tPathLength/tausmall < lambda1)) { return fDisplacement; }

  ///////////////////////////////////////////
  // Effective energy 
  G4double eloss  = 0.0;
  if (tPathLength > currentRange*dtrl) {
    eloss = kineticEnergy - 
      GetEnergy(particle,currentRange-tPathLength,currentCouple);
  } else {
    eloss = tPathLength*GetDEDX(particle,kineticEnergy,currentCouple);
  }
  /*
  G4double ttau      = kineticEnergy/electron_mass_c2;
  G4double ttau2     = ttau*ttau;
  G4double epsilonpp = eloss/kineticEnergy;
  G4double cst1  = epsilonpp*epsilonpp*(6+10*ttau+5*ttau2)/(24*ttau2+48*ttau+72);
  kineticEnergy *= (1 - cst1);
  */
  kineticEnergy -= 0.5*eloss;
 
  ///////////////////////////////////////////
  // additivity rule for mixture and compound xsection's
  const G4Material* mat = currentCouple->GetMaterial();
  const G4ElementVector* theElementVector = mat->GetElementVector();
  const G4double* theAtomNumDensityVector = mat->GetVecNbOfAtomsPerVolume();
  G4int nelm = mat->GetNumberOfElements();
  G4double s0(0.0), s1(0.0);
  lambda0 = 0.0;
  for(G4int i=0;i<nelm;i++)
    { 
      CalculateIntegrals(particle,(*theElementVector)[i]->GetZ(),kineticEnergy,s0,s1);
      lambda0 += (theAtomNumDensityVector[i]*s0);
    } 
  if(lambda0>0.0) { lambda0 =1./lambda0; }

  // Newton-Raphson root's finding method of scrA from: 
  // Sig1(PWA)/Sig0(PWA)=g1=2*scrA*((1+scrA)*log(1+1/scrA)-1)
  G4double g1=0.0;
  if(lambda1>0.0) { g1 = lambda0/lambda1; }

  G4double logx0,x1,delta;
  G4double x0=g1*0.5;
  // V.Ivanchenko added limit of the loop
  for(G4int i=0;i<1000;++i)
    {  
      logx0 = G4Log(1.+1./x0);
      x1 = x0-(x0*((1.+x0)*logx0-1.0)-g1*0.5)/( (1.+2.*x0)*logx0-2.0);

      // V.Ivanchenko cut step size of iterative procedure
      if(x1 < 0.0)         { x1 = 0.5*x0; }
      else if(x1 > 2*x0)   { x1 = 2*x0; }
      else if(x1 < 0.5*x0) { x1 = 0.5*x0; }
      delta = std::fabs( x1 - x0 );    
      x0 = x1;
      if(delta < 1.0e-3*x1) { break;}
    }
  G4double scrA = x1;

  G4double lambdan=0.;

  if(lambda0>0.0) { lambdan=tPathLength/lambda0; }
  if(lambdan<=1.0e-12) { return fDisplacement; }
 
  //G4cout << "E(eV)= " << kineticEnergy/eV << " L0= " << lambda0
  //	<< " L1= " << lambda1 << G4endl;

  G4double Qn1 = lambdan *g1;//2.* lambdan *scrA*((1.+scrA)*log(1.+1./scrA)-1.);
  G4double Qn12 = 0.5*Qn1;
  
  G4double cosTheta1,sinTheta1,cosTheta2,sinTheta2;
  G4double cosPhi1=1.0,sinPhi1=0.0,cosPhi2=1.0,sinPhi2=0.0;
  G4double us=0.0,vs=0.0,ws=1.0,wss=0.,x_coord=0.0,y_coord=0.0,z_coord=1.0;

  G4double epsilon1=G4UniformRand();
  G4double expn = G4Exp(-lambdan);

  if(epsilon1<expn)// no scattering 
    { return fDisplacement; }
  else if((epsilon1<((1.+lambdan)*expn))||(lambdan<1.))//single or plural scattering (Rutherford DCS's)
    {
      G4double xi=G4UniformRand();
      xi= 2.*scrA*xi/(1.-xi + scrA);
      if(xi<0.)xi=0.;
      else if(xi>2.)xi=2.;      
      ws=(1. - xi);
      wss=std::sqrt(xi*(2.-xi));      
      G4double phi0=CLHEP::twopi*G4UniformRand(); 
      us=wss*cos(phi0);
      vs=wss*sin(phi0);
    }
  else // multiple scattering
    {
      // Ref.2 subsection 4.4 "The best solution found"
      // Sample first substep scattering angle
      SampleCosineTheta(0.5*lambdan,scrA,cosTheta1,sinTheta1);
      G4double phi1  = CLHEP::twopi*G4UniformRand();
      cosPhi1 = cos(phi1);
      sinPhi1 = sin(phi1);

      // Sample second substep scattering angle
      SampleCosineTheta(0.5*lambdan,scrA,cosTheta2,sinTheta2);
      G4double phi2  = CLHEP::twopi*G4UniformRand();
      cosPhi2 = cos(phi2);
      sinPhi2 = sin(phi2);

      // Overall scattering direction
      us = sinTheta2*(cosTheta1*cosPhi1*cosPhi2 - sinPhi1*sinPhi2) 
	+ cosTheta2*sinTheta1*cosPhi1;
      vs = sinTheta2*(cosTheta1*sinPhi1*cosPhi2 + cosPhi1*sinPhi2) 
	+ cosTheta2*sinTheta1*sinPhi1;
      ws = cosTheta1*cosTheta2 - sinTheta1*sinTheta2*cosPhi2; 

      //small angle approximation for theta less than screening angle
      if(1. - ws < 0.5*scrA) 
      {
	G4int i=0;
	do{i++;
	  ws=1.+Qn12*G4Log(G4UniformRand());
	}while((fabs(ws)>1.)&&(i<20));//i<20 to avoid time consuming during the run
	if(i>=19)ws=cos(sqrt(scrA));
	wss=std::sqrt((1. - ws)*(1. + ws));      
	us=wss*std::cos(phi1);
	vs=wss*std::sin(phi1);
      }
    }
    
  //G4ThreeVector oldDirection = dynParticle->GetMomentumDirection();
  G4ThreeVector newDirection(us,vs,ws);
  newDirection.rotateUz(oldDirection);
  fParticleChange->ProposeMomentumDirection(newDirection);
 
  // corresponding to error less than 1% in the exact formula of <z>
  if(Qn1<0.02) { z_coord = 1.0 - Qn1*(0.5 - Qn1/6.); }
  else         { z_coord = (1.-G4Exp(-Qn1))/Qn1; }
  G4double rr = zPathLength*std::sqrt((1.- z_coord*z_coord)/(1.-ws*ws));
  x_coord  = rr*us;
  y_coord  = rr*vs;

  // displacement is computed relatively to the end point
  z_coord -= 1.0;
  z_coord *= zPathLength;
  /*
    G4cout << "G4GS::SampleSecondaries: e(MeV)= " << kineticEnergy
    << " sinTheta= " << sqrt(1.0 - ws*ws) 
    << " trueStep(mm)= " << tPathLength
    << " geomStep(mm)= " << zPathLength
    << G4endl;
  */

  fDisplacement.set(x_coord,y_coord,z_coord);
  fDisplacement.rotateUz(oldDirection);

  return fDisplacement;
    }    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void 
G4GoudsmitSaundersonMscModel::SampleCosineTheta(G4double lambdan, G4double scrA,
						G4double &cost, G4double &sint)
{
  G4double r1,tet,xi=0.;
  G4double Qn1 = 2.* lambdan;
  if(scrA < 10.) { Qn1 *= scrA*((1.+scrA)*G4Log(1.+1./scrA)-1.); }
  else { Qn1*= (1.0 - 0.5/scrA - 0.5/(scrA*scrA)) ; }
  if (Qn1<0.001)
    {
      do{
        r1=G4UniformRand();
        xi=-0.5*Qn1*G4Log(G4UniformRand());
        tet=acos(1.-xi);
      }while(tet*r1*r1>sin(tet));
    }
  else if(Qn1>0.5) { xi=2.*G4UniformRand(); }//isotropic distribution
  else{ xi=2.*(GSTable->SampleTheta(lambdan,scrA,G4UniformRand()));}


  if(xi<0.)xi=0.;
  else if(xi>2.)xi=2.;

  cost=(1. - xi);
  sint=sqrt(xi*(2.-xi));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Polynomial log-log interpolation of Lambda0 and Lambda1 between 100 eV - 1 GeV
// linear log-log extrapolation between 1 GeV - 100 TeV

void 
G4GoudsmitSaundersonMscModel::CalculateIntegrals(const G4ParticleDefinition* p,G4double Z, 
						 G4double kinEnergy,G4double &Sig0,
						 G4double &Sig1)
{ 
  G4double x1,x2,y1,y2,acoeff,bcoeff;
  G4double kineticE = kinEnergy;
  if(kineticE<lowKEnergy)kineticE=lowKEnergy;
  if(kineticE>highKEnergy)kineticE=highKEnergy;
  kineticE /= eV;
  G4double logE=G4Log(kineticE);
  
  G4int  iZ = G4int(Z);
  if(iZ > 103) iZ = 103;

  G4int enerInd=0;
  for(G4int i=0;i<105;i++)
  {
  if((logE>=ener[i])&&(logE<ener[i+1])){enerInd=i;break;}
  }

  if(p==G4Electron::Electron())        
    {
    if(kineticE<=1.0e+9)//Interpolation of the form y=ax�+b
      {
	x1=ener[enerInd];
	x2=ener[enerInd+1];       
	y1=TCSE[iZ-1][enerInd];
	y2=TCSE[iZ-1][enerInd+1];
	acoeff=(y2-y1)/(x2*x2-x1*x1);
	bcoeff=y2-acoeff*x2*x2;
	Sig0=acoeff*logE*logE+bcoeff;
	Sig0 =G4Exp(Sig0);
	y1=FTCSE[iZ-1][enerInd];
	y2=FTCSE[iZ-1][enerInd+1];
	acoeff=(y2-y1)/(x2*x2-x1*x1);
	bcoeff=y2-acoeff*x2*x2;
	Sig1=acoeff*logE*logE+bcoeff;
	Sig1=G4Exp(Sig1);
      }
    else  //Interpolation of the form y=ax+b
      {  
	x1=ener[104];
	x2=ener[105];       
	y1=TCSE[iZ-1][104];
	y2=TCSE[iZ-1][105];
	Sig0=(y2-y1)*(logE-x1)/(x2-x1)+y1;
	Sig0=G4Exp(Sig0);
	y1=FTCSE[iZ-1][104];
	y2=FTCSE[iZ-1][105];
	Sig1=(y2-y1)*(logE-x1)/(x2-x1)+y1;
	Sig1=G4Exp(Sig1);
      }
    }
  if(p==G4Positron::Positron())        
    {
    if(kinEnergy<=1.0e+9)
      {
	x1=ener[enerInd];
	x2=ener[enerInd+1];       
	y1=TCSP[iZ-1][enerInd];
	y2=TCSP[iZ-1][enerInd+1];
	acoeff=(y2-y1)/(x2*x2-x1*x1);
	bcoeff=y2-acoeff*x2*x2;
	Sig0=acoeff*logE*logE+bcoeff;
	Sig0 =G4Exp(Sig0);
	y1=FTCSP[iZ-1][enerInd];
	y2=FTCSP[iZ-1][enerInd+1];
	acoeff=(y2-y1)/(x2*x2-x1*x1);
	bcoeff=y2-acoeff*x2*x2;
	Sig1=acoeff*logE*logE+bcoeff;
	Sig1=G4Exp(Sig1);
      }
    else
      {  
	x1=ener[104];
	x2=ener[105];       
	y1=TCSP[iZ-1][104];
	y2=TCSP[iZ-1][105];
	Sig0=(y2-y1)*(logE-x1)/(x2-x1)+y1;
	Sig0 =G4Exp(Sig0);
	y1=FTCSP[iZ-1][104];
	y2=FTCSP[iZ-1][105];
	Sig1=(y2-y1)*(logE-x1)/(x2-x1)+y1;
	Sig1=G4Exp(Sig1);
      }
    }
    
  Sig0 *= barn;
  Sig1 *= barn;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4GoudsmitSaundersonMscModel::StartTracking(G4Track* track)
{
  SetParticle(track->GetDynamicParticle()->GetDefinition());
  firstStep = true; 
  inside = false;
  insideskin = false;
  tlimit = geombig;
  G4VEmModel::StartTracking(track);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//t->g->t step transformations taken from Ref.6 

G4double 
G4GoudsmitSaundersonMscModel::ComputeTruePathLengthLimit(const G4Track& track,
							 G4double& currentMinimalStep)
{
  tPathLength = currentMinimalStep;
  const G4DynamicParticle* dp = track.GetDynamicParticle();
  G4StepPoint* sp = track.GetStep()->GetPreStepPoint();
  G4StepStatus stepStatus = sp->GetStepStatus();
  currentCouple = track.GetMaterialCutsCouple();
  SetCurrentCouple(currentCouple); 
  currentMaterialIndex = currentCouple->GetIndex();
  currentKinEnergy = dp->GetKineticEnergy();
  currentRange = GetRange(particle,currentKinEnergy,currentCouple);

  lambda1 = GetTransportMeanFreePath(particle,currentKinEnergy);

  // stop here if small range particle
  if(inside || tPathLength < tlimitminfix) {
    return ConvertTrueToGeom(tPathLength, currentMinimalStep);
  }  
  if(tPathLength > currentRange) tPathLength = currentRange;

  G4double presafety = sp->GetSafety();

  //G4cout << "G4GS::StepLimit tPathLength= " 
  //	 <<tPathLength<<" safety= " << presafety
  //       << " range= " <<currentRange<< " lambda= "<<lambda1
  //	 << " Alg: " << steppingAlgorithm <<G4endl;

  // far from geometry boundary
  if(currentRange < presafety)
    {
      inside = true;
      return ConvertTrueToGeom(tPathLength, currentMinimalStep);  
    }

  // standard  version
  //
  if (steppingAlgorithm == fUseDistanceToBoundary)
    {
      //compute geomlimit and presafety 
      G4double geomlimit = ComputeGeomLimit(track, presafety, tPathLength);
   
      // is far from boundary
      if(currentRange <= presafety)
	{
	  inside = true;
	  return ConvertTrueToGeom(tPathLength, currentMinimalStep);   
	}

      smallstep += 1.;
      insideskin = false;

      if(firstStep || stepStatus == fGeomBoundary)
	{
          rangeinit = currentRange;
          if(firstStep) smallstep = 1.e10;
          else  smallstep = 1.;

          //define stepmin here (it depends on lambda!)
          //rough estimation of lambda_elastic/lambda_transport
          G4double rat = currentKinEnergy/MeV ;
          rat = 1.e-3/(rat*(10.+rat)) ;
          //stepmin ~ lambda_elastic
          stepmin = rat*lambda1;
          skindepth = skin*stepmin;
          //define tlimitmin
          tlimitmin = 10.*stepmin;
          if(tlimitmin < tlimitminfix) tlimitmin = tlimitminfix;

	  //G4cout << "rangeinit= " << rangeinit << " stepmin= " << stepmin
	  //	 << " tlimitmin= " << tlimitmin << " geomlimit= " << geomlimit <<G4endl;
	  // constraint from the geometry 
	  if((geomlimit < geombig) && (geomlimit > geommin))
	    {
	      if(stepStatus == fGeomBoundary)  
	        tgeom = geomlimit/facgeom;
	      else
	        tgeom = 2.*geomlimit/facgeom;
	    }
            else
              tgeom = geombig;

        }

      //step limit 
      tlimit = facrange*rangeinit;              
      if(tlimit < facsafety*presafety)
        tlimit = facsafety*presafety; 

      //lower limit for tlimit
      if(tlimit < tlimitmin) tlimit = tlimitmin;

      if(tlimit > tgeom) tlimit = tgeom;

      //G4cout << "tgeom= " << tgeom << " geomlimit= " << geomlimit  
      //      << " tlimit= " << tlimit << " presafety= " << presafety << G4endl;

      // shortcut
      if((tPathLength < tlimit) && (tPathLength < presafety) &&
         (smallstep >= skin) && (tPathLength < geomlimit-0.999*skindepth))
	return ConvertTrueToGeom(tPathLength, currentMinimalStep);   

      // step reduction near to boundary
      if(smallstep < skin)
	{
	  tlimit = stepmin;
	  insideskin = true;
	}
      else if(geomlimit < geombig)
	{
	  if(geomlimit > skindepth)
	    {
	      if(tlimit > geomlimit-0.999*skindepth)
		tlimit = geomlimit-0.999*skindepth;
	    }
	  else
	    {
	      insideskin = true;
	      if(tlimit > stepmin) tlimit = stepmin;
	    }
	}

      if(tlimit < stepmin) tlimit = stepmin;

      if(tPathLength > tlimit) tPathLength = tlimit; 

    }
    // for 'normal' simulation with or without magnetic field 
    //  there no small step/single scattering at boundaries
  else if(steppingAlgorithm == fUseSafety)
    {
      // compute presafety again if presafety <= 0 and no boundary
      // i.e. when it is needed for optimization purposes
      if((stepStatus != fGeomBoundary) && (presafety < tlimitminfix)) 
        presafety = ComputeSafety(sp->GetPosition(),tPathLength); 

      // is far from boundary
      if(currentRange < presafety)
        {
          inside = true;
          return ConvertTrueToGeom(tPathLength, currentMinimalStep);  
        }

      if(firstStep || stepStatus == fGeomBoundary)
	{
	  rangeinit = currentRange;
	  fr = facrange;
	  // 9.1 like stepping for e+/e- only (not for muons,hadrons)
	  if(mass < masslimite) 
	    {
	      if(lambda1 > currentRange)
		rangeinit = lambda1;
	      if(lambda1 > lambdalimit)
		fr *= 0.75+0.25*lambda1/lambdalimit;
	    }

	  //lower limit for tlimit
	  G4double rat = currentKinEnergy/MeV ;
	  rat = 1.e-3/(rat*(10.+rat)) ;
	  tlimitmin = 10.*lambda1*rat;
	  if(tlimitmin < tlimitminfix) tlimitmin = tlimitminfix;
	}
      //step limit
      tlimit = fr*rangeinit;               

      if(tlimit < facsafety*presafety)
        tlimit = facsafety*presafety;

      //lower limit for tlimit
      if(tlimit < tlimitmin) tlimit = tlimitmin;

      if(tPathLength > tlimit) tPathLength = tlimit;
    }
  
  // version similar to 7.1 (needed for some experiments)
  else
    {
      if (stepStatus == fGeomBoundary)
	{
	  if (currentRange > lambda1) tlimit = facrange*currentRange;
	  else                        tlimit = facrange*lambda1;

	  if(tlimit < tlimitmin) tlimit = tlimitmin;
	  if(tPathLength > tlimit) tPathLength = tlimit;
	}
    }
  //G4cout << "tPathLength= " << tPathLength  
  // << " currentMinimalStep= " << currentMinimalStep << G4endl;
  return ConvertTrueToGeom(tPathLength, currentMinimalStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// taken from Ref.6
G4double G4GoudsmitSaundersonMscModel::ComputeGeomPathLength(G4double)
{
  firstStep = false; 
  par1 = -1. ;  
  par2 = par3 = 0. ;  

  //  do the true -> geom transformation
  zPathLength = tPathLength;

  // z = t for very small tPathLength
  if(tPathLength < tlimitminfix) { return zPathLength; }

  // this correction needed to run MSC with eIoni and eBrem inactivated
  // and makes no harm for a normal run
  if(tPathLength > currentRange)
    { tPathLength = currentRange; }

  G4double tau   = tPathLength/lambda1 ;

  if ((tau <= tausmall) || insideskin) {
    zPathLength  = tPathLength;
    if(zPathLength > lambda1) { zPathLength = lambda1; }
    return zPathLength;
  }

  G4double zmean = tPathLength;
  if (tPathLength < currentRange*dtrl) {
    if(tau < taulim) zmean = tPathLength*(1.-0.5*tau) ;
    else             zmean = lambda1*(1.-G4Exp(-tau));
  } else if(currentKinEnergy < mass || tPathLength == currentRange) {
    par1 = 1./currentRange ;
    par2 = 1./(par1*lambda1) ;
    par3 = 1.+par2 ;
    if(tPathLength < currentRange)
      zmean = (1.-G4Exp(par3*G4Log(1.-tPathLength/currentRange)))/(par1*par3) ;
    else
      zmean = 1./(par1*par3) ;
  } else {
    G4double T1 = GetEnergy(particle,currentRange-tPathLength,currentCouple);

    lambda11 = GetTransportMeanFreePath(particle,T1);

    par1 = (lambda1-lambda11)/(lambda1*tPathLength) ;
    par2 = 1./(par1*lambda1) ;
    par3 = 1.+par2 ;
    zmean = (1.-G4Exp(par3*G4Log(lambda11/lambda1)))/(par1*par3) ;
  }

  zPathLength = zmean ;
  //  sample z
  if(samplez) {

    const G4double  ztmax = 0.99;
    G4double zt = zmean/tPathLength ;

    if (tPathLength > stepmin && zt < ztmax) {

      G4double u,cz1;
      if(zt >= 0.333333333) {

        G4double cz = 0.5*(3.*zt-1.)/(1.-zt) ;
        cz1 = 1.+cz ;
        G4double u0 = cz/cz1 ;
        G4double grej ;
        do {
	  u = G4Exp(G4Log(G4UniformRand())/cz1) ;
	  grej = exp(cz*G4Log(u/u0))*(1.-u)/(1.-u0) ;
	} while (grej < G4UniformRand()) ;

      } else {
        cz1 = 1./zt-1.;
        u = 1.-G4Exp(G4Log(G4UniformRand())/cz1) ;
      }
      zPathLength = tPathLength*u ;
    }
  }
  if(zPathLength > lambda1) zPathLength = lambda1;
  //G4cout << "zPathLength= " << zPathLength << " lambda1= " << lambda1 << G4endl;

  return zPathLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// taken from Ref.6
G4double 
G4GoudsmitSaundersonMscModel::ComputeTrueStepLength(G4double geomStepLength)
{
  // step defined other than transportation 
  if(geomStepLength == zPathLength && tPathLength <= currentRange)
    return tPathLength;

  // t = z for very small step
  zPathLength = geomStepLength;
  tPathLength = geomStepLength;
  if(geomStepLength < tlimitminfix) return tPathLength;
  
  // recalculation
  if((geomStepLength > lambda1*tausmall) && !insideskin)
    {
      if(par1 <  0.)
	tPathLength = -lambda1*G4Log(1.-geomStepLength/lambda1) ;
      else 
	{
	  if(par1*par3*geomStepLength < 1.)
	    tPathLength = (1.-G4Exp(G4Log(1.-par1*par3*geomStepLength)/par3))/par1 ;
	  else 
	    tPathLength = currentRange;
	}  
    }
  if(tPathLength < geomStepLength) tPathLength = geomStepLength;
  //G4cout << "tPathLength= " << tPathLength << " step= " << geomStepLength << G4endl;

  return tPathLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Total & first transport x sections for e-/e+ generated from ELSEPA code

void G4GoudsmitSaundersonMscModel::LoadELSEPAXSections()
{ 
  G4String filename = "XSECTIONS.dat";

  char* path = getenv("G4LEDATA");
  if (!path)
    {
      G4Exception("G4GoudsmitSaundersonMscModel::LoadELSEPAXSections()","em0006",
		  FatalException,
                  "Environment variable G4LEDATA not defined");
      return;
    }

  G4String pathString(path);
  G4String dirFile = pathString + "/msc_GS/" + filename;
  std::ifstream infile(dirFile, std::ios::in);
  if( !infile.is_open()) {
    G4ExceptionDescription ed;
    ed << "Data file <" + dirFile + "> is not opened!";
    G4Exception("G4GoudsmitSaundersonMscModel::LoadELSEPAXSections()",
		"em0003",FatalException,ed);
    return;
  }

  // Read parameters from tables and take logarithms
  G4double aRead;
  for(G4int i=0 ; i<106 ;i++){
    infile >> aRead;
    if(infile.fail()) {
      G4ExceptionDescription ed;
      ed << "Error reading <" + dirFile + "> loop #1 i= " << i;
      G4Exception("G4GoudsmitSaundersonMscModel::LoadELSEPAXSections()",
		  "em0003",FatalException,ed);
      return;
    } else {
      if(aRead > 0.0) { aRead = G4Log(aRead); }
      else            { aRead = 0.0; }
    }
    ener[i]=aRead;
  }        
  for(G4int j=0;j<103;j++){
    for(G4int i=0;i<106;i++){
      infile >> aRead;
      if(infile.fail()) {
	G4ExceptionDescription ed;
	ed << "Error reading <" + dirFile + "> loop #2 j= " << j 
	   << "; i= " << i;
	G4Exception("G4GoudsmitSaundersonMscModel::LoadELSEPAXSections()",
		    "em0003",FatalException,ed);
	return;
      } else {
	if(aRead > 0.0) { aRead = G4Log(aRead); }
	else            { aRead = 0.0; }
      }
      TCSE[j][i]=aRead;
    }        
  }
  for(G4int j=0;j<103;j++){
    for(G4int i=0;i<106;i++){
      infile >> aRead;
      if(infile.fail()) {
	G4ExceptionDescription ed;
	ed << "Error reading <" + dirFile + "> loop #3 j= " << j 
	   << "; i= " << i;
	G4Exception("G4GoudsmitSaundersonMscModel::LoadELSEPAXSections()",
		    "em0003",FatalException,ed);
	return;
      } else {
	if(aRead > 0.0) { aRead = G4Log(aRead); }
	else            { aRead = 0.0; }
      }
      FTCSE[j][i]=aRead;      
    }        
  }    
  for(G4int j=0;j<103;j++){
    for(G4int i=0;i<106;i++){
      infile >> aRead;
      if(infile.fail()) {
	G4ExceptionDescription ed;
	ed << "Error reading <" + dirFile + "> loop #4 j= " << j 
	   << "; i= " << i;
	G4Exception("G4GoudsmitSaundersonMscModel::LoadELSEPAXSections()",
		    "em0003",FatalException,ed);
	return;
      } else {
	if(aRead > 0.0) { aRead = G4Log(aRead); }
	else            { aRead = 0.0; }
      }
      TCSP[j][i]=aRead;      
    }        
  }
  for(G4int j=0;j<103;j++){
    for(G4int i=0;i<106;i++){
      infile >> aRead;
      if(infile.fail()) {
	G4ExceptionDescription ed;
	ed << "Error reading <" + dirFile + "> loop #5 j= " << j 
	   << "; i= " << i;
	G4Exception("G4GoudsmitSaundersonMscModel::LoadELSEPAXSections()",
		    "em0003",FatalException,ed);
	return;
      } else {
	if(aRead > 0.0) { aRead = G4Log(aRead); }
	else            { aRead = 0.0; }
      }
      FTCSP[j][i]=aRead;      
    }        
  }

  infile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
