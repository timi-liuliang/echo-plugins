    ///////////////////////////////////////////////////////////////////////////////
    // memfun_funop.hpp
    // Contains overloads of memfun::operator().
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
    template<typename A0>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0);
    }
    template<typename A0 , typename A1>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0 , A1 const &a1) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0 , a1);
    }
    template<typename A0 , typename A1 , typename A2>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0 , A1 const &a1 , A2 const &a2) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0 , a1 , a2);
    }
    template<typename A0 , typename A1 , typename A2 , typename A3>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0 , A1 const &a1 , A2 const &a2 , A3 const &a3) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0 , a1 , a2 , a3);
    }
    template<typename A0 , typename A1 , typename A2 , typename A3 , typename A4>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0 , A1 const &a1 , A2 const &a2 , A3 const &a3 , A4 const &a4) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0 , a1 , a2 , a3 , a4);
    }
    template<typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0 , A1 const &a1 , A2 const &a2 , A3 const &a3 , A4 const &a4 , A5 const &a5) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0 , a1 , a2 , a3 , a4 , a5);
    }
    template<typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0 , A1 const &a1 , A2 const &a2 , A3 const &a3 , A4 const &a4 , A5 const &a5 , A6 const &a6) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0 , a1 , a2 , a3 , a4 , a5 , a6);
    }
    template<typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0 , A1 const &a1 , A2 const &a2 , A3 const &a3 , A4 const &a4 , A5 const &a5 , A6 const &a6 , A7 const &a7) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7);
    }
    template<typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0 , A1 const &a1 , A2 const &a2 , A3 const &a3 , A4 const &a4 , A5 const &a5 , A6 const &a6 , A7 const &a7 , A8 const &a8) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8);
    }
    template<typename A0 , typename A1 , typename A2 , typename A3 , typename A4 , typename A5 , typename A6 , typename A7 , typename A8 , typename A9>
    HBOOST_FORCEINLINE
    result_type operator()(A0 const &a0 , A1 const &a1 , A2 const &a2 , A3 const &a3 , A4 const &a4 , A5 const &a5 , A6 const &a6 , A7 const &a7 , A8 const &a8 , A9 const &a9) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(a0 , a1 , a2 , a3 , a4 , a5 , a6 , a7 , a8 , a9);
    }
