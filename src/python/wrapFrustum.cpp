// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/gf/frustum.h>
#include <pxr/gf/ray.h>

#include <pxr/tf/pyEnum.h>
#include <pxr/tf/pyResultConversions.h>
#include <pxr/tf/wrapTypeHelpers.h>

#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/copy_const_reference.hpp>
#include <pxr/boost/python/enum.hpp>
#include <pxr/boost/python/operators.hpp>
#include <pxr/boost/python/overloads.hpp>
#include <pxr/boost/python/return_arg.hpp>
#include <pxr/boost/python/tuple.hpp>

using namespace pxr;

using namespace pxr::boost::python;

namespace {

static std::string _Repr(GfFrustum const &self)
{
    const std::string prefix = TF_PY_REPR_PREFIX + "Frustum(";
    const std::string indent(prefix.size(), ' ');
    const std::string seperator = ",\n" + indent;

    // Use keyword args for clarity.
    // Only use viewDistance if not matching default
    std::vector<std::string> kwargs;
    kwargs.push_back( "position = " + TfPyRepr(self.GetPosition()));
    kwargs.push_back( "rotation = " + TfPyRepr(self.GetRotation()));
    kwargs.push_back( "window = " + TfPyRepr(self.GetWindow()));
    kwargs.push_back( "nearFar = " + TfPyRepr(self.GetNearFar()));
    kwargs.push_back( "projectionType = " + TfPyRepr(self.GetProjectionType()));
    if (self.GetViewDistance() != 5.0) {
        kwargs.push_back( "viewDistance = " + TfPyRepr(self.GetViewDistance()));
    }
    return prefix + TfStringJoin(kwargs, seperator.c_str()) + ")";
}

static size_t __hash__(GfFrustum const &self) {
    return TfHash()(self);
}

static object
GetPerspectiveHelper( const GfFrustum &self, bool isFovVertical ) {
    double fov, aspect, nearDist, farDist;
    bool result = self.GetPerspective( isFovVertical, 
                                       &fov, &aspect, &nearDist, &farDist );
    return result ?
        pxr::boost::python::make_tuple( fov, aspect, nearDist, farDist ) : object();
}

static tuple
GetOrthographicHelper( const GfFrustum &self ) {
    double left, right, bottom, top, near, far;
    bool result =
        self.GetOrthographic( &left, &right, &bottom, &top, &near, &far );
    return result ?
        pxr::boost::python::
        make_tuple( left, right, bottom, top, near, far ) : tuple();
}

static tuple
ComputeViewFrameHelper( const GfFrustum &self ) {
    GfVec3d side, up, view;
    self.ComputeViewFrame( &side, &up, &view );
    return pxr::boost::python::make_tuple( side, up, view );
}


PXR_BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( FitToSphere_overloads,
                                        FitToSphere, 2, 3 );

} // anonymous namespace 

