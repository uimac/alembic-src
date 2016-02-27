/**
 * @file UMAbcNurbsPatch.h
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
#include "UMMacro.h"
#include "ImathVec.h"
#include "UMAbcObject.h"

namespace Alembic {
	namespace Abc {
		namespace v7 {
			template <class SCHEMA>
			class ISchemaObject;
		}
	}
	namespace AbcGeom {
		namespace v7 {
			class INuPatchSchema;
			typedef Alembic::Abc::v7::ISchemaObject<INuPatchSchema> INuPatch;
		}
	}
}

namespace umabc
{
typedef std::shared_ptr<Alembic::AbcGeom::v7::INuPatch> INuPatchPtr;
	
class UMAbcNurbsPatch;
typedef std::shared_ptr<UMAbcNurbsPatch> UMAbcNurbsPatchPtr;
typedef std::weak_ptr<UMAbcNurbsPatch> UMAbcNurbsPatchWeakPtr;

class UMAbcNurbsPatch : public UMAbcObject
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcNurbsPatch);
public:

	/**
	 * crate instance
	 */
	static UMAbcNurbsPatchPtr create(INuPatchPtr patch);

	~UMAbcNurbsPatch();

	/**
	 * initialize
	 * @param [in] recursive do children recursively
	 * @retval succsess or fail
	 */
	virtual bool init(bool recursive, UMAbcObjectPtr parent);
	
	/**
	 * set current time
	 * @param [in] time time
	 * @param [in] recursive do children recursively
	 */
	virtual void set_current_time(unsigned long time, bool recursive);
	
	/**
	 * update box
	 * @param [in] recursive do children recursively
	 */
	virtual void update_box(bool recursive);

	/**
	* get position
	*/
	const Imath::V3f * positions() const;

	/**
	* get position size
	*/
	unsigned int position_size() const;

	/**
	 * get u_knots
	 */
	const float * u_knots() const;

	/**
	* get u_knot size
	*/
	unsigned int u_knot_size() const;

	/**
	* get v_knots
	*/
	const float * v_knots() const;

	/**
	* get v_knot size
	*/
	unsigned int v_knot_size() const;

	/**
	* get u_size
	*/
	unsigned int u_size() const;

	/**
	* get v_size
	*/
	unsigned int v_size() const;

	/**
	* get u_order
	*/
	int u_order() const;

	/**
	* get v_order
	*/
	int v_order() const;


	/**
	 * update patch all
	 */
	void update_patch_all();

protected:
	UMAbcNurbsPatch(INuPatchPtr patch);

	virtual UMAbcObjectPtr self_reference();

private:
	class Impl;
	std::unique_ptr <Impl> impl_;
};

}
