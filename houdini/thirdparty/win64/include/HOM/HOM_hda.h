/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_hda_h__
#define __HOM_hda_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_ElemPtr.h"
#include "HOM_EnumModules.h"
#include <UT/UT_Experimental.h>
#include <vector>

class HOM_Node;
class HOM_EnumValue;
class HOM_HDADefinition;

SWIGOUT(%rename(hda) HOM_hda;)

class HOM_API HOM_hda
{
public:
    virtual ~HOM_hda()
    {}

    virtual std::string __repr__() = 0;

    SWIGOUT(%kwargs installFile;)
    virtual void installFile(const char *file_path,
	    const char *oplibraries_file = NULL,
	    bool change_oplibraries_file = true,
	    bool force_use_assets = false) = 0;

    SWIGOUT(%kwargs uninstallFile;)
    virtual void uninstallFile(const char *file_path,
	    const char *oplibraries_file = NULL,
	    bool change_oplibraries_file = true) = 0;

    virtual void reloadFile(const char *file_path) = 0;
    virtual void reloadAllFiles(bool rescan = true) = 0;

    virtual void reloadNamespaceOrder() = 0;

    virtual void expandToDirectory(const char *file_path,
				   const char *directory_path) = 0;

    virtual void collapseFromDirectory(const char *file_path,
				       const char *directory_path) = 0;

    virtual std::vector<std::string> loadedFiles() = 0;

    virtual void renameSource(const char *oplibraries_file,
	    const char *source_name = NULL) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_HDADefinition> >
	    definitionsInFile(const char *file_path) = 0;

    virtual std::vector<std::string> componentsFromFullNodeTypeName(
					const char *node_type_name) = 0;
    virtual std::string		     fullNodeTypeNameFromComponents(
					const char *scope_node_type,
					const char *name_space,
					const char *name,
					const char *version) = 0;

    virtual void changeCurrentStoreUser(const char *new_user) = 0;

    virtual bool safeguardHDAs() = 0;
    virtual void setSafeguardHDAs(bool on) = 0;

    SWIGOUT(%ignore addOpaqueEventCallback;)
    virtual void addOpaqueEventCallback(
	    const std::vector<HOM_EnumValue *> &event_types, void *py_callback) = 0;
    SWIGOUT(%ignore removeOpaqueEventCallback;)
    virtual void removeOpaqueEventCallback(
	    const std::vector<HOM_EnumValue *> &event_types, void *py_callback) = 0;
    virtual void removeAllEventCallbacks() = 0;
    SWIGOUT(%ignore opaqueEventCallbacks;)
    virtual std::vector<std::pair<std::vector<HOM_EnumValue *>, void *> > 
	opaqueEventCallbacks() = 0;

#ifdef SWIG
%extend
{
    void addEventCallback(
	    const std::vector<HOM_EnumValue *> &event_types,
	    InterpreterObject callback)
    { self->addOpaqueEventCallback(event_types, callback); }

    void removeEventCallback(
	    const std::vector<HOM_EnumValue *> &event_types,
	    InterpreterObject callback)
    { self->removeOpaqueEventCallback(event_types, callback); }

    std::vector<std::pair<std::vector<HOM_EnumValue *>, InterpreterObject> >
	eventCallbacks()
    {
	std::vector<std::pair<std::vector<HOM_EnumValue *>, void *> > 
	    opaque_callbacks(self->opaqueEventCallbacks());

	std::vector<std::pair<std::vector<HOM_EnumValue *>, InterpreterObject> >
	    callbacks;
	for (int i=0; i< opaque_callbacks.size(); ++i)
	    callbacks.push_back(std::make_pair(
		opaque_callbacks[i].first, 
		(InterpreterObject)opaque_callbacks[i].second));
	return callbacks;
    }
}
#endif

#ifdef ENABLE_LICENSED_ASSETS
    SWIGOUT(%kwargs encryptAsset;)
    virtual void encryptAsset(
	    HOM_Node &node,
	    const char *file_path,
	    const char *email,
	    const char *password,
	    const std::vector<std::string> &license_names,
	    bool compile_basic = true,
	    bool compile_vopnets = true,
	    bool compile_channels = true,
	    bool compile_nodenames = true) = 0;

    SWIGOUT(%kwargs createEntitlement;)
    virtual void createEntitlement(
	    const char *email,
	    const char *password,
	    const char *license_name,
	    const char *entitled_email,
	    HOM_EnumValue &license_type = HOM_hdaLicenseType::Execute,
	    double expiry = 0.0f) = 0;

    SWIGOUT(%kwargs availableEntitlements;)
    virtual std::vector<std::string> availableEntitlements(
			const char *email,
			const char *password) = 0;

    SWIGOUT(%kwargs redeemEntitlements;)
    virtual void redeemEntitlements(
	    const char *email,
	    const char *password,
	    const char *license_file = NULL,
	    const std::vector<std::string> &entitlements =
		std::vector<std::string>()) = 0;
#endif // ENABLE_LICENSED_ASSETS
};

#endif
