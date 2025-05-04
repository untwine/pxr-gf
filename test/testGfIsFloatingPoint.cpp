//
// Copyright 2026 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include <pxr/gf/pxr.h>
#include <pxr/gf/traits.h>
#include <pxr/gf/half.h>
#include <pxr/tf/type.h>
#include <pxr/tf/diagnostic.h>

#include <string>

GF_NAMESPACE_USING_DIRECTIVE

int
main(int argc, char *argv[])
{
    // Test TfType overload -- floating point types return true.
    TF_AXIOM(GfIsFloatingPointType(TfType::Find<double>()));
    TF_AXIOM(GfIsFloatingPointType(TfType::Find<float>()));
    TF_AXIOM(GfIsFloatingPointType(TfType::Find<GfHalf>()));

    // Test TfType overload -- non-floating-point types return false.
    TF_AXIOM(!GfIsFloatingPointType(TfType::Find<int>()));
    TF_AXIOM(!GfIsFloatingPointType(TfType::Find<std::string>()));
    TF_AXIOM(!GfIsFloatingPointType(TfType::Find<bool>()));
    TF_AXIOM(!GfIsFloatingPointType(TfType()));

    // Test type_info overload -- floating point types return true.
    TF_AXIOM(GfIsFloatingPointType(typeid(double)));
    TF_AXIOM(GfIsFloatingPointType(typeid(float)));
    TF_AXIOM(GfIsFloatingPointType(typeid(GfHalf)));

    // Test type_info overload -- non-floating-point types return false.
    TF_AXIOM(!GfIsFloatingPointType(typeid(int)));
    TF_AXIOM(!GfIsFloatingPointType(typeid(std::string)));
    TF_AXIOM(!GfIsFloatingPointType(typeid(bool)));

    printf("OK\n");

    return 0;
}
