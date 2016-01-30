{
	"targets": [
	{
		"target_name": "umabcio",
		"conditions": [
				[	"OS == 'win'", {
					"libraries": [
					"<(module_root_dir)/lib/hdf5/x64/Release/libhdf5_hl.lib",
					"<(module_root_dir)/lib/hdf5/x64/Release/libhdf5.lib",
					"<(module_root_dir)/lib/ilmbase2/x64/Release/Imath.lib",
					"<(module_root_dir)/lib/ilmbase2/x64/Release/IlmThread.lib",
					"<(module_root_dir)/lib/ilmbase2/x64/Release/IexMath.lib",
					"<(module_root_dir)/lib/ilmbase2/x64/Release/Iex.lib",
					"<(module_root_dir)/lib/ilmbase2/x64/Release/Half.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicAbc.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicAbcCollection.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicAbcCoreAbstract.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicAbcCoreFactory.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicAbcCoreHDF5.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicAbcCoreOgawa.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicAbcGeom.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicAbcMaterial.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicOgawa.lib",
					"<(module_root_dir)/lib/alembic/x64/Release/AlembicUtil.lib",
					"shlwapi.lib",
					"winmm.lib",
					],
					"configurations": {
						'Debug': {
							'msvs_settings': {
									'VCCLCompilerTool': {
										'RuntimeLibrary': '3' # /MDd
								},
							},
						},
						'Release': {
							'msvs_settings': {
									'VCCLCompilerTool': {
										'RuntimeLibrary': '2' # /MD
								},
							},
						},
					},
					"msvs_configuration_attributes": {
						'CharacterSet': '1'
					},
					'defines': [ '_HAS_EXCEPTIONS=1' ],
					'msvs_settings': {
						'VCCLCompilerTool': {
						'ExceptionHandling': '1',  # /EHsc
						'AdditionalOptions': [ '/GR' ]
						},
					}
				}
			],
			["OS == 'mac'", {
					"xcode_settings": {
					"GCC_ENABLE_CPP_RTTI": "YES",
					"GCC_ENABLE_CPP_EXCEPTIONS": "YES",
					"MACOSX_DEPLOYMENT_TARGET" : "10.7",
					"OTHER_CPLUSPLUSFLAGS" : ['-std=c++11','-stdlib=libc++'],
					"OTHER_LDFLAGS": ['-stdlib=libc++'],
					},
					"libraries": [
					"<(module_root_dir)/lib/hdf5/mac/Release/libhdf5_hl.a",
					"<(module_root_dir)/lib/hdf5/mac/Release/libhdf5.a",
					"<(module_root_dir)/lib/ilmbase2/mac/Release/libImath.a",
					"<(module_root_dir)/lib/ilmbase2/mac/Release/libIlmThread.a",
					"<(module_root_dir)/lib/ilmbase2/mac/Release/libIexMath.a",
					"<(module_root_dir)/lib/ilmbase2/mac/Release/libIex.a",
					"<(module_root_dir)/lib/ilmbase2/mac/Release/libHalf.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicAbc.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicAbcCollection.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicAbcCoreAbstract.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicAbcCoreFactory.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicAbcCoreHDF5.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicAbcCoreOgawa.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicAbcGeom.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicAbcMaterial.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicOgawa.a",
					"<(module_root_dir)/lib/alembic/mac/Release/libAlembicUtil.a"
					],
				}
			]
		],
		"sources": [
		"src/umabc/UMAbcCamera.cpp",
		"src/umabc/UMAbcCamera.h",
		"src/umabc/UMAbcConvert.h",
		"src/umabc/UMAbcCurve.cpp",
		"src/umabc/UMAbcCurve.h",
		"src/umabc/UMAbcMesh.cpp",
		"src/umabc/UMAbcMesh.h",
		"src/umabc/UMAbcNode.h",
		"src/umabc/UMAbcNurbsPatch.cpp",
		"src/umabc/UMAbcNurbsPatch.h",
		"src/umabc/UMAbcObject.cpp",
		"src/umabc/UMAbcObject.h",
		"src/umabc/UMAbcPoint.cpp",
		"src/umabc/UMAbcPoint.h",
		"src/umabc/UMAbcScene.cpp",
		"src/umabc/UMAbcScene.h",
		"src/umabc/UMAbcSetting.h",
		"src/umabc/UMAbcSoftwareIO.cpp",
		"src/umabc/UMAbcSoftwareIO.h",
		"src/umabc/UMAbcXform.cpp",
		"src/umabc/UMAbcXform.h",
		"src/umbase/UMAny.h",
		"src/umbase/UMBox.cpp",
		"src/umbase/UMBox.h",
		"src/umbase/UMMacro.h",
		"src/umbase/UMMath.h",
		"src/umbase/UMMathTypes.h",
		"src/umbase/UMMatrix.h",
		"src/umbase/UMPath.cpp",
		"src/umbase/UMPath.h",
		"src/umbase/UMStringUtil.h",
		"src/umbase/UMTime.cpp",
		"src/umbase/UMTime.h",
		"src/umbase/UMVector.h",
		"src/umabcio.cpp"
		],
		"include_dirs": [
		"<(module_root_dir)/lib/hdf5/include",
		"<(module_root_dir)/lib/ilmbase2/include",
		"<(module_root_dir)/lib/ilmbase2/include/Half",
		"<(module_root_dir)/lib/ilmbase2/include/Iex",
		"<(module_root_dir)/lib/ilmbase2/include/IexMath",
		"<(module_root_dir)/lib/ilmbase2/include/IlmThread",
		"<(module_root_dir)/lib/ilmbase2/include/Imath",
		"<(module_root_dir)/lib/ilmbase2/include/OpenEXR",
		"<(module_root_dir)/lib/ilmbase2/include/config.windows",
		"<(module_root_dir)/lib/alembic/include",
		"<(module_root_dir)/src/",
		"<(module_root_dir)/src/umbase",
		"<(module_root_dir)/src/umabc",
		"<(module_root_dir)/src/umdraw"
		]
	}]
}
