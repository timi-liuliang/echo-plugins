// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_SHARED_PTR_DELETER_DWA2002121_HPP
# define HBOOST_SHARED_PTR_DELETER_DWA2002121_HPP

namespace hboost { namespace python { namespace converter { 

struct HBOOST_PYTHON_DECL shared_ptr_deleter
{
    shared_ptr_deleter(handle<> owner);
    ~shared_ptr_deleter();

    void operator()(void const*);
        
    handle<> owner;
};

}}} // namespace hboost::python::converter

#endif // HBOOST_SHARED_PTR_DELETER_DWA2002121_HPP
