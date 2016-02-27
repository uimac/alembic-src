/**
 * @file UMAbcScene.h
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
#include <map>
#include "UMMacro.h"
#include "UMAbcSetting.h"

/// uimac alembic library
namespace umabc
{
	
class UMAbcObject;
typedef std::shared_ptr<UMAbcObject> UMAbcObjectPtr;

class UMAbcScene;
typedef std::shared_ptr<UMAbcScene> UMAbcScenePtr;
typedef std::vector<UMAbcScenePtr> UMAbcSceneList;

class UMAbcScene //: public UMAbc
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcScene);
public:

	UMAbcScene(UMAbcObjectPtr root);
	~UMAbcScene();
	
	/**
	 * initialize
	 */
	virtual bool init();

	/**
	 * release all resources. call this function before delete.
	 */
	virtual bool dispose();
	
	/**
	 * update scene
	 */
	virtual bool update(unsigned long time);
	
	/**
	 * clear frame
	 */
	virtual bool clear();

	/**
	 * get minimum time
	 */
	double min_time() const;
	
	/**
	 * get maximum time
	 */
	double max_time() const;
	
	/**
	 * get name list
	 */
	std::vector<std::string> object_name_list();


	/**
	* get path list
	*/
	std::vector<std::string> mesh_path_list();

	/**
	* get path list
	*/
	std::vector<std::string> point_path_list();

	/**
	* get path list
	*/
	std::vector<std::string> curve_path_list();

	/**
	* get path list
	*/
	std::vector<std::string> nurbs_path_list();

	/**
	* get path list
	*/
	std::vector<std::string> camera_path_list();

	/**
	* get path list
	*/
	std::vector<std::string> xform_path_list();

	/**
	 * find object
	 */
	UMAbcObjectPtr find_object(const std::string& object_path);

	/**
	 * get root object
	 */
	UMAbcObjectPtr root_object();
	
	/**
	 * get total polygons
	 */
	size_t total_polygon_size() const;

private:
	class SceneImpl;
	typedef std::unique_ptr<SceneImpl> SceneImplPtr;
	SceneImplPtr impl_;
};

} // umabc
