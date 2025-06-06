// Copyright 2021 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// wrapDualQuat.template.cpp file to make changes.

#include <pxr/gf/dualQuatd.h>
#include <pxr/gf/dualQuatf.h>
#include <pxr/gf/dualQuath.h>

#include <pxr/tf/hash.h>
#include <pxr/tf/pyUtils.h>
#include <pxr/tf/wrapTypeHelpers.h>
#include <pxr/tf/pyContainerConversions.h>

#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/copy_const_reference.hpp>
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/implicit.hpp>
#include <pxr/boost/python/make_constructor.hpp>
#include <pxr/boost/python/operators.hpp>
#include <pxr/boost/python/overloads.hpp>
#include <pxr/boost/python/return_arg.hpp>

#include <string>

using std::string;

using namespace pxr;

using namespace pxr::boost::python;

namespace {

static string __repr__(GfDualQuatd const &self) {
    return TF_PY_REPR_PREFIX + "DualQuatd(" +
        TfPyRepr(self.GetReal()) + ", " +
        TfPyRepr(self.GetDual()) + ")";
}

static GfDualQuatd __truediv__(const GfDualQuatd &self, double value)
{
    return self / value;
}

static GfDualQuatd& __itruediv__(GfDualQuatd &self, double value)
{
    return self /= value;
}

static size_t __hash__(GfDualQuatd const &self) {
    return TfHash{}(self);
}

// Zero-initialized default ctor for python.
static GfDualQuatd *__init__() {
    return new GfDualQuatd(GfQuatd(0), GfQuatd(0));
}

} // anonymous namespace

void wrapDualQuatd()
{
    object getReal =
        make_function(&GfDualQuatd::GetReal,
                      return_value_policy<return_by_value>());
    object setReal =
        make_function((void (GfDualQuatd::*)(const GfQuatd &))
                      &GfDualQuatd::SetReal);

    object getDual =
        make_function(&GfDualQuatd::GetDual,
                      return_value_policy<return_by_value>());
    object setDual =
        make_function((void (GfDualQuatd::*)(const GfQuatd &))
                      &GfDualQuatd::SetDual);

    def( "Dot",
         (double (*)(const GfDualQuatd &, const GfDualQuatd &))
         GfDot);

    class_<GfDualQuatd> cls("DualQuatd", no_init);
    cls
        .def("__init__", make_constructor(__init__))

        .def( TfTypePythonClass() )

        .def(init<GfDualQuatd>())
        .def(init<double>(arg("realVal")))
        .def(init< const GfQuatd & >(arg("real")))
        .def(init< const GfQuatd &, const GfQuatd & >(
                 (arg("real"), arg("dual"))))
        .def(init< const GfQuatd &, const GfVec3d & >(
                 (arg("rotation"), arg("translation"))))

        .def("GetZero", &GfDualQuatd::GetZero)
        .staticmethod("GetZero")

        .def("GetIdentity", &GfDualQuatd::GetIdentity)
        .staticmethod("GetIdentity")

        .def("GetReal", getReal)
        .def("SetReal", setReal)
        .add_property("real", getReal, setReal)

        .def("GetDual", getDual)
        .def("SetDual", setDual)
        .add_property("dual", getDual, setDual)

        .def("GetLength", &GfDualQuatd::GetLength)

        .def("GetNormalized", &GfDualQuatd::GetNormalized,
             (arg("eps")=GF_MIN_VECTOR_LENGTH))
        .def("Normalize", &GfDualQuatd::Normalize,
             (arg("eps")=GF_MIN_VECTOR_LENGTH), return_self<>())

        .def("GetConjugate", &GfDualQuatd::GetConjugate)
        .def("GetInverse", &GfDualQuatd::GetInverse)
        .def("SetTranslation",
             (void (GfDualQuatd::*)(const GfVec3d &))
             &GfDualQuatd::SetTranslation)
        .def("GetTranslation", &GfDualQuatd::GetTranslation)

        .def("Transform",
             (GfVec3d (GfDualQuatd::*)(const GfVec3d &) const)
             &GfDualQuatd::Transform)

        .def( str(self) )
        .def( self == self )
        .def( self != self )
        .def( self += self )
        .def( self -= self )
        .def( self *= self )
        .def( self *= double() )
        .def( self /= double() )
        .def( self + self )
        .def( self - self )
        .def( self * self )
        .def( self * double() )
        .def( double() * self )
        .def( self / double() )

        .def("__repr__", __repr__)
        .def("__hash__", __hash__)
        ;

    implicitly_convertible<GfDualQuatf, GfDualQuatd>();
    implicitly_convertible<GfDualQuath, GfDualQuatd>();

    to_python_converter<std::vector<GfDualQuatd>,
        TfPySequenceToPython<std::vector<GfDualQuatd> > >();

    if (!PyObject_HasAttrString(cls.ptr(), "__truediv__")) {
        // __truediv__ not added by .def( self / double() ) above, which
        // happens when building with python 2, but we need it to support
        // "from __future__ import division"
        cls.def("__truediv__", __truediv__);
    }
    if (!PyObject_HasAttrString(cls.ptr(), "__itruediv__")) {
        // __itruediv__ not added by .def( self /= double() ) above, which
        // happens when building with python 2, but we need it to support
        // "from __future__ import division". This is also a workaround for a 
        // bug in the current version of pxr::boost::python that incorrectly wraps
        // in-place division with __idiv__ when building with python 3.
        cls.def("__itruediv__", __itruediv__, return_self<>());
    }
}
