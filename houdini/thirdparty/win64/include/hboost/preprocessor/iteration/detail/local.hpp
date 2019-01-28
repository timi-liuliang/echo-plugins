# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# if !defined(HBOOST_PP_LOCAL_LIMITS)
#    error HBOOST_PP_ERROR:  local iteration boundaries are not defined
# elif !defined(HBOOST_PP_LOCAL_MACRO)
#    error HBOOST_PP_ERROR:  local iteration target macro is not defined
# else
#    if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#        define HBOOST_PP_LOCAL_S HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_LOCAL_LIMITS)
#        define HBOOST_PP_LOCAL_F HBOOST_PP_TUPLE_ELEM(2, 1, HBOOST_PP_LOCAL_LIMITS)
#    else
#        define HBOOST_PP_VALUE HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_LOCAL_LIMITS)
#        include <hboost/preprocessor/iteration/detail/start.hpp>
#        define HBOOST_PP_VALUE HBOOST_PP_TUPLE_ELEM(2, 1, HBOOST_PP_LOCAL_LIMITS)
#        include <hboost/preprocessor/iteration/detail/finish.hpp>
#        define HBOOST_PP_LOCAL_S HBOOST_PP_LOCAL_SE()
#        define HBOOST_PP_LOCAL_F HBOOST_PP_LOCAL_FE()
#    endif
# endif
#
# if (HBOOST_PP_LOCAL_S) > (HBOOST_PP_LOCAL_F)
#    include <hboost/preprocessor/iteration/detail/rlocal.hpp>
# else
#    if HBOOST_PP_LOCAL_C(0)
        HBOOST_PP_LOCAL_MACRO(0)
#    endif
#    if HBOOST_PP_LOCAL_C(1)
        HBOOST_PP_LOCAL_MACRO(1)
#    endif
#    if HBOOST_PP_LOCAL_C(2)
        HBOOST_PP_LOCAL_MACRO(2)
#    endif
#    if HBOOST_PP_LOCAL_C(3)
        HBOOST_PP_LOCAL_MACRO(3)
#    endif
#    if HBOOST_PP_LOCAL_C(4)
        HBOOST_PP_LOCAL_MACRO(4)
#    endif
#    if HBOOST_PP_LOCAL_C(5)
        HBOOST_PP_LOCAL_MACRO(5)
#    endif
#    if HBOOST_PP_LOCAL_C(6)
        HBOOST_PP_LOCAL_MACRO(6)
#    endif
#    if HBOOST_PP_LOCAL_C(7)
        HBOOST_PP_LOCAL_MACRO(7)
#    endif
#    if HBOOST_PP_LOCAL_C(8)
        HBOOST_PP_LOCAL_MACRO(8)
#    endif
#    if HBOOST_PP_LOCAL_C(9)
        HBOOST_PP_LOCAL_MACRO(9)
#    endif
#    if HBOOST_PP_LOCAL_C(10)
        HBOOST_PP_LOCAL_MACRO(10)
#    endif
#    if HBOOST_PP_LOCAL_C(11)
        HBOOST_PP_LOCAL_MACRO(11)
#    endif
#    if HBOOST_PP_LOCAL_C(12)
        HBOOST_PP_LOCAL_MACRO(12)
#    endif
#    if HBOOST_PP_LOCAL_C(13)
        HBOOST_PP_LOCAL_MACRO(13)
#    endif
#    if HBOOST_PP_LOCAL_C(14)
        HBOOST_PP_LOCAL_MACRO(14)
#    endif
#    if HBOOST_PP_LOCAL_C(15)
        HBOOST_PP_LOCAL_MACRO(15)
#    endif
#    if HBOOST_PP_LOCAL_C(16)
        HBOOST_PP_LOCAL_MACRO(16)
#    endif
#    if HBOOST_PP_LOCAL_C(17)
        HBOOST_PP_LOCAL_MACRO(17)
#    endif
#    if HBOOST_PP_LOCAL_C(18)
        HBOOST_PP_LOCAL_MACRO(18)
#    endif
#    if HBOOST_PP_LOCAL_C(19)
        HBOOST_PP_LOCAL_MACRO(19)
#    endif
#    if HBOOST_PP_LOCAL_C(20)
        HBOOST_PP_LOCAL_MACRO(20)
#    endif
#    if HBOOST_PP_LOCAL_C(21)
        HBOOST_PP_LOCAL_MACRO(21)
