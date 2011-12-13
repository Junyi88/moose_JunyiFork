/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef PENETRATIONLOCATOR_H
#define PENETRATIONLOCATOR_H

// Moose includes
#include "GeometricSearchInterface.h"

// libmesh includes
#include "libmesh_common.h"
#include "MooseMesh.h"
#include "vector_value.h"
#include "point.h"
#include "fe_type.h"
#include "fe.h"

#include <vector>
#include <map>

//Forward Declarations
class SubProblem;
class MooseMesh;
class GeometricSearchData;

class PenetrationLocator
{
public:
  PenetrationLocator(SubProblem & subproblem, GeometricSearchData & geom_search_data, MooseMesh & mesh, unsigned int master, unsigned int slave, Order order);
  ~PenetrationLocator();
  void detectPenetration();

  Real penetrationDistance(unsigned int node_id);
  RealVectorValue penetrationNormal(unsigned int node_id);

  /**
   * Data structure used to hold penetation information
   */
  class PenetrationInfo
  {
  public:
    PenetrationInfo(const Node * node, Elem * elem, Elem * side, unsigned int side_num, RealVectorValue norm, Real norm_distance, const Point & closest_point, const Point & closest_point_ref, const std::vector<std::vector<Real> > & side_phi, const std::vector<RealGradient> & dxyzdxi, const std::vector<RealGradient> & dxyzdeta, const std::vector<RealGradient> & d2xyzdxideta);

    PenetrationInfo(const PenetrationInfo & p);

    ~PenetrationInfo();
    const Node * _node;
    Elem * _elem;
    Elem * _side;
    unsigned int _side_num;
    RealVectorValue _normal;
    Real _distance;
    Point _closest_point;
    Point _closest_point_ref;
    std::vector<std::vector<Real> > _side_phi;
    std::vector<RealGradient> _dxyzdxi;
    std::vector<RealGradient> _dxyzdeta;
    std::vector<RealGradient> _d2xyzdxideta;
  };

  SubProblem & _subproblem;

  Real normDistance(const Elem & elem, const Elem & side, const Node & p0, Point & closest_point, RealVectorValue & normal);

  int intersect2D_Segments( Point S1P0, Point S1P1, Point S2P0, Point S2P1, Point* I0, Point* I1 );
  int inSegment(Point P, Point SP0, Point SP1);

  MooseMesh & _mesh;
  unsigned int _master_boundary;
  unsigned int _slave_boundary;

  FEType _fe_type;

  // One FE for each thread
  std::vector<FEBase * > _fe;

  NearestNodeLocator & _nearest_node;

  /**
   * Data structure of nodes and their associated penetration information
   */
  std::map<unsigned int, PenetrationInfo *> _penetration_info;

  std::map<unsigned int, bool> _has_penetrated;
  std::map<unsigned int, bool> _locked_this_step;
  std::map<unsigned int, bool> _unlocked_this_step;

  void setUpdate(bool update);

protected:
  bool _update_location; // Update the penetration location for nodes found last time
};

#endif //PENETRATIONLOCATOR_H