void wrapFrustum()
{    
    typedef GfFrustum This;

    // Create some objects that will be reused a few times.
    //
    object getPositionFunc =
        make_function(&This::GetPosition,
                      return_value_policy<copy_const_reference>());
    object getRotationFunc =
        make_function(&This::GetRotation,
                      return_value_policy<copy_const_reference>());
    object getWindowFunc =
        make_function(&This::GetWindow,
                      return_value_policy<copy_const_reference>());
    object getNearFarFunc =
        make_function(&This::GetNearFar,
                      return_value_policy<copy_const_reference>());

    scope thisScope =
        class_<This>("Frustum", "Basic view frustum", init<>())
        .def(init< const This & >())
        .def(init<const GfVec3d &, const GfRotation &,
                  const GfRange2d &, const GfRange1d &,
                  GfFrustum::ProjectionType, double>
             ((args("position"), args("rotation"),
               args("window"), args("nearFar"),
               args("projectionType"), args("viewDistance") = 5.0)))

        .def(init<const GfMatrix4d &,
                  const GfRange2d &, const GfRange1d &,
                  GfFrustum::ProjectionType, double>
             ((args("camToWorldXf"),
               args("window"), args("nearFar"),
               args("projectionType"), args("viewDistance") = 5.0)))

        .def( TfTypePythonClass() )
        
        .add_property( "position", getPositionFunc, &This::SetPosition )
        .add_property( "rotation", getRotationFunc, &This::SetRotation )
        .add_property( "window",   getWindowFunc,   &This::SetWindow )
        .add_property( "nearFar",  getNearFarFunc,  &This::SetNearFar )
        .add_property( "viewDistance", &This::GetViewDistance,
                       &This::SetViewDistance )
        .add_property( "projectionType", &This::GetProjectionType,
                       &This::SetProjectionType )

        .def("SetPerspective",
             (void (This::*)(double, double, double, double))&This::SetPerspective,
             (args("fovHeight"), "aspectRatio", "nearDist", "farDist"))
        .def("SetPerspective",
             (void (This::*)(double, bool, double, double, double))&This::SetPerspective,
             (args("fov"), "isFovVertical", "aspectRatio", "nearDist", "farDist"))

        .def("GetPerspective", GetPerspectiveHelper,
             (args("isFovVertical") = true),
             "Returns the current perspective frustum values suitable\n"
             "for use by SetPerspective.  If the current frustum is a\n"
             "perspective projection, the return value is a tuple of\n"
             "fieldOfView, aspectRatio, nearDistance, farDistance).\n"
             "If the current frustum is not perspective, the return\n"
             "value is None.")

        .def("GetFOV", &This::GetFOV,
             (args("isFovVertical") = false),
             "Returns the horizontal fov of the frustum. The fov of the\n"
             "frustum is not necessarily the same value as displayed in\n"
             "the viewer. The displayed fov is a function of the focal\n"
             "length or FOV avar. The frustum's fov may be different due\n"
             "to things like lens breathing.\n"
             "\n"
             "If the frustum is not of type GfFrustum::Perspective, the\n"
             "returned FOV will be 0.0.")

        .def("SetOrthographic", &This::SetOrthographic)
        .def("GetOrthographic", GetOrthographicHelper)

        .def("GetPosition", getPositionFunc)
        .def("SetPosition", &This::SetPosition)

        .def("GetRotation", getRotationFunc)
        .def("SetRotation", &This::SetRotation)

        .def("SetPositionAndRotationFromMatrix",
             &This::SetPositionAndRotationFromMatrix, (args("camToWorldXf")))

        .def("GetWindow", getWindowFunc)
        .def("SetWindow", &This::SetWindow)

        .def("GetNearFar", getNearFarFunc)
        .def("SetNearFar", &This::SetNearFar)

        .def("GetViewDistance", &This::GetViewDistance)
        .def("SetViewDistance", &This::SetViewDistance)

        .def("FitToSphere", &This::FitToSphere, FitToSphere_overloads())

        .def("Transform", &This::Transform, return_self<>())

        .def("ComputeViewDirection", &This::ComputeViewDirection )
        .def("ComputeUpVector", &This::ComputeUpVector )

        .def("ComputeViewFrame", ComputeViewFrameHelper )

        .def("ComputeLookAtPoint", &This::ComputeLookAtPoint)

        .def("ComputeViewMatrix", &This::ComputeViewMatrix)

        .def("ComputeViewInverse", &This::ComputeViewInverse)

        .def("ComputeProjectionMatrix", &This::ComputeProjectionMatrix)

        .def("ComputeAspectRatio", &This::ComputeAspectRatio)

        .def("ComputeCorners", &This::ComputeCorners,
             return_value_policy<TfPySequenceToTuple>())

        .def("ComputeCornersAtDistance", &This::ComputeCornersAtDistance,
             return_value_policy<TfPySequenceToTuple>())

        .def("ComputeNarrowedFrustum", 
            (GfFrustum (This::*)(const GfVec2d &, const GfVec2d &) const)
            &This::ComputeNarrowedFrustum)
        .def("ComputeNarrowedFrustum", 
            (GfFrustum (This::*)(const GfVec3d &, const GfVec2d &) const)
            &This::ComputeNarrowedFrustum)

        .def("ComputePickRay", (GfRay (This::*)(const GfVec2d &) const)
            &This::ComputePickRay)
        .def("ComputePickRay", (GfRay (This::*)(const GfVec3d &) const)
            &This::ComputePickRay)

        .def("Intersects", (bool (This::*)(const GfBBox3d &) const)
             &This::Intersects)
        .def("Intersects", (bool (This::*)(const GfVec3d &) const)
             &This::Intersects)
        .def("Intersects",
             (bool (This::*)(const GfVec3d &,
                             const GfVec3d &) const)
             &This::Intersects)
        .def("Intersects",
             (bool (This::*)(const GfVec3d &,
                             const GfVec3d &,
                             const GfVec3d &) const)
             &This::Intersects)

        .def("SetProjectionType", &This::SetProjectionType)
        .def("GetProjectionType", &This::GetProjectionType)

        .def("GetReferencePlaneDepth", 
                &This::GetReferencePlaneDepth)
        .staticmethod("GetReferencePlaneDepth") 

        .def("IntersectsViewVolume", &This::IntersectsViewVolume)
        .staticmethod("IntersectsViewVolume")
        
        .def(str(self))
        .def(self == self)
        .def(self != self)

        .def("__repr__", _Repr)
        .def("__hash__", __hash__)
        ;

    TfPyWrapEnum<This::ProjectionType>("Frustum.ProjectionType");
}