#    endif
#    if HBOOST_PP_LOCAL_C(22)
        HBOOST_PP_LOCAL_MACRO(22)
#    endif
#    if HBOOST_PP_LOCAL_C(23)
        HBOOST_PP_LOCAL_MACRO(23)
#    endif
#    if HBOOST_PP_LOCAL_C(24)
        HBOOST_PP_LOCAL_MACRO(24)
#    endif
#    if HBOOST_PP_LOCAL_C(25)
        HBOOST_PP_LOCAL_MACRO(25)
#    endif
#    if HBOOST_PP_LOCAL_C(26)
        HBOOST_PP_LOCAL_MACRO(26)
#    endif
#    if HBOOST_PP_LOCAL_C(27)
        HBOOST_PP_LOCAL_MACRO(27)
#    endif
#    if HBOOST_PP_LOCAL_C(28)
        HBOOST_PP_LOCAL_MACRO(28)
#    endif
#    if HBOOST_PP_LOCAL_C(29)
        HBOOST_PP_LOCAL_MACRO(29)
#    endif
#    if HBOOST_PP_LOCAL_C(30)
        HBOOST_PP_LOCAL_MACRO(30)
#    endif
#    if HBOOST_PP_LOCAL_C(31)
        HBOOST_PP_LOCAL_MACRO(31)
#    endif
#    if HBOOST_PP_LOCAL_C(32)
        HBOOST_PP_LOCAL_MACRO(32)
#    endif
#    if HBOOST_PP_LOCAL_C(33)
        HBOOST_PP_LOCAL_MACRO(33)
#    endif
#    if HBOOST_PP_LOCAL_C(34)
        HBOOST_PP_LOCAL_MACRO(34)
#    endif
#    if HBOOST_PP_LOCAL_C(35)
        HBOOST_PP_LOCAL_MACRO(35)
#    endif
#    if HBOOST_PP_LOCAL_C(36)
        HBOOST_PP_LOCAL_MACRO(36)
#    endif
#    if HBOOST_PP_LOCAL_C(37)
        HBOOST_PP_LOCAL_MACRO(37)
#    endif
#    if HBOOST_PP_LOCAL_C(38)
        HBOOST_PP_LOCAL_MACRO(38)
#    endif
#    if HBOOST_PP_LOCAL_C(39)
        HBOOST_PP_LOCAL_MACRO(39)
#    endif
#    if HBOOST_PP_LOCAL_C(40)
        HBOOST_PP_LOCAL_MACRO(40)
#    endif
#    if HBOOST_PP_LOCAL_C(41)
        HBOOST_PP_LOCAL_MACRO(41)
#    endif
#    if HBOOST_PP_LOCAL_C(42)
        HBOOST_PP_LOCAL_MACRO(42)
#    endif
#    if HBOOST_PP_LOCAL_C(43)
        HBOOST_PP_LOCAL_MACRO(43)
#    endif
#    if HBOOST_PP_LOCAL_C(44)
        HBOOST_PP_LOCAL_MACRO(44)
#    endif
#    if HBOOST_PP_LOCAL_C(45)
        HBOOST_PP_LOCAL_MACRO(45)
#    endif
#    if HBOOST_PP_LOCAL_C(46)
        HBOOST_PP_LOCAL_MACRO(46)
#    endif
#    if HBOOST_PP_LOCAL_C(47)
        HBOOST_PP_LOCAL_MACRO(47)
#    endif
#    if HBOOST_PP_LOCAL_C(48)
        HBOOST_PP_LOCAL_MACRO(48)
#    endif
#    if HBOOST_PP_LOCAL_C(49)
        HBOOST_PP_LOCAL_MACRO(49)
#    endif
#    if HBOOST_PP_LOCAL_C(50)
        HBOOST_PP_LOCAL_MACRO(50)
#    endif
#    if HBOOST_PP_LOCAL_C(51)
        HBOOST_PP_LOCAL_MACRO(51)
#    endif
#    if HBOOST_PP_LOCAL_C(52)
        HBOOST_PP_LOCAL_MACRO(52)
#    endif
#    if HBOOST_PP_LOCAL_C(53)
        HBOOST_PP_LOCAL_MACRO(53)
#    endif
#    if HBOOST_PP_LOCAL_C(54)
        HBOOST_PP_LOCAL_MACRO(54)
#    endif
#    if HBOOST_PP_LOCAL_C(55)
        HBOOST_PP_LOCAL_MACRO(55)
