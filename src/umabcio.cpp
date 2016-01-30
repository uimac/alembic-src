#include <node.h>
#include <v8.h>
#include <memory>
#include "node.h"
#include "node_buffer.h"

#include <vector>
#include <string>
#include <algorithm>
#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
namespace AbcA = Alembic::AbcCoreAbstract;

#include "UMAbcSoftwareIO.h"
#include "UMStringUtil.h"
#include "UMAbcScene.h"
#include "UMAbcObject.h"
#include "UMAbcMesh.h"
#include "UMAbcPoint.h"
#include "UMAbcCurve.h"
#include "UMAbcNurbsPatch.h"
#include "UMAbcCamera.h"
#include "UMAbcXform.h"

using namespace v8;

class UMAbcIO {
public:
	typedef std::map<umstring, umabc::UMAbcScenePtr> SceneMap;

	static UMAbcIO& instance() {
		static UMAbcIO abcio;
		return abcio;
	}

	umabc::UMAbcScenePtr get_scene(Isolate* isolate, const FunctionCallbackInfo<Value>& args) {
		if (args.Length() < 1) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return umabc::UMAbcScenePtr();
		}
		if (!args[0]->IsString()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
			return umabc::UMAbcScenePtr();
		}

		v8::String::Utf8Value utf8path(args[0]->ToString());
		umstring path = umbase::UMStringUtil::utf8_to_utf16(*utf8path);
		return scene_map_[path];
	}

	void load(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		HandleScope scope(isolate);

		if (args.Length() < 1) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}
		if (!args[0]->IsString()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
			return;
		}

		v8::String::Utf8Value utf8path(args[0]->ToString());
		umstring path = umbase::UMStringUtil::utf8_to_utf16(*utf8path);
		if (scene_map_.find(path) != scene_map_.end()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Already Loaded")));
		}

		umabc::UMAbcSoftwareIO abcio;
		umabc::UMAbcSetting setting;
		umabc::UMAbcScenePtr scene = abcio.load(path, setting);
		if (scene && scene->init()) {
			scene_map_[path] = scene;
		}
	}

	void save(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		HandleScope scope(isolate);

		if (args.Length() < 2) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}
		if (!args[1]->IsString()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
			return;
		}
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);

		v8::String::Utf8Value utf8path(args[1]->ToString());
		umstring path = umbase::UMStringUtil::utf8_to_utf16(*utf8path);

		umabc::UMAbcSoftwareIO abcio;
		umabc::UMAbcSetting setting;

		abcio.save(path, scene, setting);
	}

	void get_total_time(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);

		Local<Object> time = Object::New(isolate);
		Local<Number> min_time = Number::New(isolate, scene->min_time());
		Local<Number> max_time = Number::New(isolate, scene->max_time());

		time->Set(String::NewFromUtf8(isolate, "min"), min_time);
		time->Set(String::NewFromUtf8(isolate, "max"), max_time);
		args.GetReturnValue().Set(time);
	}

	void get_time(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);
		Local<Number> time = Number::New(isolate, scene->root_object()->current_time_ms());
		args.GetReturnValue().Set(time);
	}

	void set_time(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);
		double time = args[1]->NumberValue();
		scene->root_object()->set_current_time(static_cast<unsigned long>(time), true);
	}

	void get_mesh_path_list(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);
		std::vector<std::string> path_list = scene->mesh_path_list();
		const int list_size = static_cast<int>(path_list.size());
		Local<Array> result = Array::New(isolate, list_size);
		for (int i = 0; i < list_size; ++i) {
			result->Set(i, String::NewFromUtf8(isolate, path_list[i].c_str()));
		}
		args.GetReturnValue().Set(result);
	}

	void get_point_path_list(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);
		std::vector<std::string> path_list = scene->point_path_list();
		const int list_size = static_cast<int>(path_list.size());
		Local<Array> result = Array::New(isolate, list_size);
		for (int i = 0; i < list_size; ++i) {
			result->Set(i, String::NewFromUtf8(isolate, path_list[i].c_str()));
		}
		args.GetReturnValue().Set(result);
	}

	void get_curve_path_list(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);
		std::vector<std::string> path_list = scene->curve_path_list();
		const int list_size = static_cast<int>(path_list.size());
		Local<Array> result = Array::New(isolate, list_size);
		for (int i = 0; i < list_size; ++i) {
			result->Set(i, String::NewFromUtf8(isolate, path_list[i].c_str()));
		}
		args.GetReturnValue().Set(result);
	}

	void get_nurbs_path_list(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);
		std::vector<std::string> path_list = scene->nurbs_path_list();
		const int list_size = static_cast<int>(path_list.size());
		Local<Array> result = Array::New(isolate, list_size);
		for (int i = 0; i < list_size; ++i) {
			result->Set(i, String::NewFromUtf8(isolate, path_list[i].c_str()));
		}
		args.GetReturnValue().Set(result);
	}

	void get_camera_path_list(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);
		std::vector<std::string> path_list = scene->camera_path_list();
		const int list_size = static_cast<int>(path_list.size());
		Local<Array> result = Array::New(isolate, list_size);
		for (int i = 0; i < list_size; ++i) {
			result->Set(i, String::NewFromUtf8(isolate, path_list[i].c_str()));
		}
		args.GetReturnValue().Set(result);
	}

	void get_xform_path_list(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);
		std::vector<std::string> path_list = scene->xform_path_list();
		const int list_size = static_cast<int>(path_list.size());
		Local<Array> result = Array::New(isolate, list_size);
		for (int i = 0; i < list_size; ++i) {
			result->Set(i, String::NewFromUtf8(isolate, path_list[i].c_str()));
		}
		args.GetReturnValue().Set(result);
	}

	void assign_transform(Local<Object>& result, umabc::UMAbcNodePtr node)
	{
		Isolate* isolate = Isolate::GetCurrent();
		{
			const UMMat44d& global_transform = node->global_transform();
			Local<Array> trans = Array::New(isolate, 16);
			for (int i = 0; i < 4; ++i) {
				for (int k = 0; k < 4; ++k) {
					trans->Set(i * 4 + k, Number::New(isolate, global_transform[i][k]));
				}
			}
			result->Set(String::NewFromUtf8(isolate, "global_transform"), trans);
		}
		{
			const UMMat44d& local_transform = node->local_transform();
			Local<Array> trans = Array::New(isolate, 16);
			for (int i = 0; i < 4; ++i) {
				for (int k = 0; k < 4; ++k) {
					trans->Set(i * 4 + k, Number::New(isolate, local_transform[i][k]));
				}
			}
			result->Set(String::NewFromUtf8(isolate, "local_transform"), trans);
		}
	}

	void get_xform(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);

		v8::String::Utf8Value utf8path(args[1]->ToString());
		std::string object_path(*utf8path);
		Local<Object> result = Object::New(isolate);

		umabc::UMAbcXformPtr xform = std::dynamic_pointer_cast<umabc::UMAbcXform>(scene->find_object(object_path));
		if (xform) {
			assign_transform(result, xform);
			args.GetReturnValue().Set(result);
		}
		args.GetReturnValue().Set(result);
	}

	void get_mesh(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);

		v8::String::Utf8Value utf8path(args[1]->ToString());
		std::string object_path(*utf8path);
		Local<Object> result = Object::New(isolate);

		umabc::UMAbcMeshPtr mesh = std::dynamic_pointer_cast<umabc::UMAbcMesh>(scene->find_object(object_path));
		if (mesh) {
			if (mesh->vertex_size() > 0)
			{
				Local<ArrayBuffer> vertices = v8::ArrayBuffer::New(isolate, mesh->vertex_size() * sizeof(Imath::V3f));
				ArrayBuffer::Contents contents = vertices->GetContents();
				memcpy(contents.Data(), mesh->vertex(), mesh->vertex_size() * sizeof(Imath::V3f));
				result->Set(String::NewFromUtf8(isolate, "vertex"), Float32Array::New(vertices, 0, mesh->vertex_size() * 3));
			}

			if (mesh->normals().size() > 0)
			{
				Local<ArrayBuffer> normals = v8::ArrayBuffer::New(isolate, mesh->normals().size() * sizeof(Imath::V3f));
				ArrayBuffer::Contents contents = normals->GetContents();
				memcpy(contents.Data(), &mesh->normals()[0], mesh->normals().size() * sizeof(Imath::V3f));
				result->Set(String::NewFromUtf8(isolate, "normal"), Float32Array::New(normals, 0, mesh->normals().size() * 3));
			}

			if (mesh->triangle_index().size() > 0)
			{
				Local<ArrayBuffer> indices = v8::ArrayBuffer::New(isolate, mesh->triangle_index().size() * sizeof(umbase::UMVec3i));
				ArrayBuffer::Contents contents = indices->GetContents();
				memcpy(contents.Data(), &mesh->triangle_index()[0], mesh->triangle_index().size() * sizeof(umbase::UMVec3i));
				result->Set(String::NewFromUtf8(isolate, "index"), Int32Array::New(indices, 0, mesh->triangle_index().size() * 3));
			}

			if (mesh->uv_size() > 0) 
			{
				Local<ArrayBuffer> uvs = v8::ArrayBuffer::New(isolate, mesh->uv_size() * sizeof(Imath::V2f));
				ArrayBuffer::Contents contents = uvs->GetContents();
				memcpy(contents.Data(), mesh->uv(), mesh->uv_size() * sizeof(Imath::V2f));
				result->Set(String::NewFromUtf8(isolate, "uv"), Float32Array::New(uvs, 0, mesh->uv_size() * 2));
			}
			assign_transform(result, mesh);
		}
		args.GetReturnValue().Set(result);
	}

	void get_point(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);

		v8::String::Utf8Value utf8path(args[1]->ToString());
		std::string object_path(*utf8path);
		Local<Object> result = Object::New(isolate);

		umabc::UMAbcPointPtr point = std::dynamic_pointer_cast<umabc::UMAbcPoint>(scene->find_object(object_path));
		if (point)
		{
			if (point->position_size() > 0)
			{
				Local<ArrayBuffer> positions = v8::ArrayBuffer::New(isolate, point->position_size() * sizeof(Imath::V3f));
				ArrayBuffer::Contents contents = positions->GetContents();
				memcpy(contents.Data(), point->positions(), point->position_size() * sizeof(Imath::V3f));
				result->Set(String::NewFromUtf8(isolate, "position"), Float32Array::New(positions, 0, point->position_size() * 3));
			}

			if (point->normal_size() > 0)
			{
				Local<ArrayBuffer> normals = v8::ArrayBuffer::New(isolate, point->normal_size() * sizeof(Imath::V3f));
				ArrayBuffer::Contents contents = normals->GetContents();
				memcpy(contents.Data(), &point->normals()[0], point->normal_size() * sizeof(Imath::V3f));
				result->Set(String::NewFromUtf8(isolate, "normal"), Float32Array::New(normals, 0, point->normal_size() * 3));
			}

			if (point->color_size() > 0)
			{
				Local<ArrayBuffer> normals = v8::ArrayBuffer::New(isolate, point->color_size() * sizeof(Imath::V3f));
				ArrayBuffer::Contents contents = normals->GetContents();
				memcpy(contents.Data(), &point->colors()[0], point->color_size() * sizeof(Imath::V3f));
				result->Set(String::NewFromUtf8(isolate, "color"), Float32Array::New(normals, 0, point->color_size() * 3));
			}
			assign_transform(result, point);
		}
		args.GetReturnValue().Set(result);
	}

	void get_curve(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);

		v8::String::Utf8Value utf8path(args[1]->ToString());
		std::string object_path(*utf8path);
		Local<Object> result = Object::New(isolate);

		umabc::UMAbcCurvePtr curve = std::dynamic_pointer_cast<umabc::UMAbcCurve>(scene->find_object(object_path));
		if (curve)
		{
			if (curve->position_size() > 0)
			{
				Local<ArrayBuffer> positions = v8::ArrayBuffer::New(isolate, curve->position_size() * sizeof(Imath::V3f));
				ArrayBuffer::Contents contents = positions->GetContents();
				memcpy(contents.Data(), curve->positions(), curve->position_size() * sizeof(Imath::V3f));
				result->Set(String::NewFromUtf8(isolate, "position"), Float32Array::New(positions, 0, curve->position_size() * 3));
			}

			if (curve->vertex_count_list().size() > 0)
			{
				Local<ArrayBuffer> indices = v8::ArrayBuffer::New(isolate, curve->vertex_count_list().size() * sizeof(int));
				ArrayBuffer::Contents contents = indices->GetContents();
				memcpy(contents.Data(), &curve->vertex_count_list()[0], curve->vertex_count_list().size() * sizeof(int));
				result->Set(String::NewFromUtf8(isolate, "vertex_count_list"), Int32Array::New(indices, 0, curve->vertex_count_list().size()));
			}

			result->Set(String::NewFromUtf8(isolate, "curve"), Int32::New(isolate, curve->curve_count()));

			assign_transform(result, curve);
		}
		args.GetReturnValue().Set(result);
	}

	void get_nurbs(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);

		v8::String::Utf8Value utf8path(args[1]->ToString());
		std::string object_path(*utf8path);
		Local<Object> result = Object::New(isolate);

		umabc::UMAbcNurbsPatchPtr nurbs = std::dynamic_pointer_cast<umabc::UMAbcNurbsPatch>(scene->find_object(object_path));
		if (nurbs)
		{
			if (nurbs->position_size() > 0)
			{
				Local<ArrayBuffer> positions = v8::ArrayBuffer::New(isolate, nurbs->position_size() * sizeof(Imath::V3f));
				ArrayBuffer::Contents contents = positions->GetContents();
				memcpy(contents.Data(), nurbs->positions(), nurbs->position_size() * sizeof(Imath::V3f));
				result->Set(String::NewFromUtf8(isolate, "position"), Float32Array::New(positions, 0, nurbs->position_size() * 3));
			}

			if (nurbs->u_knot_size() > 0)
			{
				Local<ArrayBuffer> u_knots = v8::ArrayBuffer::New(isolate, nurbs->u_knot_size() * sizeof(float));
				ArrayBuffer::Contents contents = u_knots->GetContents();
				memcpy(contents.Data(), nurbs->u_knots(), nurbs->u_knot_size() * sizeof(float));
				result->Set(String::NewFromUtf8(isolate, "u_knot"), Float32Array::New(u_knots, 0, nurbs->u_knot_size()));
			}

			if (nurbs->v_knot_size() > 0)
			{
				Local<ArrayBuffer> v_knots = v8::ArrayBuffer::New(isolate, nurbs->v_knot_size() * sizeof(float));
				ArrayBuffer::Contents contents = v_knots->GetContents();
				memcpy(contents.Data(), nurbs->v_knots(), nurbs->v_knot_size() * sizeof(float));
				result->Set(String::NewFromUtf8(isolate, "v_knot"), Float32Array::New(v_knots, 0, nurbs->v_knot_size()));
			}

			result->Set(String::NewFromUtf8(isolate, "u_size"), Uint32::New(isolate, nurbs->u_size()));
			result->Set(String::NewFromUtf8(isolate, "v_size"), Uint32::New(isolate, nurbs->v_size()));
			result->Set(String::NewFromUtf8(isolate, "u_order"), Uint32::New(isolate, nurbs->u_order()));
			result->Set(String::NewFromUtf8(isolate, "v_order"), Uint32::New(isolate, nurbs->v_order()));

			assign_transform(result, nurbs);
		}
		args.GetReturnValue().Set(result);
	}

	void get_camera(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = Isolate::GetCurrent();
		umabc::UMAbcScenePtr scene = get_scene(isolate, args);

		v8::String::Utf8Value utf8path(args[1]->ToString());
		std::string object_path(*utf8path);
		Local<Object> result = Object::New(isolate);

		umabc::UMAbcCameraPtr camera = std::dynamic_pointer_cast<umabc::UMAbcCamera>(scene->find_object(object_path));
		if (camera)
		{
			assign_transform(result, camera);
		}
		args.GetReturnValue().Set(result);
	}

	void dispose() {
		SceneMap::iterator it = scene_map_.begin();
		for (; it != scene_map_.end(); ++it) {
			it->second = umabc::UMAbcScenePtr();
		}
	}

