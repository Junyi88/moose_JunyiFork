/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef RECOMPUTERADIALRETURNHYPERBOLICVISCOPLASTICITY_H
#define RECOMPUTERADIALRETURNHYPERBOLICVISCOPLASTICITY_H

#include "RecomputeRadialReturn.h"

/**
 * This class uses the Discrete material in a radial return isotropic plasticity
 * model.  This class is one of the basic radial return constitutive models;
 * more complex constitutive models combine creep and plasticity.
 *
 * This class inherits from RecomputeRadialReturn and must be used
 * in conjunction with ComputeReturnMappingStress. This uniaxial viscoplasticity
 * class computes the plastic strain as a stateful material property.  The
 * constitutive equation for scalar plastic strain rate used in this model is
 * /f$ \dot{p} = \phi (\sigma_e , r) = \alpha sinh \beta (\sigma_e -r - \sigma_y) f/$
 *
 * This class is based on the implicit integration algorithm in F. Dunne and N.
 * Petrinic's Introduction to Computational Plasticity (2004) Oxford University
 * Press, pg. 162 - 163.
 */

class RecomputeRadialReturnHyperbolicViscoplasticity : public RecomputeRadialReturn
{
public:
  RecomputeRadialReturnHyperbolicViscoplasticity(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties();
  void resetQpProperties();

  virtual void computeStressInitialize(Real effectiveTrialStress);
  virtual Real computeResidual(Real effectiveTrialStress, Real scalar);
  virtual Real computeDerivative(Real effectiveTrialStress, Real scalar);
  virtual void iterationFinalize(Real scalar);
  virtual void computeStressFinalize(const RankTwoTensor & plasticStrainIncrement);

  ///@{ Linear strain hardening parameters
  const Real _yield_stress;
  const Real _hardening_constant;
  ///@}

  ///@{ Viscoplasticity constitutive equation parameters
  const Real _c_alpha;
  const Real _c_beta;
  ///@}

  ///@{ Elastic properties
  Real _yield_condition;
  Real _shear_modulus;
  ///@}

  ///@{ Viscoplasticity terms corresponding to Dunne and Petrinic eqn 5.64
  Real _xphir;
  Real _xphidp;
  ///@}

  MaterialProperty<Real> & _hardening_variable;
  MaterialProperty<Real> & _hardening_variable_old;

  MaterialProperty<RankTwoTensor> & _plastic_strain;
  MaterialProperty<RankTwoTensor> & _plastic_strain_old;
};

template<>
InputParameters validParams<RecomputeRadialReturnHyperbolicViscoplasticity>();

#endif //RECOMPUTERADIALRETURNHYPERBOLICVISCOPLASTICITY_H
