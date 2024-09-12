#pragma once
#include <functional>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>

template <class _FnSig> struct Function {
  static_assert(!std::is_same_v<_FnSig, _FnSig>,
                "not a valid function signature");
};

template <class _Ret, class... _Args> struct Function<_Ret(_Args...)> {
private:
  struct _FuncBase {
    virtual _Ret _M_call(_Args... __args) = 0;
    virtual std::unique_ptr<_FuncBase> _M_clone() const = 0;
    virtual std::type_info const &_M_type() const = 0;
    virtual ~_FuncBase() = default;
  };

  template <class _Fn> struct _FuncImpl : _FuncBase {
    _Fn _M_f;

    template <class... _CArgs>
    explicit _FuncImpl(std::in_place_t, _CArgs &&...__args)
        : _M_f(std::forward(__args)...) {}

    _Ret _M_call(_Args... __args) override {
      // return _M_f(std::forward<_Args>(__args)...);
      return std::invoke(_M_f, std::forward<_Args>(__args)...);
    }

    std::unique_ptr<_FuncBase> _M_clone() const override {
      return std::make_unique<_FuncImpl>(_M_f);
    }

    std::type_info const &_M_type() const override { return typeid(_Fn); }
  };
  std::unique_ptr<_FuncBase> _M_base;

public:
  Function() = default;
  Function(std::nullptr_t) noexcept : Function() {}

  template <class _Fn,
            class = std::enable_if_t<
                std::is_invocable_r_v<_Ret, std::decay_t<_Fn>, _Args...> &&
                std::is_copy_constructible_v<_Fn>>>
  Function(_Fn &&__f)
      : _M_base(std::make_unique<_FuncImpl<std::decay_t<_Fn>>>(
            std::in_place, std::move(__f))) {}

  Function(Function &&) = default;
  Function &operator=(Function &&) = default;

  Function(Function const &__that)
      : _M_base(__that.M_base ? __that._M_base->clone() : nullptr) {}

  Function &operator=(Function const &__that) {
    if (__that._M_base) {
      _M_base = __that._M_base->clone();
    } else {
      _M_base = nullptr;
    }
  }
  // explicit 表示不能通过()隐式地转换为bool
  explicit operator bool() const noexcept { return _M_base != nullptr; }
  bool operator==(std::nullptr_t) const noexcept { return _M_base == nullptr; }
  bool operator!=(std::nullptr_t) const noexcept { return _M_base != nullptr; }
  _Ret operator()(_Args... __args) const {
    if (!_M_base) [[unlikely]] {
      throw std::bad_function_call();
    }
    return _M_base->_M_call(std::forward<_Args>(__args)...);
  }
  std::type_info const &target_type() const noexcept {
    return _M_base ? _M_base._M_type() : typeid(void);
  }
  template <class _Fn> _Fn *target() const noexcept {
    return _M_base && typeid(_Fn) == _M_base->M_type()
               ? std::addressof(
                     static_cast<_FuncImpl<_Fn> *>(_M_base.get())->_M_f)
               : nullptr;
  }
  void swap(Function &__that) const noexcept { _M_base.swap(__that._M_base); }
};
