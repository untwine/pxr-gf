// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/gf/color.h>
#include <pxr/gf/colorSpace.h>
#include <pxr/tf/pyStaticTokens.h>
#include <pxr/tf/pyUtils.h>
#include <pxr/tf/stringUtils.h>

#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/operators.hpp>
#include <string>

using namespace pxr;

using namespace pxr::boost::python;

namespace {

static std::string __repr__(GfColorSpace const &self)
{
    return TF_PY_REPR_PREFIX + 
        TfStringPrintf("ColorSpace(%s)", TfPyRepr(self.GetName()).c_str());
}

pxr::boost::python::tuple ConvertPrimariesAndWhitePoint(const GfColorSpace& self) {
    auto result = self.GetPrimariesAndWhitePoint();
    return pxr::boost::python::make_tuple(
        std::get<0>(result),
        std::get<1>(result),
        std::get<2>(result),
        std::get<3>(result)
    );
}

} // anon

void wrapColorSpace()
{
    class_<GfColorSpace>("ColorSpace", init<const TfToken&>())
        .def(init<const TfToken&, const GfVec2f&, const GfVec2f&, 
		  const GfVec2f&, const GfVec2f&, float, float>(
	     (arg("name"), arg("redChroma"), arg("greenChroma"), 
              arg("blueChroma"), arg("whitePoint"), arg("gamma"), 
              arg("linearBias"))))
        .def(init<const TfToken&, const GfMatrix3f&, float, float>(
	     (arg("name"), arg("rgbToXYZ"), arg("gamma"), arg("linearBias"))))
        .def("__repr__", &__repr__)
        .def("GetName", &GfColorSpace::GetName)
        .def("ConvertRGBSpan", &GfColorSpace::ConvertRGBSpan)
        .def("ConvertRGBASpan", &GfColorSpace::ConvertRGBASpan)
        .def("Convert", &GfColorSpace::Convert)
        .def("GetRGBToXYZ", &GfColorSpace::GetRGBToXYZ)
        .def("GetGamma", &GfColorSpace::GetGamma)
        .def("GetLinearBias", &GfColorSpace::GetLinearBias)
        .def("GetTransferFunctionParams", &GfColorSpace::GetTransferFunctionParams)
        .def("GetPrimariesAndWhitePoint", &ConvertPrimariesAndWhitePoint)
        .def("IsValid", &GfColorSpace::IsValid)
        .def(self == self)
        .def(self != self);

    TF_PY_WRAP_PUBLIC_TOKENS("ColorSpaceNames", GfColorSpaceNames, 
                             GF_COLORSPACE_NAME_TOKENS);
}
