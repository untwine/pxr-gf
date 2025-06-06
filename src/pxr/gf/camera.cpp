// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include "./camera.h"
#include "./frustum.h"
#include <pxr/tf/enum.h>
#include <pxr/tf/registryManager.h>

namespace pxr {

TF_REGISTRY_FUNCTION(pxr::TfEnum)
{
    TF_ADD_ENUM_NAME(GfCamera::Perspective,   "perspective");
    TF_ADD_ENUM_NAME(GfCamera::Orthographic,  "orthographic");
    TF_ADD_ENUM_NAME(GfCamera::FOVHorizontal, "FOVHorizontal");
    TF_ADD_ENUM_NAME(GfCamera::FOVVertical,   "FOVVertical");
}

// Horizontal and vertical aperture is in mm whereas most stuff is in cm.
const double
GfCamera::APERTURE_UNIT = 0.1;

// Focal length is in mm whereas most stuff is in cm.
const double
GfCamera::FOCAL_LENGTH_UNIT = 0.1;

// The default filmback size is based on a 35mm spherical
// projector aperture (0.825 x 0.602 inches, converted to
// mm). Note this is slightly different than SMPTE195-2000,
// which specifies 20.96mm (not 20.955mm) and 0.825"  Also
// note that 35mm spherical and anamorphic projector aperture
// widths are the same. Lastly, we use projection aperture
// instead of camera aperture since that's what we film out
// to, and for anyone who cares, 35mm still film has a different
// size, and we don't use that at all with our virtual movie
// camera.
const double
GfCamera::DEFAULT_HORIZONTAL_APERTURE = (
    0.825 * 2.54 / GfCamera::APERTURE_UNIT);
const double
GfCamera::DEFAULT_VERTICAL_APERTURE = (
    0.602 * 2.54 / GfCamera::APERTURE_UNIT);

GfCamera::GfCamera(
    const GfMatrix4d &transform, GfCamera::Projection projection,
    float horizontalAperture, float verticalAperture,
    float horizontalApertureOffset, float verticalApertureOffset,
    float focalLength,
    const GfRange1f &clippingRange,
    const std::vector<GfVec4f>&clippingPlanes,
    float fStop,
    float focusDistance) :
    _transform(transform), _projection(projection),
    _horizontalAperture(horizontalAperture),
    _verticalAperture(verticalAperture),
    _horizontalApertureOffset(horizontalApertureOffset),
    _verticalApertureOffset(verticalApertureOffset),
    _focalLength(focalLength),
    _clippingRange(clippingRange),
    _clippingPlanes(clippingPlanes),
    _fStop(fStop),
    _focusDistance(focusDistance)
{
}

void
GfCamera::SetTransform(const GfMatrix4d &val) {
    _transform = val;
}

void
GfCamera::SetProjection(const GfCamera::Projection &val) {
    _projection = val;
}

void
GfCamera::SetHorizontalAperture(const float val) {
    _horizontalAperture = val;
}

void
GfCamera::SetVerticalAperture(const float val) {
    _verticalAperture = val;
}

void
GfCamera::SetHorizontalApertureOffset(const float val) {
    _horizontalApertureOffset = val;
}

void
GfCamera::SetVerticalApertureOffset(const float val) {
    _verticalApertureOffset = val;
}

void
GfCamera::SetFocalLength(const float val) {
    _focalLength = val;
}

void
GfCamera::SetPerspectiveFromAspectRatioAndFieldOfView(
    float aspectRatio,
    float fieldOfView,
    GfCamera::FOVDirection direction,
    float horizontalAperture)
{
    // Perspective
    _projection = Perspective;

    // Set the vertical and horizontal aperture to achieve the aspect ratio
    _horizontalAperture = horizontalAperture;
    _verticalAperture =   horizontalAperture /
        (aspectRatio != 0.0 ? aspectRatio : 1.0);

    // Pick the right dimension based on the direction parameter
    const float aperture =
        (direction == GfCamera::FOVHorizontal) ?
                           _horizontalAperture : _verticalAperture;
    // Compute tangent for field of view
    const float tanValue = tan(0.5 * GfDegreesToRadians(fieldOfView));
    
    if (tanValue == 0) {
        // To avoid division by zero, just set default value
        _focalLength = 50.0;
        return;
    }
    
    // Do the math for the focal length.
    _focalLength =
        aperture * GfCamera::APERTURE_UNIT /
        ( 2 * tanValue) / GfCamera::FOCAL_LENGTH_UNIT;
}

void
GfCamera::SetOrthographicFromAspectRatioAndSize(
    float aspectRatio, float orthographicSize, FOVDirection direction)
{
    // Orthographic
    _projection = Orthographic;

    // Not used for orthographic cameras, but set to sane values nonetheless
    _focalLength = 50.0;

    // Set horizontal and vertial aperture
    if (direction == FOVHorizontal) {
        // We are given the width, determine height by dividing by aspect ratio
        _horizontalAperture = orthographicSize / GfCamera::APERTURE_UNIT;
        if (aspectRatio > 0.0) {
            _verticalAperture = _horizontalAperture / aspectRatio;
        } else {
            _verticalAperture = _horizontalAperture;
        }
    } else {
        // We are given the height, determine the width by multiplying
        _verticalAperture = orthographicSize / GfCamera::APERTURE_UNIT;
        _horizontalAperture = _verticalAperture * aspectRatio;
    }
}

void
GfCamera::SetFromViewAndProjectionMatrix(
    const GfMatrix4d &viewMatrix, const GfMatrix4d &projMatrix,
    const float focalLength)
{
    _transform = viewMatrix.GetInverse();

    _focalLength = focalLength;

    if (projMatrix[2][3] < -0.5) {
        // Use !(a<b) rather than a>=b so that NaN is caught.
        if (!(fabs(projMatrix[2][3] - (-1.0)) < 1e-6)) {
            TF_WARN("GfCamera: Given projection matrix does not appear to be "
                    "valid perspective matrix.");
        }

        _projection = Perspective;

        const float apertureBase =
            2.0f * focalLength * (FOCAL_LENGTH_UNIT / APERTURE_UNIT);

        _horizontalAperture =
            apertureBase / projMatrix[0][0];
        _verticalAperture =
            apertureBase / projMatrix[1][1];
        _horizontalApertureOffset =
            0.5 * _horizontalAperture * projMatrix[2][0];
        _verticalApertureOffset =
            0.5 * _verticalAperture * projMatrix[2][1];
        _clippingRange = GfRange1f(
            projMatrix[3][2] / (projMatrix[2][2] - 1.0),
            projMatrix[3][2] / (projMatrix[2][2] + 1.0));
    } else {
        if (!(fabs(projMatrix[2][3]) < 1e-6)) {
            TF_WARN("GfCamera: Given projection matrix does not appear to be "
                    "valid orthographic matrix.");
        }

        _projection = Orthographic;
        _horizontalAperture =
            (2.0 / APERTURE_UNIT) / projMatrix[0][0];
        _verticalAperture =
            (2.0 / APERTURE_UNIT) / projMatrix[1][1];
        _horizontalApertureOffset =
            -0.5 * (_horizontalAperture) * projMatrix[3][0];
        _verticalApertureOffset =
            -0.5 * (_verticalAperture) * projMatrix[3][1];
        const double nearMinusFarHalf =
            1.0 / projMatrix[2][2];
        const double nearPlusFarHalf =
            nearMinusFarHalf * projMatrix[3][2];
        _clippingRange = GfRange1f(
            nearPlusFarHalf + nearMinusFarHalf,
            nearPlusFarHalf - nearMinusFarHalf);
    }
}

void
GfCamera::SetClippingRange(const GfRange1f &val) {
    _clippingRange = val;
}

void
GfCamera::SetClippingPlanes(const std::vector<GfVec4f> &val) {
    _clippingPlanes = val;
}

void
GfCamera::SetFStop(const float val) {
    _fStop = val;
}

void
GfCamera::SetFocusDistance(const float val) {
    _focusDistance = val;
}

GfMatrix4d
GfCamera::GetTransform() const {
    return _transform;
}

GfCamera::Projection
GfCamera::GetProjection() const {
    return _projection;
}

float
GfCamera::GetHorizontalAperture() const {
    return _horizontalAperture;
}

float
GfCamera::GetVerticalAperture() const {
    return _verticalAperture;
}

float
GfCamera::GetHorizontalApertureOffset() const {
    return _horizontalApertureOffset;
}

float
GfCamera::GetVerticalApertureOffset() const {
    return _verticalApertureOffset;
}

float
GfCamera::GetAspectRatio() const {
    return (_verticalAperture == 0.0)
               ? 0.0
               : _horizontalAperture / _verticalAperture;
}

float
GfCamera::GetFocalLength() const {
    return _focalLength;
}

float
GfCamera::GetFieldOfView(FOVDirection direction) const {
    // Pick the right aperture based on direction
    const float aperture = 
        (direction == FOVHorizontal) ? _horizontalAperture
                                     : _verticalAperture;
    
    // Do the math
    const float fovRAD = 2 * atan(
        (aperture * GfCamera::APERTURE_UNIT) /
        (2 * _focalLength * GfCamera::FOCAL_LENGTH_UNIT));

    return GfRadiansToDegrees(fovRAD);
}

GfRange1f
GfCamera::GetClippingRange() const {
    return _clippingRange;
}

const std::vector<GfVec4f> &
GfCamera::GetClippingPlanes() const {
    return _clippingPlanes;
}

GfFrustum
GfCamera::GetFrustum() const {

    const GfVec2d max(_horizontalAperture / 2,
                      _verticalAperture / 2);
    GfRange2d window(-max, max);

    // Apply the aperture offset
    const GfVec2d offsetVec(
        _horizontalApertureOffset, _verticalApertureOffset);
    window += GfRange2d(offsetVec, offsetVec);

    // Up to now, all computations were done in mm, convert to cm.
    window *= GfCamera::APERTURE_UNIT;

    if (_projection != Orthographic && _focalLength != 0) {
        window /= _focalLength * GfCamera::FOCAL_LENGTH_UNIT;
    }

    const GfRange1d clippingRange(_clippingRange.GetMin(),
                                  _clippingRange.GetMax());
    
    const GfFrustum::ProjectionType projection = _projection == Orthographic
        ? GfFrustum::Orthographic
        : GfFrustum::Perspective;

    return GfFrustum(_transform, window, clippingRange, projection);
}

float
GfCamera::GetFStop() const {
    return _fStop;
}

float
GfCamera::GetFocusDistance() const {
    return _focusDistance;
}

bool
GfCamera::operator==(const GfCamera& other) const
{
    return
        _transform == other._transform && 
        _projection == other._projection && 
        _horizontalAperture == other._horizontalAperture && 
        _verticalAperture == other._verticalAperture && 
        _horizontalApertureOffset == other._horizontalApertureOffset && 
        _verticalApertureOffset == other._verticalApertureOffset && 
        _focalLength == other._focalLength && 
        _clippingRange == other._clippingRange && 
        _clippingPlanes == other._clippingPlanes &&
        _fStop == other._fStop &&
        _focusDistance == other._focusDistance;
}

bool
GfCamera::operator!=(const GfCamera& other) const
{
    return !(*this == other);
}

}  // namespace pxr
