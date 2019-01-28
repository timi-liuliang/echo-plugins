/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_ConAnchorObjRegion.h ( SIM Library, C++)
 *
 * COMMENTS:
 *     This anchor represents a region in space that surrounds goal object.
 *     This can be used to constrain a Solid Object or a Hybrid Object
 *     to a deforming region in space.
 *     In the future, the support for this may be extended to
 *     the Wire Object and Cloth Object as well.
 */

#ifndef __SIM_ConAnchorObjRegion__
#define __SIM_ConAnchorObjRegion__

#include "SIM_API.h"
#include "SIM_ConAnchor.h"

#include <UT/UT_Vector4.h>

#include <SYS/SYS_Types.h>
#include <SYS/SYS_Inline.h>

class SIM_ConAnchorObjRegion;

//
// PrimitiveSubsimplex identifies a subsimplex of a primitive.
// This works only for a specific subset of primitive types:
// particle system, polyline (open polygon), polygon (closed polygon) 
// and tetrahedron.
// PrimitiveSubsimplex is a helper struct that is used to define
// the various connector (sub-anchor) types below
//
class SIM_ConAnchorObjRegionPrimitiveSubsimplex
{
public:
    using PrimitiveSubindex = exint;

    // Default construct an invalid object
    SIM_ConAnchorObjRegionPrimitiveSubsimplex() :
        myPrimitiveIndex( GA_INVALID_INDEX ),
	mySubindex( -1 )
    {
        UT_ASSERT( !isValid() );
    }

    SIM_ConAnchorObjRegionPrimitiveSubsimplex(
        const GA_Index primitive_index,
        const PrimitiveSubindex subindex
    ) :
        myPrimitiveIndex( primitive_index ),
        mySubindex( subindex )
    {
        UT_ASSERT( primitive_index != GA_INVALID_INDEX );
	UT_ASSERT( subindex >= 0 );
        UT_ASSERT( isValid() );
    }

    SYS_FORCE_INLINE bool isValid() const
    {
        return ( myPrimitiveIndex != GA_INVALID_INDEX );
    }

    // Index to primitive of type:
    // particles, polylines, polygons, tetrahedrons
    GA_Index myPrimitiveIndex;

    // For particles, triangles and polylines, the subindex identifies
    // a subsimplex of the above primitive
    PrimitiveSubindex mySubindex;
};

//
// ConnectorRealPrimitiveSubsimplex is a connector (sub-anchor) for
// a real sub-primitive of an anchor's object geometry.
//
class SIM_ConAnchorObjRegionConnectorRealPrimitiveSubsimplex
{
public:
    using PrimitiveSubsimplex = SIM_ConAnchorObjRegionPrimitiveSubsimplex;
    using Bary = UT_Vector4T< fpreal >;
    
    // Default construct "invalid" location
    SIM_ConAnchorObjRegionConnectorRealPrimitiveSubsimplex() :
        myPrimitiveSubsimplex(), // invalid
	myBary( 0, 0, 0, 0 )
    {
        UT_ASSERT( !isValid() );
    }
    
    // Construct valid location from a point index, a primitive index and bary coords
    SIM_ConAnchorObjRegionConnectorRealPrimitiveSubsimplex(
        const PrimitiveSubsimplex subprimitive,
        const Bary& bary
    ) :
        myPrimitiveSubsimplex( subprimitive ),
	myBary( bary )
    {
        UT_ASSERT( subprimitive.isValid() );
        UT_ASSERT( isValid() );
    }

    SIM_ConAnchorObjRegionConnectorRealPrimitiveSubsimplex( const SIM_ConAnchorObjRegionConnectorRealPrimitiveSubsimplex& ) = default;
    SIM_ConAnchorObjRegionConnectorRealPrimitiveSubsimplex& operator=( const SIM_ConAnchorObjRegionConnectorRealPrimitiveSubsimplex& ) = default;

    SYS_FORCE_INLINE bool isValid() const
    {
        return myPrimitiveSubsimplex.isValid();
    }
    
    PrimitiveSubsimplex myPrimitiveSubsimplex;

    // Barycentric coordinates relative to myPrimitiveSubsimplex,
    // padded with zero values at the end for subprimitives
    // that have fewer than four vertices (e.g., triangles on a polygon prim)
    Bary myBary;
};

//
// ConnectorRealPoint is a connector (sub-anchor) for
// a real point of an anchor's object geometry.
//
class SIM_ConAnchorObjRegionConnectorRealPoint
{
public:    
    // Default construct "invalid" point
    SIM_ConAnchorObjRegionConnectorRealPoint() :
        myPointIndex( GA_INVALID_INDEX )
    {
        UT_ASSERT( !isValid() );
    }
    
    explicit SIM_ConAnchorObjRegionConnectorRealPoint(
        const GA_Index point_index
    ) :
        myPointIndex( point_index )
    {
        UT_ASSERT( point_index != GA_INVALID_INDEX );
        UT_ASSERT( isValid() );
    }
        
