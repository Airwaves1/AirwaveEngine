#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glad/glad.h>
#include "Utils/Common.h"
#include "GeometryUtils.h"

namespace Airwave {

	class Geometry {
	public:
		virtual ~Geometry() {
			if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
			if (m_VBO) glDeleteBuffers(1, &m_VBO);
			if (m_EBO) glDeleteBuffers(1, &m_EBO);
		}

		void Show() {
			glBindVertexArray(m_VAO);
			glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		}

		std::vector<Vertex>& GetVertices() { return m_Vertices; }
		std::vector<uint32_t>& GetIndices() { return m_Indices; }


	protected:
		Geometry() : m_VAO(0), m_VBO(0), m_EBO(0) { }

		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

	};

	class BoxGeometry : public Geometry {
		public:
		BoxGeometry(float width, float height, float depth);
		~BoxGeometry() = default;

		float GetBoxWidth() const { return m_Width; }
		float GetBoxHeight() const { return m_Height; }
		float GetBoxDepth() const { return m_Depth; }

	private:
		void InitBuffers(float width, float height, float depth);

		float m_Width;
		float m_Height;
		float m_Depth;
	};


}





#endif // !GEOMETRY_H