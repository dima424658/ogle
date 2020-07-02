#include <graphics/mesh.hpp>

using namespace Graphics;

CMesh::CMesh()
{
	m_name = "";
	m_VAO = m_VBO = 0;
}

CMesh::CMesh(std::string_view path)
{
	m_name = "";
	LoadModel(path);
}

CMesh::~CMesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);

	m_VAO = m_VBO = 0;
	m_data.clear();
}

void CMesh::LoadModel(std::string_view path)
{
	std::ifstream sin(path.data());
	
	if (!sin.is_open())
	{
		System::Warning() << "Failed to load obj \"" << path << "\"";
		return;
	}
	else
	if (m_VAO || m_VBO)
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		m_name = "";
		m_data.clear();
	}

	ReadFromObj(path);

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

std::string& Graphics::CMesh::GetName()
{
	return m_name;
}

bool Graphics::CMesh::isLoaded()
{
	return m_VAO || m_VBO;
}

void CMesh::Draw() const
{
	if (m_VAO && m_VBO)
	{
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, m_data.size() / 8);
	}
}

void CMesh::ReadFromObj(std::string_view path)
{
	std::ifstream sin(path.data());
	if (!sin.is_open())
	{
		System::Warning() << "Failed to load obj \"" << path << "\"";
		return;
	}

	std::vector<glm::vec3> verticies;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<Face> faces;

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

				f.vertex = std::stoll(buffer.substr(0, first));
				f.texture = std::stoll(buffer.substr(first + 1, last - first - 1));
				f.normal = std::stoll(buffer.substr(last + 1, buffer.size() - last - 1));
				faces.push_back(f);
			}
		}
	}

	for (auto& face : faces)
	{
		m_data.insert(m_data.end(), &verticies[face.vertex - 1].x, &verticies[face.vertex - 1].x + 3);
		m_data.insert(m_data.end(), &normals[face.normal - 1].x, &normals[face.normal - 1].x + 3);
		m_data.insert(m_data.end(), &textures[face.texture - 1].x, &textures[face.texture - 1].x + 2);
	}

	m_data.shrink_to_fit();

	sin.close();
}
