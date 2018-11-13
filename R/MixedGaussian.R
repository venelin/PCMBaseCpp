# Copyright 2018 Venelin Mitov
#
# This file is part of PCMBaseCpp.
#
# PCMBase is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PCMBase is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with PCMBase.  If not, see <http://www.gnu.org/licenses/>.

#'@export
PCMInfoCpp.MixedGaussian <- function(X, tree, model,
                           metaI = PCMInfo(X, tree, model, verbose), 
                           #metaI = PCMInfo(X, tree, model, verbose, preorder=PCMTreePreorderCpp(tree)), 
                           verbose = FALSE, ...) {
  
  if(is.Transformable(model)) {
    model <- PCMApplyTransformation(model)
  }
  
  spec <- attr(model, "spec", exact = TRUE)
  regimeModel <- unlist(sapply(spec, function(param) {
    if(is.PCM(param)) {
      if(startsWith(class(param)[1], "BM")) {
        "BM"
      } else if(startsWith(class(param)[1], "JOU")) {
        "JOU"
      } else if(startsWith(class(param)[1], "DOU")) {
        "DOU"
      } else if(startsWith(class(param)[1], "OU")) {
        "OU"
      } else 
        stop(paste0("ERR:03501:PCMBaseCpp:MixedGaussian.R:PCMInfoCpp.MixedGaussian:: Uknown model class ", class(param)[1]))
    } else {
      NULL
    }
  }))
  regimeModel <- regimeModel[!sapply(regimeModel, is.null)]
  
  metaI$pcListInt <- PCListInt(metaI$pc)
  
  res <- c(metaI, 
           cppObject = PCMBaseCpp__QuadraticPolynomialMixedGaussian$new(
             X, tree, model, metaI, 
             regimeModel))
  
  res$TraverseTree = res$cppObject$TraverseTree
  
  class(res) <- c("PCMInfoCpp", class(metaI))
  res
}
