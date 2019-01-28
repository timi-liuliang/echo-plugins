/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_IntrinsicMacros.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_IntrinsicMacros__
#define __GA_IntrinsicMacros__

#include <GA/GA_IntrinsicDef.h>
#include <GA/GA_IntrinsicEval.h>
#include <UT/UT_Assert.h>
#include <UT/UT_StringArray.h>
#include <stdio.h>

///
/// The intrinsic macros help declaring intrinsic attributes for an object
/// (i.e. a primitive).
/// Typically, you'll have code like @code
///	enum {
///	  ga_INTRINISIC_VAR1,
///	  ga_INTRINISIC_VAR2,
///	  ga_NUM_INTRINSICS
///	}
///	GA_START_INTRINSIC_DEF(ClassName, ga_NUM_INTRINSICS)
///	    GA_INTRINSIC_I(ClassName, ga_INTRINSIC_VAR1, "name1",
///			EvaluationCallback)
///	    GA_INTRINSIC_VARYING_F(ClassName, ga_INTRINISIC_VAR2, "name2",
///			TupleSizeMethod,
///			EvaluationCallback)
///	GA_END_INTRINSIC_DEF(ClassName, BaseClassName)
/// @endcode
///
/// There are several ways to declare intrinsic access methods:
///
///  GA_INTRINSIC_I(ClassName, Enum, Name, Callback)
///	- Tuple size is fixed at 1
///	- Callback returns value:
///		static exint callback(const ClassName *obj);
///		static fpreal callback(const ClassName *obj);
///		static const char *callback(const ClassName *obj);
///
///  GA_INTRINSIC_CONST_I(ClassName, Enum, Name, Value)
///	- Tuple size is fixed at 1
///	- int/float/(const char *) Value is specified
///
///  GA_INTRINSIC_METHOD_I(ClassName, Enum, Name, MethodName)
///	- Tuple size is fixed at 1
///	- MethodName specifies a public method on ClassName which returns the
///	  value.
///		exint ClassName::MethodName() const;
///		fpreal ClassName::MethodName() const;
///		const char *ClassName::MethodName() const;
///
///  GA_INTRINSIC_TUPLE_I(ClassName, Enum, Name, TUPLE_SIZE, Callback)
///	- TUPLE_SIZE is a constant value
///	- Callback fills value (returning number of items set):
///	    static int callback(const ClassName *obj, int64 *v, GA_Size sz);
///	    static int callback(const ClassName *obj, fpreal64 *v, GA_Size sz);
///	    static void callback(const ClassName *obj, UT_StringArray &v);
///
///  GA_INTRINSIC_TUPLE_METHOD_I(ClassName, Enum, Name, TUPLE_SIZE, MethodName)
///	- TUPLE_SIZE is a constant value
///	- MethodName is a public method on ClassName which fills the value,
///	  returning the number of items set:
///		int ClassName::MethodName(int64 *v, GA_Size sz) const;
///		int ClassName::MethodName(fpreal64 *v, GA_Size sz) const;
///		void ClassName::MethodName(UT_StringArray &v, GA_Size sz) const;
///
///  GA_INTRINSIC_ARRAY_I(ClassName, Enum, Name, ArrayType, MethodName)
///	- ArrayType specifies an array which has:
///		- entries() const: Return number of elements in the array
///		- operator()(exint i) const:  Returning the element of the array
///	- MethodName is a public method on ClassName which returns the a const
///	  reference to an ArrayType object.
///		const ArrayType &ClassName::MethodName() const
///
///  GA_INTRINSIC_VARYING_I(ClassName, Enum, Name, TupleSizeCallback, Callback)
///	- TupleSizeCallback is a function returning the tuple size
///		static int tuplesizecallback(const ClassName *obj)
///	- Callback fills value, returning number of items set:
///	    static int callback(const ClassName *obj, int64 *v, GA_Size sz);
///	    static int callback(const ClassName *obj, fpreal64 *v, GA_Size sz);
///	    static int callback(const ClassName *obj, UT_StringArray &v);
///
/// For setting values, there are similar macros.  The tuple size and name are
/// usually defined by the evaluation macros.  If there is no "get" method, you
/// can still declare the intrinsic using:
///	GA_INTRINSIC_DEF_I(Enum, Name, TupleSize)
///	GA_INTRINSIC_DEF_VARYING_I(Enum, Name, TupleSizeFunc)
///
///  GA_INTRINSIC_SET_I(ClassName, Enum, Callback)
///	- Callback specifies a static function which sets the value, returning
///	  false if unable to set (i.e. invalid value).  The function can take a
///	  32/64 bit value.
///		static bool set(ClassName *o, exint v);
///		static bool set(ClassName *o, fpreal v);
///		static bool set(ClassName *o, const char *v);
///
///  GA_INTRINSIC_SET_METHOD_I(ClassName, Enum, MethodName)
///	- MethodName specifies a public method on ClassName which sets the
///	  value.  The method can take 32/64 bit values
///		bool ClassName::MethodName(exint value);
///		bool ClassName::MethodName(fpreal value);
///
///  GA_INTRINSIC_SET_TUPLE_I(ClassName, Enum, Callback)
///	- Callback specifies a static function which sets the value, returning
///	  the number of values set.
///		static int set(ClassName *o, const int64 *v, int tuple_size);
///		static int set(ClassName *o, const fpreal64 *v, int tuple_size);
///		static int set(ClassName *o, const UT_StringArray &v);
///
///  GA_INTRINSIC_SET_TUPLE_METHOD_I(ClassName, Enum, Callback)
///	- Callback specifies a static function which sets the value, returning
///	  the number of values set.
///		int ClassName::set(const int64 *v, int tuple_size);
///		int ClassName::set(const fpreal64 *v, int tuple_size);
///		int ClassName::set(const UT_StringArray &v);

