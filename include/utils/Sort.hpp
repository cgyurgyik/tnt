#ifndef TNT_UTILS_SORT_HPP
#define TNT_UTILS_SORT_HPP

#include <algorithm>
#include <concepts>
#include <ranges>
#include <span>

#include "utils/TypeUtils.hpp"

// TODO: unfinished
// TODO: write this header using std::sortable types.

namespace tnt
{
    template <typename T>
    using element_t = decltype(*std::ranges::begin(std::declval<T &>()));

    // clang-format off
    template <std::ranges::input_range R>
    class bubble_sort_view : public std::ranges::view_interface<bubble_sort_view<R>>
    // clang-format on
    {
        R base_{};

    public:
        bubble_sort_view() noexcept = default;

        // clang-format off
        inline constexpr bubble_sort_view(R &&r)
            requires std::move_constructible<R> &&
            std::three_way_comparable<element_t<R>>
            // clang-format on
            : base_{std::move(r)}
        {
            for (std::size_t i{0}; i < base_.size(); ++i)
                for (std::size_t j{0}; j < base_.size() - 1; ++j)
                    if (base_[j] > base_[j + 1])
                    {
                        if constexpr (std::is_swappable_v<element_t<R>>)
                            std::swap(base_[j], base_[j + 1]);
                        else
                        {
                            element_t<R> const &temp{base_[j]};
                            base_[j] = base_[j + 1];
                            base_[j + 1] = temp;
                        }
                    }
        }

        inline constexpr iterator begin() const noexcept { return std::ranges::begin(base_); }
        inline constexpr auto end() const noexcept { return std::ranges::end(base_); }

        inline constexpr iterator cbegin() const noexcept { return std::ranges::cbegin(base_); }
        inline constexpr auto cend() const noexcept { return std::ranges::cend(base_); }
    };

// MSVC doesn't support std::ranges::views::all_t yet.
#if !defined(_MSC_VER) || defined(__clang__)
    template <std::ranges::input_range R>
    bubble_sort_view(R &&) noexcept
        ->bubble_sort_view<std::ranges::views::all_t<R>>;
#endif

    struct bubble_sort_fn
    {
        template <std::ranges::input_range R>
        inline constexpr auto operator()(R &&rng) const noexcept
        {
            return bubble_sort_view<R>{std::forward<R>(rng)};
        }
    };

    template <std::ranges::input_range R>
    inline constexpr auto operator|(R &&rng, bubble_sort_fn const &) noexcept
    {
        return bubble_sort_view<R>{std::forward<R>(rng)};
    }

    namespace views
    {
        inline constexpr bubble_sort_fn bubble_sort{};
    }
} // namespace tnt

#endif //! TNT_UTILS_SORT_HPP