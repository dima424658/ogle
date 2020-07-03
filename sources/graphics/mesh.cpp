#include <graphics/mesh.hpp>

using namespace Graphics;

CMesh::CMesh()
{
	m_name = "";
	m_modelPath = "";
	m_VAO = m_VBO = 0;
}

CMesh::CMesh(std::string_view path)
{
	m_name = "";
	m_modelPath = "";
	LoadModel(path);
}

CMesh::~CMesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);

	m_VAO = m_VBO = 0;
	m_name = "";
	m_data.clear();
}

void CMesh::LoadModel(std::string_view path)
{
	if(!ReadFromObj(path))
	{
		System::Warning() << "Failed to load obj \"" << path << "\"";

		if(m_VAO && m_VBO)
			return;
	}

	if (m_VAO && m_VBO)
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
	}

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_data.size(), m_data.data(), GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

const std::string& Graphics::CMesh::GetName() const
{
	return m_name;
}

const std::string& Graphics::CMesh::GetPath() const
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
				
				if(first == std::string::npos || last == std::string::npos)
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

	if(maxFace.vertex > verticies.size() || maxFace.normal > normals.size() || maxFace.texture > textures.size())
		return false;

	if(verticies.size() < 1 || normals.size() < 1 || textures.size() < 1)
		return false;

	m_data.clear();

	for (auto& face : faces)
	{
		m_data.insert(m_data.end(), &verticies[face.vertex - 1].x, &verticies[face.vertex - 1].x + 3);
		m_data.insert(m_data.end(), &normals[face.normal - 1].x, &normals[face.normal - 1].x + 3);
		m_data.insert(m_data.end(), &textures[face.texture - 1].x, &textures[face.texture - 1].x + 2);
	}

	m_data.shrink_to_fit();

	sin.close();

	m_modelPath = path;

	return true;
}
