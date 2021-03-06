/**
 * @file UMAbcCurve.cpp
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

#include "UMAbcCurve.h"

namespace umabc
{
	using namespace Alembic::Abc;
	using namespace Alembic::AbcGeom;
	
	class UMAbcCurve::Impl : public UMAbcObject
	{
		DISALLOW_COPY_AND_ASSIGN(Impl);
	public:
		Impl(ICurvesPtr curves)
			: UMAbcObject(curves)
			, curves_(curves)
		{}

		/**
		* initialize
		* @param [in] recursive do children recursively
		* @retval succsess or fail
		*/
		virtual bool init(bool recursive);

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
		* update curve all
		*/
		void update_curve_all();


		virtual UMAbcObjectPtr self_reference()
		{
			return self_reference_.lock();
		}

		UMAbcCurveWeakPtr self_reference_;

		Alembic::AbcGeom::P3fArraySamplePtr positions() const { return positions_; }

		unsigned int curve_count() const { return curve_count_; }

		Alembic::AbcGeom::Int32ArraySamplePtr vertex_count() const { return vertex_count_; }

		const std::vector<int>& vertex_count_list() const { return vertex_count_list_; }

	private:
		ICurvesPtr curves_;
		Alembic::AbcGeom::P3fArraySamplePtr positions_;
		Alembic::AbcGeom::Int32ArraySamplePtr vertex_count_;
		std::vector<int> vertex_count_list_;
		size_t curve_count_;

		Alembic::AbcGeom::ICurvesSchema::Sample initial_sample_;

		std::vector<const Imath::V3f* > points_;
	};

/**
 * create
 */
UMAbcCurvePtr UMAbcCurve::create(ICurvesPtr curves)
{
	UMAbcCurvePtr instance = UMAbcCurvePtr(new UMAbcCurve(curves));
	instance->impl_->self_reference_ = instance;
	return instance;
}

UMAbcCurve::UMAbcCurve(ICurvesPtr curves)
	: UMAbcObject(curves)
	, impl_(new UMAbcCurve::Impl(curves))
{}

UMAbcCurve::~UMAbcCurve()
{
}

/**
 * initialize
 */
bool UMAbcCurve::Impl::init(bool recursive)
{
	if (!is_valid()) return false;
	
	const size_t num_samples = curves_->getSchema().getNumSamples();
	if (num_samples > 0)
	{
		// get sample
		curves_->getSchema().get(initial_sample_);
		
		// if not consistant, we get time
		if (!curves_->getSchema().isConstant())
		{
			TimeSamplingPtr time = curves_->getSchema().getTimeSampling();
			self_reference()->set_min_time(static_cast<unsigned long>(time->getSampleTime(0) * 1000));
			self_reference()->set_max_time(static_cast<unsigned long>(time->getSampleTime(num_samples - 1) * 1000));
		}
	}
	return true;
}

/**
 * set current time
 */
void UMAbcCurve::Impl::set_current_time(unsigned long time, bool recursive)
{
	if (!is_valid()) return;
	
	update_curve_all();
}

/**
 * update curve all
 */
void UMAbcCurve::Impl::update_curve_all()
{
	if (!is_valid()) return;
	if (curves_->getSchema().getNumSamples() <= 0) return;

	ISampleSelector selector(self_reference()->current_time(), ISampleSelector::kNearIndex);
	ICurvesSchema::Sample sample;
	curves_->getSchema().get(sample, selector);

	positions_ = sample.getPositions();
	curve_count_ = sample.getNumCurves();
	vertex_count_ = sample.getCurvesNumVertices();
	vertex_count_list_.resize(vertex_count_->size());
	memcpy(&(*vertex_count_list_.begin()), vertex_count_->getData(), vertex_count_->size() * 4);
}

/**
 * update box
 */
void UMAbcCurve::Impl::update_box(bool recursive)
{
	if (!is_valid()) return;
	mutable_box().makeEmpty();
	
	if (curves_->getSchema().getNumSamples() <= 0) return;
	ISampleSelector selector(self_reference()->current_time(), ISampleSelector::kNearIndex);
	ICurvesSchema::Sample sample;
	curves_->getSchema().get(sample, selector);

	mutable_box().extendBy(sample.getSelfBounds());
}

/**
* initialize
* @param [in] recursive do children recursively
* @retval succsess or fail
*/
bool UMAbcCurve::init(bool recursive, UMAbcObjectPtr parent)
{
	impl_->init(recursive);
	return UMAbcObject::init(recursive, parent);
}

/**
* set current time
* @param [in] time time
* @param [in] recursive do children recursively
*/
void UMAbcCurve::set_current_time(unsigned long time, bool recursive)
{
	if (!impl_->is_valid()) return;
	UMAbcObject::set_current_time(time, recursive);
	impl_->set_current_time(time, recursive);
}

/**
* update box
* @param [in] recursive do children recursively
*/
void UMAbcCurve::update_box(bool recursive)
{
	impl_->update_box(recursive);
}

/**
* curve count
*/
unsigned int UMAbcCurve::curve_count() const
{
	return impl_->curve_count();
}

/**
* num verts
*/
const std::vector<int>& UMAbcCurve::vertex_count_list() const
{
	return impl_->vertex_count_list();
}

/**
* get position
*/
const Imath::V3f * UMAbcCurve::positions() const
{
	if (impl_->positions()) {
		return impl_->positions()->get();
	}
	return NULL;
}

/**
* get position size
*/
unsigned int UMAbcCurve::position_size() const
{
	if (impl_->positions()) {
		return impl_->positions()->size();
	}
	return 0;
}


/**
* update curve all
*/
void UMAbcCurve::update_curve_all()
{
	impl_->update_curve_all();
}

UMAbcObjectPtr UMAbcCurve::self_reference()
{
	return impl_->self_reference_.lock();
}

}

