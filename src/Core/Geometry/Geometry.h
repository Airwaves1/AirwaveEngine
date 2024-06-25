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

		void InitBuffers();

		std::vector<Vertex>& GetVertices() { return m_Vertices; }
		std::vector<uint32_t>& GetIndices() { return m_Indices; }


	protected:
		Geometry() : m_VAO(0), m_VBO(0), m_EBO(0){ }

		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;


	};

	//立方体
	class BoxGeometry : public Geometry {
	public:
		BoxGeometry(float width, float height, float depth);
		~BoxGeometry() = default;

		float GetBoxWidth() const { return m_Width; }
		float GetBoxHeight() const { return m_Height; }
		float GetBoxDepth() const { return m_Depth; }

	private:
		void InitData(float width, float height, float depth);

		float m_Width;
		float m_Height;
		float m_Depth;
	
		int m_Segments;
	};

	//球体
	class SphereGeometry : public Geometry {
	public:
		SphereGeometry(float radius, int segments, float startLatitude, float endLatitude, float startLongitude, float endLongitude);
		~SphereGeometry() = default;

		float GetSphereRadius() const { return m_Radius; }

	private:
		void InitData(float radius, int segments, float startLatitude, float endLatitude, float startLongitude, float endLongitude);

		float m_Radius;


		int m_Segments;
		float m_StartLatitude;
		float m_EndLatitude;
		float m_StartLongitude;
		float m_EndLongitude;

	};

	//圆柱体
	class CylinderGeometry : public Geometry {
	public:
		CylinderGeometry(float radius, float height, int segments);
		~CylinderGeometry() = default;

		float GetCylinderRadius() const { return m_Radius; }
		float GetCylinderHeight() const { return m_Height; }

	private:
		void InitData(float radius, float height, int segments);

		float m_Radius;
		float m_Height;

		int m_Segments;
	};

	//圆锥体
	class ConeGeometry : public Geometry {
	public:
		ConeGeometry(float radius, float height, int segments);
		~ConeGeometry() = default;

		float GetConeRadius() const { return m_Radius; }
		float GetConeHeight() const { return m_Height; }

	private:
		void InitData(float radius, float height, int segments);

		float m_Radius;
		float m_Height;

		int m_Segments;
	};

	//平面
	class PlaneGeometry : public Geometry {
	public:
		PlaneGeometry(float width, float height);
		~PlaneGeometry() = default;

		float GetPlaneWidth() const { return m_Width; }
		float GetPlaneHeight() const { return m_Height; }

	private:
		void InitData(float width, float height);

		float m_Width;
		float m_Height;
	};

	//圆环
	class RingGeometry : public Geometry {
	public:
		RingGeometry(float radius, float tube, int segments);
		~RingGeometry() = default;

		float GetTorusRadius() const { return m_Radius; }
		float GetTorusTube() const { return m_Tube; }

	private:
		void InitData(float radius, float tube, int segments);

		float m_Radius;
		float m_Tube;

		int m_Segments;
	};
	
}





#endif // !GEOMETRY_H