private:
	SceneMap scene_map_;
};

using node::AtExit;

static void load(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().load(args);
}

static void save(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().save(args);
}

static void get_total_time(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_total_time(args);
}

static void get_time(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_time(args);
}

static void set_time(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().set_time(args);
}

static void get_mesh_path_list(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_mesh_path_list(args);
}

static void get_point_path_list(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_point_path_list(args);
}

static void get_curve_path_list(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_curve_path_list(args);
}

static void get_nurbs_path_list(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_nurbs_path_list(args);
}

static void get_camera_path_list(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_camera_path_list(args);
}

static void get_xform_path_list(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_xform_path_list(args);
}

static void get_mesh(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_mesh(args);
}

static void get_point(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_point(args);
}

static void get_curve(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_curve(args);
}

static void get_nurbs(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_nurbs(args);
}

static void get_camera(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_camera(args);
}

static void get_xform(const FunctionCallbackInfo<Value>& args)
{
	UMAbcIO::instance().get_xform(args);
}

static void dispose(void*)
{
	UMAbcIO::instance().dispose();
}

void Init(Handle<Object> exports) {
	AtExit(dispose);
	NODE_SET_METHOD(exports, "load", load);
	NODE_SET_METHOD(exports, "save", save);
	NODE_SET_METHOD(exports, "get_total_time", get_total_time);
	NODE_SET_METHOD(exports, "get_time", get_time);
	NODE_SET_METHOD(exports, "set_time", set_time);
	NODE_SET_METHOD(exports, "get_mesh_path_list", get_mesh_path_list);
	NODE_SET_METHOD(exports, "get_point_path_list", get_point_path_list);
	NODE_SET_METHOD(exports, "get_curve_path_list", get_curve_path_list);
	NODE_SET_METHOD(exports, "get_nurbs_path_list", get_nurbs_path_list);
	NODE_SET_METHOD(exports, "get_camera_path_list", get_camera_path_list);
	NODE_SET_METHOD(exports, "get_xform_path_list", get_xform_path_list);
	NODE_SET_METHOD(exports, "get_mesh", get_mesh);
	NODE_SET_METHOD(exports, "get_point", get_point);
	NODE_SET_METHOD(exports, "get_curve", get_curve);
	NODE_SET_METHOD(exports, "get_nurbs", get_nurbs);
	NODE_SET_METHOD(exports, "get_camera", get_camera);
	NODE_SET_METHOD(exports, "get_xform", get_xform);
}

NODE_MODULE(umnode, Init)
