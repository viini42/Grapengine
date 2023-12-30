#ifndef GRAPHICENGINE3D_GE_VERTEXARRAY_HPP
#define GRAPHICENGINE3D_GE_VERTEXARRAY_HPP

#include <memory>

class VertexBuffer;
class IndexBuffer;

class VertexArray
{
public:
  VertexArray();
  ~VertexArray();

  void Bind() const;

  void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
  void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

  [[nodiscard]] u32 GetID() const { return id; }

private:
  u32 id;
  std::shared_ptr<VertexBuffer> vertex_buffer;
  std::shared_ptr<IndexBuffer> index_buffer;
};

#endif // GRAPHICENGINE3D_GE_VERTEXARRAY_HPP