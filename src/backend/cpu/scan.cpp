/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <complex>
#include <af/dim4.hpp>
#include <Array.hpp>
#include <scan.hpp>
#include <ops.hpp>
#include <platform.hpp>
#include <queue.hpp>
#include <kernel/scan.hpp>

using af::dim4;

namespace cpu
{

    template<af_op_t op, typename Ti, typename To>
    Array<To> scan(const Array<Ti>& in, const int dim, bool inclusive_scan)
    {
        dim4 dims     = in.dims();
        Array<To> out = createEmptyArray<To>(dims);
        in.eval();

        if (inclusive_scan) {
            switch (in.ndims()) {
                case 1:
                    kernel::scan_dim<op, Ti, To, 1, true> func1;
                    getQueue().enqueue(func1, out, 0, in, 0, dim);
                    break;
                case 2:
                    kernel::scan_dim<op, Ti, To, 2, true> func2;
                    getQueue().enqueue(func2, out, 0, in, 0, dim);
                    break;
                case 3:
                    kernel::scan_dim<op, Ti, To, 3, true> func3;
                    getQueue().enqueue(func3, out, 0, in, 0, dim);
                    break;
                case 4:
                    kernel::scan_dim<op, Ti, To, 4, true> func4;
                    getQueue().enqueue(func4, out, 0, in, 0, dim);
                    break;
            }
        } else {
            switch (in.ndims()) {
                case 1:
                    kernel::scan_dim<op, Ti, To, 1, false> func1;
                    getQueue().enqueue(func1, out, 0, in, 0, dim);
                    break;
                case 2:
                    kernel::scan_dim<op, Ti, To, 2, false> func2;
                    getQueue().enqueue(func2, out, 0, in, 0, dim);
                    break;
                case 3:
                    kernel::scan_dim<op, Ti, To, 3, false> func3;
                    getQueue().enqueue(func3, out, 0, in, 0, dim);
                    break;
                case 4:
                    kernel::scan_dim<op, Ti, To, 4, false> func4;
                    getQueue().enqueue(func4, out, 0, in, 0, dim);
                    break;
            }
        }

        return out;
    }

    template<af_op_t op, typename Ti, typename Tk, typename To>
    Array<To> scan(const Array<Tk>& key, const Array<Ti>& in, const int dim, bool inclusive_scan)
    {
        return scan(in, dim, inclusive_scan);
    }

#define INSTANTIATE_SCAN(ROp, Ti, To)\
    template Array<To> scan<ROp, Ti, To>(const Array<Ti> &in, const int dim, bool inclusive_scan);

#define INSTANTIATE_SCAN_BY_KEY(ROp, Ti, Tk, To)\
    template Array<To> scan(const Array<Tk>& key, const Array<Ti>& in, const int dim, bool inclusive_scan);

#define INSTANTIATE_SCAN_ALL(ROp)                       \
    INSTANTIATE_SCAN(ROp, float  , float  )             \
    INSTANTIATE_SCAN(ROp, double , double )             \
    INSTANTIATE_SCAN(ROp, cfloat , cfloat )             \
    INSTANTIATE_SCAN(ROp, cdouble, cdouble)             \
    INSTANTIATE_SCAN(ROp, int    , int    )             \
    INSTANTIATE_SCAN(ROp, uint   , uint   )             \
    INSTANTIATE_SCAN(ROp, intl   , intl   )             \
    INSTANTIATE_SCAN(ROp, uintl  , uintl  )             \
    INSTANTIATE_SCAN(ROp, char   , int    )             \
    INSTANTIATE_SCAN(ROp, char   , uint   )             \
    INSTANTIATE_SCAN(ROp, uchar  , uint   )             \
    INSTANTIATE_SCAN(ROp, short  , int    )             \
    INSTANTIATE_SCAN(ROp, ushort , uint   )

#define INSTANTIATE_SCAN_BY_KEY_ALL(ROp, Tk)            \
    INSTANTIATE_SCAN_BY_KEY(ROp, float  , Tk, float  )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, double , Tk, double )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, cfloat , Tk, cfloat )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, cdouble, Tk, cdouble)  \
    INSTANTIATE_SCAN_BY_KEY(ROp, int    , Tk, int    )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, uint   , Tk, uint   )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, intl   , Tk, intl   )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, uintl  , Tk, uintl  )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, char   , Tk, int    )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, char   , Tk, uint   )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, uchar  , Tk, uint   )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, short  , Tk, int    )  \
    INSTANTIATE_SCAN_BY_KEY(ROp, ushort , Tk, uint   )

#define INSTANTIATE_SCAN_OP(ROp)            \
    INSTANTIATE_SCAN_ALL(ROp)               \
    INSTANTIATE_SCAN_BY_KEY_ALL(ROp, int)   \
    INSTANTIATE_SCAN_BY_KEY_ALL(ROp, uint)  \
    INSTANTIATE_SCAN_BY_KEY_ALL(ROp, long)  \
    INSTANTIATE_SCAN_BY_KEY_ALL(ROp, ulong)

    //accum
    INSTANTIATE_SCAN(af_notzero_t, char, uint)
    INSTANTIATE_SCAN_OP(af_add_t)
    INSTANTIATE_SCAN_OP(af_mul_t)
    INSTANTIATE_SCAN_OP(af_min_t)
    INSTANTIATE_SCAN_OP(af_max_t)
}
