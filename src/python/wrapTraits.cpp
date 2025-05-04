//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include <pxr/gf/pxr.h>
#include <pxr/gf/traits.h>
#include <pxr/tf/type.h>

#include <pxr/boost/python/def.hpp>

GF_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

void wrapTraits()
{
    def("IsFloatingPointType",
        (bool (*)(const TfType&))GfIsFloatingPointType,
        arg("type"),
        "Return True if the TfType represents a floating-point type "
        "(double, float, or GfHalf).");
}