#    endif
#    if HBOOST_PP_LOCAL_C(56)
        HBOOST_PP_LOCAL_MACRO(56)
#    endif
#    if HBOOST_PP_LOCAL_C(57)
        HBOOST_PP_LOCAL_MACRO(57)
#    endif
#    if HBOOST_PP_LOCAL_C(58)
        HBOOST_PP_LOCAL_MACRO(58)
#    endif
#    if HBOOST_PP_LOCAL_C(59)
        HBOOST_PP_LOCAL_MACRO(59)
#    endif
#    if HBOOST_PP_LOCAL_C(60)
        HBOOST_PP_LOCAL_MACRO(60)
#    endif
#    if HBOOST_PP_LOCAL_C(61)
        HBOOST_PP_LOCAL_MACRO(61)
#    endif
#    if HBOOST_PP_LOCAL_C(62)
        HBOOST_PP_LOCAL_MACRO(62)
#    endif
#    if HBOOST_PP_LOCAL_C(63)
        HBOOST_PP_LOCAL_MACRO(63)
#    endif
#    if HBOOST_PP_LOCAL_C(64)
        HBOOST_PP_LOCAL_MACRO(64)
#    endif
#    if HBOOST_PP_LOCAL_C(65)
        HBOOST_PP_LOCAL_MACRO(65)
#    endif
#    if HBOOST_PP_LOCAL_C(66)
        HBOOST_PP_LOCAL_MACRO(66)
#    endif
#    if HBOOST_PP_LOCAL_C(67)
        HBOOST_PP_LOCAL_MACRO(67)
#    endif
#    if HBOOST_PP_LOCAL_C(68)
        HBOOST_PP_LOCAL_MACRO(68)
#    endif
#    if HBOOST_PP_LOCAL_C(69)
        HBOOST_PP_LOCAL_MACRO(69)
#    endif
#    if HBOOST_PP_LOCAL_C(70)
        HBOOST_PP_LOCAL_MACRO(70)
#    endif
#    if HBOOST_PP_LOCAL_C(71)
        HBOOST_PP_LOCAL_MACRO(71)
#    endif
#    if HBOOST_PP_LOCAL_C(72)
        HBOOST_PP_LOCAL_MACRO(72)
#    endif
#    if HBOOST_PP_LOCAL_C(73)
        HBOOST_PP_LOCAL_MACRO(73)
#    endif
#    if HBOOST_PP_LOCAL_C(74)
        HBOOST_PP_LOCAL_MACRO(74)
#    endif
#    if HBOOST_PP_LOCAL_C(75)
        HBOOST_PP_LOCAL_MACRO(75)
#    endif
#    if HBOOST_PP_LOCAL_C(76)
        HBOOST_PP_LOCAL_MACRO(76)
#    endif
#    if HBOOST_PP_LOCAL_C(77)
        HBOOST_PP_LOCAL_MACRO(77)
#    endif
#    if HBOOST_PP_LOCAL_C(78)
        HBOOST_PP_LOCAL_MACRO(78)
#    endif
#    if HBOOST_PP_LOCAL_C(79)
        HBOOST_PP_LOCAL_MACRO(79)
#    endif
#    if HBOOST_PP_LOCAL_C(80)
        HBOOST_PP_LOCAL_MACRO(80)
#    endif
#    if HBOOST_PP_LOCAL_C(81)
        HBOOST_PP_LOCAL_MACRO(81)
#    endif
#    if HBOOST_PP_LOCAL_C(82)
        HBOOST_PP_LOCAL_MACRO(82)
#    endif
#    if HBOOST_PP_LOCAL_C(83)
        HBOOST_PP_LOCAL_MACRO(83)
#    endif
#    if HBOOST_PP_LOCAL_C(84)
        HBOOST_PP_LOCAL_MACRO(84)
#    endif
#    if HBOOST_PP_LOCAL_C(85)
        HBOOST_PP_LOCAL_MACRO(85)
#    endif
#    if HBOOST_PP_LOCAL_C(86)
        HBOOST_PP_LOCAL_MACRO(86)
#    endif
#    if HBOOST_PP_LOCAL_C(87)
        HBOOST_PP_LOCAL_MACRO(87)
#    endif
#    if HBOOST_PP_LOCAL_C(88)
        HBOOST_PP_LOCAL_MACRO(88)
#    endif
#    if HBOOST_PP_LOCAL_C(89)
        HBOOST_PP_LOCAL_MACRO(89)