#define GA_START_INTRINSIC_DEF(CLASS, NUM_INTRINSICS) \
namespace \
{ \
    class _ga_intrinsicEvaluator##CLASS \
    { \
    public: \
	typedef GA_Size (*cbTupleSize)(const CLASS *obj); \
	typedef GA_Size (*cbEvalI)(const CLASS *obj, int64 *v, GA_Size size); \
	typedef GA_Size (*cbEvalF)(const CLASS *obj, fpreal64 *v, GA_Size size); \
	typedef GA_Size (*cbEvalS)(const CLASS *obj, UT_String &s); \
	typedef GA_Size (*cbEvalSA)(const CLASS *obj, UT_StringArray &s); \
	typedef GA_Size (*cbSetI)(CLASS *obj, const int64 *v, GA_Size size); \
	typedef GA_Size (*cbSetF)(CLASS *obj, const fpreal64 *v, GA_Size size); \
	typedef GA_Size (*cbSetS)(CLASS *obj, const char **v, GA_Size size); \
	typedef GA_Size (*cbSetSA)(CLASS *obj, const UT_StringArray &v); \
	static GA_Size failTS(const CLASS *) { return 0; } \
	static GA_Size failI(const CLASS *, int64 *, GA_Size) { UT_ASSERT(0); return 0; } \
	static GA_Size failF(const CLASS *, fpreal64 *, GA_Size) { UT_ASSERT(0); return 0; } \
	static GA_Size failS(const CLASS *, UT_String &) { UT_ASSERT(0); return 0; } \
	static GA_Size failSA(const CLASS *, UT_StringArray &) { UT_ASSERT(0); return 0; } \
	static GA_Size failI(CLASS *, const int64 *, GA_Size) { UT_ASSERT(0); return 0; } \
	static GA_Size failF(CLASS *, const fpreal64 *, GA_Size) { UT_ASSERT(0); return 0; } \
	static GA_Size failS(CLASS *, const char **, GA_Size) { UT_ASSERT(0); return 0; } \
	static GA_Size failSA(CLASS *, const UT_StringArray &) { UT_ASSERT(0); return 0; } \
	class Eval \
	{ \
	public: \
	    Eval() \
		: myId(-1) \
		, myName(NULL) \
		, myOptions(NULL) \
		, myStore(GA_STORECLASS_INVALID) \
		, myTupleSizeCB(failTS) \
		, myTupleSize(-1) \
		, myI(failI) \
		, myF(failF) \
		, myS(failS) \
		, mySA(failSA) \
		, mySetI(failI) \
		, mySetF(failF) \
		, mySetSS(failS) \
		, mySetSA(failSA) \
		, myReadOnly(true) \
	    { \
	    } \
	    GA_Size	getTupleSize(const CLASS *o) const \
		{ \
		    if (myTupleSize > 0) return myTupleSize; \
		    return myTupleSizeCB(o); \
		} \
	    void dump() { \
		fprintf(stderr, "Intrinsic[%d] := {\n", myId); \
		fprintf(stderr, " 'name'    : '%s'\n", myName); \
		fprintf(stderr, " 'tuple'   : %d\n", (int)myTupleSize); \
		fprintf(stderr, " 'storage' : %s\n", GAstorageClass(myStore));\
		fprintf(stderr, " 'ro'      : %d\n", myReadOnly); \
		fprintf(stderr, " 'tuple()' : %p\n", myTupleSizeCB); \
		fprintf(stderr, " 'options' : %p\n", myOptions); \
		fprintf(stderr, " 'I()'     : %p\n", myI); \
		fprintf(stderr, " 'F()'     : %p\n", myF); \
		fprintf(stderr, " 'S()'     : %p\n", myS); \
		fprintf(stderr, " 'SA()'    : %p\n", mySA); \
		fprintf(stderr, " 'setI()'  : %p\n", mySetI); \
		fprintf(stderr, " 'setF()'  : %p\n", mySetF); \
		fprintf(stderr, " 'setSS()' : %p\n", mySetSS); \
		fprintf(stderr, " 'setSA()' : %p\n", mySetSA); \
	    } \
	    bool	valid(int index) const \
	    { \
		if (myName && myId >= 0 && myStore != GA_STORECLASS_INVALID \
			&& (myTupleSize > 0 || myTupleSizeCB) \
			&& (myI || myF || myS || mySA)) \
		    return true; \
		fprintf(stderr, "Warning: missing intrinsic for %s[%d]\n", \
			#CLASS, index); \
		return false; \
	    } \
	    cbTupleSize		 myTupleSizeCB; \
	    cbEvalI		 myI; \
	    cbEvalF		 myF; \
	    cbEvalS		 myS; \
	    cbEvalSA		 mySA; \
	    cbSetI		 mySetI; \
	    cbSetF		 mySetF; \
	    cbSetS		 mySetSS; \
	    cbSetSA		 mySetSA; \
	    const char		*myName; \
	    const UT_Options	*myOptions; \
	    GA_Size		 myTupleSize; \
	    int			 myId; \
	    GA_StorageClass	 myStore; \
	    bool		 myReadOnly; \
	}; \
	Eval	myEval[NUM_INTRINSICS]; \
	int	entries() const		{ return NUM_INTRINSICS; } \
	bool	valid(int i) const	{ return myEval[i].valid(i); } \
	GA_Size	getTupleSize(int i, const CLASS *o) const \
		    { return myEval[i].getTupleSize(o); } \
	GA_Size	evalI(int i, const CLASS *o, int64 *v, GA_Size n) const \
		    { return myEval[i].myI(o, v, n); } \
	GA_Size	evalF(int i, const CLASS *o, fpreal64 *v, GA_Size n) const \
		    { return myEval[i].myF(o, v, n); } \
	GA_Size	evalS(int i, const CLASS *o, UT_String &v) const \
		    { return myEval[i].myS(o, v); } \
	GA_Size	evalSA(int i, const CLASS *o, UT_StringArray &v) const \
		    { return myEval[i].mySA(o, v); } \
	GA_Size	setI(int i, CLASS *o, const int64 *v, GA_Size n) const \
		    { return myEval[i].mySetI(o, v, n); } \
	GA_Size	setF(int i, CLASS *o, const fpreal64 *v, GA_Size n) const \
		    { return myEval[i].mySetF(o, v, n); } \
	GA_Size	setSS(int i, CLASS *o, const char **v, GA_Size n) const \
		    { return myEval[i].mySetSS(o, v, n); } \
	GA_Size	setSA(int i, CLASS *o, const UT_StringArray &v) const \
		    { return myEval[i].mySetSA(o, v); } \
	GA_StorageClass	getStorageClass(int i) const  \
		    { return myEval[i].myStore; } \
	const char	*getName(int i) const	{ return myEval[i].myName; } \
	int	 getId(int i) const	{ return myEval[i].myId; } \
	bool	 getReadOnly(int i) const { return myEval[i].myReadOnly; } \
	const UT_Options	*getOptions(int i) const \
		    { return myEval[i].myOptions; } \
	_ga_intrinsicEvaluator##CLASS() {

#define GA_END_INTRINSIC_DEF(CLASS, BASECLASS) \
	} \
    }; \
    static GA_IntrinsicDef			_theIntrinsics##CLASS; \
    static _ga_intrinsicEvaluator##CLASS	_theEval##CLASS; \
    } \
    GA_IntrinsicManager::Registrar \
    CLASS::registerIntrinsics(GA_PrimitiveDefinition &defn) \
    { \
	GA_IntrinsicManager::Registrar	r(BASECLASS::registerIntrinsics(defn)); \
	if (r.start(_theIntrinsics##CLASS)) \
	{ \
	    for (int i = 0; i < _theEval##CLASS.entries(); ++i) \
	    { \
		UT_ASSERT(_theEval##CLASS.getName(i)); \
		if (_theEval##CLASS.valid(i)) \
		{ \
		    r.addAttribute(_theEval##CLASS.getStorageClass(i), \
			_theEval##CLASS.getName(i), \
			_theEval##CLASS.getId(i), \
			_theEval##CLASS.getReadOnly(i), \
			_theEval##CLASS.getOptions(i)); \
		} \
	    } \
	} \
	return r; \
    } \
    GA_Size \
    CLASS::localIntrinsicTupleSize(const GA_IntrinsicEval &eval) const \
    { \
	int	id = eval.getUserId(_theIntrinsics##CLASS); \
	if (id >= 0 && id < _theEval##CLASS.entries()) \
	    return _theEval##CLASS.getTupleSize(id, this); \
	return BASECLASS::localIntrinsicTupleSize(eval); \
    } \
    GA_Size \
    CLASS::localGetIntrinsicI(const GA_IntrinsicEval &eval, \
		int64 *v, GA_Size size) const \
    { \
	int	id = eval.getUserId(_theIntrinsics##CLASS); \
	if (id >= 0 && id < _theEval##CLASS.entries()) \
	    return _theEval##CLASS.evalI(id, this, v, size); \
	return BASECLASS::localGetIntrinsicI(eval, v, size); \
    } \
    GA_Size \
    CLASS::localGetIntrinsicF(const GA_IntrinsicEval &eval, \
		fpreal64 *v, GA_Size size) const \
    { \
	int	id = eval.getUserId(_theIntrinsics##CLASS); \
	if (id >= 0 && id < _theEval##CLASS.entries()) \
	    return _theEval##CLASS.evalF(id, this, v, size); \
	return BASECLASS::localGetIntrinsicF(eval, v, size); \
    } \
    GA_Size \
    CLASS::localGetIntrinsicS(const GA_IntrinsicEval &eval, \
		UT_String &v) const \
    { \
	int	id = eval.getUserId(_theIntrinsics##CLASS); \
	if (id >= 0 && id < _theEval##CLASS.entries()) \
	    return _theEval##CLASS.evalS(id, this, v); \
	return BASECLASS::localGetIntrinsicS(eval, v); \
    } \
    GA_Size \
    CLASS::localGetIntrinsicSA(const GA_IntrinsicEval &eval, \
		UT_StringArray &v) const \
    { \
	int	id = eval.getUserId(_theIntrinsics##CLASS); \
	if (id >= 0 && id < _theEval##CLASS.entries()) \
	    return _theEval##CLASS.evalSA(id, this, v); \
	return BASECLASS::localGetIntrinsicSA(eval, v); \
    } \
    GA_Size \
    CLASS::localSetIntrinsicI(const GA_IntrinsicEval &eval, \
		const int64 *v, GA_Size size) \
    { \
	int	id = eval.getUserId(_theIntrinsics##CLASS); \
	if (id >= 0 && id < _theEval##CLASS.entries()) \
	    return _theEval##CLASS.setI(id, this, v, size); \
	return BASECLASS::localSetIntrinsicI(eval, v, size); \
    } \
    GA_Size \
    CLASS::localSetIntrinsicF(const GA_IntrinsicEval &eval, \
		const fpreal64 *v, GA_Size size) \
    { \
	int	id = eval.getUserId(_theIntrinsics##CLASS); \
	if (id >= 0 && id < _theEval##CLASS.entries()) \
	    return _theEval##CLASS.setF(id, this, v, size); \
	return BASECLASS::localSetIntrinsicF(eval, v, size); \
    } \
    GA_Size \
    CLASS::localSetIntrinsicSS(const GA_IntrinsicEval &eval, \
		const char **v, GA_Size size) \
    { \
	int	id = eval.getUserId(_theIntrinsics##CLASS); \
	if (id >= 0 && id < _theEval##CLASS.entries()) \
	    return _theEval##CLASS.setSS(id, this, v, size); \
	return BASECLASS::localSetIntrinsicSS(eval, v, size); \
    } \
    GA_Size \
    CLASS::localSetIntrinsicSA(const GA_IntrinsicEval &eval, \
		const UT_StringArray &v) \
    { \
	int	id = eval.getUserId(_theIntrinsics##CLASS); \
	if (id >= 0 && id < _theEval##CLASS.entries()) \
	    return _theEval##CLASS.setSA(id, this, v); \
	return BASECLASS::localSetIntrinsicSA(eval, v); \
    }

// ----------- Intrinsic definition macros start here ------------
#define GA_INTRINSIC_DEF(ID, NAME, TUPLESIZE, STORAGE) \
	myEval[ID].myId = ID; \
	myEval[ID].myName = NAME; \
	myEval[ID].myStore = STORAGE; \
	myEval[ID].myTupleSize = TUPLESIZE;
#define GA_INTRINSIC_DEF_VARYING(ID, NAME, TUPLESIZE_FUNC, STORAGE) \
	myEval[ID].myId = ID; \
	myEval[ID].myName = NAME; \
	myEval[ID].myStore = STORAGE; \
	myEval[ID].myTupleSizeCB = TUPLESIZE_FUNC;

// Set UT_Options bound to intrinsic definition.  You might consider storing
// information about attribute in the options (i.e. "type":GA_TypeInfo).  This
// is totally optional.
#define GA_INTRINSIC_OPTIONS(ID, OPTIONS)	\
	myEval[ID].myOptions = &OPTIONS;

// Integer intrinsics
#define GA_INTRINSIC_DEF_I(ID, NAME, TUPLESIZE) \
	GA_INTRINSIC_DEF(ID, NAME, TUPLESIZE, GA_STORECLASS_INT)

#define GA_INTRINSIC_DEF_VARYING_I(ID, NAME, TUPLESIZE_FUNC) \
	GA_INTRINSIC_DEF_VARYING(ID, NAME, TUPLESIZE_FUNC, GA_STORECLASS_INT)

#define GA_INTRINSIC_I(CLASS, ID, NAME, EVAL) { \
	struct callbacks { \
	    static GA_Size eval(const CLASS *o, int64 *v, GA_Size) \
	    { v[0] = EVAL(o); return 1; } \
	}; \
	GA_INTRINSIC_DEF_I(ID, NAME, 1) \
	myEval[ID].myI = callbacks::eval; \
    }
#define GA_INTRINSIC_CONST_I(CLASS, ID, NAME, VALUE) { \
	struct callbacks { \
	    static GA_Size eval(const CLASS *, int64 *v, GA_Size) \
	    { v[0] = VALUE; return 1; } \
	}; \
	GA_INTRINSIC_DEF_I(ID, NAME, 1) \
	myEval[ID].myI = callbacks::eval; \
    }
#define GA_INTRINSIC_METHOD_I(CLASS, ID, NAME, METHOD) { \
	struct callbacks { \
	    static GA_Size eval(const CLASS *o, int64 *v, GA_Size) \
	    { v[0] = o->METHOD(); return 1; } \
	}; \
	GA_INTRINSIC_DEF_I(ID, NAME, 1) \
	myEval[ID].myI = callbacks::eval; \
    }
#define GA_INTRINSIC_TUPLE_I(CLASS, ID, NAME, TUPLESIZE, EVAL) { \
	GA_INTRINSIC_DEF_I(ID, NAME, TUPLESIZE) \
	myEval[ID].myI = EVAL; \
    }
#define GA_INTRINSIC_TUPLE_METHOD_I(CLASS, ID, NAME, TUPLESIZE, EVAL) { \
	struct callbacks { \
	    static GA_Size eval(const CLASS *o, int64 *v, GA_Size sz) \
	    { return o->EVAL(v, sz); } \
	}; \
	GA_INTRINSIC_DEF_I(ID, NAME, TUPLESIZE) \
	myEval[ID].myI = callbacks::eval; \
    }
#define GA_INTRINSIC_ARRAY_I(CLASS, ID, NAME, ARRAY_TYPE, METHOD) { \
	struct callbacks { \
	    static GA_Size tsize(const CLASS *o) { return o->METHOD().entries(); } \
	    static GA_Size eval(const CLASS *o, int64 *v, GA_Size size) \
	    { \
		const ARRAY_TYPE &array = o->METHOD(); \
		size = SYSmin(size, array.entries()); \
		for (exint i = 0; i < size; ++i) { v[i] = array(i); } \
		return size; \
	    } \
	}; \
	GA_INTRINSIC_DEF_VARYING_I(ID, NAME, callbacks::tsize) \
	myEval[ID].myI = callbacks::eval; \
    }
#define GA_INTRINSIC_VARYING_I(CLASS, ID, NAME, TSIZE, EVAL) { \
	GA_INTRINSIC_DEF_VARYING_I(ID, NAME, TSIZE) \
	myEval[ID].myI = EVAL; \
    }

#define GA_INTRINSIC_SET_I(CLASS, ID, SETFUNC) { \
	struct callbacks { \
	    static GA_Size setFunc(CLASS *o, const int64 *v, GA_Size sz) \
	    { return SETFUNC(o, v[0]) ? 1 : 0; } \
	}; \
	myEval[ID].mySetI = callbacks::setFunc; \
	myEval[ID].myReadOnly = false; \
    }
#define GA_INTRINSIC_SET_METHOD_I(CLASS, ID, METHOD) { \
	struct callbacks { \
	    static GA_Size setFunc(CLASS *o, const int64 *v, GA_Size sz) \
	    { return o->METHOD(v[0]) ? 1 : 0; } \
	}; \
	myEval[ID].mySetI = callbacks::setFunc; \
	myEval[ID].myReadOnly = false; \
    }
#define GA_INTRINSIC_SET_TUPLE_I(CLASS, ID, SETFUNC) { \
	struct callbacks { \
	    static GA_Size setFunc(CLASS *o, const int64 *v, GA_Size sz) \
	    { return SETFUNC(o, v, sz); } \
	}; \
	myEval[ID].mySetI = callbacks::setFunc; \
	myEval[ID].myReadOnly = false; \
    }
#define GA_INTRINSIC_SET_TUPLE_METHOD_I(CLASS, ID, METHOD) { \
	struct callbacks { \
	    static GA_Size setFunc(CLASS *o, const int64 *v, GA_Size sz) \
	    { return o->METHOD(v[0], sz); } \
	}; \
	myEval[ID].mySetI = callbacks::setFunc; \
	myEval[ID].myReadOnly = false; \
    }

// Float intrinsics
#define GA_INTRINSIC_DEF_F(ID, NAME, TUPLESIZE) \
	GA_INTRINSIC_DEF(ID, NAME, TUPLESIZE, GA_STORECLASS_FLOAT)

#define GA_INTRINSIC_DEF_VARYING_F(ID, NAME, TUPLESIZE_FUNC) \
	GA_INTRINSIC_DEF_VARYING(ID, NAME, TUPLESIZE_FUNC, GA_STORECLASS_FLOAT)

#define GA_INTRINSIC_F(CLASS, ID, NAME, EVAL) { \
	struct callbacks { \
	    static GA_Size eval(const CLASS *o, fpreal64 *v, GA_Size) \
	    { v[0] = EVAL(o); return 1; } \
	}; \
	GA_INTRINSIC_DEF_F(ID, NAME, 1) \
	myEval[ID].myF = callbacks::eval; \
    }
#define GA_INTRINSIC_CONST_F(CLASS, ID, NAME, VALUE) { \
	struct callbacks { \
	    static GA_Size eval(const CLASS *, fpreal64 *v, GA_Size) \
	    { v[0] = VALUE; return 1; } \
	}; \
	GA_INTRINSIC_DEF_F(ID, NAME, 1) \
	myEval[ID].myF = callbacks::eval; \
    }
#define GA_INTRINSIC_METHOD_F(CLASS, ID, NAME, METHOD) { \
	struct callbacks { \
	    static GA_Size eval(const CLASS *o, fpreal64 *v, GA_Size) \
	    { v[0] = o->METHOD(); return 1; } \
	}; \
	GA_INTRINSIC_DEF_F(ID, NAME, 1) \
	myEval[ID].myF = callbacks::eval; \
    }
#define GA_INTRINSIC_TUPLE_F(CLASS, ID, NAME, TUPLESIZE, EVAL) { \
	GA_INTRINSIC_DEF_F(ID, NAME, TUPLESIZE) \
	myEval[ID].myF = EVAL; \
    }
#define GA_INTRINSIC_TUPLE_METHOD_F(CLASS, ID, NAME, TUPLESIZE, EVAL) { \
	struct callbacks { \
	    static GA_Size eval(const CLASS *o, fpreal64 *v, GA_Size sz) \
	    { return o->EVAL(v, sz); } \
	}; \
	GA_INTRINSIC_DEF_F(ID, NAME, TUPLESIZE) \
	myEval[ID].myF = callbacks::eval; \
    }
#define GA_INTRINSIC_ARRAY_F(CLASS, ID, NAME, ARRAY_TYPE, METHOD) { \
	struct callbacks { \
	    static GA_Size tsize(const CLASS *o) { return o->METHOD().entries(); } \
	    static GA_Size eval(const CLASS *o, fpreal64 *v, GA_Size size) \
	    { \
		const ARRAY_TYPE &array = o->METHOD(); \
		size = SYSmin(size, array.entries()); \
		for (exint i = 0; i < size; ++i) { v[i] = array(i); } \
		return size; \
	    } \
	}; \
	GA_INTRINSIC_DEF_VARYING_F(ID, NAME, callbacks::tsize) \
	myEval[ID].myF = callbacks::eval; \
    }
#define GA_INTRINSIC_VARYING_F(CLASS, ID, NAME, TSIZE, EVAL) { \
	GA_INTRINSIC_DEF_VARYING_F(ID, NAME, TSIZE) \
	myEval[ID].myF = EVAL; \
    }

#define GA_INTRINSIC_SET_F(CLASS, ID, SETFUNC) { \
	struct callbacks { \
	    static GA_Size setFunc(CLASS *o, const fpreal64 *v, GA_Size sz) \
	    { SETFUNC(o, v[0]); return 1; } \
	}; \
	myEval[ID].mySetF = callbacks::setFunc; \
	myEval[ID].myReadOnly = false; \
    }
#define GA_INTRINSIC_SET_METHOD_F(CLASS, ID, METHOD) { \
	struct callbacks { \
	    static GA_Size setFunc(CLASS *o, const fpreal64 *v, GA_Size sz) \
	    { o->METHOD(v[0]); return 1; } \
	}; \
	myEval[ID].mySetF = callbacks::setFunc; \
	myEval[ID].myReadOnly = false; \
    }
#define GA_INTRINSIC_SET_TUPLE_F(CLASS, ID, SETFUNC) { \
	struct callbacks { \
	    static GA_Size setFunc(CLASS *o, const fpreal64 *v, GA_Size sz) \
	    { return SETFUNC(o, v, sz); } \
	}; \
	myEval[ID].mySetF = callbacks::setFunc; \
	myEval[ID].myReadOnly = false; \
    }
#define GA_INTRINSIC_SET_TUPLE_METHOD_F(CLASS, ID, METHOD) { \
	struct callbacks { \
	    static GA_Size setFunc(CLASS *o, const fpreal64 *v, GA_Size sz) \
	    { return o->METHOD(v[0], sz); } \
	}; \
	myEval[ID].mySetF = callbacks::setFunc; \
	myEval[ID].myReadOnly = false; \
    }

// String intrinsics
#define GA_INTRINSIC_DEF_S(ID, NAME, TUPLESIZE) \
	GA_INTRINSIC_DEF(ID, NAME, TUPLESIZE, GA_STORECLASS_STRING)

#define GA_INTRINSIC_DEF_VARYING_S(ID, NAME, TUPLESIZE_FUNC) \
	GA_INTRINSIC_DEF_VARYING(ID, NAME, TUPLESIZE_FUNC, GA_STORECLASS_STRING)

#define GA_INTRINSIC_S(CLASS, ID, NAME, EVAL) { \
	struct callbacks { \
	    static GA_Size evalS(const CLASS *o, UT_String &v) \
	    { v = EVAL(o); return 1; } \
	    static GA_Size evalSA(const CLASS *o, UT_StringArray &v) \
	    { v.append(EVAL(o)); return 1; } \
	}; \
	GA_INTRINSIC_DEF_S(ID, NAME, 1) \
	myEval[ID].myS  = callbacks::evalS; \
	myEval[ID].mySA = callbacks::evalSA; \
    }
#define GA_INTRINSIC_CONST_S(CLASS, ID, NAME, VALUE) { \
	struct callbacks { \
	    static GA_Size evalS(const CLASS *, UT_String &v) \
	    { v = VALUE; return 1; } \
	    static GA_Size evalSA(const CLASS *, UT_StringArray &v) \
	    { v.append(VALUE); return 1; } \
	}; \
	GA_INTRINSIC_DEF_S(ID, NAME, 1) \
	myEval[ID].myS  = callbacks::evalS; \
	myEval[ID].mySA = callbacks::evalSA; \
    }
