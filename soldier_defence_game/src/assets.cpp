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

void SoundCache::unload_all()
{
    for (auto& it : m_sounds)
    {
        UnloadSound(it.second);
    }
    m_sounds.clear();
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

void AssetsLoader::load_textures(TextureCache& texture_cache)
{
    texture_cache.load("player", "assets/player.png");
    texture_cache.load("bullet", "assets/bullet.png");
    texture_cache.load("enemy", "assets/enemy.png");

}

void AssetsLoader::load_sounds(SoundCache& sound_cache)
{
    sound_cache.load("player_hit", "assets/player_hit_001.ogg");
    sound_cache.load("player_dead", "assets/player_dead.ogg");

    sound_cache.load("bullet_leaves_gun_001", "assets/bullet_leaves_gun_001.ogg");
    sound_cache.load("bullet_leaves_gun_002", "assets/bullet_leaves_gun_002.ogg");
    sound_cache.load("bullet_leaves_gun_003", "assets/bullet_leaves_gun_003.ogg");

    sound_cache.load("gun_empty_001", "assets/gun_empty_001.ogg");
    sound_cache.load("gun_empty_002", "assets/gun_empty_002.ogg");
    sound_cache.load("gun_empty_003", "assets/gun_empty_003.ogg");

    sound_cache.load("enemy_hit_001", "assets/enemy_hit.ogg");
    sound_cache.load("enemy_hit_002", "assets/enemy_hit_002.ogg");
    sound_cache.load("enemy_hit_003", "assets/enemy_hit_003.ogg");
    sound_cache.load("enemy_hit_004", "assets/enemy_hit_004.ogg");
    sound_cache.load("enemy_hit_005", "assets/enemy_hit_005.ogg");

    m_background_music = LoadMusicStream("assets/background_music.ogg");
}
