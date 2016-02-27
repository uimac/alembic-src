/**
 * @file UMAbc.h
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <memory>
#include <string>
#include <vector>
#include "UMMacro.h"
#include "UMAbcSetting.h"

/// uimac alembic library
namespace umabc
{
	
class UMAbcScene;
typedef std::shared_ptr<UMAbcScene> UMAbcScenePtr;

class UMAbcObject;
typedef std::shared_ptr<UMAbcObject> UMAbcObjectPtr;

class UMAbcMesh;
typedef std::shared_ptr<UMAbcMesh> UMAbcMeshPtr;

class UMAbcSoftwareIO;
typedef std::shared_ptr<UMAbcSoftwareIO> UMAbcSoftwareIOPtr;

class UMAbcSoftwareIO
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcSoftwareIO);
public:

	UMAbcSoftwareIO() {}
	~UMAbcSoftwareIO() {}
	
	/**
	 * release all scenes. call this function before delete.
	 */
	void dispose();

	/**
	 * load 3d file to UMAbcScene
	 */
	UMAbcScenePtr load(std::string path, const UMAbcSetting& setting);
	
	/**
	 * save 3d file
	 */
	bool save(std::string path, UMAbcScenePtr scene, const UMAbcSetting& setting);

	/**
	 * load settings
	 */
	bool load_setting(std::string path, UMAbcSetting& setting);
	
	/**
	 * save settings
	 */
	bool save_setting(std::string path, const UMAbcSetting& setting);

private:
};

} // umabc
