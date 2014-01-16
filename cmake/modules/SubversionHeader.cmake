# ***************************************************************************
# *                                                                         *
# *   Copyright : (C) 2010 The University of Toronto                        *
# *   email     : netterfield@astro.utoronto.ca                             *
# *                                                                         *
# *   Copyright : (C) 2010 Peter Kümmel                                     *
# *   email     : syntheticpp@gmx.net                                       *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# ***************************************************************************

macro(SubversionHeader source_dir target_name header_file modified_str)
	# a custom target that is always built
	# creates ${header_file} using SubversionGenerator.cmake script
	add_custom_target(${target_name} ALL 
		COMMAND ${CMAKE_COMMAND} 
			-Dsource_dir="${source_dir}"
			-Dheader_file="${header_file}"
			-Dmodified_str="${modified_str}"
			-P ${CMAKE_CURRENT_SOURCE_DIR}/modules/SubversionGenerator.cmake)

	# ${header_file} is a generated file
	set_source_files_properties(${header_file}
			PROPERTIES
			GENERATED TRUE
			HEADER_FILE_ONLY TRUE)
endmacro()

