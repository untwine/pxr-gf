// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// matrix3.template.cpp file to make changes.

{% extends "matrix.template.cpp" %}

{% block customIncludes %}
#include "./quat{{ SCL[0] }}.h"
#include "./rotation.h"
{% endblock customIncludes %}

{% block customConstructors %}
{{ MAT }}::{{ MAT }}(const GfRotation &rot)
{
    SetRotate(rot);
}

{{ MAT }}::{{ MAT }}(const GfQuat{{ SCL[0] }} &rot)
{
    SetRotate(rot);
}
{% endblock customConstructors %}

{% block customFunctions %}
{{ MAT }}
{{ MAT }}::GetInverse(double *detPtr, double eps) const
{
    double a00,a01,a02,a10,a11,a12,a20,a21,a22;
    double det, rcp;

    a00 = _mtx[0][0];
    a01 = _mtx[0][1];
    a02 = _mtx[0][2];
    a10 = _mtx[1][0];
    a11 = _mtx[1][1];
    a12 = _mtx[1][2];
    a20 = _mtx[2][0];
    a21 = _mtx[2][1];
    a22 = _mtx[2][2];
    det = -(a02*a11*a20) + a01*a12*a20 + a02*a10*a21 - 
	  a00*a12*a21 - a01*a10*a22 + a00*a11*a22;

    if (detPtr) {
	*detPtr = det;
    }

    {{ MAT }} inverse;

    if (GfAbs(det) > eps) {
	rcp = 1.0 / det;

{%- macro SCALAR_CAST(t) %}
{%- if SCL == 'float' %}
static_cast<float>({{ t }})
{%- else %}
{{ t }}
{%- endif %}
{% endmacro %}

        inverse._mtx[0][0] = {{ SCALAR_CAST("(-(a12*a21) + a11*a22)*rcp") }};
        inverse._mtx[0][1] = {{ SCALAR_CAST("(a02*a21 - a01*a22)*rcp") }};
        inverse._mtx[0][2] = {{ SCALAR_CAST("(-(a02*a11) + a01*a12)*rcp") }};
        inverse._mtx[1][0] = {{ SCALAR_CAST("(a12*a20 - a10*a22)*rcp") }};
        inverse._mtx[1][1] = {{ SCALAR_CAST("(-(a02*a20) + a00*a22)*rcp") }};
        inverse._mtx[1][2] = {{ SCALAR_CAST("(a02*a10 - a00*a12)*rcp") }};
        inverse._mtx[2][0] = {{ SCALAR_CAST("(-(a11*a20) + a10*a21)*rcp") }};
        inverse._mtx[2][1] = {{ SCALAR_CAST("(a01*a20 - a00*a21)*rcp") }};
        inverse._mtx[2][2] = {{ SCALAR_CAST("(-(a01*a10) + a00*a11)*rcp") }};

    }
    else {
       	inverse.SetScale(FLT_MAX);
    }

    return inverse;
}

double
{{ MAT }}::GetDeterminant() const
{
    return (_mtx[0][0] * _mtx[1][1] * _mtx[2][2] +
	    _mtx[0][1] * _mtx[1][2] * _mtx[2][0] +
	    _mtx[0][2] * _mtx[1][0] * _mtx[2][1] -
	    _mtx[0][0] * _mtx[1][2] * _mtx[2][1] -
	    _mtx[0][1] * _mtx[1][0] * _mtx[2][2] -
	    _mtx[0][2] * _mtx[1][1] * _mtx[2][0]);
}

double
{{ MAT }}::GetHandedness() const
{
    // Note: This can be computed with fewer arithmetic operations using a
    //       cross and dot product, but it is more important that the result
    //       is consistent with the way the determinant is computed.
    return GfSgn(GetDeterminant());
}

/* Make the matrix orthonormal in place using an iterative method.
 * It is potentially slower if the matrix is far from orthonormal (i.e. if
 * the row basis vectors are close to colinear) but in the common case
 * of near-orthonormality it should be just as fast. */
bool
{{ MAT }}::Orthonormalize(bool issueWarning)
{
    // orthogonalize and normalize row vectors
    GfVec3d r0(_mtx[0][0],_mtx[0][1],_mtx[0][2]);
    GfVec3d r1(_mtx[1][0],_mtx[1][1],_mtx[1][2]);
    GfVec3d r2(_mtx[2][0],_mtx[2][1],_mtx[2][2]);
    bool result = GfVec3d::OrthogonalizeBasis(&r0, &r1, &r2, true);
    _mtx[0][0] = r0[0];
    _mtx[0][1] = r0[1];
    _mtx[0][2] = r0[2];
    _mtx[1][0] = r1[0];
    _mtx[1][1] = r1[1];
    _mtx[1][2] = r1[2];
    _mtx[2][0] = r2[0];
    _mtx[2][1] = r2[1];
    _mtx[2][2] = r2[2];
    if (!result && issueWarning)
	TF_WARN("OrthogonalizeBasis did not converge, matrix may not be "
                "orthonormal.");
    return result;
}

