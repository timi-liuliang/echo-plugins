//  (C) Copyright Howard Hinnant
//  (C) Copyright 2011 Vicente J. Botet Escriba
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
// This code was adapted by Vicente from Howard Hinnant's experimental work
// on chrono i/o to Boost

#ifndef HBOOST_CHRONO_IO_DURATION_IO_HPP
#define HBOOST_CHRONO_IO_DURATION_IO_HPP

#include <hboost/chrono/duration.hpp>
#include <hboost/ratio/ratio_io.hpp>
#include <hboost/chrono/io/duration_style.hpp>
#include <hboost/chrono/io/ios_base_state.hpp>
#include <hboost/chrono/io/duration_put.hpp>
#include <hboost/chrono/io/duration_get.hpp>
#include <hboost/chrono/io/utility/manip_base.hpp>
#include <hboost/detail/no_exceptions_support.hpp>
#include <hboost/type_traits/is_integral.hpp>
#include <hboost/type_traits/is_floating_point.hpp>
#include <locale>
#include <iostream>
#include <sstream>

namespace hboost
{
  namespace chrono
  {

    /**
     * duration parameterized manipulator.
     */

    class duration_fmt: public manip<duration_fmt>
    {
      duration_style style_;
    public:

      /**
       * explicit manipulator constructor from a @c duration_style
       */
      explicit duration_fmt(duration_style style)HBOOST_NOEXCEPT
      : style_(style)
      {}

      /**
       * Change the duration_style ios state;
       */
      void operator()(std::ios_base &ios) const

      {
        set_duration_style(ios, style_);
      }
    };

    /**
     * duration_style i/o saver.
     *
     * See Boost.IO i/o state savers for a motivating compression.
     */
    struct duration_style_io_saver
    {

      //! the type of the state to restore
      typedef std::ios_base state_type;
      //! the type of aspect to save
      typedef duration_style aspect_type;

      /**
       * Explicit construction from an i/o stream.
       *
       * Store a reference to the i/o stream and the value of the associated @c duration_style.
       */
      explicit duration_style_io_saver(state_type &s) :
        s_save_(s), a_save_(get_duration_style(s))
      {
      }

      /**
       * Construction from an i/o stream and a @c duration_style to restore.
       *
       * Stores a reference to the i/o stream and the value @c new_value @c duration_style to set.
       */
      duration_style_io_saver(state_type &s, aspect_type new_value) :
        s_save_(s), a_save_(get_duration_style(s))
      {
        set_duration_style(s, new_value);
      }

      /**
       * Destructor.
       *
       * Restores the i/o stream with the duration_style to be restored.
       */
      ~duration_style_io_saver()
      {
        this->restore();
      }

      /**
       * Restores the i/o stream with the duration_style to be restored.
       */
      void restore()
      {
        set_duration_style(s_save_, a_save_);
      }

    private:
      duration_style_io_saver& operator=(duration_style_io_saver const& rhs) ;

      state_type& s_save_;
      aspect_type a_save_;
    };

    template <class Rep>
    struct duration_put_enabled
      : integral_constant<bool,
          is_integral<Rep>::value || is_floating_point<Rep>::value
        >
     {};


    /**
     * duration stream inserter
     * @param os the output stream
     * @param d to value to insert
     * @return @c os
     */

