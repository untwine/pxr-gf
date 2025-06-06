// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

///
/// \file gf/wrapCamera.h

#include <pxr/gf/camera.h>
#include <pxr/gf/frustum.h>
#include <pxr/tf/pyEnum.h>

#include <pxr/boost/python/operators.hpp>

#include <vector>

using namespace pxr;

using namespace pxr::boost::python;

namespace {

static float
_GetHorizontalFieldOfView(const GfCamera &camera) {
    return camera.GetFieldOfView(GfCamera::FOVHorizontal);
}

static float
_GetVerticalFieldOfView(const GfCamera &camera) {
    return camera.GetFieldOfView(GfCamera::FOVVertical);
}

// Required because CamCamera::GetClippingPlane returns const &std::vector
// and add_property does not allow one to specify a return_value policy.
static std::vector<GfVec4f>
_GetClippingPlanes(const GfCamera &camera) {
    return camera.GetClippingPlanes();
}

static std::string _Repr(GfCamera const &self)
{
    const std::string prefix = TF_PY_REPR_PREFIX + "Camera(";
    const std::string indent(prefix.size(), ' ');
    const std::string seperator = ",\n" + indent;
    
    // Use keyword args for clarity.
    // Only supply some arguments when different from default.
    std::vector<std::string> kwargs;
    if (self.GetTransform() != GfMatrix4d(1.0))
        kwargs.push_back("transform = " + TfPyRepr(self.GetTransform()));

    kwargs.push_back("projection = " + TfPyRepr(self.GetProjection()));
    kwargs.push_back("horizontalAperture = " +
                     TfPyRepr(self.GetHorizontalAperture()));
    kwargs.push_back("verticalAperture = " +
                     TfPyRepr(self.GetVerticalAperture()));
    if (self.GetHorizontalApertureOffset() != 0.0)
        kwargs.push_back("horizontalApertureOffset = " +
                         TfPyRepr(self.GetHorizontalApertureOffset()));
    if (self.GetVerticalApertureOffset() != 0.0)
        kwargs.push_back("verticalApertureOffset = " +
                         TfPyRepr(self.GetVerticalApertureOffset()));
    kwargs.push_back("focalLength = " +
                     TfPyRepr(self.GetFocalLength()));
    if (self.GetClippingRange() != GfRange1f(1, 1000000))
        kwargs.push_back("clippingRange = " +
                         TfPyRepr(self.GetClippingRange()));
    if (!self.GetClippingPlanes().empty())
        kwargs.push_back("clippingPlanes = " +
                         TfPyRepr(self.GetClippingPlanes()));
    if (self.GetFStop() != 0.0)
        kwargs.push_back("fStop = " + TfPyRepr(self.GetFStop()));
    if (self.GetFocusDistance() != 0.0)
        kwargs.push_back("focusDistance = " +
                         TfPyRepr(self.GetFocusDistance()));

    return prefix + TfStringJoin(kwargs, seperator.c_str()) + ")";
}

} // anonymous namespace 

void
wrapCamera()
{
    typedef GfCamera This;

    class_<This> c("Camera");

    scope s(c);

    TfPyWrapEnum<GfCamera::Projection>("Camera.Projection");
    TfPyWrapEnum<GfCamera::FOVDirection>("Camera.FOVDirection");

    c   .def(init<const This &>())
        .def(init<const GfMatrix4d &, GfCamera::Projection,
                  float, float, float, float, float,
                  const GfRange1f &, const std::vector<GfVec4f> &,
                  float, float>
             ((args("transform") = GfMatrix4d(1.0),
               args("projection") = GfCamera::Perspective,
               args("horizontalAperture") =
                                          GfCamera::DEFAULT_HORIZONTAL_APERTURE,
               args("verticalAperture") = GfCamera::DEFAULT_VERTICAL_APERTURE,
               args("horizontalApertureOffset") = 0.0,
               args("verticalApertureOffset") = 0.0,
               args("focalLength") = 50.0,
               args("clippingRange") = GfRange1f(1, 1000000),
               args("clippingPlanes") = std::vector<GfVec4f>(),
               args("fStop") = 0.0,
               args("focusDistance") = 0.0)))
        .add_property("transform",
                      &This::GetTransform,
                      &This::SetTransform)
        .add_property("projection",
                      &This::GetProjection,
                      &This::SetProjection)
        .add_property("horizontalAperture",
                      &This::GetHorizontalAperture,
                      &This::SetHorizontalAperture)
        .add_property("verticalAperture",
                      &This::GetVerticalAperture,
                      &This::SetVerticalAperture)
        .add_property("horizontalApertureOffset",
                      &This::GetHorizontalApertureOffset,
                      &This::SetHorizontalApertureOffset)
        .add_property("verticalApertureOffset",
                      &This::GetVerticalApertureOffset,
                      &This::SetVerticalApertureOffset)
        .add_property("aspectRatio",
                      &This::GetAspectRatio)
        .add_property("focalLength",
                      &This::GetFocalLength,
                      &This::SetFocalLength)
        .add_property("clippingRange",
                      &This::GetClippingRange,
                      &This::SetClippingRange)
        .add_property("clippingPlanes",
                      &_GetClippingPlanes,
                      &This::SetClippingPlanes)
        .add_property("frustum",
                      &This::GetFrustum)
        .add_property("fStop",
                      &This::GetFStop,
                      &This::SetFStop)
        .add_property("focusDistance",
                      &This::GetFocusDistance,
                      &This::SetFocusDistance)
        .add_property("horizontalFieldOfView",
                      &_GetHorizontalFieldOfView)
        .add_property("verticalFieldOfView",
                      &_GetVerticalFieldOfView)
        .def("GetFieldOfView",
                      &This::GetFieldOfView)
        .def("SetPerspectiveFromAspectRatioAndFieldOfView",
                      &This::SetPerspectiveFromAspectRatioAndFieldOfView,
             ( arg("aspectRatio"),
               arg("fieldOfView"),
               arg("direction"),
               arg("horizontalAperture") = This::DEFAULT_HORIZONTAL_APERTURE) )
        .def("SetOrthographicFromAspectRatioAndSize",
                      &This::SetOrthographicFromAspectRatioAndSize,
             ( arg("aspectRatio"),
               arg("orthographicSize"),
               arg("direction")))
        .def("SetFromViewAndProjectionMatrix",
                      &This::SetFromViewAndProjectionMatrix,
             ( arg("viewMatrix"),
               arg("projMatrix"),
               arg("focalLength") = 50))
        .setattr("APERTURE_UNIT",
            This::APERTURE_UNIT)
        .setattr("FOCAL_LENGTH_UNIT",
            This::FOCAL_LENGTH_UNIT)
        .setattr("DEFAULT_HORIZONTAL_APERTURE",
            This::DEFAULT_HORIZONTAL_APERTURE)
        .setattr("DEFAULT_VERTICAL_APERTURE",
            This::DEFAULT_VERTICAL_APERTURE)

        .def(self == self)
        .def(self != self)

        .def("__repr__", _Repr)
            ;
}