#    endif
#    if HBOOST_PP_LOCAL_C(90)
        HBOOST_PP_LOCAL_MACRO(90)
#    endif
#    if HBOOST_PP_LOCAL_C(91)
        HBOOST_PP_LOCAL_MACRO(91)
#    endif
#    if HBOOST_PP_LOCAL_C(92)
        HBOOST_PP_LOCAL_MACRO(92)
#    endif
#    if HBOOST_PP_LOCAL_C(93)
        HBOOST_PP_LOCAL_MACRO(93)
#    endif
#    if HBOOST_PP_LOCAL_C(94)
        HBOOST_PP_LOCAL_MACRO(94)
#    endif
#    if HBOOST_PP_LOCAL_C(95)
        HBOOST_PP_LOCAL_MACRO(95)
#    endif
#    if HBOOST_PP_LOCAL_C(96)
        HBOOST_PP_LOCAL_MACRO(96)
#    endif
#    if HBOOST_PP_LOCAL_C(97)
        HBOOST_PP_LOCAL_MACRO(97)
#    endif
#    if HBOOST_PP_LOCAL_C(98)
        HBOOST_PP_LOCAL_MACRO(98)
#    endif
#    if HBOOST_PP_LOCAL_C(99)
        HBOOST_PP_LOCAL_MACRO(99)
#    endif
#    if HBOOST_PP_LOCAL_C(100)
        HBOOST_PP_LOCAL_MACRO(100)
#    endif
#    if HBOOST_PP_LOCAL_C(101)
        HBOOST_PP_LOCAL_MACRO(101)
#    endif
#    if HBOOST_PP_LOCAL_C(102)
        HBOOST_PP_LOCAL_MACRO(102)
#    endif
#    if HBOOST_PP_LOCAL_C(103)
        HBOOST_PP_LOCAL_MACRO(103)
#    endif
#    if HBOOST_PP_LOCAL_C(104)
        HBOOST_PP_LOCAL_MACRO(104)
#    endif
#    if HBOOST_PP_LOCAL_C(105)
        HBOOST_PP_LOCAL_MACRO(105)
#    endif
#    if HBOOST_PP_LOCAL_C(106)
        HBOOST_PP_LOCAL_MACRO(106)
#    endif
#    if HBOOST_PP_LOCAL_C(107)
        HBOOST_PP_LOCAL_MACRO(107)
#    endif
#    if HBOOST_PP_LOCAL_C(108)
        HBOOST_PP_LOCAL_MACRO(108)
#    endif
#    if HBOOST_PP_LOCAL_C(109)
        HBOOST_PP_LOCAL_MACRO(109)
#    endif
#    if HBOOST_PP_LOCAL_C(110)
        HBOOST_PP_LOCAL_MACRO(110)
#    endif
#    if HBOOST_PP_LOCAL_C(111)
        HBOOST_PP_LOCAL_MACRO(111)
#    endif
#    if HBOOST_PP_LOCAL_C(112)
        HBOOST_PP_LOCAL_MACRO(112)
#    endif
#    if HBOOST_PP_LOCAL_C(113)
        HBOOST_PP_LOCAL_MACRO(113)
#    endif
#    if HBOOST_PP_LOCAL_C(114)
        HBOOST_PP_LOCAL_MACRO(114)
#    endif
#    if HBOOST_PP_LOCAL_C(115)
        HBOOST_PP_LOCAL_MACRO(115)
#    endif
#    if HBOOST_PP_LOCAL_C(116)
        HBOOST_PP_LOCAL_MACRO(116)
#    endif
#    if HBOOST_PP_LOCAL_C(117)
        HBOOST_PP_LOCAL_MACRO(117)
#    endif
#    if HBOOST_PP_LOCAL_C(118)
        HBOOST_PP_LOCAL_MACRO(118)
#    endif
#    if HBOOST_PP_LOCAL_C(119)
        HBOOST_PP_LOCAL_MACRO(119)
#    endif
#    if HBOOST_PP_LOCAL_C(120)
        HBOOST_PP_LOCAL_MACRO(120)
#    endif
#    if HBOOST_PP_LOCAL_C(121)
        HBOOST_PP_LOCAL_MACRO(121)
#    endif
#    if HBOOST_PP_LOCAL_C(122)
        HBOOST_PP_LOCAL_MACRO(122)
#    endif
#    if HBOOST_PP_LOCAL_C(123)
        HBOOST_PP_LOCAL_MACRO(123)
