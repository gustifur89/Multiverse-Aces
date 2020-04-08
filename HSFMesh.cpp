#include "HSFMesh.h"

HSFMesh::~HSFMesh()
{
}

void HSFMesh::bindArrays()
{
	this->bindIndexVBO(this->indexBuffer);
	this->bindVertexAttribVBO(0, 3, this->vertexBuffer);
	this->bindVertexAttribVBO(1, 3, this->normalBuffer);
	this->bindVertexAttribVBO(2, 2, this->viBuffer);
}

std::shared_ptr<HSFMesh> HSFMesh::loadFromFile(std::string fileName)
{

	fileName = std::string("src/meshes/") + fileName + std::string(".hsf");
	std::ifstream file;
	file = std::ifstream(fileName, std::ios_base::binary);

	if (file.fail())
	{
		fprintf(stderr, "failed to load file at : "); 
		fprintf(stderr, fileName.c_str());
		fprintf(stderr, "\n");
		return std::shared_ptr<HSFMesh>(NULL);
	}
	std::shared_ptr<HSFMesh> mesh(new HSFMesh());

	std::string line;


	bool alphaChannel = false;

	
	int vertexCount = 0;
	int hueCount = 0;
	int faceCount = 0;

	while (true)
	{
		file >> line;
		if (file.eof())
			return std::shared_ptr<HSFMesh>(NULL);

		if (line == "element")
		{
			std::string type;
			file >> type;
			if (type == "vertex")
			{
				file >> vertexCount;
			}
			if (type == "face")
			{
				file >> faceCount;
			}
			if (type == "color")
			{
				file >> hueCount;
			}
		}
		else if (line == "property")
		{
			std::string type;
			file >> type;
			if (type == "uchar")
			{
				file >> type;
				if (type == "alpha")
				{
					alphaChannel = true;
				}
			}
		}
		else if (line == "end_header")
		{
			//vertexes = true;
			break;
		}

	}
	
	//verts:
	for (int i = 0; i < vertexCount; i++)
	{
		float x, y, z, nx, ny, nz;
		float v, indx;// s, v, h;

		file >> x >> z >> y >> nx >> nz >> ny;	//y and z switch
		file >> v >> indx;// >> h;

		mesh->vertexBuffer.push_back(-x);
		mesh->vertexBuffer.push_back(y);
		mesh->vertexBuffer.push_back(z);

		mesh->normalBuffer.push_back(-nx);
		mesh->normalBuffer.push_back(ny);
		mesh->normalBuffer.push_back(nz);

		mesh->viBuffer.push_back(v);
		mesh->viBuffer.push_back(indx);
		//mesh->svhBuffer.push_back(h);
	}

	//hues:
	for (int i = 0; i < hueCount; i++)
	{
		float h, s;
		file >> h >> s;
		mesh->hsList.push_back(h);
		mesh->hsList.push_back(s);
	}

	mesh->colorPalette = std::shared_ptr<ColorPalette>(new ColorPalette);
	mesh->colorPalette->hsList = mesh->hsList;
	
	//faces
	int num_v;			//whether it is triangles or quads
	int v1, v2, v3, v4;
	for (int i = 0; i < faceCount; i++)
	{
		file >> num_v;

		if (num_v == 3)
		{
			file >> v1 >> v2 >> v3;
			mesh->indexBuffer.push_back(v1);
			mesh->indexBuffer.push_back(v2);
			mesh->indexBuffer.push_back(v3);
		}
		else if (num_v == 4)
		{
			file >> v1 >> v2 >> v3 >> v4;
			mesh->indexBuffer.push_back(v1);
			mesh->indexBuffer.push_back(v2);
			mesh->indexBuffer.push_back(v3);

			mesh->indexBuffer.push_back(v1);
			mesh->indexBuffer.push_back(v3);
			mesh->indexBuffer.push_back(v4);
		}
	}

	file.close();

	mesh->vertexCount = mesh->vertexBuffer.size() / 3;

	mesh->bindIndexVBO(mesh->indexBuffer);

	mesh->bindVertexAttribVBO(0, 3, mesh->vertexBuffer);
	mesh->bindVertexAttribVBO(1, 3, mesh->normalBuffer);
	mesh->bindVertexAttribVBO(2, 2, mesh->viBuffer);
	mesh->recalculateBounds();
	return mesh;
}
