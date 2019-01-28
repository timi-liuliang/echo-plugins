// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_EXCEPTION_TRANSLATOR_DWA2002810_HPP
# define HBOOST_EXCEPTION_TRANSLATOR_DWA2002810_HPP

# include <hboost/python/detail/prefix.hpp>

# include <hboost/bind.hpp>
# include <hboost/bind/placeholders.hpp>
# include <hboost/type.hpp>
# include <hboost/python/detail/translate_exception.hpp>
# include <hboost/python/detail/exception_handler.hpp>

namespace hboost { namespace python { 

template <class ExceptionType, class Translate>
void register_exception_translator(Translate translate, hboost::type<ExceptionType>* = 0)
{
    detail::register_exception_handler(
        hboost::bind<bool>(detail::translate_exception<ExceptionType,Translate>(), _1, _2, translate)
        );
}

}} // namespace hboost::python

#endif // HBOOST_EXCEPTION_TRANSLATOR_DWA2002810_HPP
