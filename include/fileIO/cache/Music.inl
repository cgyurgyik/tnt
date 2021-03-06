#ifndef TNT_MUSIC_SFX_ASSET_CACHE_INL
#define TNT_MUSIC_SFX_ASSET_CACHE_INL

#include <SDL2/SDL_mixer.h>

#include "fileIO/cache/Base.hpp"

namespace tnt
{
    template <int I>
    class asset_cache<Mix_Music, I>
    {
    public:
        inline ~asset_cache() noexcept
        {
            for (auto const &it : cache)
                Mix_FreeMusic(it.second);
        }

        inline Mix_Music *get(std::string_view path)
        {
            load(path);
            return cache[path.data()];
        }

        inline void load(std::string_view path)
        {
            [[likely]] if (!cache.contains(path.data()))
                cache[path.data()] = Mix_LoadMUS(detail::to_abs(path.data()).c_str());
        }

    private:
        std::byte memory[I * sizeof(Mix_Music *)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::map<std::string, Mix_Music *> cache{&res};
    };

    template <int I>
    using music_cache = asset_cache<Mix_Music, I>;

    using small_music_cache = music_cache<10>;
    using medium_music_cache = music_cache<50>;
    using large_music_cache = music_cache<100>;

    template <int I>
    class asset_cache<Mix_Chunk, I>
    {
    public:
        inline ~asset_cache() noexcept
        {
            for (auto const &it : cache)
                Mix_FreeChunk(it.second);
        }

        [[nodiscard]] inline Mix_Chunk *get(std::string_view path)
        {
            load(path);
            return cache[path.data()];
        }

        inline void load(std::string_view path)
        {
            cache.try_emplace(path.data(), Mix_LoadWAV(detail::to_abs(path.data()).c_str()));
        }

    private:
        std::byte memory[I * sizeof(Mix_Chunk *)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::map<std::string, Mix_Chunk *> cache{&res};
    };

    template <int I>
    using sfx_cache = asset_cache<Mix_Chunk, I>;

    using small_sfx_cache = sfx_cache<10>;
    using medium_sfx_cache = sfx_cache<50>;
    using large_sfx_cache = sfx_cache<100>;
} // namespace tnt

#endif //!TNT_MUSIC_SFX_ASSET_CACHE_INL