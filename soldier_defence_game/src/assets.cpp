//assets.cpp

#include "assets.hpp"

void TextureCache::unload_all()
{
    for (auto& it : m_textures)
    {
        UnloadTexture(it.second);
    }
    m_textures.clear();
}

bool TextureCache::load(std::string_view id, std::string_view path)
{
    const size_t id_hash = std::hash<std::string_view>{}(id);

    if (!m_textures.contains(id_hash)) 
    {
        Texture texture = LoadTexture(path.data());
        if (!IsTextureValid(texture))
        {
            TraceLog(LOG_ERROR, "Could not load texture %s", path.data());
            return false;
        }

        m_textures.emplace(id_hash, texture);
        TraceLog(LOG_INFO, "Loaded texture - id: %s path: %s", id.data(), path.data());
    }
	return true;
}

bool TextureCache::get(std::string_view id, Texture& texture)
{
    const size_t id_hash = std::hash<std::string_view>{}(id);

    if (!m_textures.contains(id_hash))
    {
        return false;
    }

    texture = m_textures.at(id_hash);

	return true;
}

bool SoundCache::load(std::string_view id, std::string_view path)
{
    const size_t id_hash = std::hash<std::string_view>{}(id);

    if (!m_sounds.contains(id_hash))
    {
        Sound sound = LoadSound(path.data());
        if (!IsSoundValid(sound))
        {
            TraceLog(LOG_ERROR, "Could not load sound %s", path.data());
            return false;
        }

        m_sounds.emplace(id_hash, sound);
        TraceLog(LOG_INFO, "Loaded sound - id: %s path: %s", id.data(), path.data());
    }
    return true;
}

bool SoundCache::get(std::string_view id, Sound& sound)
{
    const size_t id_hash = std::hash<std::string_view>{}(id);

    if (!m_sounds.contains(id_hash))
    {
        return false;
    }

    sound = m_sounds.at(id_hash);

    return true;
}