#define GA_INTRINSIC_METHOD_S(CLASS, ID, NAME, METHOD) { \
	struct callbacks { \
	    static GA_Size evalS(const CLASS *o, UT_String &v) \
	    { v = o->METHOD(); return 1; } \
	    static GA_Size evalSA(const CLASS *o, UT_StringArray &v) \
	    { v.append(o->METHOD()); return 1; } \
	}; \
	GA_INTRINSIC_DEF_S(ID, NAME, 1) \
	myEval[ID].myS  = callbacks::evalS; \
	myEval[ID].mySA = callbacks::evalSA; \
    }
#define GA_INTRINSIC_TUPLE_S(CLASS, ID, NAME, TUPLESIZE, EVAL) { \
	GA_INTRINSIC_DEF_S(ID, NAME, TUPLESIZE) \
	myEval[ID].mySA = EVAL; \
    }
#define GA_INTRINSIC_TUPLE_METHOD_S(CLASS, ID, NAME, TUPLESIZE, EVAL) { \
	struct callbacks { \
	    static GA_Size evalSA(const CLASS *o, UT_StringArray &v) \
	    { o->EVAL(v); return v.entries(); } \
	}; \
	GA_INTRINSIC_DEF_S(ID, NAME, TUPLESIZE) \
	myEval[ID].mySA = callbacks::eval; \
    }
