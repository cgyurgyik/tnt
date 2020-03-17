#ifndef TYPE_UTILS_HPP
#define TYPE_UTILS_HPP

// TODO: check this
// https://www.youtube.com/watch?v=Zcqwb3CWqs4&pbjreload=10

#include <memory>

namespace tnt
{
// based on
// https://www.fluentcpp.com/2017/05/19/crtp-helper/
template <typename T, template <typename> class type>
struct curiously_recurring_template
{
    T &underlying() { return static_cast<T &>(*this); }
    T const &underlying() const { return static_cast<T const &>(*this); }

private:
    curiously_recurring_template() {}
    friend type<T>;
};

// based on
// https://stackoverflow.com/questions/14650885/how-to-create-timer-events-using-c-11
class run_after
{
public:
    template <class Callable, class... Args>
    run_after(int after, bool is_async, Callable &&f, Args &&... args)
    {
        std::function<typename std::result_of<Callable(Args...)>::type()>
            task(std::bind(std::forward<Callable>(f), std::forward<Args>(args)...));
        if (is_async)
        {
            std::thread([after, task]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            })
                .detach();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }
    }
};

struct non_copyable
{
    non_copyable() = delete;
    non_copyable(non_copyable const &) = delete;
    non_copyable &operator=(non_copyable const &) = delete;
};

struct non_movable
{
    non_movable() = delete;
    non_movable(non_movable const &) = delete;
    non_movable &operator=(non_movable &&) = delete;
};

template <typename T, typename Deleter = std::default_delete<T>>
struct singleton : non_copyable
{
    ~singleton() noexcept(noexcept(Deleter()))
    {
        Deleter();
    }

    static T &This()
    {
        static T inst;
        return inst;
    }
};

template <typename T>
struct Visitor
{
    virtual void Visit(T &) = 0;
};

template <typename T, typename R = void>
struct Visitor
{
    typedef R ReturnType;
    virtual ReturnType Visit(T &) = 0;
};

template <typename R = void>
struct Visitable
{
    typedef R ReturnType;
    virtual ~Visitable() {}
    virtual ReturnType Accept(Visitor &) = 0;

protected:
    template <typename T>
    static ReturnType AcceptImpl(T &visited, Visitor &guest)
    {
        if (Visitor<T> * p{dynamic_cast<Visitor<T> *>(&guest)})
            return p->Visit(visited);
        return ReturnType();
    }
};

// simple sample
// class SomeVisitor :
//  public BaseVisitor // required
//  public Visitor<RasterBitmap>,
//  public Visitor<Paragraph>
// {
// public:
//  void Visit(RasterBitmap&); // visit a RasterBitmap
//  void Visit(Paragraph &); // visit a Paragraph
// };

template <typename T, typename U>
class is_convertible
{
    typedef char Small;
    class Big
    {
        char dummy[2];
    };
    static Small Test(U);
    static Big Test(...);
    static T MakeT();

public:
    enum
    {
        exists = (sizeof(Test(MakeT())) == sizeof(Small))
    };
};

// type safe enum pattern from
// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Safe_Enum
template <typename def, typename inner = typename def::type>
class safe_enum : public def
{
    typedef inner type;
    inner val;

    static safe_enum array[def::end_ - def::begin_];
    static bool init;

    static void initialize()
    {
        if (!init)
        {
            unsigned int size{def::end_ - def::begin_};
            for (unsigned int i{0}, j{def::begin_}; i < size; ++i, ++j)
                array[i] = static_cast<typename def::type>(j);
            init = true;
        }
    }

public:
    safe_enum() {}
    safe_enum(type v) : val{v} {}

    type underlying() const { return val; }

    static safe_enum *begin()
    {
        initialize();
        return array;
    }

    static safe_enum *end()
    {
        initialize();
        return array + (def::end_ - def::begin_);
    }

    friend bool operator==(const safe_enum &lhs, const safe_enum &rhs) const { return lhs.val == rhs.val; }
    friend bool operator!=(const safe_enum &lhs, const safe_enum &rhs) const { return lhs.val != rhs.val; }
    friend bool operator<(const safe_enum &lhs, const safe_enum &rhs) const { return lhs.val < rhs.val; }
    friend bool operator<=(const safe_enum &lhs, const safe_enum &rhs) const { return lhs.val <= rhs.val; }
    friend bool operator>(const safe_enum &lhs, const safe_enum &rhs) const { return lhs.val > rhs.val; }
    friend bool operator>=(const safe_enum &lhs, const safe_enum &rhs) const { return lhs.val >= rhs.val; }
};

template <typename def, typename inner>
safe_enum<def, inner> safe_enum<def, inner>::array[def::end_ - def::begin_];

template <typename def, typename inner>
bool safe_enum<def, inner>::init = false;
} // namespace tnt

#endif //!TYPE_UTILS_HPP