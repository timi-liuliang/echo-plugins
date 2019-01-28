/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CVEX Library (C++)
 *
 */

#ifndef __CVEX_Data_h__
#define __CVEX_Data_h__

#include "CVEX_API.h"
#include "CVEX_Value.h" // for CVEX_Type
#include <VEX/VEX_PodTypes.h>
#include <UT/UT_Vector.h>
#include <UT/UT_PackedArrayOfArrays.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_UniquePtr.h>

// ============================================================================
/// Class for storing CVEX data buffers, where each buffer contains data for 
/// a CVEX_Value object.  The CVEX_Data class owns the buffers and manages 
/// their allocation and deallocation. 
/// Each buffer also has a corresponding name and CVEX type,
/// which can be useful when transferring data to and from the buffers.
///
/// For example: @code
///	CVEX_Data    data;
///	CVEX_Value  *value = cvex_context.findInput("foo");
///	auto *buffer = data.addDataBuffer<fpreal32>("foo", value->getType());
///	buffer.setSize( 10 );
///	for( int i=0; i<10; ++i )
///	    buffer[i] = 0.1 * (fpreal32) i;
///	value->setTypedData( buffer->data(), buffer->size() );
/// @endcode
///
class CVEX_API CVEX_Data 
{
public:
    /// Creates an object that holds and manages the data buffers.  
		 CVEX_Data();
		~CVEX_Data();

    /// @{ Adds a new buffer and returns it as a reference.
    /// If a buffer by that name has already been added, it is returned,
    /// provinding the type matches. If type does not match, returns nullptr.
    /// The given @p type is associated with that buffer
    template<typename T> UT_Array<T> *
		addDataBuffer(const UT_StringHolder &name, CVEX_Type type);
    template<typename T> UT_PackedArrayOfArrays<T> *
		addArrayDataBuffer(const UT_StringHolder &name, CVEX_Type type);
    /// @}

    /// @{ Returns the data buffer given by its name and type T, or null 
    /// if not found.
    template<typename T> UT_Array<T> *
		findDataBuffer(const UT_StringHolder &name) const;
    template<typename T> UT_PackedArrayOfArrays<T> *
		findArrayDataBuffer(const UT_StringHolder &name) const;
    /// @}
    
    /// Returns the CVEX type associated with the data buffer given its name,
    /// or, if buffer is not found, returns CVEX_TYPE_INVALID.
    CVEX_Type	getCvexType(const UT_StringHolder &name) const;

private:
    /// @{ Private helper method templates for adding and finding buffers.
    template<typename T> T *add(const UT_StringHolder &name, CVEX_Type type);
    template<typename T> T *find(const UT_StringHolder &name) const;
    /// @}

private:
    // Base class for buffer holders.
    class Data
    {
    public:
		    Data( CVEX_Type cvex_type ) : myCvexType( cvex_type ) {}
	virtual	   ~Data() {}
	CVEX_Type   getCvexType() const { return myCvexType; }
    private:
	CVEX_Type   myCvexType;	// CVEX type for which this buffer was created
    };

    // Templetized buffer holders.
    template<typename T> class DataImpl : public Data
    {
    public:
		    DataImpl( UT_UniquePtr<T> &&data_array, CVEX_Type cvex_type)
			: Data( cvex_type )
			, myDataPtr( std::move( data_array )) {}
	T *	    getData()
		    { return myDataPtr.get(); }
    private:
	UT_UniquePtr<T>		myDataPtr;	// data buffer object (an array)
    };

private:
    /// The actual data buffers objects. 
    UT_StringMap< Data *>	myBufferMap;
};


template<typename T> 
T *
CVEX_Data::add(const UT_StringHolder &name, CVEX_Type type)
{
    if( myBufferMap.contains( name ))
	return find<T>( name );

    auto data = new DataImpl<T>( std::move( UTmakeUnique<T>() ), type);
    myBufferMap[ name ] = data;
    return data->getData();
}

template<typename T> 
T *
CVEX_Data::find( const UT_StringHolder &name ) const
{
    auto it = myBufferMap.find( name );
    if( it == myBufferMap.end() )
	return nullptr;

    auto data = dynamic_cast<DataImpl<T> *>( it->second );
    return data->getData();
}

