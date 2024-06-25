#include "Geometry.h"


namespace Airwave {

	void Geometry::InitBuffers()
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), &m_Indices[0], GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);

		// TexCoords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	BoxGeometry::BoxGeometry(float width, float height, float depth)
		: m_Width(width), m_Height(height), m_Depth(depth), m_Segments(1)
	{
		InitData( width, height, depth);
		InitBuffers();
	}

	void BoxGeometry::InitData(float width, float height, float depth)
	{
		GeometryUtils::CreateBoxData(width, height, depth, m_Vertices, m_Indices);
	}


	SphereGeometry::SphereGeometry(float radius, int segments, float startLatitude, float endLatitude, float startLongitude, float endLongitude)
		: m_Radius(radius), m_Segments(segments), m_StartLatitude(startLatitude), m_EndLatitude(endLatitude), m_StartLongitude(startLongitude), m_EndLongitude(endLongitude)
	{
		InitData(radius, segments, startLatitude, endLatitude, startLongitude, endLongitude);
		InitBuffers();
	}

	void SphereGeometry::InitData(float radius, int segments, float startLatitude, float endLatitude, float startLongitude, float endLongitude)
	{
		GeometryUtils::CreateSphereData(radius, segments, startLatitude, endLatitude, startLongitude, endLongitude, m_Vertices, m_Indices);

		
	}

	CylinderGeometry::CylinderGeometry(float radius, float height, int segments)
		: m_Radius(radius), m_Height(height), m_Segments(segments)
	{
		InitData(radius, height, segments);
		InitBuffers();
	}

	void CylinderGeometry::InitData(float radius, float height, int segments)
	{
		GeometryUtils::CreateCylinderData(radius, height, segments, m_Vertices, m_Indices);
	}


	ConeGeometry::ConeGeometry(float radius, float height, int segments)
		: m_Radius(radius), m_Height(height), m_Segments(segments)
	{
		InitData(radius, height, segments);
		InitBuffers();
	}

	void ConeGeometry::InitData(float radius, float height, int segments)
	{
		GeometryUtils::CreateConeData(radius, height, segments, m_Vertices, m_Indices);
	}

	PlaneGeometry::PlaneGeometry(float width, float height)
		: m_Width(width), m_Height(height)
	{
		InitData(width, height);
		InitBuffers();
	}

	void PlaneGeometry::InitData(float width, float height)
	{
		GeometryUtils::CreatePlaneData(width, height, m_Vertices, m_Indices);
	}

	RingGeometry::RingGeometry(float radius, float tube, int segments)
		: m_Radius(radius), m_Tube(tube), m_Segments(segments)
	{
		InitData(radius, tube, segments);
		InitBuffers();
	}

	void RingGeometry::InitData(float radius, float tube, int segments)
	{
		GeometryUtils::CreateRingData(radius, tube, segments, m_Vertices, m_Indices);
	}

}

