#include <graphics/mesh.hpp>

using namespace Graphics;

CMesh::CMesh()
{
	m_name = "";
	m_modelPath = "";
}

CMesh::CMesh(std::string_view path)
{
	m_name = "";
	m_modelPath = "";
	LoadModel(path);
}

void CMesh::LoadModel(std::string_view path)
{
	if (!ReadFromObj(path))
	{
		System::Warning() << "Failed to load obj \"" << path << "\"";
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_VBO = OpenGL::Buffer(sizeof(float) * m_data.size(), m_data.data(), GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

const std::string &Graphics::CMesh::GetName() const
{
	return m_name;
}

const std::string &Graphics::CMesh::GetPath() const
{
	return m_modelPath;
}

bool Graphics::CMesh::isLoaded() const
{
	return (m_VAO || m_VBO) && m_modelPath.compare("") != 0;
}

void CMesh::Draw() const
{
	if (m_VAO && m_VBO)
	{
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, m_data.size() / 8);
	}
}

bool CMesh::ReadFromObj(std::string_view path)
{
	std::ifstream sin(path.data());
	if (!sin.is_open())
		return false;

	std::vector<glm::vec3> verticies;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<Face> faces;

	Face maxFace = {0, 0, 0};

	std::string buffer;

	while (!sin.eof())
	{
		sin >> buffer;

		if (buffer.compare("o") == 0)
		{
			if (m_name.compare("") == 0)
				sin >> m_name;
			else
				sin >> buffer;
		}
		if (buffer.compare("v") == 0)
		{
			glm::vec3 v;
			sin >> v.x >> v.y >> v.z;
			verticies.push_back(v);
		}
		if (buffer.compare("vt") == 0)
		{
			glm::vec2 t;
			sin >> t.x >> t.y;
			t.y = 1.0f - t.y;
			textures.push_back(t);
		}
		if (buffer.compare("vn") == 0)
		{
			glm::vec3 n;
			sin >> n.x >> n.y >> n.z;
			normals.push_back(n);
		}
		if (buffer.compare("f") == 0)
		{
			Face f;
			for (int i = 0; i < 3; i++)
			{
				sin >> buffer;

				size_t first = buffer.find_first_of('/');
				size_t last = buffer.find_last_of('/');

				if (first == std::string::npos || last == std::string::npos)
					return false;

				f.vertex = std::stoll(buffer.substr(0, first));
				f.texture = std::stoll(buffer.substr(first + 1, last - first - 1));
				f.normal = std::stoll(buffer.substr(last + 1, buffer.size() - last - 1));

				maxFace.vertex = std::max(maxFace.vertex, f.vertex);
				maxFace.texture = std::max(maxFace.texture, f.texture);
				maxFace.normal = std::max(maxFace.normal, f.normal);

				faces.push_back(f);
			}
		}
	}

	if (maxFace.vertex > verticies.size() || maxFace.normal > normals.size() || maxFace.texture > textures.size())
		return false;

	if (verticies.size() < 1 || normals.size() < 1 || textures.size() < 1)
		return false;

	m_data.clear();

	std::vector<glm::vec3> aTangent;
	std::vector<glm::vec3> aBitangent;

	for (size_t i = 0; i < faces.size(); i += 3)
	{
		glm::vec3 deltaEdge1 = verticies[faces[i + 1].vertex - 1] - verticies[faces[i].vertex - 1];
		glm::vec3 deltaEdge2 = verticies[faces[i + 2].vertex - 1] - verticies[faces[i].vertex - 1];
		glm::vec2 deltaUV1 = textures[faces[i + 1].texture - 1] - textures[faces[i].texture - 1];
		glm::vec2 deltaUV2 = textures[faces[i + 2].texture - 1] - textures[faces[i].texture - 1];

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		aTangent.push_back(r * (deltaUV2.y * deltaEdge1 - deltaUV1.y * deltaEdge2));
		aTangent.push_back(r * (deltaUV2.y * deltaEdge1 - deltaUV1.y * deltaEdge2));
		aTangent.push_back(r * (deltaUV2.y * deltaEdge1 - deltaUV1.y * deltaEdge2));

		aBitangent.push_back(r * (-deltaUV2.x * deltaEdge1 + deltaUV1.x * deltaEdge2));
		aBitangent.push_back(r * (-deltaUV2.x * deltaEdge1 + deltaUV1.x * deltaEdge2));
		aBitangent.push_back(r * (-deltaUV2.x * deltaEdge1 + deltaUV1.x * deltaEdge2));
	}

	for (size_t i = 0; i < faces.size(); i++)
	{
		m_data.insert(m_data.end(), &verticies[faces[i].vertex - 1].x, &verticies[faces[i].vertex - 1].x + 3);
		m_data.insert(m_data.end(), &normals[faces[i].normal - 1].x, &normals[faces[i].normal - 1].x + 3);
		m_data.insert(m_data.end(), &textures[faces[i].texture - 1].x, &textures[faces[i].texture - 1].x + 2);
		m_data.insert(m_data.end(), &aTangent[i].x, &aTangent[i].x + 3);
		m_data.insert(m_data.end(), &aBitangent[i].x, &aBitangent[i].x + 3);
	}

	m_data.shrink_to_fit();

	sin.close();

	m_modelPath = path;

	return true;
}