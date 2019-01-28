/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        OP_FileDependencyMgr.h
 *
 * COMMENTS:
 */

#ifndef __OP_FileDependencyMgr_h__
#define __OP_FileDependencyMgr_h__

#include "OP_API.h"
#include "OP_Node.h"
#include <UT/UT_DeepString.h>
#include <UT/UT_Array.h>
class UT_StringArray;
class PRM_Parm;
class PRM_ParmList;
class OP_Node;
class OP_Network;

// This class stores a a node and the name of a file referenced by that node.
// Note that the node may be null if node tracking is not wanted.
class OP_API OP_NodeFileNamePair
{
public:
    OP_NodeFileNamePair(OP_Node *node, const char *file_name,
			PRM_Parm *parm=NULL, PRM_Parm *ref_parm=NULL)
    : myNode(node), 
      myFileName(file_name),
      myParm(parm),
      myRefParm(ref_parm)
    {}

    // This constructor is needed for UT_Array.
    OP_NodeFileNamePair()
    : myNode(NULL), 
      myParm(NULL),
      myRefParm(NULL)
    {}

    OP_Node	*myNode;
    UT_DeepString myFileName;
    PRM_Parm	*myParm;
    PRM_Parm	*myRefParm;
};

typedef UT_Array<OP_NodeFileNamePair> OP_NodeFileNameArray;


// Access this class by calling OPgetFileDependencyMgr().
class OP_API OP_FileDependencyMgr
{
public:
    OP_FileDependencyMgr();

    enum SkipHFSOption { SKIP_HFS_FILES, KEEP_HFS_FILES };
    enum TrackNodesOption { TRACK_NODES, SKIP_NODES };
    enum CondensePathOption { CONDENSE_HIP, CONDENSE_JOB };
    enum OpdefOption { EXCLUDE_OPDEFS, INCLUDE_OPDEFS };

    // Retrieves all the file paths referenced by any node 
    // in the current .hip file.
    // Files that are found are placed in `found_files`.
    // Missing files are placed in `unknown_files`.
    void getInputFilesInParms(SkipHFSOption skip_hfs_option,
			      TrackNodesOption track_nodes_option,
			      OpdefOption opdef_option,
			      OP_NodeFileNameArray &found_files,
			      OP_NodeFileNameArray &unknown_files) const;

    // Retrieves the OTL files which define any custom operators that
    // are currently in use.  Note that this method also returns other files
    // needed by the otl files, such as vex files.
    void getOTLFiles(UT_StringArray &files, OP_NodeList* otl_nodes_out, 
		     SkipHFSOption skip_hfs_option=SKIP_HFS_FILES,
		     CondensePathOption condense_path_option = CONDENSE_HIP) const;

    // Merges 2 NodeFileNameArray objects.  Removes duplicate entries.
    void mergeDependencyLists(const OP_NodeFileNameArray &src_refs1,
                              const OP_NodeFileNameArray &src_refs2,
			      OP_NodeFileNameArray &dest_refs,
			      TrackNodesOption track_nodes_option) const;

    // Attempts to add a file dependency to the specified node and parm.
    // Returns true if successful.  False otherwise.
    // If false, then `resultMsg` is populated with an error message.
    bool addFileDependency(UT_String node_path,
			   UT_String str_parm,
			   UT_String file_path,
			   UT_String resultMsg) const;
    
    // Attempts to remove a specified file dependency.
    // Returns true if successful.  False otherwise.
    // If false, then `resultMsg` is populated with an error message.
    bool removeFileDependency(UT_String node_path,
			      UT_String str_parm,
			      UT_String resultMsg) const;

    // Attempts to hide a specified file dependency.
    // Returns true if successful.  False otherwise.
    // If false, then `resultMsg` is populated with an error message.
    bool hideFileDependency(UT_String node_path,
			    UT_String str_parm,
			    UT_String resultMsg) const;

