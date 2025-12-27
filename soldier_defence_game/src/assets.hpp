//assets.hpp

#pragma once

#include "common.hpp"
#include <unordered_map>

class TextureCache {
public:
	TextureCache() = default;

	void unload_all();
	bool load(std::string_view id, std::string_view path);
	bool get(std::string_view id, Texture& texture);

private:
	std::unordered_map<size_t, Texture> m_textures;
};

class SoundCache {
public:
	SoundCache() = default;

	void unload_all();
	bool load(std::string_view id, std::string_view path);
	bool get(std::string_view id, Sound& sound);

private:
	std::unordered_map<size_t, Sound> m_sounds;
};

struct AssetsLoader
{
	AssetsLoader() = default;

	void load_textures(TextureCache& texture_cache);
	void load_sounds(SoundCache& sound_cache);
	Music m_background_music = {};
};