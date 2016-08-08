/**
 * @file UMAbcXform.cpp
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

#include "UMAbcXform.h"

namespace umabc
{
	
	using namespace Alembic::Abc;
	using namespace Alembic::AbcGeom;

	class UMAbcXform::Impl : public UMAbcObject
	{
		DISALLOW_COPY_AND_ASSIGN(Impl);
	public:
		Impl(IXformPtr xform)
			: UMAbcObject(xform)
			, xform_(xform)
		{}

		~Impl() {}

		/**
		* initialize
		* @param [in] recursive do children recursively
		* @retval succsess or fail
		*/
		bool init_(bool recursive);

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

		virtual UMAbcObjectPtr self_reference()
		{
			return self_reference_.lock();
		}
		
		UMAbcXformWeakPtr self_reference_;

	private:
		IXformPtr xform_;

		Imath::M44d static_matrix_;
		bool is_inherit_;
	};


/**
 * create
 */
UMAbcXformPtr UMAbcXform::create(IXformPtr xform)
{
	UMAbcXformPtr instance = UMAbcXformPtr(new UMAbcXform(xform));
	instance->impl_->self_reference_ = instance;
	return instance;
}

UMAbcXform::UMAbcXform(IXformPtr xform)
	: impl_(new UMAbcXform::Impl(xform))
	, UMAbcObject(xform)
{}

UMAbcXform::~UMAbcXform() {}

/**
 * initialize
 */
bool UMAbcXform::Impl::init_(bool recursive)
{
 	if (!is_valid() || xform_->getSchema().isConstantIdentity()) return false;
	
	self_reference()->mutable_local_transform().makeIdentity();
	static_matrix_.makeIdentity();

	if (xform_->getSchema().isConstant())
	{
		static_matrix_ =xform_->getSchema().getValue().getMatrix();
	}
	else
	{
		// not consistant.
		// we get time
		size_t num_samples = xform_->getSchema().getNumSamples();
		if (num_samples > 0)
		{
			TimeSamplingPtr time = xform_->getSchema().getTimeSampling();
			self_reference()->set_min_time(static_cast<unsigned long>(time->getSampleTime(0) * 1000));
			self_reference()->set_max_time(static_cast<unsigned long>(time->getSampleTime(num_samples - 1) * 1000));
		}
	}
	return true;
}

/**
 * set current time
 */
void UMAbcXform::Impl::set_current_time(unsigned long time, bool recursive)
{
	if (!is_valid()) {
		self_reference()->mutable_local_transform().makeIdentity();
		return;
	}
	
	if (self_reference()->min_time() <= time && time <= self_reference()->max_time())
	{
		ISampleSelector selector(time / 1000.0, ISampleSelector::kNearIndex);
		is_inherit_ = xform_->getSchema().getInheritsXforms(selector);

		if (xform_->getSchema().isConstant())
		{
			self_reference()->mutable_local_transform() = static_matrix_;
		}
		else
		{
			self_reference()->mutable_local_transform() =
					xform_->getSchema().getValue(selector).getMatrix();
		}
	}
}

/**
 * update box
 */
void UMAbcXform::Impl::update_box(bool recursive)
{
	UMAbcObject::update_box(recursive);

	mutable_box().makeEmpty();
	mutable_no_inherit_box().makeEmpty();

	UMAbcObjectList::iterator it = mutable_children().begin();
	for (; it != mutable_children().end(); ++it)
	{
		UMAbcObjectPtr child = *it;
		if (!child->box().isEmpty())
		{
			const Imath::Box3d& child_box = child->box();

			if (!child_box.isEmpty())
			{
				if (is_inherit_)
				{
					mutable_box().extendBy(child_box);
				}
				else
				{
					mutable_no_inherit_box().extendBy(child_box);
				}
			}

			const Imath::Box3d& child_no_inherit_box = child->no_inherit_box();
			if (!child_no_inherit_box.isEmpty())
			{
				mutable_no_inherit_box().extendBy(child_no_inherit_box);
			}
		}
	}
}

/**
* initialize
* @param [in] recursive do children recursively
* @retval succsess or fail
*/
bool UMAbcXform::init(bool recursive, UMAbcObjectPtr parent)
{
	impl_->init_(recursive);
	return UMAbcObject::init(recursive, parent);
}

/**
* set current time
* @param [in] time time
* @param [in] recursive do children recursively
*/
void UMAbcXform::set_current_time(unsigned long time, bool recursive)
{
	impl_->set_current_time(time, recursive);
	UMAbcObject::set_current_time(time, recursive);
}

/**
* update box
* @param [in] recursive do children recursively
*/
void UMAbcXform::update_box(bool recursive)
{
	impl_->update_box(recursive);
}

UMAbcObjectPtr UMAbcXform::self_reference()
{
	return impl_->self_reference();
}

/**
* get current time
*/
double UMAbcXform::current_time() const
{
	return impl_->current_time();
}

} // umabc