    template <class CharT, class Traits, class Rep, class Period>
    typename hboost::enable_if_c< ! duration_put_enabled<Rep>::value, std::basic_ostream<CharT, Traits>& >::type
    operator<<(std::basic_ostream<CharT, Traits>& os, const duration<Rep, Period>& d)
    {
      std::basic_ostringstream<CharT, Traits> ostr;
      ostr << d.count();
      duration<int, Period> dd(0);
      bool failed = false;
      HBOOST_TRY
      {
        std::ios_base::iostate err = std::ios_base::goodbit;
        HBOOST_TRY
        {
          typename std::basic_ostream<CharT, Traits>::sentry opfx(os);
          if (bool(opfx))
          {
            if (!std::has_facet<duration_put<CharT> >(os.getloc()))
            {
              if (duration_put<CharT> ().put(os, os, os.fill(), dd, ostr.str().c_str()) .failed())
              {
                err = std::ios_base::badbit;
              }
            }
            else if (std::use_facet<duration_put<CharT> >(os.getloc()) .put(os, os, os.fill(), dd, ostr.str().c_str()) .failed())
            {
              err = std::ios_base::badbit;
            }
            os.width(0);
          }
        }
        HBOOST_CATCH(...)
        {
          bool flag = false;
          HBOOST_TRY
          {
            os.setstate(std::ios_base::failbit);
          }
          HBOOST_CATCH (std::ios_base::failure )
          {
            flag = true;
          }
          HBOOST_CATCH_END
          if (flag) throw;
        }
        HBOOST_CATCH_END
        if (err) os.setstate(err);
        return os;
      }
      HBOOST_CATCH(...)
      {
        failed = true;
      }
      HBOOST_CATCH_END
      if (failed) os.setstate(std::ios_base::failbit | std::ios_base::badbit);
      return os;

    }

    template <class CharT, class Traits, class Rep, class Period>
    typename hboost::enable_if_c< duration_put_enabled<Rep>::value, std::basic_ostream<CharT, Traits>& >::type
    operator<<(std::basic_ostream<CharT, Traits>& os, const duration<Rep, Period>& d)
    {
      bool failed = false;
      HBOOST_TRY
      {
        std::ios_base::iostate err = std::ios_base::goodbit;
        HBOOST_TRY
        {
          typename std::basic_ostream<CharT, Traits>::sentry opfx(os);
          if (bool(opfx))
          {
            if (!std::has_facet<duration_put<CharT> >(os.getloc()))
            {
              if (duration_put<CharT> ().put(os, os, os.fill(), d) .failed())
              {
                err = std::ios_base::badbit;
              }
            }
            else if (std::use_facet<duration_put<CharT> >(os.getloc()) .put(os, os, os.fill(), d) .failed())
            {
              err = std::ios_base::badbit;
            }
            os.width(0);
          }
        }
        HBOOST_CATCH(...)
        {
          bool flag = false;
          HBOOST_TRY
          {
            os.setstate(std::ios_base::failbit);
          }
          HBOOST_CATCH (std::ios_base::failure )
          {
            flag = true;
          }
          HBOOST_CATCH_END
          if (flag) throw;
        }
        HBOOST_CATCH_END
        if (err) os.setstate(err);
        return os;
      }
      HBOOST_CATCH(...)
      {
        failed = true;
      }
      HBOOST_CATCH_END
      if (failed) os.setstate(std::ios_base::failbit | std::ios_base::badbit);
      return os;
    }

    /**
     *
     * @param is the input stream
     * @param d the duration
     * @return @c is
     */
    template <class CharT, class Traits, class Rep, class Period>
    std::basic_istream<CharT, Traits>&
    operator>>(std::basic_istream<CharT, Traits>& is, duration<Rep, Period>& d)
    {
      std::ios_base::iostate err = std::ios_base::goodbit;

      HBOOST_TRY
      {
        typename std::basic_istream<CharT, Traits>::sentry ipfx(is);
        if (bool(ipfx))
        {
          if (!std::has_facet<duration_get<CharT> >(is.getloc()))
          {
            duration_get<CharT> ().get(is, std::istreambuf_iterator<CharT, Traits>(), is, err, d);
          }
          else
          {
            std::use_facet<duration_get<CharT> >(is.getloc()) .get(is, std::istreambuf_iterator<CharT, Traits>(), is,
                err, d);
          }
        }
      }
      HBOOST_CATCH (...)
      {
        bool flag = false;
        HBOOST_TRY
        {
          is.setstate(std::ios_base::failbit);
        }
        HBOOST_CATCH (std::ios_base::failure )
        {
          flag = true;
        }
        HBOOST_CATCH_END
        if (flag) { HBOOST_RETHROW }
      }
      HBOOST_CATCH_END
      if (err) is.setstate(err);
      return is;
    }

  } // chrono

}

#endif  // header
