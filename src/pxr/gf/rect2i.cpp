// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include "./rect2i.h"

#include "./ostreamHelpers.h"

#include <pxr/tf/type.h>

#include <ostream>

namespace pxr {

// CODE_COVERAGE_OFF_GCOV_BUG
TF_REGISTRY_FUNCTION(pxr::TfType) {
    TfType::Define<GfRect2i>();
}
// CODE_COVERAGE_ON_GCOV_BUG

GfRect2i
GfRect2i::GetNormalized() const
{
    GfVec2i min, max;

    if (_max[0] < _min[0]) {
        min[0] = _max[0];
        max[0] = _min[0];
    }
    else {
        min[0] = _min[0];
        max[0] = _max[0];
    }

    if (_max[1] < _min[1]) {
        min[1] = _max[1];
        max[1] = _min[1];
    }
    else {
        min[1] = _min[1];
        max[1] = _max[1];
    }

    return GfRect2i(min, max);
}

std::ostream &
operator<<(std::ostream& out, const GfRect2i& r)
{
    return out << '[' << Gf_OstreamHelperP(r.GetMin()) << ":" 
        << Gf_OstreamHelperP(r.GetMax()) << ']';
}

}  // namespace pxr
