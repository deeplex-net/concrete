
// Copyright Henrik Steffen Gaßmann 2022
//
// Distributed under the Boost Software License, Version 1.0.
//         (See accompanying file LICENSE or copy at
//           https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <concepts>
#include <initializer_list>
#include <span>
#include <string_view>

#include <status-code/generic_code.hpp>
#include <status-code/status_code.hpp>
#include <status-code/status_code_domain.hpp>

#include <dplx/predef/compiler.h>

#include <dplx/cncr/workaround.h>

#if DPLX_CNCR_WORKAROUND_TESTED_AT(DPLX_COMP_CLANG, 14, 0, 6)
#define DPLX_CNCR_STATUS_CONSTEXPR inline
#else
#define DPLX_CNCR_STATUS_CONSTEXPR constexpr
#endif

namespace dplx::cncr
{

namespace system_error = SYSTEM_ERROR2_NAMESPACE;

// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)

template <typename Enum>
struct status_enum_value_descriptor
{
    Enum value;
    system_error::errc equivalent;
    std::string_view description;
};

// NOLINTEND(cppcoreguidelines-pro-type-member-init)

template <typename Enum>
struct status_enum_definition_defaults
{
    using code = Enum;
    using generic_errc = system_error::errc;

    using value_descriptor = status_enum_value_descriptor<code>;
};

template <typename Enum>
struct status_enum_definition
{
};

// clang-format off
template <typename T>
concept status_enum
        = requires {
            typename status_enum_definition<T>::code;
            requires std::same_as<T, typename status_enum_definition<T>::code>;
            requires std::default_initializable<T>;
            requires std::movable<T>;
            requires std::equality_comparable<T>;
        }
        && std::constructible_from<
                std::span<status_enum_value_descriptor<T> const>,
                decltype(status_enum_definition<T>::values)>
        && !std::span<status_enum_value_descriptor<T> const>(
                status_enum_definition<T>::values)
                .empty()
        && requires {
            { status_enum_definition<T>::domain_name }
                -> std::convertible_to<std::string_view>;
            { status_enum_definition<T>::domain_id }
                -> std::convertible_to<std::string_view>;
        };
// clang-format on

template <status_enum Enum>
consteval auto validate_status_enum_definition_data() noexcept -> bool
{
    using definition = status_enum_definition<Enum>;
    using value_descriptor = status_enum_value_descriptor<Enum>;

    value_descriptor const *const begin = std::begin(definition::values);
    value_descriptor const *const end = std::end(definition::values);
    // is not empty
    if (begin == end)
    {
        return false;
    }

    // is sorted
    for (value_descriptor const *it = begin, *next = begin + 1; next != end;
         it = next++)
    {
        if (it->value > next->value)
        {
            return false;
        }
    }

    return true;
}

// NOLINTBEGIN(cppcoreguidelines-virtual-class-destructor)
// NOLINTBEGIN(cppcoreguidelines-pro-type-static-cast-downcast)

template <status_enum Enum>
class data_defined_status_domain_type;

template <status_enum Enum>
using data_defined_status_code
        = system_error::status_code<data_defined_status_domain_type<Enum>>;

#if defined(DPLX_COMP_GNUC_AVAILABLE)
#pragma GCC diagnostic push
// status domains have no business being dynamically de-/allocated
// and therefore don't need no virtual destructor
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

template <status_enum Enum>
class data_defined_status_domain_type : public system_error::status_code_domain
{
    template <class DomainType>
    friend class system_error::status_code;

public:
    using value_type = Enum;
    using status_code_domain::string_ref;

    using definition = status_enum_definition<value_type>;
    using value_descriptor = status_enum_value_descriptor<value_type>;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    static constexpr std::string_view uuid{definition::domain_id};

    ~data_defined_status_domain_type() = default;
    constexpr explicit data_defined_status_domain_type()
        : status_code_domain(uuid.data(), _uuid_size<uuid.size()>{})
    {
    }

    static constexpr auto get() noexcept -> status_code_domain const &;

    [[nodiscard]] constexpr auto name() const noexcept -> string_ref override
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        constexpr std::string_view domainName{definition::domain_name};
        return string_ref(domainName.data(), domainName.size());
    }

    [[nodiscard]] constexpr auto payload_info() const noexcept
            -> payload_info_t override
    {
        static_assert(alignof(Enum) <= alignof(status_code_domain *));
        // NOLINTNEXTLINE(bugprone-sizeof-expression)
        return {sizeof(Enum), sizeof(status_code_domain *) + sizeof(Enum),
                alignof(status_code_domain *)};
    }

protected:
#if 1
    [[nodiscard]] static constexpr auto count_success_codes() noexcept
            -> std::size_t
    {
        std::size_t numSuccessCodes = 0U;
        for (value_descriptor const &descriptor : definition::values)
        {
            // check whether equivalence map contains errc::success
            if (descriptor.equivalent == system_error::errc::success)
            {
                numSuccessCodes += 1U;
            }
        }
        return numSuccessCodes;
    }
    static constexpr auto num_success_codes = count_success_codes();
    struct success_codes_type
    {
        Enum values[num_success_codes == 0U ? 1U : num_success_codes];
    };
    [[nodiscard]] static constexpr auto extract_success_codes()
            -> success_codes_type
    {
        success_codes_type codes{};
        auto *outputIt = std::begin(codes.values);
        if constexpr (num_success_codes > 0U)
        {
            for (value_descriptor const &descriptor : definition::values)
            {
                // check whether equivalence map contains errc::success
                if (descriptor.equivalent == system_error::errc::success)
                {
                    // this is only const evaluated => no UB allowed anyway
                    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                    *outputIt++ = descriptor.value;
                }
            }
        }
        return codes;
    }
    static constexpr success_codes_type success_codes = extract_success_codes();
#endif

