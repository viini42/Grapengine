#include "ge_batch_renderer.hpp"

#include "ge_buffer_handler.hpp"
#include "renderer/shader_programs/ge_material_shader.hpp"

#include <glad/glad.h>

using namespace GE;

BatchRenderer::BatchRenderer() :
    m_vertices_data(std::make_pair(VerticesData::Make(), std::vector<u32>{}))
{
}

void BatchRenderer::PushObject(VerticesData&& vd,
                               const std::vector<u32>& indices,
                               const Mat4& modelMat)
{
  const Ptr<VerticesData> shader_vertices_data = m_vertices_data.first;
  BufferHandler::UpdatePosition(vd, modelMat);
  shader_vertices_data->RawPushData(std::move(vd));

  std::vector<u32>& indices_data = m_vertices_data.second;
  if (indices_data.empty())
  {
    indices_data.insert(indices_data.end(), indices.begin(), indices.end());
    return;
  }
  const u32 current_max_id = indices_data.empty() ? 0 : std::ranges::max(indices_data);
  indices_data.reserve(indices_data.size() + indices.size());
  std::ranges::for_each(indices, [&](u32 i) { indices_data.push_back(i + current_max_id + 1); });
}

void BatchRenderer::Begin()
{
  m_vertices_data.first->Clear();
  m_vertices_data.second.clear();
}

void BatchRenderer::End()
{
  m_vertices_data.first->SortVertices();
  m_drawing_object.SetVerticesData(m_vertices_data.first);
  m_drawing_object.SetIndicesData(m_vertices_data.second);

  Draw();
}

void BatchRenderer::Draw() const
{
  m_drawing_object.Bind();
  glDrawElements(GL_TRIANGLES, m_drawing_object.IndicesCount(), GL_UNSIGNED_INT, nullptr);
}
