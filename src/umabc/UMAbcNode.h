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
#include "UMMathTypes.h"
#include "UMVector.h"
#include "UMMatrix.h"
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
			node_color_ = UMVec4d(0.5, 0.5, 0.5, 1.0);
		}
		virtual ~UMAbcNode() {}

		// getter
		unsigned int id() const { return id_; }
		const umstring& name() const { return name_; }
		const UMMat44d& local_transform() const { return local_transform_; }
		const UMMat44d& global_transform() const { return global_transform_; }
		const UMMat44d& initial_local_transform() const { return initial_local_transform_; }
		const UMMat44d& initial_global_transform() const { return initial_global_transform_; }
		const UMAbcNodeList& children() const { return children_; }
		UMAbcNodePtr parent() { return parent_.lock(); }
		const UMVec4d& node_color() const { return node_color_; }

		// setter
		void set_name(const umstring& name) { name_ = name; }
		UMMat44d& mutable_local_transform() { return local_transform_; }
		UMMat44d& mutable_global_transform() { return global_transform_; }
		UMMat44d& mutable_initial_local_transform() { return initial_local_transform_; }
		UMMat44d& mutable_initial_global_transform() { return initial_global_transform_; }
		UMAbcNodeList& mutable_children() { return children_; }
		void set_parent(UMAbcNodePtr parent) { parent_ = parent; }
		void set_node_color(const UMVec4d& color) { node_color_ = color; }

	private:
		unsigned int id_;
		umstring name_;

		// evaluated transform
		UMMat44d local_transform_;
		UMMat44d global_transform_;
		UMMat44d initial_local_transform_;
		UMMat44d initial_global_transform_;

		UMVec4d node_color_;

		UMAbcNodeWeakPtr parent_;
		UMAbcNodeList children_;
	};

}
