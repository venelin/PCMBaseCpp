/*
 *  QuadraticPolynomialBM.h
 *  PCMBaseCpp
 *
 * Copyright 2017 Venelin Mitov
 *
 * This file is part of PCMBaseCpp: A C++ backend for calculating the likelihood
 *  of phylogenetic comparative models.
 *
 * PCMBaseCpp is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * PCMBaseCpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with PCMBaseCpp.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * @author Venelin Mitov
 */
#ifndef QuadraticPolynomial_MRG_H_
#define QuadraticPolynomial_MRG_H_

#include "QuadraticPolynomial.h"
#include "QuadraticPolynomialBM.h"
#include "QuadraticPolynomialJOU.h"
#include "QuadraticPolynomialOU.h"
#include "QuadraticPolynomialTwoSpeedOU.h"
#include <armadillo>
#include <sstream>

namespace PCMBaseCpp {


typedef splittree::OrderedTree<splittree::uint, LengthRegimeAndJump> MRGTreeType;

class MRG: public QuadraticPolynomial<MRGTreeType> {
public:
  typedef MRGTreeType TreeType;
  typedef QuadraticPolynomial<TreeType> BaseType;
  typedef MRG MyType;
  typedef arma::vec StateType;
  typedef NumericTraitData<TreeType::NodeType> DataType;
  typedef std::vector<double> ParameterType;
  typedef splittree::PostOrderTraversal<MyType> AlgorithmType;
  
  MRG(TreeType const& tree, DataType const& input_data):
    BaseType(tree, input_data) {
    
    for(uint i = 0; i < input_data.regime_models_.size(); i++) {
      std::string modelName = input_data.regime_models_[i];
      if(modelName == "BM") {
        BaseType::ptr_cond_dist_.push_back(new CondGaussianBM<TreeType, DataType>(tree, input_data, 1));  
      } else if(modelName == "OU") {
        BaseType::ptr_cond_dist_.push_back(new CondGaussianOU<TreeType, DataType>(tree, input_data, 1));  
      } else if(modelName == "JOU") {
        BaseType::ptr_cond_dist_.push_back(new CondGaussianJOU<TreeType, DataType>(tree, input_data, 1));  
      } else if(modelName == "TwoSpeedOU") {
        BaseType::ptr_cond_dist_.push_back(new CondGaussianTwoSpeedOU<TreeType, DataType>(tree, input_data, 1));  
      } 
    }
    
  }
  
  void SetParameter(ParameterType const& par) {
    uint offset = 0;
    for(uint i = 0; i < ptr_cond_dist_.size(); i++) {
      offset += ptr_cond_dist_[i]->SetParameter(par, offset);
    }
  }
  
  ~MRG() {
    for(uint i = 0; i < BaseType::ptr_cond_dist_.size(); i++) {
      delete BaseType::ptr_cond_dist_[i];
    }
  }
};


typedef splittree::TraversalTask<MRG> QuadraticPolynomialMRG;
}

#endif // QuadraticPolynomial_MRG_H_
