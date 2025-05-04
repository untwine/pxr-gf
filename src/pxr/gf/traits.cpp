//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include <pxr/gf/pxr.h>
#include <pxr/gf/traits.h>
#include <pxr/gf/half.h>
#include <pxr/tf/type.h>

GF_NAMESPACE_OPEN_SCOPE

bool
GfIsFloatingPointType(const TfType& type)
{
    static const TfType doubleType = TfType::Find<double>();
    static const TfType floatType = TfType::Find<float>();
    static const TfType halfType = TfType::Find<GfHalf>();
    return type == doubleType || type == floatType || type == halfType;
}

bool
GfIsFloatingPointType(const std::type_info& typeInfo)
{
    return typeInfo == typeid(double) || typeInfo == typeid(float) ||
           typeInfo == typeid(GfHalf);
}

GF_NAMESPACE_CLOSE_SCOPE
