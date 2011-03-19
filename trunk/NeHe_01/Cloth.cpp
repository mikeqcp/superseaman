#include "StdAfx.h"
#include "Cloth.h"


Cloth::Cloth()
{
	showNormals = false;	
    gravity.x = 0.0f;
    gravity.y = -4.0f;
    gravity.z = 10.0f;
                
}

void Cloth::Draw(){

	meshes[0].DirectDraw(showNormals);

}

void Cloth::BuildCloth(){

        edges = createEdgeList();
        noVertices = VerticesCount();

        oldVertices = new Vector3D[noVertices];
        verticesAcc = new Vector3D[noVertices];
		masses = new short[noVertices];
		int count = 0;
        for(int i = 0; i < noVertices; i++){

                oldVertices[i] = verticesTab[i];
				masses[i] = 1;

        }

		vector<int> outline = getOutline();
        while(!outline.empty()){

			int e = outline.at(0);
            outline.erase(outline.begin());

			masses[e-1] = 0;

		}


        constraints = new GLfloat[edges.size()];
        vector<Edge> ed = edges;
		        int i = 0;
        while(!ed.empty()){

                Edge e = ed.at(0);
                ed.erase(ed.begin());
                
                Vector3D v1 = verticesTab[e.v1];
                Vector3D v2 = verticesTab[e.v2];
                Vector3D delta = v2 - v1;

				constraints[i++] = delta.length();

        }

}

Cloth::~Cloth(void)
{

	delete [] masses;

}

void Cloth::Verlet(){

        GLfloat fTimeStep = 0.01f;

        for(int i = 0; i < noVertices; i++){

			    Vector3D v = verticesTab[i];
                Vector3D temp = v;
                Vector3D old = oldVertices[i];
                Vector3D a = verticesAcc[i];

				v += v - old + a*fTimeStep*fTimeStep*(GLfloat)masses[i];

                verticesTab[i] = v;
                oldVertices[i] = temp;
		}
		

}

void Cloth::AccumulateForces(){

        for(int i = 0; i < noVertices; i++)
                verticesAcc[i] = gravity;

}

void Cloth::SatisfyConstraints(){

        vector<Edge> ed = edges;
        int i = 0;
        while(!ed.empty()){

                Edge e = ed.at(0);
                ed.erase(ed.begin());

                Vector3D v1 = verticesTab[e.v1];
                Vector3D v2 = verticesTab[e.v2];

                Vector3D delta = v2 - v1;

				short m1 = masses[e.v1];
				short m2 = masses[e.v2];

				GLfloat deltalength = delta.length();
                GLfloat diff = (deltalength - constraints[i++])/deltalength;

				v1 += delta *0.5f*diff*(GLfloat)m1;
				v2 -= delta *0.5f*diff*(GLfloat)m2;

                verticesTab[e.v1] = v1;
                verticesTab[e.v2] = v2;
        }

}


void Cloth::TimeStep(){

        AccumulateForces();
        Verlet();
        SatisfyConstraints();
		meshes[0].ComputeNormals();

}

void Cloth::ToogleNormals(){

	showNormals = !showNormals;

}

void Cloth::FlipWind(){

	gravity.z *= -1;

}