template<typename T> 
UT_Array<T> *
CVEX_Data::addDataBuffer(const UT_StringHolder &name, CVEX_Type type)
{
    return add<UT_Array<T>>( name, type );
}

template<typename T>
UT_PackedArrayOfArrays<T> *
CVEX_Data::addArrayDataBuffer( const UT_StringHolder &name, CVEX_Type type )
{
    return add<UT_PackedArrayOfArrays<T>>( name, type );
}

template<typename T> 
UT_Array<T> *
CVEX_Data::findDataBuffer( const UT_StringHolder &name ) const
{
    return find<UT_Array<T>>( name );
}

template<typename T> 
UT_PackedArrayOfArrays<T> *
CVEX_Data::findArrayDataBuffer( const UT_StringHolder &name ) const
{
    return find<UT_PackedArrayOfArrays<T>>( name );
}

// ============================================================================
/// A convenience class to store input and output data in two separate data
/// objects, to avoid name clashes between input and output CVEX_Value with
/// the same names.
class CVEX_API CVEX_InOutData 
{
public:
    /// Creates a holder for the data buffers.  
			CVEX_InOutData()	{}
    /// @{ Accessors
    CVEX_Data		&getInputData()		{ return myInputData; }
    const CVEX_Data	&getInputData() const	{ return myInputData; }
    CVEX_Data		&getOutputData()	{ return myOutputData; }
    const CVEX_Data	&getOutputData() const	{ return myOutputData; }
    /// @}

private:
    CVEX_Data		myInputData;
    CVEX_Data		myOutputData;
};

// ============================================================================
/// Precision-dependent types of CVEX data.
template <VEX_Precision PREC>
class CVEX_DataType
{
public:
    using Int  = VEXint<PREC>;
    using Float= VEXfloat<PREC>;
    using Vec2 = VEXvec2<PREC>;
    using Vec3 = VEXvec3<PREC>;
    using Vec4 = VEXvec4<PREC>;
    using Mat2 = VEXmat2<PREC>;
    using Mat3 = VEXmat3<PREC>;
    using Mat4 = VEXmat4<PREC>;
    using Quat = VEXquaternion<PREC>;
};

#define DECLARE_CVEX_DATA_TYPES \
    using Int    = typename CVEX_DataType<PREC>::Int;	\
    using Float  = typename CVEX_DataType<PREC>::Float;	\
    using String = UT_StringHolder;			\
    using Vec2 = typename CVEX_DataType<PREC>::Vec2;	\
    using Vec3 = typename CVEX_DataType<PREC>::Vec3;	\
    using Vec4 = typename CVEX_DataType<PREC>::Vec4;	\
    using Mat2 = typename CVEX_DataType<PREC>::Mat2;	\
    using Mat3 = typename CVEX_DataType<PREC>::Mat3;	\
    using Mat4 = typename CVEX_DataType<PREC>::Mat4;	\
    using Quat = typename CVEX_DataType<PREC>::Quat;	\


// ============================================================================
/// A helper class for binding data buffers to CVEX values.
/// It abstracts the buffer management by allowing to override a virtual 
/// method for the particular data type.
///
/// For example: @code
///	class MY_Binder : public CVEX_DataBinder32
///	{
///	public:	
///	    MY_Binder(CVEX_Data &data, exint size, fpreal32 multipiler)
///		: CVEX_DataBinder32(data, size), myMultiplier(multipiler) {}
///	    ...
///	    virtual bool setData( UT_Array<Float> &data,
///		const UT_StringHolder &data_name ) override
///	    {
///		for( int i=0; i < getBufferSize(); i++ )
///		    data = myMultiplier * (fpreal32) i;
///	    }
///	private:
///	    fpreal32	myMultiplier;
///	};
///
///	CVEX_Data   data;
///	MY_Binder   binder( data, 10, 0.1 );
///	binder.setAndBindData( *cvex_context.findInput( "foo_in" ));
/// @endcode
//
/// For example, binding an output buffer: @code
///	CVEX_Data	    data;
///	CVEX_DataBinder32   binder( data, 10 );
///	binder.bindData( *cvex_context.findOutput( "bar_out" )); 
/// @endcode
///
template <VEX_Precision PREC>
class CVEX_API CVEX_DataBinder
{
public:
    /// Creates a binder object for binding data buffers to CVEX values.
    /// The @data is the data holder that stores the data buffers.
    /// The @p buffer_size specifies the size of buffers this class will create,
    /// and should be equal to the value passed into CVEX_Context::run().
		 CVEX_DataBinder( CVEX_Data &data, exint buffer_size );
    virtual	~CVEX_DataBinder();