#    endif
#    if HBOOST_PP_LOCAL_C(124)
        HBOOST_PP_LOCAL_MACRO(124)
#    endif
#    if HBOOST_PP_LOCAL_C(125)
        HBOOST_PP_LOCAL_MACRO(125)
#    endif
#    if HBOOST_PP_LOCAL_C(126)
        HBOOST_PP_LOCAL_MACRO(126)
#    endif
#    if HBOOST_PP_LOCAL_C(127)
        HBOOST_PP_LOCAL_MACRO(127)
#    endif
#    if HBOOST_PP_LOCAL_C(128)
        HBOOST_PP_LOCAL_MACRO(128)
#    endif
#    if HBOOST_PP_LOCAL_C(129)
        HBOOST_PP_LOCAL_MACRO(129)
#    endif
#    if HBOOST_PP_LOCAL_C(130)
        HBOOST_PP_LOCAL_MACRO(130)
#    endif
#    if HBOOST_PP_LOCAL_C(131)
        HBOOST_PP_LOCAL_MACRO(131)
#    endif
#    if HBOOST_PP_LOCAL_C(132)
        HBOOST_PP_LOCAL_MACRO(132)
#    endif
#    if HBOOST_PP_LOCAL_C(133)
        HBOOST_PP_LOCAL_MACRO(133)
#    endif
#    if HBOOST_PP_LOCAL_C(134)
        HBOOST_PP_LOCAL_MACRO(134)
#    endif
#    if HBOOST_PP_LOCAL_C(135)
        HBOOST_PP_LOCAL_MACRO(135)
#    endif
#    if HBOOST_PP_LOCAL_C(136)
        HBOOST_PP_LOCAL_MACRO(136)
#    endif
#    if HBOOST_PP_LOCAL_C(137)
        HBOOST_PP_LOCAL_MACRO(137)
#    endif
#    if HBOOST_PP_LOCAL_C(138)
        HBOOST_PP_LOCAL_MACRO(138)
#    endif
#    if HBOOST_PP_LOCAL_C(139)
        HBOOST_PP_LOCAL_MACRO(139)
#    endif
#    if HBOOST_PP_LOCAL_C(140)
        HBOOST_PP_LOCAL_MACRO(140)
#    endif
#    if HBOOST_PP_LOCAL_C(141)
        HBOOST_PP_LOCAL_MACRO(141)
#    endif
#    if HBOOST_PP_LOCAL_C(142)
        HBOOST_PP_LOCAL_MACRO(142)
#    endif
#    if HBOOST_PP_LOCAL_C(143)
        HBOOST_PP_LOCAL_MACRO(143)
#    endif
#    if HBOOST_PP_LOCAL_C(144)
        HBOOST_PP_LOCAL_MACRO(144)
#    endif
#    if HBOOST_PP_LOCAL_C(145)
        HBOOST_PP_LOCAL_MACRO(145)
#    endif
#    if HBOOST_PP_LOCAL_C(146)
        HBOOST_PP_LOCAL_MACRO(146)
#    endif
#    if HBOOST_PP_LOCAL_C(147)
        HBOOST_PP_LOCAL_MACRO(147)
#    endif
#    if HBOOST_PP_LOCAL_C(148)
        HBOOST_PP_LOCAL_MACRO(148)
#    endif
#    if HBOOST_PP_LOCAL_C(149)
        HBOOST_PP_LOCAL_MACRO(149)
#    endif
#    if HBOOST_PP_LOCAL_C(150)
        HBOOST_PP_LOCAL_MACRO(150)
#    endif
#    if HBOOST_PP_LOCAL_C(151)
        HBOOST_PP_LOCAL_MACRO(151)
#    endif
#    if HBOOST_PP_LOCAL_C(152)
        HBOOST_PP_LOCAL_MACRO(152)
#    endif
#    if HBOOST_PP_LOCAL_C(153)
        HBOOST_PP_LOCAL_MACRO(153)
#    endif
#    if HBOOST_PP_LOCAL_C(154)
        HBOOST_PP_LOCAL_MACRO(154)
#    endif
#    if HBOOST_PP_LOCAL_C(155)
        HBOOST_PP_LOCAL_MACRO(155)
#    endif
#    if HBOOST_PP_LOCAL_C(156)
        HBOOST_PP_LOCAL_MACRO(156)
