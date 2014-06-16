/*
 *  Multi2Sim
 *  Copyright (C) 2014  Yifan Sun (yifansun@coe.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef ARCH_HSA_EMU_CONTEXT_H
#define ARCH_HSA_EMU_CONTEXT_H

#include "arch/x86/emu/FileTable.h"

#include "Emu.h"

namespace HSA
{

class Emu;

/// HSA Context
class Context{
 	// Emulator that is belongs to 
 	Emu *emu;

 	// Process ID
 	int pid;

	// File descriptor table, shared by contexts
	std::shared_ptr<x86::FileTable> file_table;

 	///
 	/// Functions and data stuctures related to the program loader
 	/// Functions implemented in CopntextLoader.cc
 	///

 	/// Structure containing information initialized by the program loader,
 	/// associated with a context. When a context is created from a program 
 	/// executable, a Loader object is associtaed to it. All child contexts
 	/// spawned by it will share the same Loader object
 	struct Loader
 	{
 		// Binary file in brig format
 		std::unique_ptr<BrigFile> binary;	

 		// Command-line arguments
 		std::vector<std::string> args;

 		// Executable file name
 		std::string exe;

 		// Current working directory
 		std::string cwd;

 		// File name for standard input and output
 		std::string stdin_file_name;
 		std::string stdout_file_name;
 	};

 	// Loader information. This information can be shared among multiple 
 	// contexts. For this reason, it is declared as a shared pointer. The
 	// last destructed context sharing this variable will automatically free
 	// it.
 	std::shared_ptr<Loader> loader;

 	// Load Brig ELF binary, as alread decoded in 'loader.binary'
 	void LoadBinary();

 public:
 	/// Create a context from a command line. To safely create a context 
 	/// function Emu::NewContext should be used instead. After the creation 
 	/// of a context, its basic data structures are initialized with Load(),
 	/// Clone(), or Fork()
 	Context();

 	/// Destructor
 	~Context();

 	/// Load a program on the xontext. The meaning of each argument is 
 	/// identical to the prototype of comm::Emy::Load()
 	void Load(const std::vector<std::string> &args,
 			const std::vector<std::string> &env = { },
 			const std::string &cwd = "",
 			const std::string &stdin_file_name = "",
 			const std::string &stdout_file_name = "");
};

}// namespace HSA

 #endif