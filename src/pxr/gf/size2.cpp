// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include "./size2.h"
#include <pxr/tf/type.h>

#include <ostream>

namespace pxr {

TF_REGISTRY_FUNCTION(pxr::TfType) {
    TfType::Define<GfSize2>();
}


std::ostream &operator<<(std::ostream &o, GfSize2 const &v) {
    return o << "( " << v._vec[0] << " " << v._vec[1] << " )";
}

}  // namespace pxr