#    endif
#    if HBOOST_PP_LOCAL_C(157)
        HBOOST_PP_LOCAL_MACRO(157)
#    endif
#    if HBOOST_PP_LOCAL_C(158)
        HBOOST_PP_LOCAL_MACRO(158)
#    endif
#    if HBOOST_PP_LOCAL_C(159)
        HBOOST_PP_LOCAL_MACRO(159)
#    endif
#    if HBOOST_PP_LOCAL_C(160)
        HBOOST_PP_LOCAL_MACRO(160)
#    endif
#    if HBOOST_PP_LOCAL_C(161)
        HBOOST_PP_LOCAL_MACRO(161)
#    endif
#    if HBOOST_PP_LOCAL_C(162)
        HBOOST_PP_LOCAL_MACRO(162)
#    endif
#    if HBOOST_PP_LOCAL_C(163)
        HBOOST_PP_LOCAL_MACRO(163)
#    endif
#    if HBOOST_PP_LOCAL_C(164)
        HBOOST_PP_LOCAL_MACRO(164)
#    endif
#    if HBOOST_PP_LOCAL_C(165)
        HBOOST_PP_LOCAL_MACRO(165)
#    endif
#    if HBOOST_PP_LOCAL_C(166)
        HBOOST_PP_LOCAL_MACRO(166)
#    endif
#    if HBOOST_PP_LOCAL_C(167)
        HBOOST_PP_LOCAL_MACRO(167)
#    endif
#    if HBOOST_PP_LOCAL_C(168)
        HBOOST_PP_LOCAL_MACRO(168)
#    endif
#    if HBOOST_PP_LOCAL_C(169)
        HBOOST_PP_LOCAL_MACRO(169)
#    endif
#    if HBOOST_PP_LOCAL_C(170)
        HBOOST_PP_LOCAL_MACRO(170)
#    endif
#    if HBOOST_PP_LOCAL_C(171)
        HBOOST_PP_LOCAL_MACRO(171)
#    endif
#    if HBOOST_PP_LOCAL_C(172)
        HBOOST_PP_LOCAL_MACRO(172)
#    endif
#    if HBOOST_PP_LOCAL_C(173)
        HBOOST_PP_LOCAL_MACRO(173)
#    endif
#    if HBOOST_PP_LOCAL_C(174)
        HBOOST_PP_LOCAL_MACRO(174)
#    endif
#    if HBOOST_PP_LOCAL_C(175)
        HBOOST_PP_LOCAL_MACRO(175)
#    endif
#    if HBOOST_PP_LOCAL_C(176)
        HBOOST_PP_LOCAL_MACRO(176)
#    endif
#    if HBOOST_PP_LOCAL_C(177)
        HBOOST_PP_LOCAL_MACRO(177)
#    endif
#    if HBOOST_PP_LOCAL_C(178)
        HBOOST_PP_LOCAL_MACRO(178)
#    endif
#    if HBOOST_PP_LOCAL_C(179)
        HBOOST_PP_LOCAL_MACRO(179)
#    endif
#    if HBOOST_PP_LOCAL_C(180)
        HBOOST_PP_LOCAL_MACRO(180)
#    endif
#    if HBOOST_PP_LOCAL_C(181)
        HBOOST_PP_LOCAL_MACRO(181)
#    endif
#    if HBOOST_PP_LOCAL_C(182)
        HBOOST_PP_LOCAL_MACRO(182)
#    endif
#    if HBOOST_PP_LOCAL_C(183)
        HBOOST_PP_LOCAL_MACRO(183)
#    endif
#    if HBOOST_PP_LOCAL_C(184)
        HBOOST_PP_LOCAL_MACRO(184)
#    endif
#    if HBOOST_PP_LOCAL_C(185)
        HBOOST_PP_LOCAL_MACRO(185)
#    endif
#    if HBOOST_PP_LOCAL_C(186)
        HBOOST_PP_LOCAL_MACRO(186)
#    endif
#    if HBOOST_PP_LOCAL_C(187)
        HBOOST_PP_LOCAL_MACRO(187)
#    endif
#    if HBOOST_PP_LOCAL_C(188)
        HBOOST_PP_LOCAL_MACRO(188)
#    endif
#    if HBOOST_PP_LOCAL_C(189)
        HBOOST_PP_LOCAL_MACRO(189)
#    endif
#    if HBOOST_PP_LOCAL_C(190)
        HBOOST_PP_LOCAL_MACRO(190)