    SYS_FORCE_INLINE bool isValid() const
    {
        return ( myPointIndex != GA_INVALID_INDEX );
    }

    SIM_ConAnchorObjRegionConnectorRealPoint( const SIM_ConAnchorObjRegionConnectorRealPoint& ) = default;
    SIM_ConAnchorObjRegionConnectorRealPoint& operator=( const SIM_ConAnchorObjRegionConnectorRealPoint& ) = default;
    
    GA_Index myPointIndex;
};

//
// ConnectorVirtualPoint is a connector (sub-anchor) for
// a virtual vertex that is defined relative to an anchor's object geometry.
// This virtual vertex is represented using barycentric coordinates
// relative to a real sub-primitive of the anchor geometry.
//
class SIM_ConAnchorObjRegionConnectorVirtualPoint
{
public:
    using PrimitiveSubsimplex = SIM_ConAnchorObjRegionPrimitiveSubsimplex;
    using Bary = UT_Vector4T< fpreal >;
    
    // Default construct "invalid" location
    SIM_ConAnchorObjRegionConnectorVirtualPoint() :
        myPrimitiveSubsimplex(), // invalid
	myBary( 0, 0, 0, 0 )
    {
        UT_ASSERT( !isValid() );
    }
    
    // Construct valid location from a point index,
    // a primitive index and bary coords
    SIM_ConAnchorObjRegionConnectorVirtualPoint(
        const PrimitiveSubsimplex subprimitive,
        const Bary& bary
    ) :
        myPrimitiveSubsimplex( subprimitive ),
	myBary( bary )
    {
        UT_ASSERT( subprimitive.isValid() );
        UT_ASSERT( isValid() );
    }
    
    SIM_ConAnchorObjRegionConnectorVirtualPoint( const SIM_ConAnchorObjRegionConnectorVirtualPoint& ) = default;
    SIM_ConAnchorObjRegionConnectorVirtualPoint& operator=( const SIM_ConAnchorObjRegionConnectorVirtualPoint& ) = default;
    
    SYS_FORCE_INLINE bool isValid() const
    {
        return myPrimitiveSubsimplex.isValid();
    }
    
    PrimitiveSubsimplex myPrimitiveSubsimplex;

    // Barycentric coordinates relative to simplex defined by myPrimitiveSubsimplex
    // with zero at the end for simplices.
    // that have fewer than four vertices (e.g., triangles on a polygon prim)
    Bary myBary;
};

//
// RealInterface is an ordered array of connectors
// at real subprimitives and real points.
// A region constraint connects these connectors to corresponding connectors
// on the other anchor.
// Connections are implied between connectors of matching types
// that have the same index in the real and virtual interface.
//
// This information is precomputed on constraint initialization and stored in
// SIM_ConAnchorObjRegion so that the information can be re-used
// for an entire simulation.
//
class SIM_ConAnchorObjRegionRealInterface
{
public:
    using ConnectorRealPrimitiveSubsimplex = SIM_ConAnchorObjRegionConnectorRealPrimitiveSubsimplex;
    using ConnectorsRealPrimitiveSubsimplex = std::vector< SIM_ConAnchorObjRegionConnectorRealPrimitiveSubsimplex >;

    using ConnectorRealPoint = SIM_ConAnchorObjRegionConnectorRealPoint;
    using ConnectorsRealPoint = std::vector< ConnectorRealPoint >;
    
    //TODO: use move semantics to pass in here:
    explicit SIM_ConAnchorObjRegionRealInterface(
        const ConnectorsRealPoint& connectors_at_rpoint_for_vpoint
    ) :
        myConnectorsAtRealPointForVirtualPoint( connectors_at_rpoint_for_vpoint )
    {
    }
    
    SIM_ConAnchorObjRegionRealInterface( const SIM_ConAnchorObjRegionRealInterface& ) = default;
    
    //TODO: ConnectorsRealPrimitiveSubsimplex myConnectorsAtRealPrimitiveSubsimplexForVirtualPoint
    const ConnectorsRealPoint myConnectorsAtRealPointForVirtualPoint;
    
private:
    // Disallow
    SIM_ConAnchorObjRegionRealInterface& operator=( const SIM_ConAnchorObjRegionRealInterface& ) = delete;
};

//
// VirtualInterface is an ordered array of connectors
// at virtual points (defined relative to real primitives in terms of
// barycentric coordinates).
// A region constraint connects these connectors to corresponding connectors
// on the other anchor.
// Connections are implied between connectors of matching types
// that have the same index in the real and virtual interface.
//
// This information is precomputed on constraint initialization and stored in
// SIM_ConAnchorObjRegion so that the information can be re-used
// for an entire simulation.
//
class SIM_ConAnchorObjRegionVirtualInterface
{
public:
    using ConnectorVirtualPoint = SIM_ConAnchorObjRegionConnectorVirtualPoint;
    using ConnectorsVirtualPoint = std::vector< ConnectorVirtualPoint >;
    