#define GA_INTRINSIC_ARRAY_S(CLASS, ID, NAME, ARRAY_TYPE, METHOD) { \
	struct callbacks { \
	    static GA_Size tsize(const CLASS *o) { return o->METHOD().entries(); } \
	    static GA_Size eval(const CLASS *o, UT_StringArray &v) \
	    { \
		const ARRAY_TYPE &array = o->METHOD(); \
		for (exint i = 0; i < size; ++i) { v[i].append(array(i)); } \
		return v.entries(); \
	    } \
	}; \
	GA_INTRINSIC_DEF_VARYING_S(ID, NAME, callbacks::tsize) \
	myEval[ID].mySA = callbacks::eval; \
    }
#define GA_INTRINSIC_VARYING_S(CLASS, ID, NAME, TSIZE, EVAL) { \
	GA_INTRINSIC_DEF_VARYING_S(ID, NAME, TSIZE) \
	myEval[ID].mySA = EVAL; \
    }

#define GA_INTRINSIC_SET_S(CLASS, ID, SETFUNC) { \
	struct callbacks { \
	    static GA_Size setSS(CLASS *o, const char **v, GA_Size sz) \
	    { return SETFUNC(o, v[0]) ? 1 : 0; } \
	    static GA_Size setSA(CLASS *o, const UT_StringArray &a) \
	    { return SETFUNC(o, a(0)) ? 1 : 0; } \
	}; \
	myEval[ID].mySetSS = callbacks::setSS; \
	myEval[ID].mySetSA = callbacks::setSA; \
	myEval[ID].myReadOnly = false; \
    }
