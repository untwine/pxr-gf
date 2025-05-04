//
// Copyright 2026 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include <pxr/gf/pxr.h>
#include <pxr/gf/timeCode.h>
#include <pxr/tf/registryManager.h>
#include <pxr/tf/type.h>

#include <ostream>

GF_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION(TfType) {
    TfType::Define<GfTimeCode>();
}

std::ostream&
operator<<(std::ostream& out, const GfTimeCode& tc)
{
    return out << tc.GetValue();
}

GF_NAMESPACE_CLOSE_SCOPE
