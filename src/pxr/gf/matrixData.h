// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_GF_MATRIX_DATA_H
#define PXR_GF_MATRIX_DATA_H

#include "./api.h"

namespace pxr {

/// \class GfMatrixData
///
/// A class template used by GfMatrixXX to store values.
template <class T, int Rows, int Columns>
class GfMatrixData {
public:

    /// Return a pointer to a \a row of data.
    T *operator[](int row) {
        return _data + (row * Columns);
    }

    /// Return a const pointer to a \a row of data.
    T const *operator[](int row) const {
        return _data + (row * Columns);
    }

    /// Return a pointer to the start of all the data.
    T *GetData() {
        return _data;
    }

    /// Return a const pointer to the start of all the data.
    T const *GetData() const {
        return _data;
    }

private:

    T _data[Rows * Columns];
};

}  // namespace pxr

#endif // PXR_GF_MATRIX_DATA_H
