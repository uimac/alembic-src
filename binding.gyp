{
  "targets": [
    {
      "target_name": "umabcio",
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
			"src/umdraw/UMCamera.cpp",
			"src/umdraw/UMCamera.h",
			"src/umdraw/UMCurve.cpp",
			"src/umdraw/UMCurve.h",
			"src/umdraw/UMLight.cpp",
			"src/umdraw/UMLight.h",
			"src/umdraw/UMLine.cpp",
			"src/umdraw/UMLine.h",
			"src/umdraw/UMMaterial.cpp",
			"src/umdraw/UMMaterial.h",
			"src/umdraw/UMMesh.cpp",
			"src/umdraw/UMMesh.h",
			"src/umdraw/UMMeshGroup.h",
			"src/umdraw/UMNode.h",
			"src/umdraw/UMPoint.cpp",
			"src/umdraw/UMPoint.h",
			"src/umdraw/UMScene.cpp",
			"src/umdraw/UMScene.h",
			"src/umdraw/UMShaderEntry.cpp",
			"src/umdraw/UMShaderEntry.h",
			"src/umdraw/UMSkin.h",
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
          "<(module_root_dir)/src/umdraw" ],
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
             'AdditionalOptions': [
              '/GR',
             ]
          },
        }
    }
  ]
}
