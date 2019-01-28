//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#ifndef TF_PYINTERPRETER_H
#define TF_PYINTERPRETER_H

/// \file tf/pyInterpreter.h
/// Python runtime utilities.

#include "pxr/pxr.h"

#include "pxr/base/tf/api.h"
#include <hboost/python/handle.hpp>
#include <hboost/python/object.hpp>
#include <string>

PXR_NAMESPACE_OPEN_SCOPE

/// Starts up the python runtime.
/// IMPORTANT: The GIL will NOT be held when this returns.
/// (If Python is already initialized, the GIL will be unchanged)
///
/// The program name and arguments are set automatically. sys.argv has no
/// arguments other than an argv[0] matching the program name.
TF_API
extern void TfPyInitialize();

/// Runs the given string using PyRun_SimpleString().
///
/// Starts the interpreter if necessary. Deals with necessary thread state
/// setup.
TF_API
extern int TfPyRunSimpleString(const std::string & cmd);

/// Runs the given string using PyRun_String().
///
/// \a start is Py_eval_input, Py_single_input or Py_file_input.
/// \a globals and locals can be dictionaries to use when evaluating the 
///    string in python. Defaults to reusing globals from main module. If
///    only the globals are provided, they will also be used as locals.
///
/// Starts the interpreter if necessary. Deals with necessary thread state
/// setup.
TF_API
extern hboost::python::handle<>
TfPyRunString(const std::string & cmd, int start,
              hboost::python::object const &globals = hboost::python::object(),
              hboost::python::object const &locals = hboost::python::object()
              );

/// Runs the given file using PyRun_File().
///
/// \a start is Py_eval_input, Py_single_input or Py_file_input.
/// \a globals and locals can be dictionaries to use when evaluating the 
///    string in python. Defaults to reusing globals from main module. If
///    only the globals are provided, they will also be used as locals.
///
/// Starts the interpreter if necessary. Deals with necessary thread state
/// setup.
TF_API
extern hboost::python::handle<>
TfPyRunFile(const std::string &filename, int start,
            hboost::python::object const &globals = hboost::python::object(),
            hboost::python::object const &locals = hboost::python::object()
            );

/// Returns the disk path to the given module as an NSString.
///
/// Starts the interpreter if necessary. Deals with necessary thread state
/// setup.
TF_API
extern std::string TfPyGetModulePath(const std::string & moduleName);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // TF_PYINTERPRETER_H