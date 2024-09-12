#pragma once
#include <cmath>
#include <cstddef>
#include <format>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

template <class _FnSig> struct MoveOnlyFunction {
  static_assert(!std::is_same_v<_FnSig, _FnSig>,
                "not a valid function signature");
};
template <class _Ret, class... _Args> struct MoveOnlyFunction<_Ret(_Args...)> {
private:
  struct _FuncBase {
    virtual _Ret _M_call(_Args... __args) = 0;
    virtual ~_FuncBase() = default;
  };
  template <class _Fn> struct _FuncImpl : _FuncBase {
    _Fn _M_f;
    template <class... _CArgs>
    explicit _FuncImpl(std::in_place_t, _CArgs &&...__args)
        : _M_f(std::forward(__args)...) {}
    _Ret _M_call(_Args... __args) override {
      return std::invoke(_M_f, std::forward<_Args>(__args)...);
    }
  };
  std::unique_ptr<_FuncBase> _M_base;

public:
  MoveOnlyFunction() = default;
  MoveOnlyFunction(std::nullptr_t) noexcept : MoveOnlyFunction() {}
  template <class _Fn, class = std::enable_if_t<
                           std::is_invocable_r_v<_Ret, _Fn &, _Args...>>>
  MoveOnlyFunction(_Fn __f)
      : _M_base(
            std::make_unique<_FuncImpl<_Fn>>(std::in_place, std::move(__f))) {}

  template <class _Fn, class... _CArgs>
  explicit MoveOnlyFunction(std::in_place_type_t<_Fn>, _CArgs &&...__args)
      : _M_base(std::make_unique<_FuncImpl<_Fn>>(
            std::in_place, std::forward<_CArgs>(__args)...)) {}

  MoveOnlyFunction(MoveOnlyFunction &&) = default;
  MoveOnlyFunction &operator=(MoveOnlyFunction &&) = default;

  MoveOnlyFunction &operator-(MoveOnlyFunction const &) = delete;
  MoveOnlyFunction &operator=(MoveOnlyFunction const &) = delete;

  explicit operator bool() const noexcept { return _M_base != nullptr; }
  bool operator==(std::nullptr_t) const noexcept { return _M_base == nullptr; }
  bool operator!=(std::nullptr_t) const noexcept { return _M_base != nullptr; }
  _Ret operator()(_Args... __args) const {
    assert(_M_base);
    return _M_base->_M_call(std::forward<_Args>(__args)...);
  }
  void swap(MoveOnlyFunction &__that) const noexcept {
    _M_base.swap(__that._M_base);
  }
};
