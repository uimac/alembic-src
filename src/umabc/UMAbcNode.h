/**
* @file UMAbcNode.h
*
* @author tori31001 at gmail.com
*
* Copyright (C) 2013 Kazuma Hatta
* Licensed  under the MIT license.
*
*/
#pragma once

#include "UMMacro.h"
#include "ImathVec.h"
#include "ImathMatrix.h"
#include <vector>

namespace umabc
{

	class UMAbcNode;
	typedef std::shared_ptr<UMAbcNode> UMAbcNodePtr;
	typedef std::weak_ptr<UMAbcNode> UMAbcNodeWeakPtr;
	typedef std::vector<UMAbcNodePtr> UMAbcNodeList;

	/**
	* a node
	*/
	class UMAbcNode
	{
		DISALLOW_COPY_AND_ASSIGN(UMAbcNode);
	public:
		UMAbcNode() {
			static unsigned int counter = 0;
			id_ = ++counter;
			node_color_ = Imath::V4d(0.5, 0.5, 0.5, 1.0);
		}
		virtual ~UMAbcNode() {}

		// getter
		unsigned int id() const { return id_; }
		const std::string& name() const { return name_; }
		const Imath::M44d& local_transform() const { return local_transform_; }
		const Imath::M44d& global_transform() const { return global_transform_; }
		const Imath::M44d& initial_local_transform() const { return initial_local_transform_; }
		const Imath::M44d& initial_global_transform() const { return initial_global_transform_; }
		const UMAbcNodeList& children() const { return children_; }
		UMAbcNodePtr parent() { return parent_.lock(); }
		const Imath::V4d& node_color() const { return node_color_; }

		// setter
		void set_name(const std::string& name) { name_ = name; }
		Imath::M44d& mutable_local_transform() { return local_transform_; }
		Imath::M44d& mutable_global_transform() { return global_transform_; }
		Imath::M44d& mutable_initial_local_transform() { return initial_local_transform_; }
		Imath::M44d& mutable_initial_global_transform() { return initial_global_transform_; }
		UMAbcNodeList& mutable_children() { return children_; }
		void set_parent(UMAbcNodePtr parent) { parent_ = parent; }
		void set_node_color(const Imath::V4d& color) { node_color_ = color; }

	private:
		unsigned int id_;
		std::string name_;

		// evaluated transform
		Imath::M44d local_transform_;
		Imath::M44d global_transform_;
		Imath::M44d initial_local_transform_;
		Imath::M44d initial_global_transform_;

		Imath::V4d node_color_;

		UMAbcNodeWeakPtr parent_;
		UMAbcNodeList children_;
	};

}