    //TODO: use move semantics to pass in here:
    explicit SIM_ConAnchorObjRegionVirtualInterface(
        const ConnectorsVirtualPoint& connectors_at_vpoint_for_rpoint
    ) :
        myConnectorsAtVirtualPointForRealPoint( connectors_at_vpoint_for_rpoint )
    {
    }
    
    SIM_ConAnchorObjRegionVirtualInterface( const SIM_ConAnchorObjRegionVirtualInterface& ) = default;
    
    // TODO: ConnectorsVirtualPoint myConnectorsAtVirtualPointForRealPrimitiveSubsimplex;
    const ConnectorsVirtualPoint myConnectorsAtVirtualPointForRealPoint;
    
private:
    SIM_ConAnchorObjRegionVirtualInterface& operator=( const SIM_ConAnchorObjRegionVirtualInterface& ) = delete;
    // Return a reference to the set of virtual point connectors of
};

#define SIM_NAME_REGION_REGISTRATION_ATTRIBUTE "registrationattribute"
#define SIM_NAME_REGION_ALLOW_PARTIAL_OVERLAP "allowpartialoverlap"
#define SIM_NAME_REGION_ENABLE_MATCHING "enablematching"
#define SIM_NAME_REGION_PART_ID_ATTRIBUTE "identifierattribute"

#define SIM_NAME_REGION_ENABLE_MULTIPLIERS "enablemultipliers"
#define SIM_NAME_REGION_STRENGTH_ATTRIBUTE "strengthattribute"
#define SIM_NAME_REGION_DAMPING_ATTRIBUTE "dampingattribute"

class SIM_API SIM_ConAnchorObjRegion : public SIM_ConAnchor
{
public:
    using RealInterface = SIM_ConAnchorObjRegionRealInterface;
    using RealInterfaceUCP = UT_UniquePtr< const RealInterface >;

    using VirtualInterface = SIM_ConAnchorObjRegionVirtualInterface;
    using VirtualInterfaceUCP = UT_UniquePtr< const VirtualInterface >;

    GETSET_DATA_FUNCS_S( SIM_NAME_OBJECT, ObjectName );

    // Use a custom registration attribute
    GETSET_DATA_FUNCS_S( SIM_NAME_REGION_REGISTRATION_ATTRIBUTE, RegistrationAttribute );

    GETSET_DATA_FUNCS_B( SIM_NAME_REGION_ALLOW_PARTIAL_OVERLAP, AllowPartialOverlap );

    // Support region matching using a primitive integer attribute
    GETSET_DATA_FUNCS_B( SIM_NAME_REGION_ENABLE_MATCHING, EnableMatching );
    GETSET_DATA_FUNCS_S( SIM_NAME_REGION_PART_ID_ATTRIBUTE, PartIdAttribute );

    // Support local scaling of stiffness using 
    GETSET_DATA_FUNCS_B( SIM_NAME_REGION_ENABLE_MULTIPLIERS, EnableMultipliers );
    GETSET_DATA_FUNCS_S( SIM_NAME_REGION_STRENGTH_ATTRIBUTE, StrengthAttribute );
    GETSET_DATA_FUNCS_S( SIM_NAME_REGION_DAMPING_ATTRIBUTE, DampingAttribute );

    // Initialize the anchor by transferring ownership of
    // the real and virtual interfaces
    void initializeInterfaces(
        RealInterfaceUCP& real_interface,
        VirtualInterfaceUCP& virtual_interface
    );
    
    // Return a non-owning pointer to the real interface that's held
    // by this anchor; return nullptr when not available
    // (do not delete this pointer)
    const RealInterface* realInterface() const;
     
    // Return a non-owning pointer to the virtual interface that's held
    // by this anchor; return nullptr when not available
    // (do not delete this pointer)
    const VirtualInterface* virtualInterface() const;

protected:
    explicit SIM_ConAnchorObjRegion( const SIM_DataFactory *f );
    virtual ~SIM_ConAnchorObjRegion();

    bool getNeedsInitSubclass() const;
    
    virtual void initAnchorSubclass( const SIM_Object *object );
    
    virtual bool getObjectSpecificationSubclass( UT_String &str ) const;
 
    virtual void buildAnchorGuideGeometrySubclass(
        const SIM_Options& options,
        const GU_DetailHandle& gdh,
        const SIM_Relationship& rel,
        const SIM_Time& t
    ) const;

    virtual void        makeEqualSubclass( const SIM_Data* source );
    
    //TODO:
    //virtual void        saveSubclass(std::ostream &os) const;
    //virtual bool        loadSubclass(UT_IStream &is);
    
private:
    RealInterfaceUCP myRealInterface;
    VirtualInterfaceUCP myVirtualInterface;
    
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjRegion,
			SIM_ConAnchor,
			"Anchor: Object Region",
			getDopDescription());
};

#endif