    // Get/set the set of file patterns that the user explicitly unselected
    // from the file dependency dialog.  By default, all file references that
    // are created are implicitly selected, so we explicitly store the
    // unselected ones instead of the selected ones.
    void setUnselectedFilePatterns(const UT_StringArray &unselected_patterns)
    { myUnselectedFilePatterns = unselected_patterns; }
    void getUnselectedFilePatterns(UT_StringArray &result)
    { result = myUnselectedFilePatterns; }

private:
    // Retrieves the OTL files which define any custom operators that are
    // currently in use.  Recursive version of getOTLFiles().
    void getOTLFiles(
	OP_Node &node, UT_StringArray &files, OP_NodeList* otl_nodes_out,
	SkipHFSOption skip_hfs_option=SKIP_HFS_FILES,
	CondensePathOption condense_path_option = CONDENSE_HIP) const;

    // Returns true if the given file.  Returns false otherwise.
    // Uses Houdini search paths to locate the file.
    bool findPathToFileName(UT_String &file_name) const;

    // Returns true if the given parameter references an external file.
    // Returns false otherwise.
    bool parmReferencesInputFile(OP_Node *owner,
				 PRM_Parm &parm,
				 OpdefOption opdef_option) const;

    // Returns true if the $HFS path appears in the given file path.
    // Returns false otherwise.
    bool fileIsInHFS(const char *file_name) const;

    // Retrieves all the input files referenced by the network and its children.
    // Recursive version of getInputFilesInParms().
    // Files that are found are placed in `found_files`.
    // Missing files are placed in `unknown_files`.
    void getInputFilesInParms(OP_Node &network,
			      SkipHFSOption skip_hfs_option,
			      TrackNodesOption track_nodes_option,
			      OpdefOption opdef_option,
			      OP_NodeFileNameArray &found_files,
			      OP_NodeFileNameArray &unknown_files) const;

    // Retrieves all the input files referenced by parameters in
    // the given parameter list.
    // Files that are found are placed in `found_files`.
    // Missing files are placed in `unknown_files`.
    void getInputFilesInParmList(PRM_ParmList *parm_list,
				 OP_Node *owner,
				 SkipHFSOption skip_hfs_option,
				 OpdefOption opdef_option,
				 OP_NodeFileNameArray &found_files,
				 OP_NodeFileNameArray &unknown_files) const;

    // Removes duplicate entries from `files`.
    void removeDuplicatesFromNodeFileNameArray
					(OP_NodeFileNameArray &files,
					 TrackNodesOption track_nodes_option)
					const;

    // Removes duplicate entries from the string array.
    void removeDuplicatesFromStringArray(UT_StringArray &string_array) const;

    // Removes any objects from set1 that appear in set2.
    void subtractSets (OP_NodeFileNameArray &set1,
                       const OP_NodeFileNameArray &set2) const;

    /// Determines whether there are more references in found parms to paths
    /// starting with $HIP or whether there are more references to the paths
    /// beginning with $JOB.
    bool determineRootType(
	OP_NodeFileNameArray &found_files,
	OP_NodeFileNameArray &unknown_files) const;

    // Given an absolute or relative path, modify the string to contain a
    // a condensed path relative to a variable (e.g. the result will start
    // with "$HIP/" or "$JOB/".
    void condensePath(
	UT_String &path, CondensePathOption condense_path_option) const;

    // Try to get the unembedded vex file corresponding to a operator type.
    // Returns false if there isn't one.
    bool getUnembeddedVexFileForOpType(
	OP_Operator &op_type, UT_String &result) const;

    // Some file references, such as FBX and Collada, may store extra 
    // information in the path. This is done by appending a hash and then
    // a string with options after the file exention. This function removes
    // all such information from the file names passed in.
    void cleanupExtraFileInfo(OP_NodeFileNameArray& file_names) const;

    UT_StringArray myUnselectedFilePatterns;
};

// Use OPsetFileDependencyMgr() to set the dependency manager to a subclass
// of OP_FileDependencyMgr.  The function takes ownership of the object
// passed into it.
OP_API OP_FileDependencyMgr &OPgetFileDependencyMgr();
OP_API void OPsetFileDependencyMgr(OP_FileDependencyMgr &file_dependency_mgr);

#endif
