#include "UT_API.h"
#include "UT_Array.h"
#include "UT_Vector2.h"

/// Returns radius squared
template<typename T>
UT_API T UTboundingCircle(const T *coords, exint n, T *centre_out = nullptr);

template<typename T>
inline T UTboundingCircle(const UT_Array<UT_Vector2T<T> > &coords, UT_Vector2T<T> *centre_out = nullptr)
{
    return UTboundingCircle((const T*)coords.getArray(), coords.size(), centre_out ? centre_out->data() : nullptr);
}

template<typename T>
inline T UTboundingCircle(const UT_Vector2T<T> *coords, exint n, UT_Vector2T<T> *centre_out = nullptr)
{
    return UTboundingCircle((const T*)coords, n, centre_out ? centre_out->data() : nullptr);
}
