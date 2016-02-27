/**
 * @file UMAbcSetting.h
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

/// uimac alembic library
namespace umabc
{

class UMAbcSetting
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcSetting);
public:

	UMAbcSetting() {}
	~UMAbcSetting() {}

	std::string export_type() const { return "ogawa"; }

private:
};

} // umabc
