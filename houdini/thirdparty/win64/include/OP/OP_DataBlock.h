/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_DataBlock.h (OP Library, C++)
 *
 * COMMENTS:	Simple class for holding a chunk of arbitrary binary data
 *		owned by a node.
 */

#ifndef __OP_DATABLOCK_H_INCLUDED__
#define __OP_DATABLOCK_H_INCLUDED__

#include "OP_API.h"
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_StringHolder.h>
#include <iosfwd>

class UT_IStream;

class OP_API OP_DataBlockInterpreter
{
public:
			 OP_DataBlockInterpreter(const UT_StringHolder &type);
    virtual		~OP_DataBlockInterpreter();

    virtual const void	*createNativeData(const char *data,
				exint length) const = 0;
    virtual exint	 memoryUsedByNativeData(
				const void *nativedata) const = 0;
    virtual void	 deleteNativeData(const void *nativedata) const = 0;
    virtual void	 saveNativeData(const void *nativedata,
				std::ostream &os) const = 0;

private:
    UT_StringHolder	 myType;
};

class OP_API OP_DataBlock : public UT_IntrusiveRefCounter<OP_DataBlock>
{
public:
				 OP_DataBlock(const UT_StringHolder &type);
				~OP_DataBlock();

    static const OP_DataBlockInterpreter *
				 getInterpreter(const UT_StringRef &blocktype);

    const UT_StringHolder	&type() const
				 { return myType; }
    const char			*data() const
				 { return myData; }
    exint			 length() const
				 { return myLength; }
    exint			 getMemoryUsage() const;

    const void			*nativeData();
    void			 adoptData(const char *data, exint length);
    void			 adoptNativeData(const void *nativedata);
    bool			 saveData(std::ostream &os) const;
    bool			 loadData(UT_IStream &is);

private:
    void			 deleteData();
    void			 deleteNativeData();

    const UT_StringHolder	 myType;
    const char			*myData;
    exint			 myLength;
    const void			*myNativeData;
    bool			 myNativeDataIsValid;
};

#endif // __OP_DATABLOCK_H_INCLUDED__