#    endif
#    if HBOOST_PP_LOCAL_C(191)
        HBOOST_PP_LOCAL_MACRO(191)
#    endif
#    if HBOOST_PP_LOCAL_C(192)
        HBOOST_PP_LOCAL_MACRO(192)
#    endif
#    if HBOOST_PP_LOCAL_C(193)
        HBOOST_PP_LOCAL_MACRO(193)
#    endif
#    if HBOOST_PP_LOCAL_C(194)
        HBOOST_PP_LOCAL_MACRO(194)
#    endif
#    if HBOOST_PP_LOCAL_C(195)
        HBOOST_PP_LOCAL_MACRO(195)
#    endif
#    if HBOOST_PP_LOCAL_C(196)
        HBOOST_PP_LOCAL_MACRO(196)
#    endif
#    if HBOOST_PP_LOCAL_C(197)
        HBOOST_PP_LOCAL_MACRO(197)
#    endif
#    if HBOOST_PP_LOCAL_C(198)
        HBOOST_PP_LOCAL_MACRO(198)
#    endif
#    if HBOOST_PP_LOCAL_C(199)
        HBOOST_PP_LOCAL_MACRO(199)
#    endif
#    if HBOOST_PP_LOCAL_C(200)
        HBOOST_PP_LOCAL_MACRO(200)
#    endif
#    if HBOOST_PP_LOCAL_C(201)
        HBOOST_PP_LOCAL_MACRO(201)
#    endif
#    if HBOOST_PP_LOCAL_C(202)
        HBOOST_PP_LOCAL_MACRO(202)
#    endif
#    if HBOOST_PP_LOCAL_C(203)
        HBOOST_PP_LOCAL_MACRO(203)
#    endif
#    if HBOOST_PP_LOCAL_C(204)
        HBOOST_PP_LOCAL_MACRO(204)
#    endif
#    if HBOOST_PP_LOCAL_C(205)
        HBOOST_PP_LOCAL_MACRO(205)
#    endif
#    if HBOOST_PP_LOCAL_C(206)
        HBOOST_PP_LOCAL_MACRO(206)
#    endif
#    if HBOOST_PP_LOCAL_C(207)
        HBOOST_PP_LOCAL_MACRO(207)
#    endif
#    if HBOOST_PP_LOCAL_C(208)
        HBOOST_PP_LOCAL_MACRO(208)
#    endif
#    if HBOOST_PP_LOCAL_C(209)
        HBOOST_PP_LOCAL_MACRO(209)
#    endif
#    if HBOOST_PP_LOCAL_C(210)
        HBOOST_PP_LOCAL_MACRO(210)
#    endif
#    if HBOOST_PP_LOCAL_C(211)
        HBOOST_PP_LOCAL_MACRO(211)
#    endif
#    if HBOOST_PP_LOCAL_C(212)
        HBOOST_PP_LOCAL_MACRO(212)
#    endif
#    if HBOOST_PP_LOCAL_C(213)
        HBOOST_PP_LOCAL_MACRO(213)
#    endif
#    if HBOOST_PP_LOCAL_C(214)
        HBOOST_PP_LOCAL_MACRO(214)
#    endif
#    if HBOOST_PP_LOCAL_C(215)
        HBOOST_PP_LOCAL_MACRO(215)
#    endif
#    if HBOOST_PP_LOCAL_C(216)
        HBOOST_PP_LOCAL_MACRO(216)
#    endif
#    if HBOOST_PP_LOCAL_C(217)
        HBOOST_PP_LOCAL_MACRO(217)
#    endif
#    if HBOOST_PP_LOCAL_C(218)
        HBOOST_PP_LOCAL_MACRO(218)
#    endif
#    if HBOOST_PP_LOCAL_C(219)
        HBOOST_PP_LOCAL_MACRO(219)
#    endif
#    if HBOOST_PP_LOCAL_C(220)
        HBOOST_PP_LOCAL_MACRO(220)
#    endif
#    if HBOOST_PP_LOCAL_C(221)
        HBOOST_PP_LOCAL_MACRO(221)
#    endif
#    if HBOOST_PP_LOCAL_C(222)
        HBOOST_PP_LOCAL_MACRO(222)
#    endif
#    if HBOOST_PP_LOCAL_C(223)
        HBOOST_PP_LOCAL_MACRO(223)
#    endif
#    if HBOOST_PP_LOCAL_C(224)
        HBOOST_PP_LOCAL_MACRO(224)