#define GA_INTRINSIC_SET_METHOD_S(CLASS, ID, METHOD) { \
	struct callbacks { \
	    static GA_Size setSS(CLASS *o, const char **v, GA_Size) \
	    { return o->METHOD(v[0]) ? 1 : 0; } \
	    static GA_Size setSA(CLASS *o, const UT_StringArray &a) \
	    { return o->METHOD(a(0)) ? 1 : 0; } \
	}; \
	myEval[ID].mySetSS = callbacks::setSS; \
	myEval[ID].mySetSA = callbacks::setSA; \
	myEval[ID].myReadOnly = false; \
    }
#define GA_INTRINSIC_SET_TUPLE_S(CLASS, ID, SETFUNC) { \
	struct callbacks { \
	    static GA_Size setSA(CLASS *o, const UT_StringArray &a) \
	    { return SETFUNC(o, a); } \
	    static GA_Size setSS(CLASS *o, const char **v, GA_Size sz) \
	    { \
		UT_StringArray	array; \
		array.resize(sz); \
		for (exint i = 0; i < sz; ++i) array(i) = v[i]; \
		return SETFUNC(o, array); \
	    } \
	}; \
	myEval[ID].mySetSS = callbacks::setSS; \
	myEval[ID].mySetSA = callbacks::setSA; \
	myEval[ID].myReadOnly = false; \
    }
#define GA_INTRINSIC_SET_TUPLE_METHOD_S(CLASS, ID, METHOD) { \
	struct callbacks { \
	    static GA_Size setSA(CLASS *o, const UT_StringArray &a) \
	    { return o->METHOD(a); } \
	    static GA_Size setSS(CLASS *o, const char **v, GA_Size sz) \
	    { \
		UT_StringArray	array; \
		array.resize(sz); \
		for (exint i = 0; i < sz; ++i) array(i) = v[i]; \
		return o->METHOD(array); \
	    } \
	}; \
	myEval[ID].mySetSS = callbacks::setSS; \
	myEval[ID].mySetSA = callbacks::setSA; \
	myEval[ID].myReadOnly = false; \
    }

#endif

