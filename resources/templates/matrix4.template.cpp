// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// matrix4.template.cpp file to make changes.

{% extends "matrix.template.cpp" %}
{% from "matrix.template.cpp" import DIAGONAL %}

{% block customIncludes %}
#include "./homogeneous.h"
#include "./matrix3{{ SCL[0] }}.h"
#include "./quat{{ SCL[0] }}.h"
#include "./rotation.h"
{% endblock customIncludes %}

{% block customConstructors %}
{% for S in SCALARS %}
{{ MAT }}::{{ MAT }}(const std::vector<{{ S }}>& r0,
                     const std::vector<{{ S }}>& r1,
                     const std::vector<{{ S }}>& r2,
                     const std::vector<{{ S }}>& r3)
{
    {{ SCL }} m[{{ DIM }}][{{ DIM }}] = {{ DIAGONAL(DIM, 22) }}

    for (size_t col = 0; col < {{ DIM }} && col < r0.size(); ++col) {
        m[0][col] = r0[col];
    }

    for (size_t col = 0; col < {{ DIM }} && col < r1.size(); ++col) {
        m[1][col] = r1[col];
    }

    for (size_t col = 0; col < {{ DIM }} && col < r2.size(); ++col) {
        m[2][col] = r2[col];
    }

    for (size_t col = 0; col < {{ DIM }} && col < r3.size(); ++col) {
        m[3][col] = r3[col];
    }

    Set(m);
}

{% endfor %}
{{ MAT }}::{{ MAT }}(const GfRotation& rotate,
                       const GfVec3{{ SCL[0] }}& translate)
{
    SetTransform(rotate, translate);
}

{{ MAT }}::{{ MAT }}(const GfMatrix3{{ SCL[0] }}& rotmx,
                       const GfVec3{{ SCL[0] }}& translate)
{
    SetTransform(rotmx, translate);
}
{% endblock customConstructors %}

{% block customFunctions %}
{{ MAT }} &
{{ MAT }}::SetTransform(const GfRotation& rotate,
			const GfVec3{{ SCL[0] }}& translate)
{
    SetRotate(rotate);
    return SetTranslateOnly(translate);
}

{{ MAT }} &
{{ MAT }}::SetTransform(const GfMatrix3{{ SCL[0] }}& rotate,
			const GfVec3{{ SCL[0] }}& translate)
{
    SetRotate(rotate);
    return SetTranslateOnly(translate);
}   