{{ MAT }}
{{ MAT }}::GetOrthonormalized(bool issueWarning) const
{
    {{ MAT }} result = *this;
    result.Orthonormalize(issueWarning);
    return result;
}
{% endblock customFunctions %}

{% block customXformFunctions %}
{{ MAT }} &
{{ MAT }}::SetScale({{ SCL }} s)
{
    _mtx[0][0] = s;   _mtx[0][1] = 0.0; _mtx[0][2] = 0.0;
    _mtx[1][0] = 0.0; _mtx[1][1] = s;   _mtx[1][2] = 0.0;
    _mtx[2][0] = 0.0; _mtx[2][1] = 0.0; _mtx[2][2] = s;

    return *this;
}

{{ MAT }} &
{{ MAT }}::SetRotate(const GfQuat{{ SCL[0] }} &rot)
{
    _SetRotateFromQuat(rot.GetReal(), rot.GetImaginary());
    return *this;
}

{{ MAT }} &
{{ MAT }}::SetRotate(const GfRotation &rot)
{
    GfQuaternion quat = rot.GetQuaternion();
    _SetRotateFromQuat(quat.GetReal(), GfVec3{{ SCL[0] }}(quat.GetImaginary()));
    return *this;
}

void
{{MAT}}::_SetRotateFromQuat({{ SCL }} r, const GfVec3{{ SCL[0] }}& i)
{
    _mtx[0][0] = 1.0 - 2.0 * (i[1] * i[1] + i[2] * i[2]);
    _mtx[0][1] =       2.0 * (i[0] * i[1] + i[2] *    r);
    _mtx[0][2] =       2.0 * (i[2] * i[0] - i[1] *    r);

    _mtx[1][0] =       2.0 * (i[0] * i[1] - i[2] *    r);
    _mtx[1][1] = 1.0 - 2.0 * (i[2] * i[2] + i[0] * i[0]);
    _mtx[1][2] =       2.0 * (i[1] * i[2] + i[0] *    r);

    _mtx[2][0] =       2.0 * (i[2] * i[0] + i[1] *    r);
    _mtx[2][1] =       2.0 * (i[1] * i[2] - i[0] *    r);
    _mtx[2][2] = 1.0 - 2.0 * (i[1] * i[1] + i[0] * i[0]);
}
                            

{{ MAT }} &
{{ MAT }}::SetScale(const GfVec3{{ SCL[0] }} &s)
{
    _mtx[0][0] = s[0]; _mtx[0][1] = 0.0;  _mtx[0][2] = 0.0;
    _mtx[1][0] = 0.0;  _mtx[1][1] = s[1]; _mtx[1][2] = 0.0;
    _mtx[2][0] = 0.0;  _mtx[2][1] = 0.0;  _mtx[2][2] = s[2];

    return *this;
}

GfQuaternion
{{ MAT }}::ExtractRotationQuaternion() const
{
    // This was adapted from the (open source) Open Inventor
    // SbRotation::SetValue(const SbMatrix &m)

    int i;

    // First, find largest diagonal in matrix:
    if (_mtx[0][0] > _mtx[1][1])
	i = (_mtx[0][0] > _mtx[2][2] ? 0 : 2);
    else
	i = (_mtx[1][1] > _mtx[2][2] ? 1 : 2);

    GfVec3d im;
    double  r;

    if (_mtx[0][0] + _mtx[1][1] + _mtx[2][2] > _mtx[i][i]) {
	r = 0.5 * sqrt(_mtx[0][0] + _mtx[1][1] +
		       _mtx[2][2] + 1);
	im.Set((_mtx[1][2] - _mtx[2][1]) / (4.0 * r),
	       (_mtx[2][0] - _mtx[0][2]) / (4.0 * r),
	       (_mtx[0][1] - _mtx[1][0]) / (4.0 * r));
    }
    else {
	int j = (i + 1) % 3;
	int k = (i + 2) % 3;
	double q = 0.5 * sqrt(_mtx[i][i] - _mtx[j][j] -
			      _mtx[k][k] + 1); 

	im[i] = q;
	im[j] = (_mtx[i][j] + _mtx[j][i]) / (4 * q);
	im[k] = (_mtx[k][i] + _mtx[i][k]) / (4 * q);
	r     = (_mtx[j][k] - _mtx[k][j]) / (4 * q);
    }

    return GfQuaternion(GfClamp(r, -1.0, 1.0), im);
}

GfRotation
{{ MAT }}::ExtractRotation() const
{
    return GfRotation( ExtractRotationQuaternion() );
}

GfVec3{{ SCL[0] }}
{{ MAT }}::DecomposeRotation(const GfVec3{{ SCL[0] }} &axis0,
                             const GfVec3{{ SCL[0] }} &axis1,
                             const GfVec3{{ SCL[0] }} &axis2) const
{
    return {% if SCL != 'double' %}GfVec3{{ SCL[0] }}{% endif -%}
    (ExtractRotation().Decompose(axis0, axis1, axis2));
}
{% endblock customXformFunctions %}
