#include "StdAfx.h"
#include "Model.h"
#include <fstream>

Model::Model(){
}

Model::Model(string fileName){

	Load(fileName);

}


Model::~Model(void){
}

void Model::Load(string fileName){

	Helper helper;
	ifstream file;

	vector<Face> fs;

	int loaded = 0;
	string name;

	vector<Vertex> vertices;
	vector<Vertex> normals;
	vector<Texture> textureCoords;

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

					Texture t;
					
					t.u = u;
					t.v = v;

					textureCoords.push_back(t);

				} else if(split[0].compare("f") == 0){
				
					int ind, normal, tex;
					Face f;
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

				}
			}
		}
	}
	file.close();

	unsigned nov = vertices.size();
	unsigned non = normals.size();
	unsigned not = textureCoords.size();

	verticesTab = new Vertex[nov];
	normalsTab = new Vertex[non];
	textureCoordsTab = new Texture[not];

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
		meshes[i].UpdateVerticesNormalsTextures(verticesTab, normalsTab, textureCoordsTab);
		meshes[i].BuildLists();
	}

}