{{ MAT }}
{{ MAT }}::GetInverse(double *detPtr, double eps) const
{
    {{ SCL }} x00, x01, x02, x03;
    {{ SCL }} x10, x11, x12, x13;
    {{ SCL }} x20, x21, x22, x23;
    {{ SCL }} x30, x31, x32, x33;
    double y01, y02, y03, y12, y13, y23;
    {{ SCL }} z00, z10, z20, z30;
    {{ SCL }} z01, z11, z21, z31;
    double z02, z03, z12, z13, z22, z23, z32, z33;

    // Pickle 1st two columns of matrix into registers
    x00 = _mtx[0][0];
    x01 = _mtx[0][1];
    x10 = _mtx[1][0];
    x11 = _mtx[1][1];
    x20 = _mtx[2][0];
    x21 = _mtx[2][1];
    x30 = _mtx[3][0];
    x31 = _mtx[3][1];

    // Compute all six 2x2 determinants of 1st two columns
    y01 = x00*x11 - x10*x01;
    y02 = x00*x21 - x20*x01;
    y03 = x00*x31 - x30*x01;
    y12 = x10*x21 - x20*x11;
    y13 = x10*x31 - x30*x11;
    y23 = x20*x31 - x30*x21;

    // Pickle 2nd two columns of matrix into registers
    x02 = _mtx[0][2];
    x03 = _mtx[0][3];
    x12 = _mtx[1][2];
    x13 = _mtx[1][3];
    x22 = _mtx[2][2];
    x23 = _mtx[2][3];
    x32 = _mtx[3][2];
    x33 = _mtx[3][3];

    // Compute all 3x3 cofactors for 2nd two columns */
    z33 = x02*y12 - x12*y02 + x22*y01;
    z23 = x12*y03 - x32*y01 - x02*y13;
    z13 = x02*y23 - x22*y03 + x32*y02;
    z03 = x22*y13 - x32*y12 - x12*y23;
    z32 = x13*y02 - x23*y01 - x03*y12;
    z22 = x03*y13 - x13*y03 + x33*y01;
    z12 = x23*y03 - x33*y02 - x03*y23;
    z02 = x13*y23 - x23*y13 + x33*y12;

    // Compute all six 2x2 determinants of 2nd two columns
    y01 = x02*x13 - x12*x03;
    y02 = x02*x23 - x22*x03;
    y03 = x02*x33 - x32*x03;
    y12 = x12*x23 - x22*x13;
    y13 = x12*x33 - x32*x13;
    y23 = x22*x33 - x32*x23;

    // Compute all 3x3 cofactors for 1st two columns
    z30 = x11*y02 - x21*y01 - x01*y12;
    z20 = x01*y13 - x11*y03 + x31*y01;
    z10 = x21*y03 - x31*y02 - x01*y23;
    z00 = x11*y23 - x21*y13 + x31*y12;
    z31 = x00*y12 - x10*y02 + x20*y01;
    z21 = x10*y03 - x30*y01 - x00*y13;
    z11 = x00*y23 - x20*y03 + x30*y02;
    z01 = x20*y13 - x30*y12 - x10*y23;

    // compute 4x4 determinant & its reciprocal
    double det = x30*z30 + x20*z20 + x10*z10 + x00*z00;
    if (detPtr) {
	*detPtr = det;
    }

    {{ MAT }} inverse;

    if (GfAbs(det) > eps) {

        double rcp = 1.0 / det;

{%- macro SCALAR_CAST(t) %}
{%- if SCL == 'float' %}
static_cast<float>({{ t }})
{%- else %}
{{ t }}
{%- endif %}
{% endmacro %}

	// Multiply all 3x3 cofactors by reciprocal & transpose
        inverse._mtx[0][0] = {{ SCALAR_CAST("z00*rcp") }};
        inverse._mtx[0][1] = {{ SCALAR_CAST("z10*rcp") }};
        inverse._mtx[1][0] = {{ SCALAR_CAST("z01*rcp") }};
        inverse._mtx[0][2] = {{ SCALAR_CAST("z20*rcp") }};
        inverse._mtx[2][0] = {{ SCALAR_CAST("z02*rcp") }};
        inverse._mtx[0][3] = {{ SCALAR_CAST("z30*rcp") }};
        inverse._mtx[3][0] = {{ SCALAR_CAST("z03*rcp") }};
        inverse._mtx[1][1] = {{ SCALAR_CAST("z11*rcp") }};
        inverse._mtx[1][2] = {{ SCALAR_CAST("z21*rcp") }};
        inverse._mtx[2][1] = {{ SCALAR_CAST("z12*rcp") }};
        inverse._mtx[1][3] = {{ SCALAR_CAST("z31*rcp") }};
        inverse._mtx[3][1] = {{ SCALAR_CAST("z13*rcp") }};
        inverse._mtx[2][2] = {{ SCALAR_CAST("z22*rcp") }};
        inverse._mtx[2][3] = {{ SCALAR_CAST("z32*rcp") }};
        inverse._mtx[3][2] = {{ SCALAR_CAST("z23*rcp") }};
        inverse._mtx[3][3] = {{ SCALAR_CAST("z33*rcp") }};

    }
    else {
	inverse.SetScale(FLT_MAX);
    }

    return inverse;

}

double
{{ MAT }}::GetDeterminant() const
{
    return (- _mtx[0][3] * _GetDeterminant3(1, 2, 3, 0, 1, 2)
	    + _mtx[1][3] * _GetDeterminant3(0, 2, 3, 0, 1, 2)
	    - _mtx[2][3] * _GetDeterminant3(0, 1, 3, 0, 1, 2)
	    + _mtx[3][3] * _GetDeterminant3(0, 1, 2, 0, 1, 2));
}

