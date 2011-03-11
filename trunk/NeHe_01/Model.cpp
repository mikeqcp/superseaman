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
					
				}
				if(split[0].compare("v") == 0){

					GLfloat x, y, z;
					sscanf_s(split[1].c_str(), "%f", &x);
					sscanf_s(split[2].c_str(), "%f", &y);
					sscanf_s(split[3].c_str(), "%f", &z);

					Vertex v;
					v.x = x;
					v.y = y;
					v.z = z;

					vertices.push_back(v);
				}
				if(split[0].compare("f") == 0){
				
					int ind, normal;
					Face f;
					for(unsigned i = 1; i < split.size(); i++){

						string sub = split[i].substr(0, split[i].find_first_of('/'));
						
						sscanf_s(sub.c_str(), "%d", &ind);
						f.vertices.push_back(ind);

						sub = split[i].substr(split[i].find_last_of('/')+1);
						sscanf_s(sub.c_str(), "%d", &normal);
						f.normalIndex.push_back(normal);

					}
					fs.push_back(f);

				}

				if(split[0].compare("vn") == 0){

					GLfloat x, y, z;
					sscanf_s(split[1].c_str(), "%f", &x);
					sscanf_s(split[2].c_str(), "%f", &y);
					sscanf_s(split[3].c_str(), "%f", &z);

					Vertex v;
					v.x = x;
					v.y = y;
					v.z = z;

					normals.push_back(v);

				}

			}
		}

	}
	file.close();

	verticesTab = new Vertex[vertices.size()];
	normalsTab = new Vertex[normals.size()];

	int nov = vertices.size();
	int non = normals.size();

	for(unsigned i = 0; i < nov; i++){
		verticesTab[i] = vertices[0];
		vertices.erase(vertices.begin());
	}

	for(unsigned i = 0; i < non; i++){
		normalsTab[i] = normals[0];
		normals.erase(normals.begin());
	}

	for(unsigned i = 0; i < meshes.size(); i++){
		meshes[i].UpdateVerticesNormals(verticesTab, normalsTab);
		meshes[i].BuildLists();
	}

}