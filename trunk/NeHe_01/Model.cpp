#include "StdAfx.h"
#include "Model.h"
#include <fstream>

Model::Model(){
}

Model::Model(string fileName, Texture *textures, int textureCount) : textureCount(textureCount){

	Load(fileName, textures,  textureCount);

}


Model::~Model(void){
}

void Model::Load(string fileName, Texture *textures, int textureCount){

	this ->textures = textures;
	this ->textureCount = textureCount;

	Helper helper;
	
	vector<Face> fs;

	int loaded = 0;
	string name;

	vector<Vertex> vertices;
	vector<Vertex> normals;
	vector<TextureUV> textureCoords;

	materials = NULL;
	string materialFileName;
	string currentMaterialName;

	ifstream file;
	file.open(fileName);
	{
		if(file.is_open()){
			while(!file.eof()){

				string read;
				getline(file, read);
				vector<string> split = helper.SplitString(read);

				if(split[0].compare("o") == 0 || file.eof()){

					if(loaded){
						
						Mesh *newMesh = new Mesh(fs, name);
						meshes.push_back(*newMesh);
						delete newMesh;
						fs.clear();

					}
					loaded++;
					name.clear();
					if(!file.eof())
						name = split[1];
					
				} else if(split[0].compare("v") == 0){

					GLfloat x, y, z;
					sscanf_s(split[1].c_str(), "%f", &x);
					sscanf_s(split[2].c_str(), "%f", &y);
					sscanf_s(split[3].c_str(), "%f", &z);

					Vertex v;
					v.x = x;
					v.y = y;
					v.z = z;

					vertices.push_back(v);

				} else if(split[0].compare("vn") == 0){

					GLfloat x, y, z;
					sscanf_s(split[1].c_str(), "%f", &x);
					sscanf_s(split[2].c_str(), "%f", &y);
					sscanf_s(split[3].c_str(), "%f", &z);

					Vertex v;
					v.x = x;
					v.y = y;
					v.z = z;

					normals.push_back(v);

				} else if(split[0].compare("vt") == 0){

					GLfloat u, v;
					sscanf_s(split[1].c_str(), "%f", &u);
					sscanf_s(split[2].c_str(), "%f", &v);

					TextureUV t;
					
					t.u = u;
					t.v = v;

					textureCoords.push_back(t);

				} else if(split[0].compare("f") == 0){
				
					int ind, normal, tex;
					Face f;
					f.materialName = currentMaterialName;
					for(unsigned i = 1; i < split.size(); i++){

						int first = split[i].find_first_of('/');
						int last =  split[i].find_last_of('/');

						if(first == string::npos){

							sscanf_s(split[i].c_str(), "%d", &ind);
							f.vertices.push_back(ind);
							f.normalIndex.push_back(-1);
							f.textures.push_back(-1);

						} else {

							string sub = split[i].substr(0, first);
						
							sscanf_s(sub.c_str(), "%d", &ind);
							f.vertices.push_back(ind);

							sub = split[i].substr(last + 1);
							sscanf_s(sub.c_str(), "%d", &normal);
							f.normalIndex.push_back(normal);

							if (first + 1 != last){

								sub = split[i].substr(first + 1, last);
								sscanf_s(sub.c_str(), "%d", &tex);
								f.textures.push_back(tex);

							} else 
								f.textures.push_back(-1);

						} 

					}
					fs.push_back(f);

				} else if(split[0].compare("mtllib") == 0){

					materialFileName = split[1];

				} else if(split[0].compare("usemtl") == 0){
				
					currentMaterialName = split[1];

				}
			}
		}
	}
	file.close();

	//Materials data
	ifstream materialFile;
	int materialCount = 0, currentMaterial = -1;
	materialFile.open(materialFileName);
	{
		if(materialFile.is_open()){
			while(!materialFile.eof()){

				string read;
				getline(materialFile, read);
				vector<string> split = helper.SplitString(read);

				if(split[0].compare("#") == 0){

					if(split.size() == 4 && split[1].compare("Material") == 0 && split[2].compare("Count:") == 0){

						sscanf_s(split[3].c_str(), "%d", &materialCount);
						if(materialCount > 0)
							materials = new Material[materialCount];

					}

				} else if (split[0].compare("newmtl") == 0){

					currentMaterial++;
					materials[currentMaterial].setName(split[1]);

				} else if (split[0].compare("Ns") == 0){

					GLfloat Ns;
					sscanf_s(split[1].c_str(), "%f", &Ns);
					materials[currentMaterial].setNs(Ns);

				} else if (split[0].compare("Ni") == 0){

					GLfloat Ni;
					sscanf_s(split[1].c_str(), "%f", &Ni);
					materials[currentMaterial].setNi(Ni);

				} else if (split[0].compare("d") == 0){

					GLfloat d;
					sscanf_s(split[1].c_str(), "%f", &d);
					materials[currentMaterial].setD(d);

				} else if (split[0].compare("illum") == 0){

					int illum;
					sscanf_s(split[1].c_str(), "%d", &illum);
					materials[currentMaterial].setIllum(illum);

				} else if (split[0].compare("Ka") == 0){

					GLfloat x, y, z;
					
					sscanf_s(split[1].c_str(), "%f", &x);
					sscanf_s(split[2].c_str(), "%f", &y);
					sscanf_s(split[3].c_str(), "%f", &z);
					
					Vertex Ka;

					Ka.x = x;
					Ka.y = y;
					Ka.z = z;

					materials[currentMaterial].setKa(Ka);

				} else if (split[0].compare("Kd") == 0){

					GLfloat x, y, z;
					
					sscanf_s(split[1].c_str(), "%f", &x);
					sscanf_s(split[2].c_str(), "%f", &y);
					sscanf_s(split[3].c_str(), "%f", &z);
					
					Vertex Kd;

					Kd.x = x;
					Kd.y = y;
					Kd.z = z;

					materials[currentMaterial].setKd(Kd);

				} else if (split[0].compare("Ks") == 0){

					GLfloat x, y, z;
					
					sscanf_s(split[1].c_str(), "%f", &x);
					sscanf_s(split[2].c_str(), "%f", &y);
					sscanf_s(split[3].c_str(), "%f", &z);
					
					Vertex Ks;

					Ks.x = x;
					Ks.y = y;
					Ks.z = z;

					materials[currentMaterial].setKs(Ks);

				} else if (split[0].compare("map_Kd") == 0){

					materials[currentMaterial].setMap_Kd(split[1]);

				}
			}
		}
	}
	materialFile.close();

	unsigned nov = vertices.size();
	unsigned non = normals.size();
	unsigned not = textureCoords.size();

	noVertices = nov;

	verticesTab = new Vertex[nov];
	normalsTab = new Vertex[non];
	textureCoordsTab = new TextureUV[not];

	for(unsigned i = 0; i < nov; i++){
		verticesTab[i] = vertices[0];
		vertices.erase(vertices.begin());
	}

	for(unsigned i = 0; i < non; i++){
		normalsTab[i] = normals[0];
		normals.erase(normals.begin());
	}

	for(unsigned i = 0; i < not; i++){
		textureCoordsTab[i] = textureCoords[0];
		textureCoords.erase(textureCoords.begin());
	}

	for(unsigned i = 0; i < meshes.size(); i++){
		meshes[i].UpdateVerticesNormalsTexturesMaterials(
			verticesTab, 
			normalsTab, 
			textures, 
			textureCount, 
			textureCoordsTab, 
			materials, 
			materialCount
			);

		meshes[i].BuildLists();
	}

}

vector<Edge> Model::createEdgeList(){

	return meshes[0].createEdgeList(noVertices);

}

vector<int> Model::getOutline(){

	return meshes[0].getOutline();

}