    /// Return the size of the data buffers.
    exint	getBufferSize() const
		{ return myBufferSize; }

    /// If set to true, for convenience, the size of the buffer will be set 
    /// prior to calling the setData() virtuals. Otherwise, it will be up to 
    /// setData() to do so (eg, in cases where data can be non-varying).
    void	setShouldPreSizeBuffers( bool flag )
		{ myShouldPreSizeBuffers = flag; }

    /// Creates a new data buffer, calls the virtuals to fill it with data,
    /// then binds it to the given CVEX value.
    bool	setAndBindData( CVEX_Value &value );

    /// Creates a new data buffer, initializes it to zero (without calling
    /// the virtuals), then binds it to the given CVEX value.
    bool	bindData( CVEX_Value &value ); 

    /// Precision-dependent data types (Int, Float, Vec2, ...) used in virtuals.
    DECLARE_CVEX_DATA_TYPES 

protected:
    /// @{ Virtuals intended to be overriden by the subclass. 
    /// Their purpose is to get the values from some custom data source 
    /// and set the value of the @data parameter.
    /// The @p data is the data buffer over which the CVEX program will run.
    /// The @p data_name is the name of the data buffer.
    //
    /// Note, for non-array data types the virtuals don't have @array_sizes
    /// parameter but their @data buffer parameters have the size of the
    /// desired buffer size (specified in the constructor).
    //
    /// For array data types, the virtuals have @array_sizes parameter, whose
    /// size is set to the desired data buffer size (given in the constructor). 
    /// The @array_sizes elements should be set to the sizes of individual 
    /// arrays, and the elements of each consecutive data array should be 
    /// appended to the @packed_arrays_data, thus forming a packed array.
    virtual bool setData( UT_Array<Int> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_Array<Float> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_Array<String> &data,
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_Array<Vec2> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_Array<Vec3> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_Array<Vec4> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_Array<Mat2> &data,
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_Array<Mat3> &data,
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_Array<Mat4> &data,
	    const UT_StringHolder &data_name );

    virtual bool setData( UT_PackedArrayOfArrays<Int> &data,
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_PackedArrayOfArrays<Float> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_PackedArrayOfArrays<String> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_PackedArrayOfArrays<Vec2> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_PackedArrayOfArrays<Vec3> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_PackedArrayOfArrays<Vec4> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_PackedArrayOfArrays<Mat2> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_PackedArrayOfArrays<Mat3> &data, 
	    const UT_StringHolder &data_name );
    virtual bool setData( UT_PackedArrayOfArrays<Mat4> &data, 
	    const UT_StringHolder &data_name );
    /// @}

private:
    /// @{ Helper methods and templates for managing the buffers.
    bool	setIfRequestedAndBindData( CVEX_Value &value, bool set_data );

    template<typename T, typename BIND> 
    bool	addBufferAndBind( CVEX_Value &value, bool set_data, BIND bind );
    template<typename T> 
    bool	addBufferAndBindAsPtr( CVEX_Value &value, bool set_data );
    template<typename T> 
    bool	addBufferAndBindAsObj( CVEX_Value &value, bool set_data );

    template<typename T>
    bool	addSclrArrayBufferAndBind( CVEX_Value &value, bool set_data );
    template<typename T>
    bool	addVecArrayBufferAndBind( CVEX_Value &value, bool set_data );
    bool	addStrArrayBufferAndBind( CVEX_Value &value, bool set_data );
    /// @}

private:
    /// Data buffers to fill and to bind to CVEX values.
    CVEX_Data	&myData;    

    /// The size of each data buffer created and bound by this class.
    exint	 myBufferSize;	
    
    /// If true, the buffer size will be set prior to calling setData virtuals.
    bool	 myShouldPreSizeBuffers;
};

