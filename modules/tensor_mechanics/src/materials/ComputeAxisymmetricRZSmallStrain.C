/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "ComputeAxisymmetricRZSmallStrain.h"
#include "FEProblem.h"
#include "MooseMesh.h"

template<>
InputParameters validParams<ComputeAxisymmetricRZSmallStrain>()
{
  InputParameters params = validParams<Compute2DSmallStrain>();
  params.addClassDescription("Compute a small strain in an Axisymmetric geometry");
  return params;
}

ComputeAxisymmetricRZSmallStrain::ComputeAxisymmetricRZSmallStrain(const InputParameters & parameters) :
    Compute2DSmallStrain(parameters)
{
}

void
ComputeAxisymmetricRZSmallStrain::initialSetup()
{
  const auto & subdomainIDs = _mesh.meshSubdomains();
  for (auto subdomainID : subdomainIDs)
    if (_fe_problem.getCoordSystem(subdomainID) != Moose::COORD_RZ)
      mooseError("The coordinate system must be set to RZ for Axisymmetric geometries.");
}

Real
ComputeAxisymmetricRZSmallStrain::computeStrainZZ()
{
  if (_q_point[_qp](0) != 0.0)
    return (*_disp[0])[_qp] / _q_point[_qp](0);
  else
    return 0.0;
}
