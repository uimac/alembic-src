/**
 * @file UMAbcCamera.cpp
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include <algorithm>
#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>

#include "UMAbcCamera.h"

namespace umabc
{
	using namespace Alembic::Abc;
	using namespace Alembic::AbcGeom;

	class UMAbcCamera::Impl : public UMAbcObject
	{
		DISALLOW_COPY_AND_ASSIGN(Impl);
	public:
		Impl(ICameraPtr camera)
			: UMAbcObject(camera)
			, camera_(camera)
		{
		}
		~Impl() {}

		bool init(bool recursive);

		void set_current_time(unsigned long time, bool recursive);

		virtual UMAbcObjectPtr self_reference()
		{
			return self_reference_.lock();
		}
		
		UMAbcCameraWeakPtr self_reference_;

	private:
		ICameraPtr camera_;
		Alembic::AbcGeom::CameraSample sample_;
	};


bool UMAbcCamera::Impl::init(bool recursive)
{
	if (!is_valid()) return false;

	ICameraSchema &schema = camera_->getSchema();

	const size_t num_samples = schema.getNumSamples();
	if (num_samples > 0)
	{
		// get sample
		schema.get(sample_);

		// if not consistant, we get time
		//if (!schema.isConstant())
		{
			TimeSamplingPtr time = schema.getTimeSampling();
			unsigned long min = static_cast<unsigned long>(time->getSampleTime(0) * 1000);
			unsigned long max = static_cast<unsigned long>(time->getSampleTime(num_samples - 1) * 1000);
			self_reference()->set_min_time(min);
			self_reference()->set_max_time(max);
		}
	}
	return true;
}

void UMAbcCamera::Impl::set_current_time(unsigned long time, bool recursive)
{
}

/**
 * create
 */
UMAbcCameraPtr UMAbcCamera::create(ICameraPtr camera)
{
	UMAbcCameraPtr instance = UMAbcCameraPtr(new UMAbcCamera(camera));
	instance->impl_->self_reference_ = instance;
	return instance;
}

/**
 * construtor
 */
UMAbcCamera::UMAbcCamera(ICameraPtr camera)
	: UMAbcObject(camera)
	, impl_(new UMAbcCamera::Impl(camera))
{
}

/**
 * destructor
 */
UMAbcCamera::~UMAbcCamera()
{
}

/**
 * initialize
 */
bool UMAbcCamera::init(bool recursive, UMAbcObjectPtr parent)
{
	impl_->init(recursive);
	return UMAbcObject::init(recursive, parent);
}

/**
 * set current time
 */
void UMAbcCamera::set_current_time(unsigned long time, bool recursive)
{
	if (!impl_->is_valid()) return;
	impl_->set_current_time(time, recursive);
	UMAbcObject::set_current_time(time, recursive);
}

/**
 * update box
 */
void UMAbcCamera::update_box(bool recursive)
{
}

UMAbcObjectPtr UMAbcCamera::self_reference()
{
	return impl_->self_reference();
}

}