double
{{ MAT }}::_GetDeterminant3(size_t row1, size_t row2, size_t row3,
			    size_t col1, size_t col2, size_t col3) const
{
    return (_mtx[row1][col1] * _mtx[row2][col2] * _mtx[row3][col3] +
	    _mtx[row1][col2] * _mtx[row2][col3] * _mtx[row3][col1] +
	    _mtx[row1][col3] * _mtx[row2][col1] * _mtx[row3][col2] -
	    _mtx[row1][col1] * _mtx[row2][col3] * _mtx[row3][col2] -
	    _mtx[row1][col2] * _mtx[row2][col1] * _mtx[row3][col3] -
	    _mtx[row1][col3] * _mtx[row2][col2] * _mtx[row3][col1]);
}

double
{{ MAT }}::GetHandedness() const
{
    // Note: This can be computed with fewer arithmetic operations using a
    //       cross and dot product, but it is more important that the result
    //       is consistent with the way the determinant is computed.
    return GfSgn(GetDeterminant3());
}

/*
 * Make the matrix orthonormal in place using an iterative method.
 * It is potentially slower if the matrix is far from orthonormal (i.e. if
 * the row basis vectors are close to colinear) but in the common case
 * of near-orthonormality it should be just as fast.
 *
 * The translation part is left intact.  If the translation is represented as
 * a homogenous coordinate (i.e. a non-unity lower right corner), it is divided
 * out.
 */
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

    // divide out any homogeneous coordinate - unless it's zero
    if (_mtx[3][3] != 1.0 && !GfIsClose(_mtx[3][3], 0.0, GF_MIN_VECTOR_LENGTH))
    {
	_mtx[3][0] /= _mtx[3][3];
	_mtx[3][1] /= _mtx[3][3];
	_mtx[3][2] /= _mtx[3][3];
	_mtx[3][3] = 1.0;
    }

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
// Leaves the [3][3] element as 1
{{ MAT }} &
{{ MAT }}::SetScale({{ SCL }} s)
{
    _mtx[0][0] = s;   _mtx[0][1] = 0.0; _mtx[0][2] = 0.0; _mtx[0][3] = 0.0;
    _mtx[1][0] = 0.0; _mtx[1][1] = s;   _mtx[1][2] = 0.0; _mtx[1][3] = 0.0;
    _mtx[2][0] = 0.0; _mtx[2][1] = 0.0; _mtx[2][2] = s;   _mtx[2][3] = 0.0;
    _mtx[3][0] = 0.0; _mtx[3][1] = 0.0; _mtx[3][2] = 0.0; _mtx[3][3] = 1.0;

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
{{ MAT }}::SetRotate(const GfQuat{{ SCL[0] }} &rot)
{
    SetRotateOnly(rot);

    _mtx[0][3] = 0.0;
    _mtx[1][3] = 0.0;
    _mtx[2][3] = 0.0;

    _mtx[3][0] = 0.0;
    _mtx[3][1] = 0.0;
    _mtx[3][2] = 0.0;
    _mtx[3][3] = 1.0;

    return *this;
}

{{ MAT }} &
{{ MAT }}::SetRotateOnly(const GfQuat{{ SCL[0] }} &rot)
{
    _SetRotateFromQuat(rot.GetReal(), rot.GetImaginary());
    return *this;
}

{{ MAT }} &
{{ MAT }}::SetRotate(const GfRotation &rot)
{
    SetRotateOnly(rot);

    _mtx[0][3] = 0.0;
    _mtx[1][3] = 0.0;
    _mtx[2][3] = 0.0;

    _mtx[3][0] = 0.0;
    _mtx[3][1] = 0.0;
    _mtx[3][2] = 0.0;
    _mtx[3][3] = 1.0;

    return *this;
}

{{ MAT }} &
{{ MAT }}::SetRotateOnly(const GfRotation &rot)
{
    GfQuaternion quat = rot.GetQuaternion();
    _SetRotateFromQuat(quat.GetReal(), GfVec3{{ SCL[0] }}(quat.GetImaginary()));
    return *this;
}

{{ MAT }} &
{{ MAT }}::SetRotate(const GfMatrix3{{ SCL[0] }} &mx3)
{
    _mtx[0][0] = mx3[0][0];
    _mtx[0][1] = mx3[0][1];
    _mtx[0][2] = mx3[0][2];
    _mtx[0][3] = 0.0;

    _mtx[1][0] = mx3[1][0];
    _mtx[1][1] = mx3[1][1];
    _mtx[1][2] = mx3[1][2];
    _mtx[1][3] = 0.0;

    _mtx[2][0] = mx3[2][0];
    _mtx[2][1] = mx3[2][1];
    _mtx[2][2] = mx3[2][2];
    _mtx[2][3] = 0.0;

    _mtx[3][0] = 0.0;
    _mtx[3][1] = 0.0;
    _mtx[3][2] = 0.0;
    _mtx[3][3] = 1.0;

    return *this;
}

{{ MAT }} &
{{ MAT }}::SetRotateOnly(const GfMatrix3{{ SCL[0] }} &mx3)
{
    _mtx[0][0] = mx3[0][0];
    _mtx[0][1] = mx3[0][1];
    _mtx[0][2] = mx3[0][2];

    _mtx[1][0] = mx3[1][0];
    _mtx[1][1] = mx3[1][1];
    _mtx[1][2] = mx3[1][2];

    _mtx[2][0] = mx3[2][0];
    _mtx[2][1] = mx3[2][1];
    _mtx[2][2] = mx3[2][2];

    return *this;
}

{{ MAT }} &
{{ MAT }}::SetScale(const GfVec3{{ SCL[0] }} &s)
{
    _mtx[0][0] = s[0]; _mtx[0][1] = 0.0;  _mtx[0][2] = 0.0;  _mtx[0][3] = 0.0;
    _mtx[1][0] = 0.0;  _mtx[1][1] = s[1]; _mtx[1][2] = 0.0;  _mtx[1][3] = 0.0;
    _mtx[2][0] = 0.0;  _mtx[2][1] = 0.0;  _mtx[2][2] = s[2]; _mtx[2][3] = 0.0;
    _mtx[3][0] = 0.0;  _mtx[3][1] = 0.0;  _mtx[3][2] = 0.0;  _mtx[3][3] = 1.0;

    return *this;
}

{{ MAT }} &
{{ MAT }}::SetTranslate(const GfVec3{{ SCL[0] }} &t)
{
    _mtx[0][0] = 1.0;  _mtx[0][1] = 0.0;  _mtx[0][2] = 0.0;  _mtx[0][3] = 0.0;
    _mtx[1][0] = 0.0;  _mtx[1][1] = 1.0;  _mtx[1][2] = 0.0;  _mtx[1][3] = 0.0;
    _mtx[2][0] = 0.0;  _mtx[2][1] = 0.0;  _mtx[2][2] = 1.0;  _mtx[2][3] = 0.0;
    _mtx[3][0] = t[0]; _mtx[3][1] = t[1]; _mtx[3][2] = t[2]; _mtx[3][3] = 1.0;

    return *this;
}

{{ MAT }} &
{{ MAT }}::SetTranslateOnly(const GfVec3{{ SCL[0] }} &t)
{
    _mtx[3][0] = t[0]; _mtx[3][1] = t[1]; _mtx[3][2] = t[2]; _mtx[3][3] = 1.0;

    return *this;
}

{{ MAT }} &
{{ MAT }}::SetLookAt(const GfVec3{{ SCL[0] }} &eyePoint,
		     const GfVec3{{ SCL[0] }} &centerPoint,
		     const GfVec3{{ SCL[0] }} &upDirection)
{
    // Get the normalized view vector
    GfVec3{{ SCL[0] }} view = (centerPoint - eyePoint).GetNormalized();

    // Compute vector orthogonal to view and up
    GfVec3{{ SCL[0] }} right = GfCross(view, upDirection).GetNormalized();

    // Compute the "real" up vector orthogonal to both view and right
    GfVec3{{ SCL[0] }} realUp = GfCross(right, view);

    // Set matrix to rotate from world-space coordinate system,
    // translating eye position to origin.
    _mtx[0][0] = right[0];
    _mtx[1][0] = right[1];
    _mtx[2][0] = right[2];
    _mtx[3][0] = -(right[0] * eyePoint[0] +
		   right[1] * eyePoint[1] +
		   right[2] * eyePoint[2]);

    _mtx[0][1] = realUp[0];
    _mtx[1][1] = realUp[1];
    _mtx[2][1] = realUp[2];
    _mtx[3][1] = -(realUp[0] * eyePoint[0] +
		   realUp[1] * eyePoint[1] +
		   realUp[2] * eyePoint[2]);

    _mtx[0][2] = -view[0];
    _mtx[1][2] = -view[1];
    _mtx[2][2] = -view[2];
    _mtx[3][2] = (view[0] * eyePoint[0] +
		  view[1] * eyePoint[1] +
		  view[2] * eyePoint[2]);

    _mtx[0][3] = 0.0;
    _mtx[1][3] = 0.0;
    _mtx[2][3] = 0.0;
    _mtx[3][3] = 1.0;

    return *this;
}

{{ MAT }} &
{{ MAT }}::SetLookAt(const GfVec3{{ SCL[0] }} &eyePoint, const GfRotation &orientation)
{
    // To go from world space to eye space, first translate the
    // world-space eye point to the origin, then rotate by the inverse
    // of the orientation rotation to bring the world-space view
    // direction to (0,0,-1);
    {{ MAT }} m1, m2;
    return *this = (m1.SetTranslate(-eyePoint) *
		    m2.SetRotate(orientation.GetInverse()));
}

bool
{{ MAT }}::Factor({{ MAT }}* r, GfVec3{{ SCL[0] }}* s, {{ MAT }}* u,
		  GfVec3{{ SCL[0] }}* t, {{ MAT }}* p, {{ SCL }} eps) const
{
    // This was adapted from the (open source) Open Inventor
    // SbMatrix::Factor().

    p->SetDiagonal(1);

    // Set A to the upper 3x3 and set t to the translation part of
    // this matrix
    GfMatrix4d a;
    for (int i = 0; i < 3; i++) {
	for (int j = 0; j < 3; j++)
	    a._mtx[i][j] = _mtx[i][j];
	a._mtx[3][i] = a._mtx[i][3] = 0.0;
	(*t)[i] = _mtx[3][i];
    }
    a._mtx[3][3] = 1.0;
    
    // Compute the determinant of A. If its absolute value is real
    // small, the matrix is singular.
    double det     = a.GetDeterminant3();
    double detSign = (det < 0.0 ? -1.0 : 1.0);
    bool isSingular = det * detSign < eps;

    // Compute B = A * A-transpose and find its eigenvalues and
    // eigenvectors. Use the eigenvectors as the rotation matrix R.
    GfMatrix4d b = a * a.GetTranspose();
    GfVec3d	eigenvalues;
    GfVec3d	eigenvectors[3];
    b._Jacobi3(&eigenvalues, eigenvectors);
{% if SCL == 'double' %}
    r->Set(eigenvectors[0][0], eigenvectors[0][1], eigenvectors[0][2], 0.0, 
	   eigenvectors[1][0], eigenvectors[1][1], eigenvectors[1][2], 0.0, 
	   eigenvectors[2][0], eigenvectors[2][1], eigenvectors[2][2], 0.0, 
	   0.0, 0.0, 0.0, 1.0);
{% else %}
    GfMatrix4d rTmp(
        eigenvectors[0][0], eigenvectors[0][1], eigenvectors[0][2], 0.0, 
        eigenvectors[1][0], eigenvectors[1][1], eigenvectors[1][2], 0.0, 
        eigenvectors[2][0], eigenvectors[2][1], eigenvectors[2][2], 0.0, 
        0.0, 0.0, 0.0, 1.0);
{% endif %}

    // Compute s = sqrt(eigenvalues), with sign. Set sInv to the
    // inverse of s, or if eigenvalue < eps, let s = eps.  This allows
    // us to compute factors for singular matrices.
    GfMatrix4d sInv;
    sInv.SetIdentity();
    for (int i = 0; i < 3; i++) {
        if (eigenvalues[i] < eps) {
            (*s)[i] = detSign * eps;
        } else {
            (*s)[i] = detSign * sqrt(eigenvalues[i]);
        }
        sInv._mtx[i][i] = 1.0 / (*s)[i];
    }

    // Compute U = R S-inverse R-transpose A
{% if SCL == 'double' %}
    *u = *r * sInv * r->GetTranspose() * a;
{% else %}
    *u = {{ MAT }}(rTmp * sInv * rTmp.GetTranspose() * a);
    *r = {{ MAT }}(rTmp);
{% endif %}
    
    return !isSingular;
}

void
{{ MAT }}::_Jacobi3(GfVec3d *eigenvalues, GfVec3d eigenvectors[3]) const
{
    // This was adapted from the (open source) Open Inventor
    // SbMatrix::Jacobi3().

    // Initialize eigenvalues to the diagonal of the 3x3 matrix and
    // eigenvectors to the principal axes.
    eigenvalues->Set(_mtx[0][0], _mtx[1][1], _mtx[2][2]);
    eigenvectors[0] = GfVec3d::XAxis();
    eigenvectors[1] = GfVec3d::YAxis();
    eigenvectors[2] = GfVec3d::ZAxis();

    {{ MAT }} a = (*this);
    GfVec3d   b = *eigenvalues;
    GfVec3d   z = GfVec3d(0);

    for (int i = 0; i < 50; i++) {
	double sm = 0.0;
	for (int p = 0; p < 2; p++)
	    for (int q = p+1; q < 3; q++)
		sm += GfAbs(a._mtx[p][q]);
	
	if (sm == 0.0)
	    return;
	
	double thresh = (i < 3 ? (.2 * sm / (3 * 3)) : 0.0);
	
	for (int p = 0; p < 3; p++) {
	    for (int q = p+1; q < 3; q++) {
		
		double g = 100.0 * GfAbs(a._mtx[p][q]);
		
		if (i > 3 &&
		    (GfAbs((*eigenvalues)[p]) + g ==
		     GfAbs((*eigenvalues)[p])) && 
		    (GfAbs((*eigenvalues)[q]) + g ==
		     GfAbs((*eigenvalues)[q])))
		    a._mtx[p][q] = 0.0;
		
		else if (GfAbs(a._mtx[p][q]) > thresh) {
		    double h = (*eigenvalues)[q] - (*eigenvalues)[p];
		    double t;

		    if (GfAbs(h) + g == GfAbs(h)) {
			t = a._mtx[p][q] / h;
		    } else {
			double theta = 0.5 * h / a._mtx[p][q];
			t = 1.0 / (GfAbs(theta) + sqrt(1.0 + theta * theta));
			if (theta < 0.0)
			    t = -t;
		    }

		    // End of computing tangent of rotation angle
		    
		    double c = 1.0 / sqrt(1.0 + t*t);
		    double s = t * c;
		    double tau = s / (1.0 + c);
		    h = t * a._mtx[p][q];
		    z[p]    -= h;
		    z[q]    += h;
		    (*eigenvalues)[p] -= h;
		    (*eigenvalues)[q] += h;
		    a._mtx[p][q] = 0.0;
		    
		    for (int j = 0; j < p; j++) {
			g = a._mtx[j][p];
			h = a._mtx[j][q];
			a._mtx[j][p] = g - s * (h + g * tau);
			a._mtx[j][q] = h + s * (g - h * tau);
		    }
		    
		    for (int j = p+1; j < q; j++) {
			g = a._mtx[p][j];
			h = a._mtx[j][q];
			a._mtx[p][j] = g - s * (h + g * tau);
			a._mtx[j][q] = h + s * (g - h * tau);
		    }
		    
		    for (int j = q+1; j < 3; j++) {
			g = a._mtx[p][j];
			h = a._mtx[q][j];
			a._mtx[p][j] = g - s * (h + g * tau);
			a._mtx[q][j] = h + s * (g - h * tau);
		    }
		    
		    for (int j = 0; j < 3; j++) {
			g = eigenvectors[j][p];
			h = eigenvectors[j][q];
			eigenvectors[j][p] = g - s * (h + g * tau);
			eigenvectors[j][q] = h + s * (g - h * tau);
		    }
		}
	    }
	}
	for (int p = 0; p < 3; p++) {
	    (*eigenvalues)[p] = b[p] += z[p];
	    z[p] = 0;
	}
    }
}

{{ MAT }}
{{ MAT }}::RemoveScaleShear() const
{
    {{ MAT }} scaleOrientMat, factoredRotMat, perspMat;
    GfVec3{{ SCL[0] }} scale, translation;
    if (!{{ MAT }}::Factor(&scaleOrientMat, &scale, &factoredRotMat,
                  &translation, &perspMat)) {
        // unable to decompose, so return the matrix
        return *this;
    }

    // Remove shear
    factoredRotMat.Orthonormalize();
    // compose matrix with new rotation / translation (no scale/shear)
    return factoredRotMat * {{ MAT }}(1.0).SetTranslate(translation);
}

GfQuat{{ SCL[0] }}
{{ MAT }}::ExtractRotationQuat() const
{
    // This was adapted from the (open source) Open Inventor
    // SbRotation::SetValue(const SbMatrix &m)

    int i;

    // First, find largest diagonal in matrix:
    if (_mtx[0][0] > _mtx[1][1])
	i = (_mtx[0][0] > _mtx[2][2] ? 0 : 2);
    else
	i = (_mtx[1][1] > _mtx[2][2] ? 1 : 2);

    GfVec3{{ SCL[0] }} im;
    ScalarType  r;

    if (_mtx[0][0] + _mtx[1][1] + _mtx[2][2] > _mtx[i][i]) {
	r = 0.5 * sqrt(_mtx[0][0] + _mtx[1][1] +
		       _mtx[2][2] + _mtx[3][3]);
	im.Set((_mtx[1][2] - _mtx[2][1]) / (4.0 * r),
	       (_mtx[2][0] - _mtx[0][2]) / (4.0 * r),
	       (_mtx[0][1] - _mtx[1][0]) / (4.0 * r));
    }
    else {
	int j = (i + 1) % 3;
	int k = (i + 2) % 3;
	ScalarType q = 0.5 * sqrt(_mtx[i][i] - _mtx[j][j] -
			      _mtx[k][k] + _mtx[3][3]); 

	im[i] = q;
	im[j] = (_mtx[i][j] + _mtx[j][i]) / (4 * q);
	im[k] = (_mtx[k][i] + _mtx[i][k]) / (4 * q);
	r     = (_mtx[j][k] - _mtx[k][j]) / (4 * q);
    }

    return GfQuat{{ SCL[0] }}(
        GfClamp(r, (ScalarType)-1.0, (ScalarType)1.0), im);
}

GfRotation
{{ MAT }}::ExtractRotation() const
{
    return GfRotation(ExtractRotationQuat());
}

GfVec3{{ SCL[0] }}
{{ MAT }}::DecomposeRotation(const GfVec3{{ SCL[0] }} &axis0,
                             const GfVec3{{ SCL[0] }} &axis1,
                             const GfVec3{{ SCL[0] }} &axis2) const
{
    return {% if SCL != 'double' %}GfVec3{{ SCL[0] }}{% endif -%}
    (ExtractRotation().Decompose(axis0, axis1, axis2));
}

GfMatrix3{{ SCL[0] }}
{{ MAT }}::ExtractRotationMatrix() const
{
    return GfMatrix3{{ SCL[0] }}(
	_mtx[0][0],
	_mtx[0][1],
	_mtx[0][2],

	_mtx[1][0],
	_mtx[1][1],
	_mtx[1][2],

	_mtx[2][0],
	_mtx[2][1],
	_mtx[2][2]);
}
{% endblock customXformFunctions %}
