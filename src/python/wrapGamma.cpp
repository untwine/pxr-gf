// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/boost/python/def.hpp>

#include <pxr/gf/gamma.h>
#include <pxr/gf/vec3f.h>
#include <pxr/gf/vec3d.h>
#include <pxr/gf/vec4f.h>
#include <pxr/gf/vec4d.h>

#include <pxr/tf/wrapTypeHelpers.h>

using namespace pxr;

using namespace pxr::boost::python;

void wrapGamma()
{    
    def("ApplyGamma", (GfVec3f(*)(GfVec3f const &,double))GfApplyGamma);
    def("ApplyGamma", (GfVec3d(*)(GfVec3d const &,double))GfApplyGamma);
    def("ApplyGamma", (GfVec3h(*)(GfVec3h const &,double))GfApplyGamma);
    def("ApplyGamma", (GfVec4f(*)(GfVec4f const &,double))GfApplyGamma);
    def("ApplyGamma", (GfVec4d(*)(GfVec4d const &,double))GfApplyGamma);
    def("ApplyGamma", (GfVec4h(*)(GfVec4h const &,double))GfApplyGamma);
    def("GetDisplayGamma",GfGetDisplayGamma);
    def("ConvertDisplayToLinear",(GfVec3f(*)(GfVec3f const &))GfConvertDisplayToLinear);
    def("ConvertDisplayToLinear",(GfVec3d(*)(GfVec3d const &))GfConvertDisplayToLinear);
    def("ConvertDisplayToLinear",(GfVec3h(*)(GfVec3h const &))GfConvertDisplayToLinear);
    def("ConvertDisplayToLinear",(GfVec4f(*)(GfVec4f const &))GfConvertDisplayToLinear);
    def("ConvertDisplayToLinear",(GfVec4d(*)(GfVec4d const &))GfConvertDisplayToLinear);
    def("ConvertDisplayToLinear",(GfVec4h(*)(GfVec4h const &))GfConvertDisplayToLinear);
    def("ConvertLinearToDisplay",(GfVec3f(*)(GfVec3f const &))GfConvertLinearToDisplay);
    def("ConvertLinearToDisplay",(GfVec3d(*)(GfVec3d const &))GfConvertLinearToDisplay);
    def("ConvertLinearToDisplay",(GfVec3h(*)(GfVec3h const &))GfConvertLinearToDisplay);
    def("ConvertLinearToDisplay",(GfVec4f(*)(GfVec4f const &))GfConvertLinearToDisplay);
    def("ConvertLinearToDisplay",(GfVec4d(*)(GfVec4d const &))GfConvertLinearToDisplay);
    def("ConvertLinearToDisplay",(GfVec4h(*)(GfVec4h const &))GfConvertLinearToDisplay);
}
