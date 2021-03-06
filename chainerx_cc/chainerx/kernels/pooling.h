#pragma once

#include <cstdint>
#include <memory>
#include <tuple>

#include <absl/types/optional.h>

#include "chainerx/array.h"
#include "chainerx/constant.h"
#include "chainerx/dims.h"
#include "chainerx/kernel.h"

namespace chainerx {

class MaxPoolGradState {
public:
    MaxPoolGradState() = default;

    virtual ~MaxPoolGradState() = default;

    MaxPoolGradState(const MaxPoolGradState&) = default;
    MaxPoolGradState(MaxPoolGradState&&) = default;
    MaxPoolGradState& operator=(const MaxPoolGradState&) = default;
    MaxPoolGradState& operator=(MaxPoolGradState&&) = default;
};

class MaxPoolKernel : public Kernel {
public:
    static const char* name() { return "MaxPool"; }

    virtual std::tuple<Array, std::unique_ptr<MaxPoolGradState>>
    Call(const Array& x, Dims kernel_size, Dims stride, Dims pad, bool cover_all, bool return_state, const absl::optional<Array>& out) = 0;
};

class MaxPoolGradGradState {
public:
    MaxPoolGradGradState() = default;

    virtual ~MaxPoolGradGradState() = default;

    MaxPoolGradGradState(const MaxPoolGradGradState&) = default;
    MaxPoolGradGradState(MaxPoolGradGradState&&) = default;
    MaxPoolGradGradState& operator=(const MaxPoolGradGradState&) = default;
    MaxPoolGradGradState& operator=(MaxPoolGradGradState&&) = default;
};

class MaxPoolGradKernel : public Kernel {
public:
    static const char* name() { return "MaxPoolGrad"; }

    virtual std::tuple<Array, std::unique_ptr<MaxPoolGradGradState>> Call(
            const Array& gout,
            const Dims& kernel_size,
            const Dims& stride,
            const Dims& pad,
            const std::shared_ptr<MaxPoolGradState>& state,
            bool return_state,
            const absl::optional<Array>& gx) = 0;
};

class MaxPoolGradGradKernel : public Kernel {
public:
    static const char* name() { return "MaxPoolGradGrad"; }

    virtual Array Call(
            const Array& ggx,
            const Dims& kernel_size,
            const Dims& stride,
            const Dims& pad,
            bool cover_all,
            const std::shared_ptr<MaxPoolGradGradState>& state,
            const absl::optional<Array>& ggout) = 0;
};

class AveragePoolGradState {
public:
    AveragePoolGradState() = default;

    virtual ~AveragePoolGradState() = default;

    AveragePoolGradState(const AveragePoolGradState&) = default;
    AveragePoolGradState(AveragePoolGradState&&) = default;
    AveragePoolGradState& operator=(const AveragePoolGradState&) = default;
    AveragePoolGradState& operator=(AveragePoolGradState&&) = default;
};

class AveragePoolKernel : public Kernel {
public:
    static const char* name() { return "AveragePool"; }

    virtual std::tuple<Array, std::unique_ptr<AveragePoolGradState>> Call(
            const Array& x,
            const Dims& kernel_size,
            const Dims& stride,
            const Dims& pad,
            AveragePoolPadMode pad_mode,
            bool return_state,
            const absl::optional<Array>& out) = 0;
};

class AveragePoolGradKernel : public Kernel {
public:
    static const char* name() { return "AveragePoolGrad"; }

    virtual Array Call(
            const Array& gout,
            const Dims& kernel_size,
            const Dims& stride,
            const Dims& pad,
            AveragePoolPadMode pad_mode,
            const std::shared_ptr<AveragePoolGradState>& state,
            const absl::optional<Array>& gx) = 0;
};

}  // namespace chainerx
