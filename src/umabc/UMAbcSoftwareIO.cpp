/**
 * @file UMAbcSoftwareIO.cpp
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */

#include <memory>
#include <string>
#include <assert.h>
#include <fstream>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <functional>

#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/AbcCoreOgawa/All.h>

#include "UMAbcSoftwareIO.h"
#include "UMAbcScene.h"
#include "UMAbcMesh.h"
//#include "UMMaterial.h"

#include "UMStringUtil.h"
#include "UMPath.h"
//#include "UMImage.h"
//#include "UMScene.h"
//#include "UMMesh.h"

namespace umabc
{
	using namespace Alembic::Abc;
	using namespace Alembic::AbcGeom;

/**
 * load 3d file to UMAbcScene
 */
UMAbcScenePtr UMAbcSoftwareIO::load(umstring path, const UMAbcSetting& setting)
{
	Alembic::AbcCoreFactory::IFactory factory;
	IArchive archive = factory.getArchive(umbase::UMStringUtil::utf16_to_utf8(path));

	if (!archive.valid()) { return UMAbcScenePtr(); }

	IObjectPtr top_object(new IObject(archive, kTop));
	UMAbcObjectPtr object = UMAbcObject::create(top_object);
	if (!top_object) { return UMAbcScenePtr(); }
	if (!object) { return UMAbcScenePtr(); }

	AbcA::MetaData meta_data = archive.getPtr()->getMetaData();

	UMAbcScenePtr scene = std::make_shared<UMAbcScene>(object);
	scene->init();
	//if (!umabc::UMAbcSoftwareIO::import_material_map(scene->material_map(), path))
	//{
	//	assign_default_material(scene);
	//}

	//if (umdraw::UMScenePtr reference_scene = setting.reference_scene())
	//{
	//	scene->set_umdraw_scene(reference_scene);
	//}
	return scene;
}

static void copy_props(Alembic::Abc::ICompoundProperty & iRead, Alembic::Abc::OCompoundProperty & iWrite)
{
	std::size_t numChildren = iRead.getNumProperties();
	for (std::size_t i = 0; i < numChildren; ++i)
	{
		Alembic::AbcCoreAbstract::PropertyHeader header =
			iRead.getPropertyHeader(i);
		if (header.isArray())
		{
			Alembic::Abc::IArrayProperty inProp(iRead, header.getName());
			Alembic::Abc::OArrayProperty outProp(iWrite, header.getName(),
				header.getDataType(), header.getMetaData(),
				header.getTimeSampling());

			std::size_t numSamples = inProp.getNumSamples();

			for (std::size_t j = 0; j < numSamples; ++j)
			{
				Alembic::AbcCoreAbstract::ArraySamplePtr samp;
				Alembic::Abc::ISampleSelector sel((Alembic::Abc::index_t) j);
				inProp.get(samp, sel);
				outProp.set(*samp);
			}
		}
		else if (header.isScalar())
		{
			Alembic::Abc::IScalarProperty inProp(iRead, header.getName());
			Alembic::Abc::OScalarProperty outProp(iWrite, header.getName(),
				header.getDataType(), header.getMetaData(),
				header.getTimeSampling());

			std::size_t numSamples = inProp.getNumSamples();
			std::vector<std::string> sampStrVec;
			std::vector<std::wstring> sampWStrVec;
			if (header.getDataType().getPod() == Alembic::AbcCoreAbstract::kStringPOD)
			{
				sampStrVec.resize(header.getDataType().getExtent());
			}
			else if (header.getDataType().getPod() == Alembic::AbcCoreAbstract::kWstringPOD)
			{
				sampWStrVec.resize(header.getDataType().getExtent());
			}

			char samp[4096];

			for (std::size_t j = 0; j < numSamples; ++j)
			{
				Alembic::Abc::ISampleSelector sel(
					(Alembic::Abc::index_t) j);

				if (header.getDataType().getPod() ==
					Alembic::AbcCoreAbstract::kStringPOD)
				{
					inProp.get(&sampStrVec.front(), sel);
					outProp.set(&sampStrVec.front());
				}
				else if (header.getDataType().getPod() ==
					Alembic::AbcCoreAbstract::kWstringPOD)
				{
					inProp.get(&sampWStrVec.front(), sel);
					outProp.set(&sampWStrVec.front());
				}
				else
				{
					inProp.get(samp, sel);
					outProp.set(samp);
				}
			}
		}
		else if (header.isCompound())
		{
			Alembic::Abc::OCompoundProperty outProp(iWrite, header.getName(), header.getMetaData());
			Alembic::Abc::ICompoundProperty inProp(iRead, header.getName());
			copy_props(inProp, outProp);
		}
	}
}

static void copy_object(Alembic::Abc::IObject & iIn, Alembic::Abc::OObject & iOut)
{
	std::size_t numChildren = iIn.getNumChildren();

	Alembic::Abc::ICompoundProperty inProps = iIn.getProperties();
	Alembic::Abc::OCompoundProperty outProps = iOut.getProperties();
	copy_props(inProps, outProps);

	for (std::size_t i = 0; i < numChildren; ++i)
	{
		Alembic::Abc::IObject childIn(iIn.getChild(i));
		Alembic::Abc::OObject childOut(iOut, childIn.getName(), childIn.getMetaData());
		copy_object(childIn, childOut);
	}
}

/**
* save 3d file
*/
bool UMAbcSoftwareIO::save(umstring path, UMAbcScenePtr scene, const UMAbcSetting& setting)
{
	Alembic::Abc::OArchive archive;
	UMAbcObjectPtr root = scene->root_object();
	if (!root) { return false; }
	IObject top_object = *root->object();

	std::string out_file = umbase::UMStringUtil::utf16_to_utf8(path);
	if (setting.export_type() == "ogawa")
	{
		archive = Alembic::Abc::OArchive(
			Alembic::AbcCoreHDF5::WriteArchive(),
			out_file, top_object.getMetaData(),
			Alembic::Abc::ErrorHandler::kQuietNoopPolicy);
	}
	else 
	{
		archive = Alembic::Abc::OArchive(
			Alembic::AbcCoreOgawa::WriteArchive(),
			out_file, top_object.getMetaData(),
			Alembic::Abc::ErrorHandler::kQuietNoopPolicy);
	}

	for (Alembic::Util::uint32_t i = 1; i < archive.getNumTimeSamplings(); ++i)
	{
		archive.addTimeSampling(*archive.getTimeSampling(i));
	}
	Alembic::Abc::OObject out_top_object = archive.getTop();
	copy_object(top_object, out_top_object);

	return true;
}

//void UMAbcSoftwareIO::assign_default_material(UMAbcScenePtr scene)
//{
//	std::vector<std::string> name_list = scene->object_name_list();
//	for (int i = 0, size = static_cast<int>(name_list.size()); i < size; ++i)
//	{
//		umstring material_name = umbase::UMStringUtil::utf8_to_utf16(name_list.at(i));
//		scene->material_map()[material_name] = umdraw::UMMaterial::default_material();
//	}
//}

/**
 * load settings
 */
bool UMAbcSoftwareIO::load_setting(std::string path, UMAbcSetting& setting)
{
	return false;
}

/**
 * save settings
 */
bool UMAbcSoftwareIO::save_setting(std::string path, const UMAbcSetting& setting)
{
	return false;
}


///** 
// * import umabc material list
// */
//bool UMAbcSoftwareIO::import_material_map(
//	umdraw::UMMaterialMap& dst, 
//	const umstring& abc_mtl_filepath)
//{
//	umstring mtl_path = abc_mtl_filepath;
//	mtl_path.replace(abc_mtl_filepath.size() - 3, 3, 
//		umbase::UMStringUtil::utf8_to_utf16("mtl"));
//
//	std::ifstream ifs(mtl_path.c_str());
//	if (!ifs.good())
//	{
//		return false;
//	}
//	std::string line;
//	umdraw::UMMaterialPtr current_mat;
//	for (int line_count = 0; ifs && std::getline(ifs, line); ++line_count) 
//	{
//		if ( line.empty() || line.at(0)=='#' )
//			continue;
//
//		std::istringstream iss(line);
//		std::string id;
//		bool is_load_id = !!(iss >> id);
//		if (!is_load_id) continue;
//
//		if (id == "newmtl")
//		{
//			std::string name;
//			if (iss >> name)
//			{
//				current_mat = umdraw::UMMaterial::default_material();
//				std::istringstream f(name);
//				std::string s;
//				std::string mesh_index;
//				std::string material_index;
//				std::getline(f, s, '_'); //"material"
//				std::getline(f, s, '_');
//				mesh_index = umbase::UMStringUtil::number_to_string(s);
//				std::getline(f, s, '_') >> material_index;
//				material_index = umbase::UMStringUtil::number_to_string(s);
//				name = "mesh_" 
//					+ mesh_index
//					+ "_material_"
//					+ material_index;
//
//				umstring material_name = umbase::UMStringUtil::utf8_to_utf16(name);
//				current_mat->set_name(material_name);
//				dst[material_name] = current_mat;
//			}
//		}
//		else if (id == "Ka")
//		{
//			float r, g, b;
//			if (current_mat 
//				&& (iss >> r) 
//				&& (iss >> g) 
//				&& (iss >> b))
//			{
//				current_mat->set_ambient(UMVec4d(r, g, b, 1.0));
//			}
//		}
//		else if (id == "Kd")
//		{
//			float r, g, b;
//			if (current_mat 
//				&& (iss >> r) 
//				&& (iss >> g) 
//				&& (iss >> b))
//			{
//				current_mat->set_diffuse(UMVec4d(r, g, b, 1.0));
//			}
//		}
//		else if (id == "Ks")
//		{
//			float r, g, b;
//			if (current_mat 
//				&& (iss >> r) 
//				&& (iss >> g) 
//				&& (iss >> b))
//			{
//				current_mat->set_specular(UMVec4d(r, g, b, 1.0));
//			}
//		}
//		else if (id == "Ns")
//		{
//			float power;
//			if (current_mat 
//				&& (iss >> power))
//			{
//				UMVec4d specular = current_mat->specular();
//				specular.w = power;
//				current_mat->set_specular(specular);
//			}
//		}
//		else if (id == "lum")
//		{
//		}
//		else if (id == "map_Kd")
//		{
//			std::string texture_name;
//			if (current_mat 
//				&& (iss >> texture_name))
//			{
//                umstring tex = umbase::UMStringUtil::utf8_to_utf16(texture_name);
//				umstring path = 
//					umbase::UMPath::get_absolute_path(
//					abc_mtl_filepath,
//					tex);
//
//				current_mat->mutable_texture_path_list().push_back(path);
//			}
//		}
//	}
//	ifs.close();
//	return true;
//}

///**
// * convert abc mesh to mesh
// */
//umdraw::UMMeshPtr UMAbcSoftwareIO::convert_abc_mesh_to_mesh(
//	umabc::UMAbcMeshPtr src)
//{
//	if (!src) return umdraw::UMMeshPtr();
//	
//	//UMAbcMesh::IndexList& faces = src->triangle_index();
//	const Imath::V3f *points = src->vertex();
//	const Imath::V2f *uvs = src->uv();
//	std::vector<Imath::V3f>& normals = src->normals();
//
//	umdraw::UMMeshPtr mesh = std::make_shared<umdraw::UMMesh>();
//	
//	unsigned int vertex_size = src->vertex_size();
//	const size_t face_size = vertex_size / 3;
//	mesh->mutable_vertex_list().resize(vertex_size);
//	mesh->mutable_normal_list().resize(vertex_size);
//	mesh->mutable_uv_list().resize(vertex_size);
//	for (size_t i = 0; i < face_size; ++i)
//	{
//		const size_t vi0 = i * 3 + 0;
//		const size_t vi1 = i * 3 + 2;
//		const size_t vi2 = i * 3 + 1;
//		mesh->mutable_vertex_list()[i * 3 + 0] = UMVec3d(points[vi0].x, points[vi0].y, points[vi0].z);
//		mesh->mutable_vertex_list()[i * 3 + 1] = UMVec3d(points[vi1].x, points[vi1].y, points[vi1].z);
//		mesh->mutable_vertex_list()[i * 3 + 2] = UMVec3d(points[vi2].x, points[vi2].y, points[vi2].z);
//		mesh->mutable_normal_list()[i * 3 + 0] = UMVec3d(normals[vi0].x, normals[vi0].y, normals[vi0].z);
//		mesh->mutable_normal_list()[i * 3 + 1] = UMVec3d(normals[vi1].x, normals[vi1].y, normals[vi1].z);
//		mesh->mutable_normal_list()[i * 3 + 2] = UMVec3d(normals[vi2].x, normals[vi2].y, normals[vi2].z);
//		mesh->mutable_uv_list()[i * 3 + 0] = UMVec2d(uvs[vi0].x, 1.0- uvs[vi0].y);
//		mesh->mutable_uv_list()[i * 3 + 1] = UMVec2d(uvs[vi1].x, 1.0- uvs[vi1].y);
//		mesh->mutable_uv_list()[i * 3 + 2] = UMVec2d(uvs[vi2].x, 1.0- uvs[vi2].y);
//	}
//	//// material
//	//umdraw::UMMaterialList::iterator it = src->mutable_material_list().begin();
//	//for (; it != src->mutable_material_list().end(); ++it)
//	//{
//	//	umdraw::UMMaterialPtr mat = *it;
//	//	mesh->mutable_material_list().push_back(mat);
//
//	//	if (mat->mutable_texture_list().empty())
//	//	{
//	//		umdraw::UMMaterial::TexturePathList::const_iterator pt = mat->texture_path_list().begin();
//	//		for (; pt != mat->texture_path_list().end(); ++pt)
//	//		{
//	//			const umstring& path = *pt;
//	//			//if (UMImagePtr image = UMImage::load(path))
//	//			//{
//	//			//	mat->mutable_texture_list().push_back(image);
//	//			//}
//	//		}
//	//	}
//	//}
//	mesh->update_box();
//	return mesh;
//}

/**
 * get abc mesh list from abc object
 */
static void get_abc_mesh_list(std::vector<UMAbcMeshPtr>& dst, umabc::UMAbcObjectPtr src)
{
	if (UMAbcMeshPtr mesh = std::dynamic_pointer_cast<UMAbcMesh>(src))
	{
		dst.push_back(mesh);
	}
	for (int i = 0, size = static_cast<int>(src->children().size()); i < size; ++i)
	{
		get_abc_mesh_list(dst, src->children().at(i));
	}
}

///**
// * convert ccw(wrong) mesh to cw mesh
// */
//void UMAbcSoftwareIO::convert_abc_mesh_ccw_to_cw(umabc::UMAbcObjectPtr src)
//{
//	std::vector<UMAbcMeshPtr> mesh_list;
//	get_abc_mesh_list(mesh_list, src);
//	
//	for (int i = 0, size = static_cast<int>(mesh_list.size()); i < size; ++i)
//	{
//		UMAbcMeshPtr mesh = mesh_list.at(i);
//		mesh->update_mesh_all();
//	}
//
//}

} // umabc