    [[nodiscard]] DPLX_CNCR_STATUS_CONSTEXPR auto
    _do_failure(system_error::status_code<void> const &code) const noexcept
            -> bool override
    {
        auto const &typedCode
                = static_cast<data_defined_status_code<Enum> const &>(code);
        auto &&value = typedCode.value();

#if 1
        if constexpr (num_success_codes > 0U)
        {
            // Does the success code list contain the given value?
            for (auto const &descriptor : success_codes.values)
            {
                if (descriptor == value)
                {
                    return false;
                }
            }
        }
#else
        constexpr std::span<value_descriptor const> descriptors(
                definition::values);
        for (value_descriptor const &descriptor : descriptors)
        {
            if (descriptor.value == value)
            {
                return descriptor.equivalent != system_error::errc::success;
            }
        }
#endif
        return true;
    }

    [[nodiscard]] DPLX_CNCR_STATUS_CONSTEXPR auto
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    _do_equivalent(system_error::status_code<void> const &self,
                   system_error::status_code<void> const &other) const noexcept
            -> bool override
    {
        auto const &typedSelf
                = static_cast<data_defined_status_code<Enum> const &>(self);

        // same domain => strict value comparison
        if (other.domain() == *this)
        {
            auto const &typedOther
                    = static_cast<data_defined_status_code<Enum> const &>(
                            other);

            return typedSelf.value() == typedOther.value();
        }

        // generic code domain => apply data defined equivalence mapping
        if (other.domain() == system_error::generic_code_domain)
        {
            auto const &genericOther
                    = static_cast<system_error::generic_code const &>(other);
            auto &&selfValue = typedSelf.value();

            // find matching descriptor
            constexpr std::span<value_descriptor const> descriptors(
                    definition::values);
            for (value_descriptor const &descriptor : descriptors)
            {
                if (descriptor.value == selfValue)
                {
                    // check whether equivalence map contains the given code
                    return descriptor.equivalent == genericOther.value();
                }
            }
        }

        return false;
    }

    [[nodiscard]] DPLX_CNCR_STATUS_CONSTEXPR auto
    _generic_code(system_error2::status_code<void> const &self) const noexcept
            -> system_error::generic_code override
    {
        auto const &typedCode
                = static_cast<data_defined_status_code<Enum> const &>(self);
        auto &&value = typedCode.value();

        // find matching descriptor
        constexpr std::span<value_descriptor const> descriptors(
                definition::values);
        for (value_descriptor const &descriptor : descriptors)
        {
            if (descriptor.value == value)
            {
                return descriptor.equivalent;
            }
        }
        // the given code can't be represented by the generic coding.
        return system_error::errc::unknown;
    }

    [[nodiscard]] DPLX_CNCR_STATUS_CONSTEXPR auto
    _do_message(system_error2::status_code<void> const &self) const noexcept
            -> string_ref override
    {
        auto const &typedCode
                = static_cast<data_defined_status_code<Enum> const &>(self);
        auto &&value = typedCode.value();
        constexpr std::span<value_descriptor const> descriptors(
                definition::values);

        // find matching descriptor
        for (value_descriptor const &descriptor : descriptors)
        {
            if (descriptor.value == value)
            {
                return string_ref(descriptor.description.data(),
                                  descriptor.description.size());
            }
        }

        return string_ref("unknown error code value");
    }

    [[noreturn]] void _do_throw_exception(
            system_error::status_code<void> const &code) const override
    {
        auto const &typedCode
                = static_cast<data_defined_status_code<Enum> const &>(code);
        throw system_error::status_error<data_defined_status_domain_type>(
                typedCode);
    }
};

// NOLINTEND(cppcoreguidelines-pro-type-static-cast-downcast)
// NOLINTEND(cppcoreguidelines-virtual-class-destructor)

#if defined(DPLX_COMP_GNUC_AVAILABLE)
#pragma GCC diagnostic pop
#endif

template <status_enum Enum>
inline constexpr data_defined_status_domain_type<Enum>
        data_defined_status_domain{};

template <status_enum Enum>
inline constexpr auto data_defined_status_domain_type<Enum>::get() noexcept
        -> status_code_domain const &
{
    return data_defined_status_domain<Enum>;
}

#if 0
template <status_enum Enum>
constexpr auto make_status_code(Enum c) noexcept
        -> data_defined_status_code<Enum>
{
    return data_defined_status_code<Enum>(c);
}
#endif

} // namespace dplx::cncr

namespace SYSTEM_ERROR2_NAMESPACE
{

template <dplx::cncr::status_enum Enum>
struct quick_status_code_from_enum<Enum>
{
    using code_type = dplx::cncr::data_defined_status_code<Enum>;
};

} // namespace SYSTEM_ERROR2_NAMESPACE

#undef DPLX_CNCR_STATUS_CONSTEXPR
