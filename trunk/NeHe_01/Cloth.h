#pragma once
#include "modelstructures.h"

class Cloth : public Model
{
private:

        Vector3D gravity;

        void Verlet();
        void SatisfyConstraints();
        void AccumulateForces();

        vector<Edge> edges;

        Vector3D *oldVertices;
        Vector3D *verticesAcc;
        GLfloat *constraints;
		short *masses;


		bool showNormals;

public:

        Cloth();

        ~Cloth(void);

        void BuildCloth();

        void TimeStep();

		void Draw();

		void ToogleNormals();

		void FlipWind();

};