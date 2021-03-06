#ifndef TNT_DOO_ECS_SPRITES_HPP
#define TNT_DOO_ECS_SPRITES_HPP

#include "core/Window.hpp"
#include "doo_ecs/Base.hpp"

// TODO: find a way to store the medium_texture_cache* on sprites_sys.

namespace tnt::doo
{
    /// @brief A struct that handles an object's sprite data.
    struct sprite_comp final
    {
        /// @brief Create a new sprite component from an image file.
        /// @param win The window where the sprite will be drawed.
        /// @param file The name of the image file to load.
        sprite_comp(Window const &win, std::string_view filename) noexcept;

        /// @brief Create a new sprite component from an image file.
        /// @param win The window where the sprite will be drawed.
        /// @param file The name of the image file to load.
        /// @param rect The area of the image you want to have on the sprite.
        sprite_comp(Window const &win, std::string_view filename,
                    Rectangle const &rect) noexcept;

        SDL_Texture *tex; /// < The texture handle for the object.
        SDL_Rect crop;    /// < The area of the image represented on the sprite.

    private:
        int w, h;
    };

    /// @brief A struct that handles the sprite data for all the objects.
    inline struct sprites_sys final
    {
        inline sprites_sys() noexcept = default;

        sprites_sys(sprites_sys const &) = delete;
        sprites_sys &operator=(sprites_sys const &) = delete;

        /// @brief Add a new object to the sprites system.
        /// @param sprite The sprite component of the object.
        void add_object(sprite_comp const &sprite);

        /// @brief Load the sprite data of the objects from a given json chunk.
        /// @param j The json chunk from where to load the sprite data of the objects.
        void from_json(Window const &win, nlohmann::json const &j);

        /// @brief Draw object with the given id on the given window.
        /// @param id The id of the object to draw.
        /// @param win The window where the object will be drawed.
        void Draw(object const &id, Window const &win) noexcept;

        std::vector<object> draw_queue; /// < All the id-s of the objects that should be drawed.
        std::vector<SDL_Texture *> tex; /// < The texture data of the objects.
        std::vector<SDL_Rect> clip;     /// < The cropped area from the image.
    } sprites;                          /// < An instance of sprites_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_SPRITES_HPP