#    endif
#    if HBOOST_PP_LOCAL_C(225)
        HBOOST_PP_LOCAL_MACRO(225)
#    endif
#    if HBOOST_PP_LOCAL_C(226)
        HBOOST_PP_LOCAL_MACRO(226)
#    endif
#    if HBOOST_PP_LOCAL_C(227)
        HBOOST_PP_LOCAL_MACRO(227)
#    endif
#    if HBOOST_PP_LOCAL_C(228)
        HBOOST_PP_LOCAL_MACRO(228)
#    endif
#    if HBOOST_PP_LOCAL_C(229)
        HBOOST_PP_LOCAL_MACRO(229)
#    endif
#    if HBOOST_PP_LOCAL_C(230)
        HBOOST_PP_LOCAL_MACRO(230)
#    endif
#    if HBOOST_PP_LOCAL_C(231)
        HBOOST_PP_LOCAL_MACRO(231)
#    endif
#    if HBOOST_PP_LOCAL_C(232)
        HBOOST_PP_LOCAL_MACRO(232)
#    endif
#    if HBOOST_PP_LOCAL_C(233)
        HBOOST_PP_LOCAL_MACRO(233)
#    endif
#    if HBOOST_PP_LOCAL_C(234)
        HBOOST_PP_LOCAL_MACRO(234)
#    endif
#    if HBOOST_PP_LOCAL_C(235)
        HBOOST_PP_LOCAL_MACRO(235)
#    endif
#    if HBOOST_PP_LOCAL_C(236)
        HBOOST_PP_LOCAL_MACRO(236)
#    endif

#    if HBOOST_PP_LOCAL_C(237)
        HBOOST_PP_LOCAL_MACRO(237)
#    endif
#    if HBOOST_PP_LOCAL_C(238)
        HBOOST_PP_LOCAL_MACRO(238)
#    endif
#    if HBOOST_PP_LOCAL_C(239)
        HBOOST_PP_LOCAL_MACRO(239)
#    endif
#    if HBOOST_PP_LOCAL_C(240)
        HBOOST_PP_LOCAL_MACRO(240)
#    endif
#    if HBOOST_PP_LOCAL_C(241)
        HBOOST_PP_LOCAL_MACRO(241)
#    endif
#    if HBOOST_PP_LOCAL_C(242)
        HBOOST_PP_LOCAL_MACRO(242)
#    endif
#    if HBOOST_PP_LOCAL_C(243)
        HBOOST_PP_LOCAL_MACRO(243)
#    endif
#    if HBOOST_PP_LOCAL_C(244)
        HBOOST_PP_LOCAL_MACRO(244)
#    endif
#    if HBOOST_PP_LOCAL_C(245)
        HBOOST_PP_LOCAL_MACRO(245)
#    endif
#    if HBOOST_PP_LOCAL_C(246)
        HBOOST_PP_LOCAL_MACRO(246)
#    endif
#    if HBOOST_PP_LOCAL_C(247)
        HBOOST_PP_LOCAL_MACRO(247)
#    endif
#    if HBOOST_PP_LOCAL_C(248)
        HBOOST_PP_LOCAL_MACRO(248)
#    endif
#    if HBOOST_PP_LOCAL_C(249)
        HBOOST_PP_LOCAL_MACRO(249)
#    endif
#    if HBOOST_PP_LOCAL_C(250)
        HBOOST_PP_LOCAL_MACRO(250)
#    endif
#    if HBOOST_PP_LOCAL_C(251)
        HBOOST_PP_LOCAL_MACRO(251)
#    endif
#    if HBOOST_PP_LOCAL_C(252)
        HBOOST_PP_LOCAL_MACRO(252)
#    endif
#    if HBOOST_PP_LOCAL_C(253)
        HBOOST_PP_LOCAL_MACRO(253)
#    endif
#    if HBOOST_PP_LOCAL_C(254)
        HBOOST_PP_LOCAL_MACRO(254)
#    endif
#    if HBOOST_PP_LOCAL_C(255)
        HBOOST_PP_LOCAL_MACRO(255)
#    endif
#    if HBOOST_PP_LOCAL_C(256)
        HBOOST_PP_LOCAL_MACRO(256)
#    endif
# endif
#
# undef HBOOST_PP_LOCAL_LIMITS
#
# undef HBOOST_PP_LOCAL_S
# undef HBOOST_PP_LOCAL_F
#
# undef HBOOST_PP_LOCAL_MACRO