using CVEX_DataBinder32 = CVEX_DataBinder<VEX_32>;
using CVEX_DataBinder64 = CVEX_DataBinder<VEX_64>;


// ============================================================================
/// A helper class for retrieving data from CVEX buffers to custom destination.
/// It abstracts the buffer management by allowing to override virtual
/// methods for the particular data types.
///
/// For example: @code
///	class MY_Printer : public CVEX_DataRetriever32
///	{
///	public:
///	    MyPrinter( const CVEX_Data &data )
///		: CVEX_DataRetriever32( data ) {}
///	    ...
///	    virtual bool takeData( const UT_Array<Float> &data,
///		    const UT_StringHolder &data_name ) override
///	    {
///		for( exint i = 0; i < data.size(); i++ )
///		    cout << "Entry " << i << ": " << data[i] << "\n";
///	    }
///	};
///	    
///	CVEX_Data   data;
///	...	    // ... bind data buffers and run CVEX...
///	MY_Printer  retriever( data );
///	cout << "Computed data for 'bar_out' buffer:\n";
///	retriever.retrieveData( "bar_out" );
/// @endcode
///
template <VEX_Precision PREC>
class CVEX_API CVEX_DataRetriever
{
public: 
    /// Creates a retriever object for obtaining data from the buffers.
		 CVEX_DataRetriever( const CVEX_Data &data );
    virtual	~CVEX_DataRetriever();

    /// @{ Looks for a data buffer given the CVEX value or a name,
    /// and calls the virtuals to retrieve the data from the bufer.
    bool	 retrieveData( const CVEX_Value &value );
    bool	 retrieveData( const UT_StringHolder &data_name );
    /// @}

protected:
    /// Precision-dependent data types (Int, Float, Vec2, ...) used in virtuals.
    DECLARE_CVEX_DATA_TYPES 

    /// @{ Virtuals intended to be overriden by the subclasses. 
    /// Their purpose is to take the value given in the @data parameter
    /// and transfer it to the intended destination.
    /// The @p data represents a data buffer on which the CVEX program was run.
    /// The @p name is the name of the data buffer.
    /// 
    /// For array type data the virtuals have the @p array_sizes parameter,
    /// which conains the sizes for each individual arrays. For these virtuals,
    /// the array elements are packed in the @packed_arrays_data parameter.
    virtual bool takeData( const UT_Array<Int> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_Array<Float> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_Array<String> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_Array<Vec2> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_Array<Vec3> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_Array<Vec4> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_Array<Mat2> &data,
	    const UT_StringHolder &data_name  );
    virtual bool takeData( const UT_Array<Mat3> &data,
	    const UT_StringHolder &data_name  );
    virtual bool takeData( const UT_Array<Mat4> &data,
	    const UT_StringHolder &data_name  );

    virtual bool takeData( const UT_PackedArrayOfArrays<Int> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_PackedArrayOfArrays<Float> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_PackedArrayOfArrays<String> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_PackedArrayOfArrays<Vec2> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_PackedArrayOfArrays<Vec3> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_PackedArrayOfArrays<Vec4> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_PackedArrayOfArrays<Mat2> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_PackedArrayOfArrays<Mat3> &data,
	    const UT_StringHolder &data_name );
    virtual bool takeData( const UT_PackedArrayOfArrays<Mat4> &data,
	    const UT_StringHolder &data_name );
    /// @}

private:
    /// @{ Helper methods and templates for managing the buffers.
    template<typename T> 
    bool	 retrieveBuffer(const UT_StringHolder &data_name);

    template<typename T>
    bool	 retrieveSclrArrayBuffer(const UT_StringHolder &data_name);
    template<typename T>
    bool	 retrieveVecArrayBuffer(const UT_StringHolder &data_name);
    bool	 retrieveStrArrayBuffer(const UT_StringHolder &data_name);
    /// @}

private:
    /// Data buffers from which to retrieve the data.
    const CVEX_Data	&myData; 
};

using CVEX_DataRetriever32 = CVEX_DataRetriever<VEX_32>;
using CVEX_DataRetriever64 = CVEX_DataRetriever<VEX_64>;


#endif
