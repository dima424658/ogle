#include <graphics/mesh.hpp>

CMesh::CMesh(const std::string& path)
{
	// читаем модель из obj
	ReadFromObj(path);
	  
    // создаем VAO и потом VBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    // что такое VBO, VAO, EBO(indicies) и аттрибуты см. в инете, на learnopengl вполне доходчиво все расписано
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_data.size(), m_data.data(), GL_STATIC_DRAW);

	// делаем m_VAO текущим
    glBindVertexArray(m_VAO);

	// ф-ция glVertexAttribPointer;
	// 1 агрумент - индекс аттрибута (location в вершинном шейдере)
	// 2 - кол-во(size) данных, которые передаем
	// 3 - тип данных, которые передаем
	// 4 - это нам не нужно, всегда GL_FALSE
	// 5 - шаг в массиве m_data в байтах (прыгаем через каждые 32 байта, т.к. каждый полигон состоит из 8 float'ов)
	// 6 - смещение
	// допустим у нас есть данные
	// * отмечаю данные которые мы передаем в шейдер
	// 1 2 3 4 5 6 7 8
	// если 2 аргумент равен двум - передаем два эл-та
	// * * 3 4 5 6 7 8
	// если 5 аргумент равен 4 - передаем по два эл-та два раза
	// * * 3 4 * * 7 8
	// если 6 аргумент равен 1 - передаем со смещением
	// 1 * * 4 5 * * 8

    // аттрибут позиции (aPosition)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // аттрибут нормали (aNormal)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // аттрибут текстур (aTextureUV)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // не забываем все занулить
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

CMesh::~CMesh()
{
    // не забываем за собой убрать
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void CMesh::Draw() const
{
    // все как обычно: говорим какую память рисовать
    glBindVertexArray(m_VAO);
    // и рисуем полигоны
	// m_data.size() / 8 потому что для каждого полигона 8 float
    glDrawArrays(GL_TRIANGLES, 0, m_data.size() / 8);
}

// формат obj:
// v x y z - координаты вершины
// ...
// vn x y z - координаты нормали 
// ...
// vt x y - координаты текстуры
// ...
// f v/t/n vv/tt/nn vvv/ttt/nnn - полигон с индексом вершин v vv vvv, нормалей n nn nnn и коорд. текстур t tt tt
// то есть у нас есть четыре массива, в последнем из которых хранятся индексы к первым трем
// ...
// соотвественно нам все это добро нужно преобразовать в:
// v.x v.y v.z n.x n.y n.z t.x t.y
// vv.x vv.y vv.z nn.x nn.y nn.z tt.x tt.y
// vvv.x vvv.y vvv.z nnn.x nnn.y nnn.z ttt.x ttt.y
// ...
void CMesh::ReadFromObj(const std::string& path)
{
	// открываем файл, если не смогли открыть - сказали об этом и закрыли программу.
	std::ifstream sin(path);
	if (!sin.is_open())
	{
		std::cout << "Не удалось открыть файл " << path << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// сначала мы читаем в формат

	std::vector<glm::vec3> verticies; // тут у нас вершины
	std::vector<glm::vec3> normals; // нормали
	std::vector<glm::vec2> textures; // текстуры
	std::vector<Face> faces; // тут у нас полигоны

	std::string buffer;

	// читаем пока файл не закончится...
	while (!sin.eof())
	{
		sin >> buffer;
		// если наткнулись на вершину - читаем ее в verticies
		// v x y z
		if (buffer.compare("v") == 0)
		{
			glm::vec3 v;
			sin >> v.x >> v.y >> v.z;
			verticies.push_back(v);
		}
		// то же самое с текстурой
		// vt x y z
		if (buffer.compare("vt") == 0)
		{
			glm::vec2 t;
			sin >> t.x >> t.y;
			textures.push_back(t);
		}
		// с нормалью
		// vn x y z
		if (buffer.compare("vn") == 0)
		{
			glm::vec3 n;
			sin >> n.x >> n.y >> n.z;
			normals.push_back(n);
		}
		// ну и с полигонами
		// f v/t/n vv/tt/nn vvv/ttt/nnn
		if (buffer.compare("f") == 0)
		{
			Face f;
			for (int i = 0; i < 3; i++)
			{
				sin >> buffer;
				// здесь мы "вырезаем" необходимые нам данные из каждого полигона формата v/t/n
				// соотвественно трижды т.к. полигон состоит из трех точек
				size_t first = buffer.find_first_of('/');
				size_t last = buffer.find_last_of('/');

				f.vertex = std::stoll(buffer.substr(0, first));
				f.texture = std::stoll(buffer.substr(first + 1, last - first - 1));
				f.normal = std::stoll(buffer.substr(last + 1, buffer.size() - last - 1));
				faces.push_back(f);
			}
		}
	}

	// потом все наши данные записываем в vector из float
	for (auto& face : faces)
	{
		// вставляем в конец вершину полигона face (вычитаем единичку потому, что счет индексов в obj начинается с 1)
		m_data.insert(m_data.end(), &verticies[face.vertex - 1].x, &verticies[face.vertex - 1].x + 3);
		// с нормалями и тектсурами то же самое
		m_data.insert(m_data.end(), &normals[face.normal - 1].x, &normals[face.normal - 1].x + 3);
		m_data.insert(m_data.end(), &textures[face.texture - 1].x, &textures[face.texture - 1].x + 2);
		// по итогу мы добавили 8 эл-тов
		// v.x v.y v.z n.x n.y n.z t.x t.y
		// ну и повторяем пока эти полигоны не закончатся... 
	}

	sin.close();
}
