#pragma once

#include <string>
#include <vector>
#include <span>
#include <memory>

#include <glm/glm.hpp>

enum class VertexAttribute
{
	None = 0,
	Position,
	Color,
	TexCoord,
	Instance
};



struct Position   { glm::vec3 value; };
struct Color      { glm::vec3 value; };
struct TexCoord   { glm::vec2 value; };
struct Instance   { glm::vec3 value; };





class VertexBufferLayout
{
public:
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	struct BufferElement
	{
		VertexAttribute Attribute;
		ShaderDataType Type = ShaderDataType::None;
		uint32_t Size = 0;
		uint32_t Offset = 0;
		bool Normalized = false;

		uint32_t GetComponentCount() const;
	};


public:
	VertexBufferLayout() = default;
	VertexBufferLayout(const std::initializer_list<VertexAttribute>& vertexElements);

	void AddElement(BufferElement element) { m_BufferElements.emplace_back(element); }
	void SetStride(uint32_t stride) { m_Stride = stride; }

	const std::vector<BufferElement>& GetElements() const { return m_BufferElements; }
	uint32_t GetStride() const { return m_Stride; }

	static uint32_t ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
	static uint32_t GetShaderDataTypeSize(ShaderDataType type);

private:
	std::vector<BufferElement> m_BufferElements;
	uint32_t m_Stride = 0;
};






template<typename... Attributes>
struct Vertex : public Attributes...
{
	Vertex() = default;
	Vertex(Attributes... args)
		: Attributes(args)...
	{}
};










template<typename T> struct ComponentTraits;

template<> struct ComponentTraits<Position>
{
	static constexpr VertexAttribute Attribute = VertexAttribute::Position;
	static constexpr VertexBufferLayout::ShaderDataType Type = VertexBufferLayout::ShaderDataType::Float3;
};

template<> struct ComponentTraits<Color>
{
	static constexpr VertexAttribute Attribute = VertexAttribute::Color;
	static constexpr VertexBufferLayout::ShaderDataType Type = VertexBufferLayout::ShaderDataType::Float3;
};

template<> struct ComponentTraits<TexCoord>
{
	static constexpr VertexAttribute Attribute = VertexAttribute::TexCoord;
	static constexpr VertexBufferLayout::ShaderDataType Type = VertexBufferLayout::ShaderDataType::Float2;
};

template<> struct ComponentTraits<Instance>
{
	static constexpr VertexAttribute Attribute = VertexAttribute::Instance;
	static constexpr VertexBufferLayout::ShaderDataType Type = VertexBufferLayout::ShaderDataType::Float3;
};



template<typename T>
struct BufferLayoutTraits
{
	static VertexBufferLayout Get()
	{
		static_assert(sizeof(T) == 0, "VertexBufferLayout is not defined for this type!");
		return {};
	}
};




template<typename... Ts>
struct BufferLayoutTraits<Vertex<Ts...>>
{
	static VertexBufferLayout Get()
	{
		VertexBufferLayout layout;
		uint32_t currentOffset = 0;

		([&]()
			{
				VertexBufferLayout::BufferElement element;

				element.Attribute = ComponentTraits<Ts>::Attribute;
				element.Type = ComponentTraits<Ts>::Type;
				element.Size = VertexBufferLayout::GetShaderDataTypeSize(ComponentTraits<Ts>::Type);
				element.Offset = currentOffset;
				element.Normalized = false;

				layout.AddElement(element);
				currentOffset += element.Size;
			}(), ...);

		layout.SetStride(sizeof(Vertex<Ts...>));
		return layout;
	}
};




class VertexBuffer
{
public:

	template<typename T>
	static std::shared_ptr<VertexBuffer> Create(const std::vector<T>& data)
	{
		return std::make_shared<VertexBuffer>(data);
	}

	template<typename T>
	VertexBuffer(const std::vector<T>& data)
		: VertexBuffer(data.data(), static_cast<uint32_t>(data.size() * sizeof(T)), BufferLayoutTraits<T>::Get()) {}

	VertexBuffer(const void* data, uint32_t size, const VertexBufferLayout& layout = {});

	~VertexBuffer();

	void SetLayout(const std::initializer_list<VertexAttribute>& bufferElements);
	void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }


	uint32_t GetID() const { return m_VBO; }
	const VertexBufferLayout& GetLayout() const { return m_Layout; }

private:
	uint32_t m_VBO;
	VertexBufferLayout m_Layout;
};





class IndexBuffer
{
public:

	IndexBuffer(std::span<uint32_t> indexBuffer);
	~IndexBuffer();

	uint32_t GetID() const { return m_IBO; }

private:
	uint32_t m_IBO;
};
